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

#ifndef SVGPARSER_H
#define SVGPARSER_H

#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QVector>
#include <QTextStream>
#include "svgelement.h"
#include "material.h"
#include "transformationstack.h"

class SVGParser
{
    private:
        QVector<SVGElement*> mElements;
        void processElement(QXmlStreamReader* reader, QTextStream* gcode, Material* material, TransformationStack trans);
        CutSpecification* findSpecification(QXmlStreamReader* reader, Material* material);
        void configureTool(CutSpecification* spec, QTextStream* gcode);
        static QString getQString(QXmlStreamAttributes* attribs, QString attributeName);
        static QString findAttributeValue(QXmlStreamAttributes* attribs, QString attributeName);

    public:
        SVGParser();

        void process(QXmlStreamReader* reader, QTextStream* gcode, Material* material);
};

#endif // SVGPARSER_H
