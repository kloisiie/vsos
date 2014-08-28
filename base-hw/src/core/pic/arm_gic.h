/*
 * \brief  Programmable interrupt controller for core
 * \author Martin stein
 * \date   2011-10-26
 */

/*
 * Copyright (C) 2011-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _PIC__ARM_GIC_H_
#define _PIC__ARM_GIC_H_

/* Genode includes */
#include <util/mmio.h>

namespace Arm_gic
{
	using namespace Genode;

	/**
	 * Programmable interrupt controller for core
	 *
	 * ARM generic interrupt controller, Architecture version 2.0
	 */
	class Pic
	{
		public:

			enum { MAX_INTERRUPT_ID = 1023 };

		protected:

			enum {
				MIN_SPI  = 32,
				SPURIOUS_ID = 1023,
			};

			/**
			 * Distributor interface
			 */
			struct Distr : public Mmio
			{
				/**
				 * Constructor
				 */
				Distr(addr_t const base) : Mmio(base) { }

				/**
				 * Control register
				 */
				struct Ctlr : Register<0x000, 32>
				{
					struct Enable : Bitfield<0,1> { };
				};

				/**
				 * Controller type register
				 */
				struct Typer : Register<0x004, 32>
				{
					struct It_lines_number : Bitfield<0,5> { };
					struct Cpu_number      : Bitfield<5,3> { };
				};

				/**
				 * Interrupt group register
				 */
				struct Igroupr :
					Register_array<0x80, 32, MAX_INTERRUPT_ID + 1, 1>
				{
					struct Group_status : Bitfield<0, 1> { };
				};

				/**
				 * Interrupt set enable registers
				 */
				struct Isenabler :
					Register_array<0x100, 32, MAX_INTERRUPT_ID + 1, 1, true>
				{
					struct Set_enable : Bitfield<0, 1> { };
				};

				/**
				 * Interrupt clear enable registers
				 */
				struct Icenabler :
					Register_array<0x180, 32, MAX_INTERRUPT_ID + 1, 1, true>
				{
					struct Clear_enable : Bitfield<0, 1> { };
				};

				/**
				 * Interrupt priority level registers
				 */
				struct Ipriorityr :
					Register_array<0x400, 32, MAX_INTERRUPT_ID + 1, 8>
				{
					enum { GET_MIN = 0xff };

					struct Priority : Bitfield<0, 8> { };
				};

				/**
				 * Interrupt processor target registers
				 */
				struct Itargetsr :
					Register_array<0x800, 32, MAX_INTERRUPT_ID + 1, 8>
				{
					enum { ALL = 0xff };

					struct Cpu_targets : Bitfield<0, 8> { };
				};

				/**
				 * Interrupt configuration registers
				 */
				struct Icfgr :
					Register_array<0xc00, 32, MAX_INTERRUPT_ID + 1, 2>
				{
					struct Edge_triggered : Bitfield<1, 1> { };
				};

				/**
				 * Minimum supported interrupt priority
				 */
				Ipriorityr::access_t min_priority()
				{
					write<Ipriorityr::Priority>(Ipriorityr::GET_MIN, 0);
					return read<Ipriorityr::Priority>(0);
				}

				/**
				 * Maximum supported interrupt priority
				 */
				Ipriorityr::access_t max_priority() { return 0; }

				/**
				 * ID of the maximum supported interrupt
				 */
				Typer::access_t max_interrupt()
				{
					enum { LINE_WIDTH_LOG2 = 5 };
					Typer::access_t lnr = read<Typer::It_lines_number>();
					return ((lnr + 1) << LINE_WIDTH_LOG2) - 1;
				}

			} _distr;

			/**
			 * CPU interface
			 */
			struct Cpu : public Mmio
			{
				/**
				 * Constructor
				 */
				Cpu(addr_t const base) : Mmio(base) { }

				/**
				 * Control register
				 */
				struct Ctlr : Register<0x00, 32>
				{
					/* Without security extension */
					struct Enable : Bitfield<0,1> { };

					/* In a secure world */
					struct Enable_grp0 : Bitfield<0,1> { };
					struct Enable_grp1 : Bitfield<1,1> { };
					struct Fiq_en      : Bitfield<3,1> { };
				};

				/**
				 * Priority mask register
				 */
				struct Pmr : Register<0x04, 32>
				{
					struct Priority : Bitfield<0,8> { };
				};

				/**
				 * Binary point register
				 */
				struct Bpr : Register<0x08, 32>
				{
					enum { NO_PREEMPTION = 7 };

					struct Binary_point : Bitfield<0,3> { };
				};

				/**
				 * Interrupt acknowledge register
				 */
				struct Iar : Register<0x0c, 32, true>
				{
					struct Irq_id : Bitfield<0,10> { };
					struct Cpu_id : Bitfield<10,3> { };
				};

				/**
				 * End of interrupt register
				 */
				struct Eoir : Register<0x10, 32, true>
				{
					struct Irq_id : Bitfield<0,10> { };
					struct Cpu_id : Bitfield<10,3> { };
				};
			} _cpu;

			unsigned const _max_interrupt;
			unsigned       _last_request;

			/**
			 * Wether the security extension is used or not
			 */
			inline static bool _use_security_ext();

		public:

			/**
			 * Constructor, all interrupts get masked
			 */
			Pic(addr_t const distr_base, addr_t const cpu_base) :
				_distr(distr_base), _cpu(cpu_base),
				_max_interrupt(_distr.max_interrupt()),
				_last_request(SPURIOUS_ID)
			{
				/* with security extension any board has its own init */
				if (_use_security_ext()) return;

				/* disable device */
				_distr.write<Distr::Ctlr::Enable>(0);

				/* supported priority range */
				unsigned const min_prio = _distr.min_priority();
				unsigned const max_prio = _distr.max_priority();

				/* configure every shared peripheral interrupt */
				for (unsigned i=MIN_SPI; i <= _max_interrupt; i++)
				{
					_distr.write<Distr::Icfgr::Edge_triggered>(0, i);
					_distr.write<Distr::Ipriorityr::Priority>(max_prio, i);
					_distr.write<Distr::Itargetsr::Cpu_targets>(
						Distr::Itargetsr::ALL, i);
				}

				/* disable the priority filter */
				_cpu.write<Cpu::Pmr::Priority>(min_prio);

				/* disable preemption of interrupt handling by interrupts */
				_cpu.write<Cpu::Bpr::Binary_point>(Cpu::Bpr::NO_PREEMPTION);

				/* enable device */
				_distr.write<Distr::Ctlr::Enable>(1);
				_cpu.write<Cpu::Ctlr::Enable>(1);
			}

			/**
			 * Get the ID of the last interrupt request
			 *
			 * \return  True if the request with ID 'i' is treated as accepted
			 *          by the CPU and awaits an subsequently 'finish_request'
			 *          call. Otherwise this returns false and the value of 'i'
			 *          remains useless.
			 */
			bool take_request(unsigned & i)
			{
				_last_request = _cpu.read<Cpu::Iar::Irq_id>();
				i = _last_request;
				return valid(i);
			}

			/**
			 * Complete the last request that was taken via 'take_request'
			 */
			void finish_request()
			{
				if (!valid(_last_request)) return;
				_cpu.write<Cpu::Eoir>(Cpu::Eoir::Irq_id::bits(_last_request) |
				                      Cpu::Eoir::Cpu_id::bits(0) );
				_last_request = SPURIOUS_ID;
			}

			/**
			 * Check if 'i' is a valid interrupt request ID at the device
			 */
			bool valid(unsigned const i) const { return i <= _max_interrupt; }

			/**
			 * Unmask all interrupts
			 */
			void unmask()
			{
				for (unsigned i=0; i <= _max_interrupt; i++)
					_distr.write<Distr::Isenabler::Set_enable>(1, i);
			}

			/**
			 * Unmask interrupt 'i'
			 */
			void unmask(unsigned const i) {
				_distr.write<Distr::Isenabler::Set_enable>(1, i); }

			/**
			 * Mask all interrupts
			 */
			void mask()
			{
				for (unsigned i=0; i <= _max_interrupt; i++)
					_distr.write<Distr::Icenabler::Clear_enable>(1, i);
			}

			/**
			 * Mask interrupt 'i'
			 */
			void mask(unsigned const i) {
				_distr.write<Distr::Icenabler::Clear_enable>(1, i); }
	};
}

#endif /* _PIC__ARM_GIC_H_ */

