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

#include "gcoderect.h"
#include <QTextStream>

GCodeRect::GCodeRect(double x, double y, double width, double height) : startX(x), startY(y), rectWidth(width), rectHeight(height)
{
}

QString GCodeRect::toString()
{
    //Rect:
    //  1.  Tool up
    //  2.  Move to start location
    //  3.  Tool down
    //  4.  Move to x + width, y
    //  5.  Move to x + width, y + height
    //  6.  Move to x, y + height
    //  7.  Move to x, y
    //  8.  Tool up
    QString gcode = "";

    QTextStream stream(&gcode);
    stream << toolUp();
    stream << move(startX, startY);
    stream << toolDown();
    stream << line(startX + rectWidth, startY);
    stream << line(startX + rectWidth, startY + rectHeight);
    stream << line(startX, startY + rectHeight);
    stream << line(startX, startY);
    stream << toolUp();
    return gcode;

}

