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

#include "svgelement.h"
#include <QtDebug>
#include "plotterstate.h"
#include "tool.h"
#include "gcodelaseroff.h"
#include "gcodelaseron.h"
#include "gcodependown.h"
#include "gcodepenup.h"

SVGElement::SVGElement()
{

}

bool SVGElement::acceptsElement(QStringRef elementName)
{
    //qDebug() << "This parser accepts " << mElementName;
    return QString::compare(elementName.toString(), mElementName, Qt::CaseInsensitive) == 0;
}

QString SVGElement::findAttributeValue(QXmlStreamAttributes* attribs, QString attributeName)
{
    QString at = "";
    foreach(const QXmlStreamAttribute &attr, *attribs)
    {
        if(QString::compare(attributeName, attr.name().toString(), Qt::CaseInsensitive) == 0)
        {
            at = attr.value().toString();
        }
    }

    return at;
}

double SVGElement::getDouble(QXmlStreamAttributes* attribs, QString attributeName)
{
    double retval = 0;
    QString attValue = SVGElement::findAttributeValue(attribs, attributeName);
    if(attValue.length() > 0)
    {
        retval = attValue.toDouble();
    }

    return retval;
}

int SVGElement::getInt(QXmlStreamAttributes* attribs, QString attributeName)
{
    double retval = 0;
    QString attValue = SVGElement::findAttributeValue(attribs, attributeName);
    if(attValue.length() > 0)
    {
        retval = attValue.toDouble();
    }

    return retval;
}

QString SVGElement::getQString(QXmlStreamAttributes* attribs, QString attributeName)
{
    return SVGElement::findAttributeValue(attribs, attributeName);
}

void SVGElement::headDown(QTextStream* gcode)
{
    //Check if we need to raise the head before the move
    if(!PlotterState::isHeadDown())
    {
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

void SVGElement::headUp(QTextStream* gcode)
{
    if(PlotterState::isHeadDown())
    {
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
