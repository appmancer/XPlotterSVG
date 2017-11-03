/*  XPlotterSVG - Convert SVG to GCode
    Copyright (C) 2017  Samuel Pickard

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include "transformationstack.h"
#include <QtMath>

TransformationStack::TransformationStack() : mStack()
{
}

void TransformationStack::push(double a, double b, double c, double d, double e, double f)
{
    Transformation t;
    t.a = a;
    t.b = b;
    t.c = c;
    t.d = d;
    t.e = e;
    t.f = f;

    push(t);
}

void TransformationStack::push(Transformation trans)
{
    mStack.append(trans);
}

void TransformationStack::pop()
{
    mStack.pop_back();
}

void TransformationStack::pushMatrix(Transformation trans)
{
    push(trans);
}

void TransformationStack::pushRotate(double rotation)
{
    //Convert degress to radians
    rotation = qDegreesToRadians(rotation);
    Transformation t;
    //Formatted like matrix representation
    t.a = qCos(rotation);   t.c = -qSin(rotation);   t.e = 0;
    t.b = qSin(rotation);   t.d =  qCos(rotation);   t.f = 0;

    push(t);
}

void TransformationStack::pushRotate(double rotation, double cx, double cy)
{
    //https://www.w3.org/TR/SVG/coords.html#TransformAttribute
    /* If optional parameters <cx> and <cy> are supplied,
     * the rotate is about the point (cx, cy).
     * The operation represents the equivalent of the following
     * specification: translate(<cx>, <cy>) rotate(<rotate-angle>) translate(-<cx>, -<cy>).*/
    pushTranslate(cx, cy);

    pushRotate(rotation);

    pushTranslate(-cx, -cy);
}

void TransformationStack::pushScale(double sx, double sy)
{
    Transformation t;
    t.a = sx;
    t.c = 0;
    t.e = 0;
    t.b = 0;
    t.d = sy;
    t.f = 0;

    push(t);
}

void TransformationStack::pushTranslate(double tx, double ty)
{

    Transformation t;
    t.a = 1;
    t.c = 0;
    t.e = tx;
    t.b = 0;
    t.d = 1;
    t.f = ty;

    push(t);
}

void TransformationStack::pushSkew(double sx, double sy)
{
    Transformation t;
    t.a = 1;
    t.c = qTan(sx);
    t.e = 0;
    t.b = qTan(sy);
    t.d = 1;
    t.f = 0;

    push(t);
}

Point2D TransformationStack::process(double x, double y)
{
    //Standard layout of a matrix is
    // { a, c, e }  {x}
    // { b, d, f }  {y}

    //Process each transformation in order, newest first
    // x' = a*x + c*y + e
    // y' = b*x + d*y + f

    Point2D result;
    result.x = x;
    result.y = y;
    for (int i = mStack.size(); i > 0; --i)
    {
        Transformation t = mStack.at(i - 1);
        result.x = t.a * x + t.c * y + t.e;
        result.y = t.b * x + t.d * y + t.f;

        x = result.x;
        y = result.y;
    }

    return result;
}
