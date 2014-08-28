/*
 * \brief  Implementation of the Genode IPC-framework
 * \author Martin Stein
 * \date   2012-02-12
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/ipc.h>
#include <base/thread.h>
#include <kernel/syscalls.h>
#include <kernel/log.h>

using namespace Genode;

enum
{
	/* size of the callee-local name of a targeted RPC object */
	RPC_OBJECT_ID_SIZE = sizeof(umword_t),

	/*
	 * The RPC framework marshalls a return value into reply messages to
	 * deliver exceptions, wich occured during the RPC call to the caller.
	 * This defines the size of this value.
	 */
	RPC_RETURN_VALUE_SIZE = sizeof(umword_t),
};


/***************
 ** Utilities **
 ***************/

/**
 * Limit message size to the size of UTCB and message buffer
 */
void limit_msg_size(Msgbuf_base * const msgbuf, Native_utcb * const utcb,
                    size_t & size)
{
	if (size > utcb->size() || size > msgbuf->size()) {
		kernel_log() << __PRETTY_FUNCTION__ << ": truncate message\n";
		size = utcb->size() < msgbuf->size() ? utcb->size() : msgbuf->size();
	}
}


/**
 * Copy message payload to message buffer
 */
static void utcb_to_msgbuf(Msgbuf_base * const msgbuf, size_t size)
{
	Native_utcb * const utcb = Thread_base::myself()->utcb();
	limit_msg_size(msgbuf, utcb, size);
	memcpy(msgbuf->buf, utcb->base(), size);
}


/**
 * Copy message payload to the UTCB
 */
static void msgbuf_to_utcb(Msgbuf_base * const msgbuf, size_t size,
                           unsigned const local_name)
{
	Native_utcb * const utcb = Thread_base::myself()->utcb();
	*(unsigned *)utcb->base() = local_name;
	size += sizeof(local_name);
	limit_msg_size(msgbuf, utcb, size);
	memcpy((unsigned *)utcb->base() + 1, (unsigned *)msgbuf->buf + 1, size);
}


/*****************
 ** Ipc_ostream **
 *****************/

Ipc_ostream::Ipc_ostream(Native_capability dst, Msgbuf_base *snd_msg)
:
	Ipc_marshaller(&snd_msg->buf[0], snd_msg->size()),
	_snd_msg(snd_msg), _dst(dst)
{
	_write_offset = RPC_OBJECT_ID_SIZE;
}


/*****************
 ** Ipc_istream **
 *****************/

void Ipc_istream::_wait()
{
	/* FIXME this shall be not supported */
	Kernel::pause_thread();
}


Ipc_istream::Ipc_istream(Msgbuf_base *rcv_msg)
:
	Ipc_unmarshaller(&rcv_msg->buf[0], rcv_msg->size()),
	Native_capability(Genode::thread_get_my_native_id(), 0),
	_rcv_msg(rcv_msg), _rcv_cs(-1)
{ _read_offset = RPC_OBJECT_ID_SIZE; }


Ipc_istream::~Ipc_istream() { }


/****************
 ** Ipc_client **
 ****************/

void Ipc_client::_call()
{
	using namespace Kernel;

	/* send request and receive reply */
	msgbuf_to_utcb(_snd_msg, _write_offset, Ipc_ostream::_dst.local_name());
	size_t const s = request_and_wait(Ipc_ostream::_dst.dst(), _write_offset);
	utcb_to_msgbuf(_rcv_msg, s);

	/* reset unmarshaller */
	_write_offset = _read_offset = RPC_OBJECT_ID_SIZE;
}


Ipc_client::Ipc_client(Native_capability const &srv,
                       Msgbuf_base *snd_msg, Msgbuf_base *rcv_msg)
: Ipc_istream(rcv_msg), Ipc_ostream(srv, snd_msg), _result(0) { }


/****************
 ** Ipc_server **
 ****************/

Ipc_server::Ipc_server(Msgbuf_base *snd_msg,
                       Msgbuf_base *rcv_msg) :
	Ipc_istream(rcv_msg),
	Ipc_ostream(Native_capability(), snd_msg),
	_reply_needed(false)
{ }


void Ipc_server::_prepare_next_reply_wait()
{
	/* now we have a request to reply */
	_reply_needed = true;

	/* leave space for RPC method return value */
	_write_offset = RPC_OBJECT_ID_SIZE + RPC_RETURN_VALUE_SIZE;

	/* reset unmarshaller */
	_read_offset  = RPC_OBJECT_ID_SIZE;
}


void Ipc_server::_wait()
{
	/* receive next request */
	utcb_to_msgbuf(_rcv_msg, Kernel::wait_for_request());

	/* update server state */
	_prepare_next_reply_wait();
}


void Ipc_server::_reply() { Kernel::reply(_write_offset, 0); }


void Ipc_server::_reply_wait()
{
	/* if there is no reply simply do wait for request */
	/* FIXME this shall be not supported */
	if (!_reply_needed) {
		_wait();
		return;
	}
	/* send reply and receive next request */
	msgbuf_to_utcb(_snd_msg, _write_offset, Ipc_ostream::_dst.local_name());
	utcb_to_msgbuf(_rcv_msg, Kernel::reply(_write_offset, 1));

	/* update server state */
	_prepare_next_reply_wait();
}

