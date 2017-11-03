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

#include "svgpolyline.h"
#include <QDebug>
#include "gcodecomment.h"
#include "gcodemove.h"
#include "gcodeline.h"

SVGPolyline::SVGPolyline()
{
    mElementName = "polyline";
}

void SVGPolyline::process(QXmlStreamReader *reader, QTextStream *gcode, TransformationStack trans)
{
    //qDebug() << "Processing polyline element";
    GCodeComment startComment("Starting Polyline");
    *gcode << startComment.toString();

    bool isOk = true;

    QXmlStreamAttributes attribs = reader->attributes();
    QString polyData = SVGElement::getQString(&attribs, "points").simplified();
    QStringList pointData = polyData.split(" ");
    //Convert the list of points into Point2D
    QString pair;
    QList<Point2D> points;
    foreach(pair, pointData)
    {
        QStringList pointValues = pair.split(",");
        Point2D thisPoint;
        QString x = pointValues.at(0);
        QString y = pointValues.at(1);
        thisPoint.x = x.toDouble(&isOk);
        thisPoint.y = y.toDouble(&isOk);

        points.append(thisPoint);
    }

    if(isOk)
    {
        if(points.length() > 0)
        {
            //Move to the first point
            headUp(gcode);
            mStartPoint = points.at(0);
            points.pop_front();
            GCodeMove move(mStartPoint.x, mStartPoint.y);
            move.setTransformationStack(trans);
            *gcode << move.toString();
            headDown(gcode);
        }
        Point2D nextPoint;
        foreach(nextPoint, points)
        {
            GCodeLine line(nextPoint.x, nextPoint.y);
            line.setTransformationStack(trans);
            *gcode << line.toString();
        }
    }

    //qDebug() << "Reading polygon";
}

