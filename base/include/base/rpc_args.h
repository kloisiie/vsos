/*
 * \brief  Helpers for non-ordinary RPC arguments
 * \author Norman Feske
 * \date   2011-04-06
 */

/*
 * Copyright (C) 2011-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__BASE__RPC_ARGS_H_
#define _INCLUDE__BASE__RPC_ARGS_H_

#include <util/string.h>
#include <base/stdint.h>
#include <base/printf.h>

namespace Genode {

	/**
	 * Base class of 'Rpc_in_buffer'
	 */
	class Rpc_in_buffer_base
	{
		protected:

			const char *_base;
			size_t      _size;

			/**
			 * Construct buffer from null-terminated string
			 */
			explicit Rpc_in_buffer_base(const char *str)
			: _base(str), _size(strlen(str) + 1) { }

			/**
			 * Construct an empty buffer by default
			 */
			Rpc_in_buffer_base(): _base(0), _size(0) { }

		public:

			/**
			 * Construct buffer
			 */
			Rpc_in_buffer_base(const char *base, size_t size)
			: _base(base), _size(size) { }

			const char *base() const { return _base; }
			size_t      size() const { return _size; }
			//virtual char* getball() { return 0; }
	};


	/**
	 * Buffer with size constrain
	 */
	template <size_t MAX>
	class Rpc_in_buffer : public Rpc_in_buffer_base
	{
		private:

			/*
			 * This member is only there to pump up the size of the object such
			 * that 'sizeof()' returns the maximum buffer size when queried by
			 * the RPC framework.
			 */
			char _balloon[MAX];

		public:

			enum { MAX_SIZE = MAX };

			/**
			 * Construct buffer
			 */
			Rpc_in_buffer(const char *base, size_t size)
			: Rpc_in_buffer_base(base, min(size, (size_t)MAX_SIZE)) { }

			/**
			 * Construct buffer from null-terminated string
			 */
			Rpc_in_buffer(const char *str) : Rpc_in_buffer_base(str)
			{
				if (_size >= MAX_SIZE - 1)
					_size = MAX_SIZE - 1;
			}

			/**
			 * Default constructor creates invalid buffer
			 */
			Rpc_in_buffer() { }

			void operator = (Rpc_in_buffer<MAX_SIZE> const &from)
			{
				_base = from.base();
				_size = from.size();
			}
			//virtual char* getball() { return 0; }
			//const char* getball() { return 0; }

			/**
			 * Return true if buffer contains a valid null-terminated string
			 */
			bool is_valid_string() const {
				return (_size < MAX_SIZE) && (_size > 0) && (_base[_size - 1] == '\0'); }

			/**
			 * Return buffer content as null-terminated string
			 *
			 * \return  pointer to null-terminated string
			 *
			 * The function returns an empty string if the buffer does not hold
			 * a valid null-terminated string. To distinguish a buffer holding
			 * an invalid string from a buffer holding a valid empty string,
			 * the function 'is_valid_string' can be used.
			 */
			char const *string() const { return is_valid_string() ? base() : ""; }
	};
	template <size_t MAX>
	class Rpc_out_buffer : public Rpc_in_buffer_base
	{
		private:

			/*
			 * This member is only there to pump up the size of the object such
			 * that 'sizeof()' returns the maximum buffer size when queried by
			 * the RPC framework.
			 */
			char _balloon[MAX];

		public:

			enum { MAX_SIZE = MAX };

			/**
			 * Construct buffer
			 */
			Rpc_out_buffer(const char *base, size_t size)
			: Rpc_in_buffer_base(base, min(size, (size_t)MAX_SIZE)) { }

			/**
			 * Construct buffer from null-terminated string
			 */
			Rpc_out_buffer(const char *str) : Rpc_in_buffer_base(str)
			{
				if (_size >= MAX_SIZE - 1)
					_size = MAX_SIZE - 1;
			}

			/**
			 * Default constructor creates invalid buffer
			 */
			Rpc_out_buffer() { }

			void operator = (Rpc_out_buffer<MAX_SIZE> const &from)
			{
				_base = from.base();
				_size = from.size();
				//PERR("%d",_base);
				for(int i=0;i<from.size();i++)
				{
					//PERR(" == Rpc out buffer %d: %d",i,from.getball()[i]);
					_balloon[i]=from.getball()[i];
				}
/*
				_balloon[from.size()] = '\0';
				_base = _balloon;
				if(from.size()==0)
				{
					PERR("0  %d",_base);
				}	
				PERR("%d",_base);
*/
			}
			bool setball(const char *str,size_t size)
			{
				for(int i=0;i<size;i++)
				{
					//PERR("set Rpc out buffer %d: %d",i,str[i]);
					_balloon[i]=str[i];
				}

			}
			const char* getball() const { return _balloon; }

			/**
			 * Return true if buffer contains a valid null-terminated string
			 */
			bool is_valid_string() const {
				return (_size < MAX_SIZE) && (_size > 0) && (_base[_size - 1] == '\0'); }

			/**
			 * Return buffer content as null-terminated string
			 *
			 * \return  pointer to null-terminated string
			 *
			 * The function returns an empty string if the buffer does not hold
			 * a valid null-terminated string. To distinguish a buffer holding
			 * an invalid string from a buffer holding a valid empty string,
			 * the function 'is_valid_string' can be used.
			 */
			char const *string() const { return is_valid_string() ? base() : ""; }
	};
}

#endif /* _INCLUDE__BASE__RPC_ARGS_H_ */
