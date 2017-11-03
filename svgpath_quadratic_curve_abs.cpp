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

#include "svgpath_quadratic_curve_abs.h"
#include "plotterstate.h"
#include "gcodeline.h"
#include <QtMath>

SVGPath_Quadratic_Curve_Abs::SVGPath_Quadratic_Curve_Abs()
{

}

void SVGPath_Quadratic_Curve_Abs::process(QQueue<QString>* commandQueue, QTextStream* gcode, TransformationStack trans)
{
    mGcode = gcode;
    mTrans = &trans;
    bool isOk = true;
    Point2D startPoint;
    Point2D endPoint;
    Point2D controlPoint;

    comment("Starting quadratic curve", gcode);
    //Head down if necessary
    headDown(gcode);

    //Start point is the current head positon
    startPoint = PlotterState::getPosition();

    while(isOk)
    {
        controlPoint.x = readDouble(commandQueue, &isOk);
        controlPoint.y = readDouble(commandQueue, &isOk);
        endPoint.x = readDouble(commandQueue, &isOk);
        endPoint.y = readDouble(commandQueue, &isOk);

        if(isOk)
        {
            double stepSize = 0.01; //Lets see how that goes.  This will mean that there will always be 100 steps per
                                    //curve.  Lets make it more sophisticated once its working
                                    //TODO: Calculate an appropriate resolution for each curve

            plotCurve(stepSize, startPoint, endPoint, controlPoint);

            //Make the next start point this endpoint
            startPoint = endPoint;
        }
    }
}

void SVGPath_Quadratic_Curve_Abs::plotCurve(double stepSize, Point2D startPoint, Point2D endPoint, Point2D controlPoint)
{
    double stepCounter = 0;
    Point2D newPoint;
    //I'm not even going to pretend I can interpret this.  This terrible code is lifted from https://github.com/avwuff/SVG-to-GCode/blob/master/src/Bezier.bas

    double t1; //What?
    double A;
    double b;
    double c; //Why is this one lower case?

    while(stepCounter < 1)
    {
        t1 = 1 - stepCounter;
        A = t1 * t1;
        b = 2 * stepCounter * t1;
        c = stepCounter * stepCounter;

        Point2D newPoint;
        //Some ill-defined magic
        newPoint.x = A * startPoint.x + b * controlPoint.x + c * endPoint.x;
        newPoint.y = A * startPoint.y + b * controlPoint.y + c * endPoint.y;

        GCodeLine line(newPoint.x, newPoint.y);
        line.setTransformationStack(*mTrans);
        *mGcode << line.toString();

        stepCounter += stepSize;
    }

    //Make sure we hit the target.  The final point will be our destination
    GCodeLine line(endPoint.x, endPoint.y);
    line.setTransformationStack(*mTrans);
    *mGcode << line.toString();

    //We might be connecting this curve to a subsequent curve, which might assume that the second control point
    //can be used as the first control point.  Save the endControlPoint to the state so we can use it later
    PlotterState::setControlPosition(controlPoint);
}

