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

#ifndef SVGELEMENT_H
#define SVGELEMENT_H

#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QXmlStreamAttribute>
#include <QTextStream>
#include <QString>
#include "transformationstack.h"

//Abstract class for SVG parsers to build upon
class SVGElement
{
    protected:
        QString mElementName;

        static double getDouble(QXmlStreamAttributes* attribs, QString attributeName);
        static int    getInt(QXmlStreamAttributes* attribs, QString attributeName);
        static QString getQString(QXmlStreamAttributes* attribs, QString attributeName);

        static QString findAttributeValue(QXmlStreamAttributes* attribs, QString attributeName);
        void headDown(QTextStream* gcode);
        void headUp(QTextStream* gcode);


    public:
        SVGElement();
        virtual void process(QXmlStreamReader* reader, QTextStream* gcode, TransformationStack trans) = 0;
        bool acceptsElement(QStringRef elementName);
};

#endif // SVGELEMENT_H
