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

#include "transformationparser.h"
#include <QDebug>
#include "plotterstate.h"
TransformationParser::TransformationParser()
{

}

TransformationStack TransformationParser::process(const QString input, const TransformationStack trans)
{
    //Take a copy of the input stack
    TransformationStack newStack = trans;

    QVector<QString> tforms;
    QString current = "";
    int c = 0;
    while(c < input.length())
    {
        QChar nextChar = input.at(c++);
        current.append(nextChar);
        if(nextChar == ')')
        {
            //Add this to the current and push to the vector
            tforms.append(current.simplified());
            current.clear();
        }
    }

    //Each transformation has a name, and then 1 or more parameters, separated by commas
    QString tform;
    foreach(tform, tforms)
    {
        QString command;
        QVector<double> params;
        c = 0;
        QString buffer;
        while(c < tform.length())
        {
            QChar nextChar = tform.at(c++);
            if(nextChar == '(')
            {
                //We've got all the command data
                command = buffer;
                buffer.clear();
            }
            else if(nextChar == ')' || nextChar == ' ' || nextChar == ',' )
            {
                //End of parameter
                params.append(buffer.toDouble());
                buffer.clear();
            }
            else
            {
                //add to the buffer
                buffer.append(nextChar);
            }
        }

        //We've got a complete transformation now
        // qDebug() << "Found transformation: " << command;

        //TODO:: Obvs make this a class.  So tired.
        if(QString::compare(command, "rotate") == 0)
        {
            //We should have 1,2 or 3 values
            switch(params.length())
            {
                case 1:
                {
                    newStack.pushRotate(params[0]);
                }
                break;
                case 2:
                    newStack.pushRotate(params[0], params[1], params[1]);
                break;
                case 3:
                    newStack.pushRotate(params[0], params[1], params[2]);
                break;
            }
        }
        else if(QString::compare(command, "translate") == 0)
        {
            //We should have 1 or 2 params
            switch(params.length())
            {
                case 1:
                    newStack.pushTranslate(params[0], 0);
                break;
                case 2:
                    newStack.pushTranslate(params[0], params[1]);
                break;
            }
        }
        else if(QString::compare(command, "scale") == 0)
        {
            //We should have 1 or 2 params
            switch(params.length())
            {
                case 1:
                    newStack.pushScale(params[0], params[0]);
                break;
                case 2:
                    newStack.pushScale(params[0], params[1]);
                break;
            }
        }
        else if(QString::compare(command, "skewX") == 0)
        {
            if(params.length() == 1)
            {
                newStack.pushSkew(params[0], 0);
            }
        }
        else if(QString::compare(command, "skewY") == 0)
        {
            if(params.length() == 1)
            {
                newStack.pushSkew(0, params[0]);
            }
        }
        else if(QString::compare(command, "matrix") == 0)
        {
            if(params.length() == 6)
            {
                newStack.push(params[0], params[1], params[2], params[3], params[4], params[5]);
            }
        }
    }

    return newStack;
}
