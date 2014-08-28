/*
 * \brief  Nitpicker background
 * \author Norman Feske
 * \date   2006-06-22
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "view.h"
#include "clip_guard.h"

class Background : public Session, public View
{
	public:

		/*
		 * The background uses no texture. Therefore
		 * we can pass a null pointer as texture argument
		 * to the Session constructor.
		 */
		Background(Area size) :
			Session("", 0, 0, BLACK),
			View(this, View::BACKGROUND, Rect(Point(0, 0), size)) { }


		/********************
		 ** View interface **
		 ********************/

		int  frame_size(Mode *mode) { return 0; }
		void frame(Canvas *canvas, Mode *mode) { }

		void draw(Canvas *canvas, Mode *mode)
		{
			Clip_guard clip_guard(canvas, *this);
			canvas->draw_box(*this, Color(25, 37, 50));
		}
};

#endif
