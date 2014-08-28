/*
 * \brief  Nitpicker menubar interface
 * \author Norman Feske
 * \date   2006-08-22
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _MENUBAR_H_
#define _MENUBAR_H_

#include "view.h"
#include "draw_label.h"
#include "mode.h"

class Menubar : public View
{
	private:

		Canvas *_canvas;

	protected:

		Menubar(Canvas *canvas, Area size, Session *session):
			View(session, View::STAY_TOP, Rect(Point(0, 0), size)), _canvas(canvas) { }

	public:

		/**
		 * Set state that is displayed in the trusted menubar
		 */
		void state(Mode mode, const char *session_label, const char *view_title,
		           Color session_color)
		{
			/* choose base color dependent on the Nitpicker state */
			int r = (mode.kill()) ? 200 : (mode.xray()) ? session_color.r : (session_color.r + 100) >> 1;
			int g = (mode.kill()) ?  70 : (mode.xray()) ? session_color.g : (session_color.g + 100) >> 1;
			int b = (mode.kill()) ?  70 : (mode.xray()) ? session_color.b : (session_color.b + 100) >> 1;

			/* highlight first line with slightly brighter color */
			_canvas->draw_box(Rect(Point(0, 0), Area(w(), 1)),
			                  Color(r + (r / 2), g + (g / 2), b + (b / 2)));

			/* draw slightly shaded background */
			for (int i = 1; i < h() - 1; i++) {
				r -= r > 3 ? 4 : 0;
				g -= g > 3 ? 4 : 0;
				b -= b > 4 ? 4 : 0;
				_canvas->draw_box(Rect(Point(0, i), Area(w(), 1)), Color(r, g, b));
			}

			/* draw last line darker */
			_canvas->draw_box(Rect(Point(0, h() - 1), Area(w(), 1)),
			                  Color(r / 4, g / 4, b / 4));

			/* draw label */
			draw_label(_canvas, center(label_size(session_label, view_title)),
			           session_label, WHITE, view_title, session_color);
		}
};

#endif
