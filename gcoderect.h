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

#ifndef GCODERECT_H
#define GCODERECT_H

#include "gcodepath.h"

class GCodeRect : public GCodePath
{
    protected:
        double startX      = 0;
        double startY      = 0;
        double rectWidth   = 0;
        double rectHeight  = 0;


    public:
        GCodeRect(double x, double y, double width, double height);


        QString toString();
};

#endif // GCODERECT_H
