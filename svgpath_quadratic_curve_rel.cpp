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
