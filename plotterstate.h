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

#ifndef PLOTTERSTATE_H
#define PLOTTERSTATE_H
#include "point2d.h"

class PlotterState
{
    protected:
        static double sx;
        static double sy;
        static bool sHeadDown;
        static Point2D sMarkedPosition;
        static Point2D sControlPos; //TODO: Work out if your going to store positions as points or not, but stop doing both
        static Point2D sViewBox; // Width and Height of the view box. Defaults to 300, 300
        static Point2D sLogicalPosition;

    public:
        PlotterState();

        static Point2D getPosition();
        static void setPosition(double x, double y);
        static void setPosition(Point2D p);
        static void setLogicalPosition(double x, double y);
        static void setLogicalPosition(Point2D p);
        static bool isHeadDown();
        static void setHeadDown();
        static void setHeadUp();
        static void markPosition();
        static Point2D getMarkedPosition();
        static void setControlPosition(Point2D point);
        static Point2D getControlPosition();
        static Point2D getLogicalPosition();
        static Point2D getViewBox();
};

#endif // PLOTTERSTATE_H
