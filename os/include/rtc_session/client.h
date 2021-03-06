/*
 * \brief  Client-side RTC session interface
 * \author Markus Partheymueller
 * \date   2012-11-15
 */

/*
 * Copyright (C) 2012 Intel Corporation
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__RTC_SESSION__CLIENT_H_
#define _INCLUDE__RTC_SESSION__CLIENT_H_

#include <rtc_session/rtc_session.h>
#include <base/rpc_client.h>
#include <base/snprintf.h>
namespace Rtc {

	struct Session_client : Genode::Rpc_client<Session>
	{
		Session_client(Genode::Capability<Session> cap)
		: Genode::Rpc_client<Session>(cap) {}

		Genode::uint64_t get_current_time(int& year,int& mon,int& day,int& hour,int& min,int& sec,bool bigInt=false)
		{
			return call<Rpc_get_current_time>(year,mon,day,hour,min,sec,bigInt);
		}
		void get_current_time_string(char *buf){
			int year,mon,day,hour,min,sec;
			get_current_time(year,mon,day,hour,min,sec);
			Genode::snprintf(buf,20,"%04d.%02d.%02d %02d:%02d:%02d", year, mon, day,hour, min, sec);
		}
	};
}

#endif /* _INCLUDE__RTC_SESSION__CLIENT_H_ */
