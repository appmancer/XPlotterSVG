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

#include "svgparser.h"
#include <QtDebug>
#include "svgrect.h"
#include "svgpath.h"
#include "svgcircle.h"
#include "svgline.h"
#include "svgellipse.h"
#include "svgpolygon.h"
#include "svgpolyline.h"
#include "svgdesc.h"
#include "svggroup.h"
#include "gcodefeed.h"
#include "gcodecomment.h"
#include "tool.h"
#include "transformationparser.h"
#include "plotterstate.h"

SVGParser::SVGParser()
{
    //Create command parsers
    mElements.append(new SVGRect());
    mElements.append(new SVGPath());
    mElements.append(new SVGCircle());
    mElements.append(new SVGLine());
    mElements.append(new SVGEllipse());
    mElements.append(new SVGPolygon());
    mElements.append(new SVGPolyline());
    mElements.append(new SVGDesc());
    mElements.append(new SVGGroup());
}
//TODO:  Destroy all the SVG parsers cleanly in the destructor

void SVGParser::process(QXmlStreamReader* reader, QTextStream* gcode, Material* material)
{
    TransformationStack trans;
    //trans.pushTranslate(0, 3);

    //Standard header
    *gcode << "M0" << endl; //Disable screen
    *gcode << "G21" << endl; //Units in mm
    *gcode << "G28" <<  endl; //Reset to origin
    *gcode << "G90" << endl; //all positions are absolute

    while (!reader->atEnd())
    {
        QXmlStreamReader::TokenType xtype = reader->readNext();

        if (!reader->error())
        {
            if(xtype == QXmlStreamReader::StartElement)
            {
                processElement(reader, gcode, material, trans);
            }
        }
    }
    *gcode << "M1" << endl; //Enable screen
}

//Process a single element. When we find a StartElement, recursively call this function
void SVGParser::processElement(QXmlStreamReader* reader, QTextStream* gcode, Material* material, TransformationStack trans)
{
    while (!reader->atEnd())
    {
        if (!reader->error())
        {
            //qDebug() << "Found XML Element " << reader->name().toString();
            QXmlStreamAttributes attribs = reader->attributes();

            CutSpecification* spec = findSpecification(reader, material);

            //Get any transformations that need to applied to this path
            QString transformationData = getQString(&attribs, "transform");
            if(transformationData.simplified().length() > 0)
            {
                TransformationParser tparser;
                trans = tparser.process(transformationData, trans);
            }

            //Use the appropriate parser
            if(spec != NULL)
            {
                for (int i = 0; i < mElements.size(); i++)
                {
                    // qDebug() << "Testing parser for element " << reader->name();
                    if(mElements.at(i)->acceptsElement(reader->name()))
                    {
                       // qDebug() << "Parser accepts element";

                        configureTool(spec, gcode);

                        // Repeat as required by the material
                        for(int c=0; c < spec->repeat; c++)
                        {
                            mElements.at(i)->process(reader, gcode, trans);
                        }
                    }
                }
            }
        }
        //Read next at end of loop
        QXmlStreamReader::TokenType xtype = reader->readNext();

        //If thie next element is child of this one, then recurse
        //with a copy of the transformation stack
        if(xtype == QXmlStreamReader::StartElement)
        {
            //qDebug() << "Found a start element, recursing!";
            processElement(reader, gcode, material, trans);
        }
        else if(xtype == QXmlStreamReader::EndElement)
        {
            //Reached the end of this element, recurse back up
            //qDebug() << "end " << reader->name() << ": " << reader->text();
            return;
        }
    }
}

CutSpecification* SVGParser::findSpecification(QXmlStreamReader* reader, Material* material)
{
    //Setup for the material for this element.  Get the stroke property of this element.
    //If no matching stroke hex value can be matched, then don't process this element
    CutSpecification* spec = NULL;
    foreach(const QXmlStreamAttribute &attr, reader->attributes())
    {
        //qDebug() << "attr name: " << attr.name().toString() << " : " << attr.value().toString();
        if(QString::compare(attr.name().toString(), "stroke", Qt::CaseInsensitive) == 0)
        {
            spec = material->getSpecification(attr.value().toString());
            break;
        }
        //TODO:: Style tag processing needs its own class.  It needs to handle transformation as wel
        //       as style tags
        //Extract stroke colour from style tags as well
        else if(QString::compare(attr.name().toString(), "style", Qt::CaseInsensitive) == 0)
        {
            QString pair;
            QStringList pairs = attr.value().toString().simplified().split(";", QString::SkipEmptyParts, Qt::CaseInsensitive);
            foreach(pair, pairs)
            {
                QStringList nvp = pair.split(":", QString::KeepEmptyParts, Qt::CaseInsensitive);
                if(QString::compare(nvp.at(0), "stroke", Qt::CaseInsensitive) == 0)
                {
                    spec = material->getSpecification(nvp.at(1));
                    break;
                }
            }
        }
    }
    return spec;
}

void SVGParser::configureTool(CutSpecification* spec, QTextStream* gcode)
{
    //Set the feedrate
    GCodeComment comment("Setting feed rate");
    *gcode << comment.toString();

    GCodeFeed feed(spec->feedrate);
    *gcode << feed.toString();
    Tool::setFeedrate(spec->feedrate);

    //Set the tool
    Tool::setTool(spec->tool);

    //Set the power (laser only)
    Tool::setPower(spec->power);
}

//TODO: This code is duplicated in SVGElement
QString SVGParser::getQString(QXmlStreamAttributes* attribs, QString attributeName)
{
    return SVGParser::findAttributeValue(attribs, attributeName);
}
QString SVGParser::findAttributeValue(QXmlStreamAttributes* attribs, QString attributeName)
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
