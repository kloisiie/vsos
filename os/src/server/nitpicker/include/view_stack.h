/*
 * \brief  Nitpicker view stack
 * \author Norman Feske
 * \date   2006-08-08
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _VIEW_STACK_H_
#define _VIEW_STACK_H_

#include "view.h"

class Session;

class View_stack
{
	private:

		Canvas                *_canvas;
		Mode                  *_mode;
		List<View_stack_elem>  _views;
		View                  *_default_background;

		/**
		 * Return outline geometry of a view
		 *
		 * This geometry depends on the view geometry and the currently
		 * active Nitpicker mode. In non-flat modes, we incorporate the
		 * surrounding frame.
		 */
		Rect _outline(View *view);

		/**
		 * Return top-most view of the view stack
		 */
		View *_first_view() { return static_cast<View *>(_views.first()); }

		/**
		 * Find position in view stack for inserting a view
		 */
		View *_target_stack_position(View *neighbor, bool behind);

		/**
		 * Find best visible label position
		 */
		void _optimize_label_rec(View *cv, View *lv, Rect rect, Rect *optimal);

		/**
		 * Position labels that are affected by specified area
		 */
		void _place_labels(Rect);

		/**
		 * Return view following the specified view in the view stack
		 */
		View *_next_view(View *view);

	public:

		/**
		 * Constructor
		 */
		View_stack(Canvas *canvas, Mode *mode) :
			_canvas(canvas), _mode(mode), _default_background(0) { }

		/**
		 * Return size
		 */
		Area size() { return _canvas->size(); }

		/**
		 * Draw views in specified area (recursivly)
		 *
		 * \param view      current view in view stack
		 * \param dst_view  desired view to draw or NULL
		 *                  if all views should be drawn
		 * \param exclude   do not draw views of this session
		 */
		void draw_rec(View *view, View *dst_view, Session *exclude, Rect);

		/**
		 * Draw whole view stack
		 */
		void update_all_views()
		{
			_place_labels(Rect(Point(), _canvas->size()));
			draw_rec(_first_view(), 0, 0, Rect(Point(), _canvas->size()));
		}

		/**
		 * Update all views belonging to the specified session
		 *
		 * \param Session *  Session that created the view
		 * \param Rect       Buffer area to update
		 *
		 * Note: For now, we perform an independent view-stack traversal
		 *       for each view when calling 'refresh_view'. This becomes
		 *       a potentially high overhead with many views. Having
		 *       a tailored 'draw_rec_session' function would overcome
		 *       this problem.
		 */
		void update_session_views(Session *session, Rect rect)
		{
			for (View *view = _first_view(); view; view = view->view_stack_next()) {

				if (view->session() != session)
					continue;

				/*
				 * Determine view portion that displays the buffer portion
				 * specified by 'rect'.
				 */
				Point offset = view->p1() + view->buffer_off();
				Rect r = Rect::intersect(Rect(rect.p1() + offset,
				                              rect.p2() + offset), *view);
				refresh_view(view, view, r);
			}
		}

		/**
		 * Refresh area within a view
		 *
		 * \param view  view that should be updated on screen
		 * \param dst   NULL if all views in the specified area should be
		 *              refreshed or 'view' if the refresh should be limited to
		 *              the specified view.
		 */
		void refresh_view(View *view, View *dst, Rect);

		/**
		 * Define position and viewport
		 *
		 * \param pos         position of view on screen
		 * \param buffer_off  view offset of displayed buffer
		 * \param do_redraw   perform screen update immediately
		 */
		void viewport(View *view, Rect pos, Point buffer_off, bool do_redraw);

		/**
		 * Insert view at specified position in view stack
		 *
		 * \param behind  insert view in front (true) or
		 *                behind (false) the specified neighbor
		 *
		 * To insert a view at the top of the view stack, specify
		 * neighbor = 0 and behind = true. To insert a view at the
		 * bottom of the view stack, specify neighbor = 0 and
		 * behind = false.
		 */
		void stack(View *view, View *neighbor = 0, bool behind    = true,
		                                           bool do_redraw = true);

		/**
		 * Set view title
		 */
		void title(View *view, const char *title);

		/**
		 * Find view at specified position
		 */
		View *find_view(Point);

		/**
		 * Remove view from view stack
		 */
		void remove_view(View *);

		/**
		 * Define default background
		 */
		void default_background(View *view) { _default_background = view; }

		/**
		 * Return true if view is the default background
		 */
		bool is_default_background(View *view) { return view == _default_background; }

		/**
		 * Remove all views of specified session from view stack
		 *
		 * Rather than removing the views from the view stack, this function moves
		 * the session views out of the visible screen area.
		 */
		void lock_out_session(Session *session)
		{
			View *view = _first_view(), *next_view = view->view_stack_next();
			while (view) {
				if (view->session() == session) remove_view(view);
				view = next_view;
				next_view = view ? view->view_stack_next() : 0;
			}
		}
};

#endif
