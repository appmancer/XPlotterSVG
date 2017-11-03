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

#include <QtMath>
#include "svgcircle.h"
#include "gcodecircle.h"

//https://www.w3.org/TR/SVG/shapes.html#CircleElement

SVGCircle::SVGCircle()
{
    mElementName = "circle";
}


void SVGCircle::process(QXmlStreamReader* reader, QTextStream* gcode, TransformationStack trans)
{
    //We need to extract the start X, Y and radius attributes.
    double startX      = 0;
    double startY      = 0;
    double radius      = 0;

    QXmlStreamAttributes attribs = reader->attributes();
    startX      = SVGElement::getDouble(&attribs, "cx");
    startY      = SVGElement::getDouble(&attribs, "cy");
    radius      = SVGElement::getDouble(&attribs, "r");
    //We have all of the value from the attributes
    GCodeCircle circle(startX, startY, radius);
    circle.setTransformationStack(trans);
    *gcode << circle.toString();

}
