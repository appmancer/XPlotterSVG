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

#include "plotterstate.h"

double PlotterState::sx = 0;
double PlotterState::sy = 0;
bool PlotterState::sHeadDown = false;
Point2D PlotterState::sMarkedPosition(0, 0);
Point2D PlotterState::sControlPos(0, 0);
Point2D PlotterState::sViewBox(300, 300);
Point2D PlotterState::sLogicalPosition(0, 0);

PlotterState::PlotterState()
{
}

Point2D PlotterState::getPosition()
{
    return Point2D { sx, sy };
}

void PlotterState::PlotterState::setLogicalPosition(double x, double y)
{
    sLogicalPosition.x = x;
    sLogicalPosition.y = y;
}

void PlotterState::setLogicalPosition(Point2D p)
{
    sLogicalPosition = p;
}

Point2D PlotterState::getLogicalPosition()
{
    return sLogicalPosition;
}

void PlotterState::PlotterState::setPosition(double x, double y)
{
    sx = x;
    sy = y;
}

void PlotterState::setPosition(Point2D p)
{
    sx = p.x;
    sy = p.y;
}

void PlotterState::setHeadDown()
{
    sHeadDown = true;
}

void PlotterState::setHeadUp()
{
    sHeadDown = false;
}

bool PlotterState::isHeadDown()
{
    return sHeadDown;
}

void PlotterState::markPosition()
{
    sMarkedPosition.x = sLogicalPosition.x;
    sMarkedPosition.y = sLogicalPosition.y;
}

Point2D PlotterState::getMarkedPosition()
{
    return sMarkedPosition;
}

void PlotterState::setControlPosition(Point2D point)
{
    sControlPos = point;
}

Point2D PlotterState::getControlPosition()
{
    return sControlPos;
}

Point2D PlotterState::getViewBox()
{
    return sViewBox;
}
