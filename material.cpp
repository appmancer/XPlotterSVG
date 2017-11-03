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

#include "material.h"
#include "tool.h"

Material::Material()
{
}

Material::~Material()
{
}

bool Material::load(QXmlStreamReader* reader)
{
    CutSpecification* currentSpec = NULL;
    while (!reader->atEnd())
    {
        QXmlStreamReader::TokenType xtype = reader->readNext();

        if (!reader->error())
        {
            if(xtype == QXmlStreamReader::StartElement)
            {
               if(QString::compare(reader->name().toString(), "Light") == 0)
               {
                   currentSpec = &mLight;
               }
               if(QString::compare(reader->name().toString(), "Medium") == 0)
               {
                   currentSpec = &mMedium;
               }
               if(QString::compare(reader->name().toString(), "Dark") == 0)
               {
                   currentSpec = &mDark;
               }
               if(QString::compare(reader->name().toString(), "Cut") == 0)
               {
                   currentSpec = &mCut;
               }
            }

            if(currentSpec != NULL)
            {
                //Read the attribs and populate currentSpec

                foreach(const QXmlStreamAttribute &attr, reader->attributes())
                {
                    QString attribute_name = attr.name().toString();
                    QString attribute_value = attr.value().toString();
                    if(QString::compare(attribute_name, "feedrate") == 0)
                    {
                        currentSpec->feedrate = attribute_value.toInt();
                    }
                    if(QString::compare(attribute_name, "power") == 0)
                    {
                        currentSpec->power = attribute_value.toInt();
                    }
                    if(QString::compare(attribute_name, "repeat") == 0)
                    {
                        currentSpec->repeat = attribute_value.toInt();
                    }
                    if(QString::compare(attribute_name, "hexcode") == 0)
                    {
                        currentSpec->hexcode = attribute_value;
                    }
                    if(QString::compare(attribute_name, "tool") == 0)
                    {
                        currentSpec->tool = attribute_value.toInt();
                    }
                }
            }

            currentSpec = NULL;
        }
    }

    return reader->hasError();
}

CutSpecification* Material::getSpecification(QString hexcode)
{
    if(QString::compare(hexcode, mLight.hexcode, Qt::CaseInsensitive) == 0)
    {
        return &mLight;
    }
    else if(QString::compare(hexcode, mMedium.hexcode, Qt::CaseInsensitive) == 0)
    {
        return &mMedium;
    }
    else if(QString::compare(hexcode, mDark.hexcode, Qt::CaseInsensitive) == 0)
    {
        return &mDark;
    }
    else if(QString::compare(hexcode, mCut.hexcode, Qt::CaseInsensitive) == 0)
    {
        return &mCut;
    }
    else
        return NULL;
}
