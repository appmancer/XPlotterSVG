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

#include "svgpath_horizontal_line_rel.h"
#include "plotterstate.h"
#include "gcodeline.h"

SVGPath_Horizontal_Line_Rel::SVGPath_Horizontal_Line_Rel()
{

}

void SVGPath_Horizontal_Line_Rel::process(QQueue<QString>* commandQueue, QTextStream* gcode, TransformationStack trans)
{
    bool isOk = true;
     double x;

     comment("Horizontal line", gcode);
     //Head down if necessary
     headDown(gcode);

     while(isOk)
     {
         //Read x value
         x = readDouble(commandQueue, &isOk);

         if(isOk)
         {
             //Get the y value from the current state
             Point2D p = PlotterState::getLogicalPosition();

             //We have an X double from the command queue
             GCodeLine line(x + p.x, p.y);
             line.setTransformationStack(trans);
             *gcode << line.toString();
         }

         //Loop until we are not OK (end of data or found another command)
     }

}
