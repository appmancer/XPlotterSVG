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

#include "svgpath_smoothto_rel.h"
#include "plotterstate.h"

SVGPath_SmoothTo_Rel::SVGPath_SmoothTo_Rel()
{

}


void SVGPath_SmoothTo_Rel::process(QQueue<QString>* commandQueue, QTextStream* gcode, TransformationStack trans)
{
    mGcode = gcode;
    mTrans = &trans;
    bool isOk = true;
    Point2D startPoint;
    Point2D endPoint;
    Point2D startControlPoint;
    Point2D endControlPoint;
    startPoint = PlotterState::getLogicalPosition();
    startControlPoint = PlotterState::getControlPosition();

    comment("Starting curve smooth to", gcode);

    //Head down if necessary
    headDown(gcode);

    while(isOk)
    {
        endControlPoint.x = readDouble(commandQueue, &isOk);
        endControlPoint.y = readDouble(commandQueue, &isOk);
        endPoint.x = readDouble(commandQueue, &isOk);
        endPoint.y = readDouble(commandQueue, &isOk);

        if(isOk)
        {
            //I'm assuming that a 0,0 startControlPoint means that it hasn't been set
            //TODO: Well, this might actually be the control position!
            if(startControlPoint.x == 0 && startControlPoint.y == 0)
            {
                startControlPoint = startPoint;
            }
            {
                //We have to make the new control point the reflection of the original control point
                startControlPoint.x = (-(startControlPoint.x - startPoint.x)) + startPoint.x;
                startControlPoint.y = (-(startControlPoint.y - startPoint.y)) + startPoint.y;
            }

            double stepSize = 0.01; //Lets see how that goes.  This will mean that there will always be 100 steps per
                                    //curve.  Lets make it more sophisticated once its working
                                    //TODO: Calculate an appropriate resolution for each curve

            TransformationStack inputTrans;
            inputTrans.pushTranslate(startPoint.x, startPoint.y);

            //Perform transformations, then calculate curve
            Point2D transEndPoint = inputTrans.process(endPoint.x, endPoint.y);
            Point2D transStartControlPoint = inputTrans.process(startControlPoint.x, startControlPoint.y);
            Point2D transEndControlPoint = inputTrans.process(endControlPoint.x, endControlPoint.y);

            //We've inhertited plotCurve from SVGPath_Curve_Abs
            plotCurve(stepSize, startPoint, transEndPoint, transStartControlPoint, transEndControlPoint);

            //Make the next start point this endpoint
            startPoint = transEndPoint;
            startControlPoint = transEndControlPoint;
            //And save it for a following curve command
            PlotterState::setControlPosition(transEndControlPoint);
        }
    }
}
