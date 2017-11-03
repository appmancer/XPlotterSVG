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

#include "svgpath_closepath.h"
#include "gcodeline.h"
#include "gcodecomment.h"
#include "plotterstate.h"

SVGPath_ClosePath::SVGPath_ClosePath()
{

}
/*
 * The closepath command takes no data, it returns the marked position in the plotter state from
 * our last move.
 * */
void SVGPath_ClosePath::process(QQueue<QString>* commandQueue, QTextStream* gcode, TransformationStack trans)
{
    comment("Closing path", gcode);

    //Head down if necessary
    headDown(gcode);

    Point2D p = PlotterState::getMarkedPosition();
    GCodeLine line(p.x, p.y);
    line.setTransformationStack(trans);
    *gcode << line.toString();

    //Any chained control point is now invalid
    Point2D empty;
    PlotterState::setControlPosition(empty);
}
