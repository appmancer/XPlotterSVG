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

#include "gcodeellipse.h"
#include "gcodecomment.h"
#include <QtMath>

GCodeEllipse::GCodeEllipse(double x, double y, double radiusX, double radiusY) : mX(x), mY(y), mRadiusX(radiusX), mRadiusY(radiusY)
{

}


QString GCodeEllipse::toString()
{
    //We need to convert a circle in to a series of short lines.  Some experimentation is
    //needed, but I'm going to assume that line lengths of 1mm are about right
    //Calculate the length of the circumference, and that will be the number of steps
    int steps = qFloor(2 * ((mRadiusX + mRadiusY) / 2) * M_PI);


    //Fast mid-point circle algorithms aren't going to help us here, lets do it the old-fashioned way
    //Realtime processing is not a target, the user can wait for 1 second.
    //
    double radiansPerStep = M_PI * 2 / steps;

    //Move to the zenith of the circle

    QString gcode = "";
    GCodeComment comment(QString::asprintf("Ellipse x:%.4f y:%.4f rX:%.4f rY:%.4f", mX, mY, mRadiusX, mRadiusY));
    gcode.append(comment.toString());
    gcode.append("\n");

    gcode.append(toolUp());
    gcode.append(move(mX, mY + mRadiusY));
    gcode.append(toolDown());

    double theta = 0;
    for(int i = 0; i < steps; i++)
    {
        double nx = mX + mRadiusX * qSin(theta);
        double ny = mY + mRadiusY * qCos(theta);
        gcode.append(line(nx, ny));
        theta += radiansPerStep;
    }
    //Go back to the start
    gcode.append(line(mX, mY + mRadiusY));

    gcode.append(toolUp());

    return gcode;
}
