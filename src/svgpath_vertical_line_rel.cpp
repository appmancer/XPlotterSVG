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

#include "svgpath_vertical_line_rel.h"
#include "plotterstate.h"
#include "gcodeline.h"

SVGPath_Vertical_Line_Rel::SVGPath_Vertical_Line_Rel()
{

}

void SVGPath_Vertical_Line_Rel::process(QQueue<QString>* commandQueue, QTextStream* gcode, TransformationStack trans)
{
    //This is a relative position, so we must add a transformation of the current head position
    //IMPORTANT: This draws a horizontal line, so translate the end point, but the x position
    //           is set in SVGPath_Horizontal_Line_Abs, so set it to 0 here.

    bool isOk = true;
    double y;

    while(isOk)
    {
        //Read x value
        y = readDouble(commandQueue, &isOk);

        if(isOk)
        {
            comment("Starting vertical line", gcode);
            //Head down if necessary
            headDown(gcode);

            //Get the x value from the current state
            Point2D p = PlotterState::getLogicalPosition();

            //We have an y double from the command queue
            GCodeLine line(p.x, y + p.y);
            line.setTransformationStack(trans);
            *gcode << line.toString();
        }

        //Loop until we are not OK (end of data or found another command)
    }
}
