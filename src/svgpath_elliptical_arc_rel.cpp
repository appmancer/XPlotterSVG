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

#include "svgpath_elliptical_arc_rel.h"
#include "plotterstate.h"
#include "gcodeline.h"

SVGPath_Elliptical_Arc_Rel::SVGPath_Elliptical_Arc_Rel()
{
}


void SVGPath_Elliptical_Arc_Rel::process(QQueue<QString> *commandQueue, QTextStream *gcode, TransformationStack trans)
{
    mGCode = gcode;
    mTrans = &trans;
    // SVG Params
    //(rx ry x-axis-rotation large-arc-flag sweep-flag x y)+
    bool isOk = true;
    double rx;
    double ry;
    double xAxisRotation;
    double largeArcFlag;
    double sweepFlag;

    //These are endpoint of the ellipse, and come from data
    double x;
    double y;

    //This is a relative position, so we must add a transformation of the current head position
    Point2D startPoint = PlotterState::getLogicalPosition();


    while(isOk)
    {
        //Read x value
        rx = readDouble(commandQueue, &isOk);
        ry = readDouble(commandQueue, &isOk);
        xAxisRotation = readDouble(commandQueue, &isOk);
        largeArcFlag = readDouble(commandQueue, &isOk);
        sweepFlag = readDouble(commandQueue, &isOk);
        x = readDouble(commandQueue, &isOk);
        y = readDouble(commandQueue, &isOk);

        if(isOk)
        {
            //Radius should not be negative, but apparently often are
            rx = qAbs(rx);
            ry = qAbs(ry);

            //If either radius is 0, then we should just draw a straight line
            if(rx == 0 || ry == 0)
            {
                GCodeLine line(x, y);
                line.setTransformationStack(trans);
                *gcode << line.toString();

                //and exit
                isOk = false;
                break;
            }
        }

        if(isOk)
        {
            comment("Starting elliptical arc", gcode);
            startPoint = PlotterState::getLogicalPosition();
            drawArc(rx, ry, xAxisRotation, largeArcFlag, sweepFlag, x + startPoint.x, y + startPoint.y);
        }
    }
}
