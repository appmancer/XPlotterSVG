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

#include "svgpath.h"
#include <QDebug>
#include <QQueue>
#include "svgpath_command.h"
#include "svgpath_move_abs.h"
#include "svgpath_move_rel.h"
#include "svgpath_closepath.h"
#include "svgpath_curve_abs.h"
#include "svgpath_curve_rel.h"
#include "svgpath_elliptical_arc_abs.h"
#include "svgpath_elliptical_arc_rel.h"
#include "svgpath_horizontal_line_abs.h"
#include "svgpath_horizontal_line_rel.h"
#include "svgpath_line_abs.h"
#include "svgpath_line_rel.h"
#include "svgpath_quadratic_curve_abs.h"
#include "svgpath_quadratic_curve_rel.h"
#include "svgpath_quadratic_smoothto_abs.h"
#include "svgpath_quadratic_smoothto_rel.h"
#include "svgpath_smoothto_abs.h"
#include "svgpath_smoothto_rel.h"
#include "svgpath_vertical_line_abs.h"
#include "svgpath_vertical_line_rel.h"
#include "gcodemove.h"
#include "gcodeline.h"
#include "gcoderect.h"
#include "gcodecomment.h"
#include "plotterstate.h"

//The workhorse of SVG

//https://www.w3.org/TR/SVG/paths.html#PathElement

SVGPath::SVGPath()
{
    mElementName = "path";
}

void SVGPath::process(QXmlStreamReader* reader, QTextStream* gcode, TransformationStack trans)
{
    //a path element will typically hold a lot of movement data as a property.  Its the complete hack that holds SVG together
    //This data will contain commands and data.
    //   <path d="M 100 100 L 300 100 L 200 300 z" />

    QXmlStreamAttributes attribs = reader->attributes();
    QString pathData = SVGElement::getQString(&attribs, "d");
    pathData = pathData.simplified();

    PlotterState::setPosition(0, 0);
    PlotterState::setLogicalPosition(0, 0);

    qDebug() << "Processing path element";
    qDebug() << pathData;

    //There is a great deal of variety in formatting this data.  These string are identical in meaning
    //C100,-100
    //C 100 100
    //C100-100
    //C 100, -100

    //We will have to parse the data character by character
    QQueue<QString> commandQueue;
    int c = 0;
    QString current = "";
    QString commands = "MmCcSsZzLlHhVvQqTtAa";
    QString delimiters = " ,";
    while(c < pathData.length())
    {
        QChar nextChar = pathData.at(c++);
        if(commands.contains(nextChar))
        {
            //This is a new command
            if(current.length() > 0)
            {
                commandQueue.append(current);
                current.clear();
            }

            current.append(nextChar);
            if(!current.trimmed().isEmpty())
                commandQueue.append(current);
            current.clear();
        }
        else if(delimiters.contains(nextChar))
        {
            if(!current.trimmed().isEmpty())
                commandQueue.append(current);

            current.clear();
        }
        else if(nextChar == '-')
        {
            //Append the current string without adding nextChar
            if(current.endsWith('e'))
            {
                //We're processing an exponent, so this - is part of the current
                current.append(nextChar);
            }
            else
            {
                //We can send the current string to the queue, but we need to keep nextChar as part of the new string
                if(!current.trimmed().isEmpty())
                    commandQueue.append(current);

                current.clear();
                current.append(nextChar);
            }
        }
        else if(nextChar.isDigit() || nextChar == '.' || nextChar == 'e')
        {
            //We've got a number or a decimal point or and exponent
            current.append(nextChar);
        }
        //else ignore it and move on
    }
    if(current.length() > 0)
    {
        //One last one...
        if(!current.trimmed().isEmpty())
            commandQueue.append(current);
    }

    qDebug() << "Path has been parsed";

    //Process the queue
    QString lastCommand = "";
    while(!commandQueue.isEmpty())
    {
        //Get the next command and remove it from the queue
        QString nextCommand = commandQueue.at(0);
        commandQueue.pop_front();

        if(commands.contains(nextCommand))
        {
            lastCommand = nextCommand;
            switchCommand(&commandQueue, nextCommand, gcode, trans);
        }
        else
        {
            qDebug() << "Unrecognised command " << nextCommand;
        }
    }

    GCodeComment endComment("Path complete");
    *gcode << endComment.toString();
}

void SVGPath::switchCommand(QQueue<QString>* commandQueue, QString currentCommand, QTextStream* gcode, TransformationStack trans)
{
    if(QString::compare(currentCommand, "M") == 0)
    {
        SVGPath_Move_Abs mabs;
        mabs.process(commandQueue, gcode, trans);
    }
    else if(QString::compare(currentCommand, "m") == 0)
    {
        SVGPath_Move_Rel mrel;
        mrel.process(commandQueue, gcode, trans);
    }
    else if(QString::compare(currentCommand, "Z", Qt::CaseInsensitive) == 0)
    {
        SVGPath_ClosePath cpath;
        cpath.process(commandQueue, gcode, trans);
    }
    if(QString::compare(currentCommand, "L") == 0)
    {
        SVGPath_Line_Abs labs;
        labs.process(commandQueue, gcode, trans);
    }
    else if(QString::compare(currentCommand, "l") == 0)
    {
        SVGPath_Line_Rel lrel;
        lrel.process(commandQueue, gcode, trans);
    }
    if(QString::compare(currentCommand, "H") == 0)
    {
        SVGPath_Horizontal_Line_Abs hlabs;
        hlabs.process(commandQueue, gcode, trans);
    }
    else if(QString::compare(currentCommand, "h") == 0)
    {
        SVGPath_Horizontal_Line_Rel hlrel;
        hlrel.process(commandQueue, gcode, trans);
    }
    if(QString::compare(currentCommand, "V") == 0)
    {
        SVGPath_Vertical_Line_Abs vlabs;
        vlabs.process(commandQueue, gcode, trans);
    }
    else if(QString::compare(currentCommand, "v") == 0)
    {
        SVGPath_Vertical_Line_Rel vlrel;
        vlrel.process(commandQueue, gcode, trans);
    }
    if(QString::compare(currentCommand, "C") == 0)
    {
        SVGPath_Curve_Abs cabs;
        cabs.process(commandQueue, gcode, trans);
    }
    else if(QString::compare(currentCommand, "c") == 0)
    {
        SVGPath_Curve_Rel crel;
        crel.process(commandQueue, gcode, trans);
    }
    if(QString::compare(currentCommand, "S") == 0)
    {
        SVGPath_SmoothTo_Abs sabs;
        sabs.process(commandQueue, gcode, trans);
    }
    else if(QString::compare(currentCommand, "s") == 0)
    {
        SVGPath_SmoothTo_Rel srel;
        srel.process(commandQueue, gcode, trans);
    }
    if(QString::compare(currentCommand, "Q") == 0)
    {
        SVGPath_Quadratic_Curve_Abs qcabs;
        qcabs.process(commandQueue, gcode, trans);
    }
    else if(QString::compare(currentCommand, "q") == 0)
    {
        SVGPath_Quadratic_Curve_Rel qcrel;
        qcrel.process(commandQueue, gcode, trans);
    }
    if(QString::compare(currentCommand, "T") == 0)
    {
        SVGPath_Quadratic_SmoothTo_Abs qsabs;
        qsabs.process(commandQueue, gcode, trans);
    }
    else if(QString::compare(currentCommand, "t") == 0)
    {
        SVGPath_Quadratic_SmoothTo_Rel qsrel;
        qsrel.process(commandQueue, gcode, trans);
    }
    if(QString::compare(currentCommand, "A") == 0)
    {
        SVGPath_Elliptical_Arc_Abs eaabs;
        eaabs.process(commandQueue, gcode, trans);
    }
    else if(QString::compare(currentCommand, "a") == 0)
    {
        SVGPath_Elliptical_Arc_Rel earel;
        earel.process(commandQueue, gcode, trans);
    }
}

double SVGPath::readDouble(QQueue<QString>* commandQueue, bool* ok)
{
    QString nextValue;
    nextValue = commandQueue->at(0);
    double retval = nextValue.toDouble(ok);
    if(*ok)
    {
        //Looks fine, we can remove it from the queue
        commandQueue->pop_front();
    }
    return retval;
}
