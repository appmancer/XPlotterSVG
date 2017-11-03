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

#include "svgpath_quadratic_curve_rel.h"
#include "plotterstate.h"

SVGPath_Quadratic_Curve_Rel::SVGPath_Quadratic_Curve_Rel()
{

}

void SVGPath_Quadratic_Curve_Rel::process(QQueue<QString>* commandQueue, QTextStream* gcode, TransformationStack trans)
{
    mGcode = gcode;
    mTrans = &trans;

    bool isOk = true;
    Point2D startPoint;
    Point2D endPoint;
    Point2D controlPoint;

    //This is a relative position, so we must add a transformation of the current head position
    startPoint = PlotterState::getLogicalPosition();

    comment("Starting quadratic curve", gcode);

    //Head down if necessary
    headDown(gcode);

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
            TransformationStack inputTrans;
            inputTrans.pushTranslate(startPoint.x, startPoint.y);

            //Perform transformations, then calculate curve
            Point2D transEndPoint = inputTrans.process(endPoint.x, endPoint.y);
            Point2D transControlPoint = inputTrans.process(controlPoint.x, controlPoint.y);

            //Call the abs version of plotCurve, but with translated points
            plotCurve(stepSize, startPoint, transEndPoint, transControlPoint);

            //Make the next start point this endpoint
            startPoint = transEndPoint;
        }
    }
    comment("Curve complete", gcode);
}
