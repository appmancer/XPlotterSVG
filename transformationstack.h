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

#ifndef TRANSFORMATIONSTACK_H
#define TRANSFORMATIONSTACK_H

/* This class is the transformation stack.  It will resolve affine transformations into absolute positions
 * It is a stack, so push a new Transformation onto it, and pop it off when you have finished.
 * All transformations in the stack are processed
 * Everything I know about affine transformations I learned from
 * https://stackoverflow.com/questions/14684846/flattening-svg-matrix-transforms-in-inkscape
 * https://stackoverflow.com/questions/15133977/how-to-calculate-svg-transform-matrix-from-rotate-translate-scale-values
 * http://www.useragentman.com/blog/2011/01/07/css3-matrix-transform-for-the-mathematically-challenged/
 * https://math.stackexchange.com/questions/132402/what-do-the-parameters-skewx-and-skewy-mean-in-the-transform-specified-by-flash
*/
#include <QVector>
#include "point2d.h"

struct Transformation
{
    double a;
    double b;
    double c;
    double d;
    double e;
    double f;
};

class TransformationStack
{
    protected:
        QVector<Transformation> mStack;

public:
        TransformationStack();

        void push(Transformation trans);
        void push(double a, double b, double c, double d, double e, double f);
        void pop();

        void pushMatrix(Transformation trans);
        void pushRotate(double rotation);
        void pushRotate(double rotation, double cx, double cy);
        void pushScale(double sx, double sy);
        void pushTranslate(double tx, double ty);
        void pushSkew(double sx, double sy);

        Point2D process(double x, double y);
};

#endif // TRANSFORMATIONSTACK_H
