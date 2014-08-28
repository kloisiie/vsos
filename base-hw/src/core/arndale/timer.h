/*
 * \brief  Timer for kernel
 * \author Martin Stein
 * \date   2012-04-23
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _ARNDALE__TIMER_H_
#define _ARNDALE__TIMER_H_

/* core includes */
#include <board.h>
#include <timer/exynos_pwm.h>

namespace Kernel
{
	/**
	 * Kernel timer
	 */
	class Timer : public Exynos_pwm::Timer
	{
		public:

			enum { IRQ = Genode::Board::PWM_IRQ_0 };

			/**
			 * Constructor
			 */
			Timer() : Exynos_pwm::Timer(Genode::Board::PWM_MMIO_BASE,
			                            Genode::Board::PWM_CLOCK) { }
	};
}

#endif /* _ARNDALE__TIMER_H_ */

