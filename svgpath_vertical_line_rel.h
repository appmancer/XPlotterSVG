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

#ifndef SVGPATH_VERTICAL_LINE_REL_H
#define SVGPATH_VERTICAL_LINE_REL_H

#include "svgpath_vertical_line_abs.h"

class SVGPath_Vertical_Line_Rel : public SVGPath_Vertical_Line_Abs
{
    public:
        SVGPath_Vertical_Line_Rel();
        void process(QQueue<QString>* commandQueue, QTextStream* gcode, TransformationStack trans);
};

#endif // SVGPATH_VERTICAL_LINE_REL_H
