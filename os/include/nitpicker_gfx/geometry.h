/*
 * \brief  Geometric primitives
 * \author Norman Feske
 * \date   2006-08-05
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__NITPICKER_GFX__GEOMETRY_H_
#define _INCLUDE__NITPICKER_GFX__GEOMETRY_H_

#include "miscmath.h"

class Point
{
	private:

		int _x, _y;

	public:

		Point(int x, int y): _x(x), _y(y) { }
		Point(): _x(0), _y(0) { }

		int x() const { return _x; }
		int y() const { return _y; }

		/**
		 * Operator for adding points
		 */
		Point operator + (Point p) { return Point(_x + p.x(), _y + p.y()); }

		/**
		 * Operator for subtracting points
		 */
		Point operator - (Point p) { return Point(_x - p.x(), _y - p.y()); }

		/**
		 * Operator for testing non-equality of two points
		 */
		bool operator != (Point p) { return p.x() != _x || p.y() != _y; }
};


class Area
{
	private:

		int _w, _h;

	public:

		Area(int w, int h): _w(w), _h(h) { }
		Area(): _w(0), _h(0) { }

		int w() const { return _w; }
		int h() const { return _h; }

		bool valid() const { return _w > 0 && _h > 0; }

		int num_pixels() const { return _w*_h; }
};


/*
 * A valid rectangle consists of two points wheras point 2 has
 * higher or equal coordinates than point 1. All other cases
 * are threated as invalid rectangles.
 */
class Rect
{
	private:

		Point _p1, _p2;

	public:

		/**
		 * Constructors
		 */
		Rect(Point p1, Point p2): _p1(p1), _p2(p2) { }

		Rect(Point p, Area a):
			_p1(p), _p2(p.x() + a.w() - 1, p.y() + a.h() - 1) { }

		Rect() { }

		/**
		 * Assign new coordinates
		 */
		void rect(Rect r) { _p1 = r.p1(); _p2 = r.p2(); }

		/**
		 * Accessors
		 */
		int   x1()   const { return _p1.x(); }
		int   y1()   const { return _p1.y(); }
		int   x2()   const { return _p2.x(); }
		int   y2()   const { return _p2.y(); }
		int   w()    const { return _p2.x() - _p1.x() + 1; }
		int   h()    const { return _p2.y() - _p1.y() + 1; }
		Point p1()   const { return _p1; }
		Point p2()   const { return _p2; }
		Area  area() const { return Area(w(), h()); }

		/**
		 * Return true if rectangle area is greater than zero
		 */
		bool valid() const { return _p1.x() <= _p2.x() && _p1.y() <= _p2.y(); }

		/**
		 * Return true if area fits in rectangle
		 */
		bool fits(Area area) const { return w() >= area.w() && h() >= area.h(); }

		/**
		 * Create new rectangle by intersecting two rectangles
		 */
		static Rect intersect(Rect r1, Rect r2) {
			return Rect(Point(max(r1.x1(), r2.x1()), max(r1.y1(), r2.y1())),
			            Point(min(r1.x2(), r2.x2()), min(r1.y2(), r2.y2()))); }

		/**
		 * Compute compounding rectangle of two rectangles
		 */
		static Rect compound(Rect r1, Rect r2) {
			return Rect(Point(min(r1.x1(), r2.x1()), min(r1.y1(), r2.y1())),
			            Point(max(r1.x2(), r2.x2()), max(r1.y2(), r2.y2()))); }

		/**
		 * Cut out rectangle from rectangle
		 *
		 * \param r  rectangle to cut out
		 *
		 * In the worst case (if we cut a hole into the rectangle) we get
		 * four valid resulting rectangles.
		 */
		void cut(Rect r, Rect *top, Rect *left, Rect *right, Rect *bottom)
		{
			/* limit the cut-out area to the actual rectangle */
			r = intersect(r, *this);

			*top    = Rect(Point(x1(), y1()),         Point(x2(), r.y1() - 1));
			*left   = Rect(Point(x1(), r.y1()),       Point(r.x1() - 1, r.y2()));
			*right  = Rect(Point(r.x2() + 1, r.y1()), Point(x2(), r.y2()));
			*bottom = Rect(Point(x1(), r.y2() + 1),   Point(x2(), y2()));
		}

		/**
		 * Return position of an area when centered within the rectangle
		 */
		Point center(Area area) {
			return Point((w() - area.w())/2, (h() - area.h())/2) + p1(); }
};

#endif /* _INCLUDE__NITPICKER_GFX__GEOMETRY_H_ */
