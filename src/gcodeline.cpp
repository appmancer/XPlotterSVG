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

#include "gcodeline.h"
#include "plotterstate.h"
#include <QTextStream>
#include <QDebug>
#include "tool.h"

GCodeLine::GCodeLine(double x, double y) : GCodeMove(x, y)
{

}

QString GCodeLine::toString(bool translate)
{
    Point2D translatedPoint(endX, endY);
    if(translate)
    {
        //Calculate the effect any translations will have on this point
        translatedPoint = mTrans.process(endX, endY);

        //Update the physical position of the print head
        PlotterState::setPosition(translatedPoint.x, translatedPoint.y);

        //Update the logical position
        PlotterState::setLogicalPosition(endX, endY);
    }

    //We have to invert the y-coordinates to stop getting a mirror image
    double viewY = PlotterState::getViewBox().y;

    QString gcode;
    QTextStream stream(&gcode);

    stream << "G1 X";
    stream << QString::number(translatedPoint.x, 'f', 8);
    stream << " Y";
    stream << QString::number(viewY - translatedPoint.y, 'f', 8);
    //For Candle to show the rendering, we need to add a Z axis. The XPlotter ignores this value
    stream << " Z -1.000";
    stream << " F";
    stream << QString::number(Tool::getFeedrate());
    stream << endl;

    return gcode;
}

QString GCodeLine::toString()
{
    return toString(true);
}


