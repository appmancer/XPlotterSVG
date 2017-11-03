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

#include "gcodepath.h"
#include "tool.h"
#include "gcodepenup.h"
#include "gcodependown.h"
#include "gcodelaseron.h"
#include "gcodelaseroff.h"
#include "gcodemove.h"
#include "gcodeline.h"

GCodePath::GCodePath()
{

}

//TODO::  I've clearly had some hesitation to whether this logic belongs in GCode parsing or SVG parsing.

QString GCodePath::toolUp()
{
    QString tool;
    switch(Tool::currentTool())
    {
        case Tool::TOOL_PEN:
        {
            GCodePenUp pen;
            tool = pen.toString();
        }
        break;

        case Tool::TOOL_LASER:
        {
            GCodeLaserOn laser;
            tool = laser.toString();
        }
        break;
    }
    return tool.append("\n");
}

QString GCodePath::toolDown()
{
    QString tool;
    switch(Tool::currentTool())
    {
        case Tool::TOOL_PEN:
        {
            GCodePenDown pen;
            tool = pen.toString();
        }
        break;

        case Tool::TOOL_LASER:
        {
            GCodeLaserOff laser;
            tool = laser.toString();
        }
        break;
    }
    return tool.append("\n");
}

QString GCodePath::move(double x, double y)
{
    GCodeMove gcmove(x, y);
    gcmove.setTransformationStack(mTrans);
    return gcmove.toString();
}

QString GCodePath::line(double x, double y)
{
    GCodeLine gcline(x, y);
    gcline.setTransformationStack(mTrans);
    return gcline.toString();
}
