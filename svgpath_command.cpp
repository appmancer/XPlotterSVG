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

#include "svgpath_command.h"
#include "gcodecomment.h"
#include "gcodelaseroff.h"
#include "gcodelaseron.h"
#include "gcodependown.h"
#include "gcodepenup.h"
#include "plotterstate.h"
#include "tool.h"

SVGPath_Command::SVGPath_Command()
{

}

double SVGPath_Command::readDouble(QQueue<QString>* commandQueue, bool* ok)
{
    //Check that the queue isn't empty
    if(commandQueue->length() == 0)
    {
        *ok = false;
        return 0;
    }

    QString nextValue;

    nextValue = commandQueue->at(0);
    double retval = nextValue.toDouble(ok);
    if(*ok)
    {
        //Looks fine, we can remove it from the queue
        commandQueue->pop_front();
    }
    return retval;
}

void SVGPath_Command::comment(QString comment, QTextStream* gcode)
{
    GCodeComment c(comment);
    *gcode << c.toString();
}


void SVGPath_Command::headDown(QTextStream* gcode)
{
    //Check if we need to raise the head before the move
    if(!PlotterState::isHeadDown())
    {
        PlotterState::setHeadDown();
        if(Tool::currentTool() == Tool::TOOL_LASER)
        {
            GCodeLaserOn laser;
            *gcode << laser.toString() << endl;
        }
        else
        {
            GCodePenDown pen;
            *gcode << pen.toString() << endl;
        }
    }
}

void SVGPath_Command::headUp(QTextStream* gcode)
{
    if(PlotterState::isHeadDown())
    {
        PlotterState::setHeadUp();
        if(Tool::currentTool() == Tool::TOOL_LASER)
        {
            GCodeLaserOff laser;
            *gcode << laser.toString() << endl;
        }
        else
        {
            GCodePenUp pen;
            *gcode << pen.toString() << endl;
        }
    }
}
