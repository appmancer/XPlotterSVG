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

#ifndef SVGPATH_QUADRATIC_SMOOTHTO_ABS_H
#define SVGPATH_QUADRATIC_SMOOTHTO_ABS_H

#include "svgpath_quadratic_curve_abs.h"

class SVGPath_Quadratic_SmoothTo_Abs : public SVGPath_Quadratic_Curve_Abs
{
    public:
        SVGPath_Quadratic_SmoothTo_Abs();
        void process(QQueue<QString>* commandQueue, QTextStream* gcode, TransformationStack trans);
};

#endif // SVGPATH_QUADRATIC_SMOOTHTO_ABS_H
