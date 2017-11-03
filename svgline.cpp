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

#include <QDebug>
#include "svgline.h"
#include "gcodemove.h"
#include "gcodeline.h"
#include "gcodecomment.h"

SVGLine::SVGLine()
{
    mElementName = "line";
}

void SVGLine::process(QXmlStreamReader* reader, QTextStream* gcode, TransformationStack trans)
{
    //qDebug() << "Processing line element";

    double startX    = 0;
    double startY    = 0;
    double endX      = 0;
    double endY      = 0;

    QXmlStreamAttributes attribs = reader->attributes();
    startX      = SVGElement::getDouble(&attribs, "x1");
    startY      = SVGElement::getDouble(&attribs, "y1");
    endX        = SVGElement::getDouble(&attribs, "x2");
    endY        = SVGElement::getDouble(&attribs, "y2");

    GCodeComment starting("Drawing line");
    *gcode << starting.toString();

    headUp(gcode);
    GCodeMove move(startX, startY);
    move.setTransformationStack(trans);
    *gcode << move.toString();
    headDown(gcode);
    GCodeLine line(endX, endY);
    line.setTransformationStack(trans);
    *gcode << line.toString();

}
