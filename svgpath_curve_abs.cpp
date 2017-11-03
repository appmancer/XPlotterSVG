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

#include "svgpath_curve_abs.h"
#include "plotterstate.h"
#include "gcodeline.h"
#include <QtMath>
#include "gcodecomment.h"
#include <QDebug>

SVGPath_Curve_Abs::SVGPath_Curve_Abs()
{

}

void SVGPath_Curve_Abs::process(QQueue<QString>* commandQueue, QTextStream* gcode, TransformationStack trans)
{
    mGcode = gcode;
    mTrans = &trans;
    bool isOk = true;
    Point2D startPoint;
    Point2D endPoint;
    Point2D startControlPoint;
    Point2D endControlPoint;

    startPoint = PlotterState::getLogicalPosition();

    comment("Starting curve", gcode);

    //Head down if necessary
    headDown(gcode);

    while(isOk)
    {
        startControlPoint.x = readDouble(commandQueue, &isOk);
        startControlPoint.y = readDouble(commandQueue, &isOk);
        endControlPoint.x = readDouble(commandQueue, &isOk);
        endControlPoint.y = readDouble(commandQueue, &isOk);
        endPoint.x = readDouble(commandQueue, &isOk);
        endPoint.y = readDouble(commandQueue, &isOk);

        if(isOk)
        {
            double stepSize = 0.1; //Lets see how that goes.  This will mean that there will always be 100 steps per
                                    //curve.  Lets make it more sophisticated once its working
                                    //TODO: Calculate an appropriate resolution for each curve

            plotCurve(stepSize, startPoint, endPoint, startControlPoint, endControlPoint);

            //Make the next start point this endpoint
            startPoint = endPoint;
        }
    }
    comment("Curve complete", gcode);
}

void SVGPath_Curve_Abs::plotCurve(double stepSize, Point2D startPoint, Point2D endPoint, Point2D startControlPoint, Point2D endControlPoint)
{
    //qDebug() << "Plotting cubic bezier";
    //qDebug() << "Starting " << startPoint.x << ":" << startPoint.y;
    //qDebug() << "Ending " << endPoint.x << ":" << endPoint.y;
    double stepCounter = 0;
    Point2D newPoint;

    //From the magic code at https://github.com/mythagel/nc_tools/blob/master/src/nc_svgpath/svgpath.cpp
    for(float t = 0.0; t < 1.0; t += stepSize)
    {
        auto ab = lerp(startPoint, startControlPoint, t);
        auto bc = lerp(startControlPoint, endControlPoint, t);
        auto cd = lerp(endControlPoint, endPoint, t);
        auto newPoint = lerp(lerp(ab, bc, t), lerp(bc, cd, t), t);

        GCodeLine line(newPoint.x, newPoint.y);
        if(mTrans != NULL)
            line.setTransformationStack(*mTrans);

        *mGcode << line.toString();
    }
    //Make sure we hit the target.  The final point will be our destination
    GCodeLine line(endPoint.x, endPoint.y);
    line.setTransformationStack(*mTrans);
    *mGcode << line.toString();

    //We might be connecting this curve to a subsequent curve, which might assume that the second control point
    //can be used as the first control point.  Save the endControlPoint to the state so we can use it later
    PlotterState::setControlPosition(endControlPoint);

    //Roger and out
}

Point2D SVGPath_Curve_Abs::lerp(const Point2D& p0, const Point2D& p1, double t)
{
    return { (1-t)*p0.x + t*p1.x, (1-t)*p0.y + t*p1.y };
}
