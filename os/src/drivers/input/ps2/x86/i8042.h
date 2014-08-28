/*
 * \brief  Interface to i8042 controller
 * \author Norman Feske
 * \date   2007-09-21
 */

/*
 * Copyright (C) 2007-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _I8042_H_
#define _I8042_H_

#include <io_port_session/connection.h>
#include <base/env.h>
#include <os/ring_buffer.h>

#include "serial_interface.h"


class I8042
{
	/*
	 * XXX: To be moved into the implementation file
	 */
	enum Register
	{
		REG_DATA         = 0x60,
		REG_STATUS       = 0x64,
		REG_COMMAND      = 0x64,
	};

	enum Flag
	{
		/* status register */
		STAT_OBF         = 0x01,
		STAT_IBF         = 0x02,
		STAT_04          = 0x04,
		STAT_08          = 0x08,
		STAT_10          = 0x10,
		STAT_AUX_DATA    = 0x20,
		STAT_40          = 0x40,
		STAT_80          = 0x80,

		/* control register */
		CTRL_KBD_INT     = 0x01,
		CTRL_AUX_INT     = 0x02,
		CTRL_04          = 0x04,
		CTRL_08          = 0x08,
		CTRL_KBD_DISABLE = 0x10,
		CTRL_AUX_DISABLE = 0x20,
		CTRL_XLATE       = 0x40,
		CTRL_80          = 0x80,
	};

	enum Command
	{
		CMD_READ         = 0x20,
		CMD_WRITE        = 0x60,
		CMD_TEST         = 0xaa,

		CMD_AUX_DISABLE  = 0xa7,
		CMD_AUX_ENABLE   = 0xa8,
		CMD_AUX_TEST     = 0xa9,

		CMD_KBD_DISABLE  = 0xad,
		CMD_KBD_ENABLE   = 0xae,
		CMD_KBD_TEST     = 0xab,

		CMD_AUX_WRITE    = 0xd4,
	};

	enum Return
	{
		RET_TEST_OK      = 0x55,
		RET_KBD_TEST_OK  = 0x00,
		RET_AUX_TEST_OK  = 0x00,
	};

	class _Channel : public Serial_interface,
	                 public Ring_buffer<unsigned char, 1024>
	{
		private:

			I8042  &_i8042;
			bool    _aux;

		public:

			/**
			 * Constructor
			 *
			 * \param i8042  controller
			 * \param aux    channel type is aux
			 *
			 * The 'aux' parameter is used to direct write operations. Read
			 * operations always block on the channel's ring buffer, which
			 * gets fed from outside this class.
			 */
			_Channel(I8042 &i8042, bool aux) : _i8042(i8042), _aux(aux) { }

			/**
			 * Read all available data from controller
			 */
			void flush_read()
			{
				Genode::Lock::Guard guard(_i8042._lock);

				while (_i8042._output_buffer_full())
					_i8042._read_and_route();
			}

			unsigned char read()
			{
				while (empty())
					flush_read();

				return get();
			}

			void write(unsigned char value)
			{
				Genode::Lock::Guard guard(_i8042._lock);

				if (_aux)
					_i8042._command(CMD_AUX_WRITE);

				_i8042._data(value);
			}

			bool data_read_ready()
			{
				flush_read();
				return !empty();
			}
	};

	private:

		Genode::Io_port_connection _data_port;  /* data port */
		Genode::Io_port_connection _stat_port;  /* status/command port */
		bool                       _kbd_xlate;  /* translation mode to scan-code set 1 */

		/**
		 * Read controller status
		 */
		unsigned char _status() { return _stat_port.inb(REG_STATUS); }

		/**
		 * Read data from controller
		 */
		unsigned char _data() { return _data_port.inb(REG_DATA); }

		/**
		 * Issue command to controller
		 */
		void _command(unsigned char cmd)
		{
			while (_input_buffer_full());
			_stat_port.outb(REG_STATUS, cmd);
		}

		/**
		 * Send data to controller
		 */
		void _data(unsigned char value)
		{
			while (_input_buffer_full());
			_data_port.outb(REG_DATA, value);
		}

		/**
		 * Convenience functions for accessing the controller status
		 */
		bool _output_buffer_full() { return _status() & STAT_OBF; }
		bool _input_buffer_full()  { return _status() & STAT_IBF; }
		bool _aux_data_pending()   { return _status() & STAT_AUX_DATA; }

		/**
		 * Wait for data and read
		 */
		unsigned char _wait_data()
		{
			while (!_output_buffer_full());
			return _data();
		}

		/**
		 * Read character from controller and route it to serial channel
		 */
		void _read_and_route()
		{
			while (!_output_buffer_full());

			if (_aux_data_pending())
				_aux_interface.add(_data());
			else
				_kbd_interface.add(_data());
		}

		_Channel _kbd_interface;
		_Channel _aux_interface;

		/*
		 * Both serial interfaces may be used by different
		 * threads, e.g., interrupt handlers. Hence, controller
		 * transactions (read/write sequences) must be protected
		 * with a lock.
		 */
		Genode::Lock _lock;

	public:

		/**
		 * Constructor
		 */
		I8042() :
			_data_port(REG_DATA,   1),
			_stat_port(REG_STATUS, 1),
			_kbd_interface(*this, false),
			_aux_interface(*this, true)
		{
			reset();
		}

		/**
		 * Test and initialize controller
		 */
		void reset()
		{
			int ret;

			/* read remaining data in controller */
			while (_output_buffer_full()) _data();

			/* run self tests */
			_command(CMD_TEST);
			if ((ret = _wait_data()) != RET_TEST_OK) {
				Genode::printf("i8042: self test failed (%x)\n", ret);
				return;
			}
			_command(CMD_KBD_TEST);
			if ((ret = _wait_data()) != RET_KBD_TEST_OK) {
				Genode::printf("i8042: kbd test failed (%x)\n", ret);
				return;
			}
			_command(CMD_AUX_TEST);
			if ((ret = _wait_data()) != RET_AUX_TEST_OK) {
				Genode::printf("i8042: aux test failed (%x)\n", ret);
				return;
			}

			/* query xlate bit */
			_command(CMD_READ);
			ret = _wait_data();
			_kbd_xlate = !!(ret & CTRL_XLATE);

			/* enable interrupts for keyboard and mouse at the controller */
			ret |= CTRL_KBD_INT | CTRL_AUX_INT;
			_command(CMD_WRITE);
			_data(ret);

			/* enable keyboard and mouse */
			_command(CMD_KBD_ENABLE);
			_command(CMD_AUX_ENABLE);
		}

		/**
		 * Return true if controller operates in translation mode
		 *
		 * If xlate is set, the controller keyboard events to scan-code set 1.
		 * We just detect the setting as defined by the BIOS. If xlate is
		 * clear, we have to decode scan-code-set 2 packets.
		 */
		bool kbd_xlate() { return _kbd_xlate; }

		/**
		 * Request serial keyboard interface
		 */
		Serial_interface *kbd_interface() { return &_kbd_interface; }

		/**
		 * Request serial mouse interface
		 */
		Serial_interface *aux_interface() { return &_aux_interface; }
};

#endif
