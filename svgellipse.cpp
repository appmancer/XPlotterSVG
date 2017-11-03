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

#include "svgellipse.h"
#include "gcodeellipse.h"

// https://www.w3.org/TR/SVG/shapes.html#EllipseElement

SVGEllipse::SVGEllipse()
{
    mElementName = "ellipse";
}

void SVGEllipse::process(QXmlStreamReader* reader, QTextStream* gcode, TransformationStack trans)
{
    //We need to extract the start X, Y and radius attributes.
    double startX      = 0;
    double startY      = 0;
    double radiusX     = 0;
    double radiusY     = 0;

    QXmlStreamAttributes attribs = reader->attributes();
    startX      = SVGElement::getDouble(&attribs, "cx");
    startY      = SVGElement::getDouble(&attribs, "cy");
    radiusX     = SVGElement::getDouble(&attribs, "rx");
    radiusY     = SVGElement::getDouble(&attribs, "ry");
    //We have all of the value from the attributes

    GCodeEllipse ellipse(startX, startY, radiusX, radiusY);
    ellipse.setTransformationStack(trans);

    *gcode << ellipse.toString();
}

