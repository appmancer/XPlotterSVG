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

#include "svgpath_move_rel.h"
#include "plotterstate.h"
#include "gcodemove.h"
#include "gcodeline.h"

SVGPath_Move_Rel::SVGPath_Move_Rel()
{

}

/*
 * SVG path commands are valid until you run out of data, so keep processing pairs of doubles until
 * you hit the next command or run out of data
 * */
void SVGPath_Move_Rel::process(QQueue<QString>* commandQueue, QTextStream* gcode, TransformationStack trans)
{

    //We have an X and Y double from the command queue
    //We have to add the current position as a transformation
    bool isFirst = true;
    bool isOk = true;
    double y;
    double x;
    while(isOk)
    {
        //Read x value
        x = readDouble(commandQueue, &isOk);
        if(isOk)
        {
            y = readDouble(commandQueue, &isOk);
        }

        if(isOk)
        {
            Point2D p = PlotterState::getLogicalPosition();

            //VERY ANNOYING
            //*sometimes* path move commands are actually lineto!
            if(isFirst)
            {
                //Head up if necessary
                headUp(gcode);

                //We have an X and Y double from the command queue
                GCodeMove move(x + p.x, y + p.y);
                move.setTransformationStack(trans);

                *gcode << move.toString();
                isFirst = false;
            }
            else
            {
                //Head up if necessary
                headDown(gcode);

                //We have an X and Y double from the command queue
                GCodeLine line(x + p.x, y + p.y);
                line.setTransformationStack(trans);

                *gcode << line.toString();

            }
        }
        //Loop until we are not OK (end of data or found another command)
    }
}
