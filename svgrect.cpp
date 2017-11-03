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

#include "svgrect.h"
#include <QDebug>
#include "gcoderect.h"

//For reference, see https://www.w3.org/TR/SVG/shapes.html#RectElement

SVGRect::SVGRect()
{
    mElementName = "rect";
}

void SVGRect::process(QXmlStreamReader* reader, QTextStream* gcode, TransformationStack trans)
{
    // qDebug() << "Processing rect element";

    //We need to extract the start X, Y and width and height attributes.

    double startX      = 0;
    double startY      = 0;
    double rectWidth   = 0;
    double rectHeight  = 0;
    double rx          = 0;
    double ry          = 0;

    QXmlStreamAttributes attribs = reader->attributes();
    startX      = SVGElement::getDouble(&attribs, "x");
    startY      = SVGElement::getDouble(&attribs, "y");
    rectWidth   = SVGElement::getDouble(&attribs, "width");
    rectHeight  = SVGElement::getDouble(&attribs, "height");
    rx          = SVGElement::getDouble(&attribs, "rx");
    ry          = SVGElement::getDouble(&attribs, "ry");

    //We have all of the value from the attributes
    GCodeRect rect(startX, startY, rectWidth, rectHeight);
    rect.setTransformationStack(trans);
    *gcode << rect.toString();
}
