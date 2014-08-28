/*
 * \brief  eMMC driver for Arndale/Exynos5 platform
 * \author Sebastian Sumpf
 * \date   2013-03-06
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/sleep.h>
#include <base/printf.h>
#include <cap_session/connection.h>

/* local includes */
#include <driver.h>


int main(int argc, char **argv)
{
	using namespace Genode;

	printf("--- Arndale eMMC card driver ---\n");

	/**
	 * Factory used by 'Block::Root' at session creation/destruction time
	 */
	struct Driver_factory : Block::Driver_factory
	{
		Block::Driver *create()
		{
			bool use_dma = true;
			return new (env()->heap()) Block::Exynos5_driver(use_dma);
		}

		void destroy(Block::Driver *driver)
		{
			Genode::destroy(env()->heap(),
			                static_cast<Block::Exynos5_driver *>(driver));
		}

	} driver_factory;

	enum { STACK_SIZE = 8192 };
	static Cap_connection cap;
	static Rpc_entrypoint ep(&cap, STACK_SIZE, "block_ep");

	static Block::Root block_root(&ep, env()->heap(), driver_factory);
	env()->parent()->announce(ep.manage(&block_root));

	sleep_forever();
	return 0;
}
