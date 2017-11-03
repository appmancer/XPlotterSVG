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

#include "svgpath_vertical_line_abs.h"
#include "gcodeline.h"
#include "gcodependown.h"
#include "gcodelaseron.h"
#include "plotterstate.h"
#include "tool.h"

SVGPath_Vertical_Line_Abs::SVGPath_Vertical_Line_Abs()
{

}

/*
 * SVG path commands are valid until you run out of data, so keep processing values until
 * you hit the next command or run out of data
 * */
void SVGPath_Vertical_Line_Abs::process(QQueue<QString>* commandQueue, QTextStream* gcode, TransformationStack trans)
{
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
            GCodeLine line(p.x, y);
            line.setTransformationStack(trans);
            *gcode << line.toString();
        }

        //Loop until we are not OK (end of data or found another command)
    }
}
