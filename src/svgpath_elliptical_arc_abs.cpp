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

#include <QtMath>
#include <QDebug>
#include "svgpath_elliptical_arc_abs.h"
#include "gcodeline.h"
#include "gcodependown.h"
#include "gcodelaseron.h"
#include "gcodecomment.h"
#include "plotterstate.h"
#include "tool.h"

SVGPath_Elliptical_Arc_Abs::SVGPath_Elliptical_Arc_Abs()
{

}

void SVGPath_Elliptical_Arc_Abs::process(QQueue<QString> *commandQueue, QTextStream *gcode, TransformationStack trans)
{
    mGCode = gcode;
    mTrans = &trans;
    // SVG Params
    //(rx ry x-axis-rotation large-arc-flag sweep-flag x y)+
    bool isOk = true;
    double rx;
    double ry;
    double xAxisRotation;
    double largeArcFlag;
    double sweepFlag;

    //These are endpoint of the ellipse, and come from data
    double x;
    double y;

    comment("Starting elliptical arc", gcode);

    while(isOk)
    {
        //Read x value
        rx = readDouble(commandQueue, &isOk);
        ry = readDouble(commandQueue, &isOk);
        xAxisRotation = readDouble(commandQueue, &isOk);
        largeArcFlag = readDouble(commandQueue, &isOk);
        sweepFlag = readDouble(commandQueue, &isOk);
        x = readDouble(commandQueue, &isOk);
        y = readDouble(commandQueue, &isOk);
        if(isOk)
        {
            drawArc(rx, ry, xAxisRotation, largeArcFlag, sweepFlag, x, y);
        }
    }
}

void SVGPath_Elliptical_Arc_Abs::drawArc(double rx, double ry, double xAxisRotation, bool largeArc, bool sweep, double x, double y)
{
    //Straight port; http://svn.apache.org/repos/asf/xmlgraphics/batik/branches/svg11/sources/org/apache/batik/ext/awt/geom/ExtendedGeneralPath.java
    //
    // Elliptical arc implementation based on the SVG specification notes
    //
    Point2D startPoint = PlotterState::getLogicalPosition();
    long double x0 = startPoint.x;
    long double y0 = startPoint.y;

    // Compute the half distance between the current and the final point
    long double dx2 = (x0 - x) / 2.0;
    long double dy2 = (y0 - y) / 2.0;
    // Convert angle from degrees to radians
    long double angle = qDegreesToRadians(fmod(xAxisRotation, 360.0));
    long double cosAngle = qCos(angle);
    long double sinAngle = qSin(angle);

    //
    // Step 1 : Compute (x1, y1)
    //
    long double x1 =  (cosAngle * dx2 + sinAngle * dy2);
    long double y1 = (-sinAngle * dx2 + cosAngle * dy2);

    Point2D p1Prime;
    p1Prime.x = x1;
    p1Prime.y = y1;

    // Ensure radii are large enough
    rx = qAbs(rx);
    ry = qAbs(ry);
    long double Prx = rx * rx;
    long double Pry = ry * ry;
    long double Px1 = x1 * x1;
    long double Py1 = y1 * y1;
    // check that radii are large enough
    long double radiiCheck = Px1/Prx + Py1/Pry;
    if (radiiCheck > 1)
    {
      double mpl = qSqrt(radiiCheck);
      rx = mpl * rx;
      ry = mpl * ry;
      Prx = rx * rx;
      Pry = ry * ry;
    }

    //
    // Step 2 : Compute (cx1, cy1)
    //
    double sign = (largeArc == sweep) ? -1 : 1;
    long double coNum = ((Prx*Pry)-(Prx*Py1)-(Pry*Px1));
    if(coNum < 0)
        coNum = 0;
    long double sq = coNum / ((Prx*Py1)+(Pry*Px1));
    sq = (sq < 0) ? 0 : sq;
    long double coef = (sign * qSqrt(sq));
    long double cx1 = coef * ((rx * y1) / ry);
    long double cy1 = coef * -((ry * x1) / rx);

    Point2D centerPrime;
    centerPrime.x = cx1;
    centerPrime.y = cy1;

    //
    // Step 3 : Compute (cx, cy) from (cx1, cy1)
    //
    double sx2 = (x0 + x) / 2.0;
    double sy2 = (y0 + y) / 2.0;
    double cx = sx2 + (cosAngle * cx1 - sinAngle * cy1);
    double cy = sy2 + (sinAngle * cx1 + cosAngle * cy1);

    Point2D centerPoint;
    centerPoint.x = cx;
    centerPoint.y = cy;
/*
    //
    // Step 4 : Compute the angleStart (angle1) and the angleExtent (dangle)
    //
    double ux = (x1 - cx1) / rx;
    double uy = (y1 - cy1) / ry;
    double vx = (-x1 - cx1) / rx;
    double vy = (-y1 - cy1) / ry;
    double p, n;
    // Compute the angle start
    n = qSqrt((ux * ux) + (uy * uy));
    p = ux; // (1 * ux) + (0 * uy)
    sign = (uy < 0) ? -1 : 1;
    double angleStart = qRadiansToDegrees(sign * qAcos(p / n));

    // Compute the angle extent
    n = qSqrt((ux * ux + uy * uy) * (vx * vx + vy * vy));
    p = ux * vx + uy * vy;
    sign = (ux * vy - uy * vx < 0) ? -1 : 1;
    double angleExtent = qRadiansToDegrees(sign * qAcos(p / n));
    if(!sweep && angleExtent > 0) {
      angleExtent -= 360.00;
    } else if (sweep && angleExtent < 0) {
      angleExtent += 360.00;
    }
    angleExtent %= 360.00;
    angleStart %= 360.00;
*/
    //From here on there is a difference of opinion.  The above code is specified in the
    //notes at https://www.w3.org/TR/SVG/implnote.html#ArcImplementationNotes
    //but it is what you do with it.
    drawArc_nc(p1Prime, centerPrime, centerPoint, rx, ry, sweep > 0, angle);
}

// Draws the arc using the method from https://github.com/mythagel/nc_tools/blob/master/src/nc_svgpath/svgpath.cpp
void SVGPath_Elliptical_Arc_Abs::drawArc_nc(Point2D p1Prime, Point2D centerPrime, Point2D center, double rx, double ry, bool sweep, double phi)
{
    Point2D u;
    u.x = 1;
    u.y = 0;

    Point2D v;
    v.x = (p1Prime.x - centerPrime.x) / rx;
    v.y = (p1Prime.y - centerPrime.y) / ry;

    double theta1 = deltaAngle(u, v);

    u.x = (p1Prime.x - centerPrime.x) / rx;
    u.y = (p1Prime.y - centerPrime.y) / ry;

    v.x = (-p1Prime.x - centerPrime.x) / rx;
    v.y = (-p1Prime.y - centerPrime.y) / ry;

    double deltaTheta = deltaAngle(u, v);

    if(!sweep && deltaTheta > 0)
        deltaTheta -= 2 * M_PI;
    else if(sweep && deltaTheta < 0)
        deltaTheta += 2 * M_PI;

    //Calculate an appropriate number of steps
    //deltaTheta is the amount of the ellipse being drawn, in radians
    //there are 2pi radians in a circle (6.28ish)
    //We can use this to estimate the length of the arc using the mean
    //radius, measured in millimetres.
    int steps = qCeil(((rx + ry) / 2) * qAbs(deltaTheta));
    double step = deltaTheta / steps;
    double theta = theta1;

    for(int i = 0; i < steps; ++i, theta += step)
    {
        Point2D m1;
        m1.x = rx * qCos(theta);
        m1.y = ry * qSin(theta);

        Point2D p;
        p.x = (qCos(phi) * m1.x) + (-qSin(phi) * m1.y) + center.x;
        p.y = (qSin(phi) * m1.x) + ( qCos(phi) * m1.y) + center.y;

        GCodeLine line(p.x, p.y);
        //Make sure we don't try to reapply transformations
        line.setTransformationStack(*mTrans);
        *mGCode << line.toString();
    }
}


//Draws the arc using the method from https://github.com/avwuff/SVG-to-GCode/blob/master/src/SVGParse.bas
void SVGPath_Elliptical_Arc_Abs::drawArc_avwuff(Point2D p1Prime, Point2D p2Prime, Point2D cPrime, Point2D centerPoint, double rx, double ry, double theta, int sweep)
{
    //That's a lot of work to get the centrepoint, but now we've got it, we can calculate the arc
    double startTheta = 0;
    double endTheta = 0;

    startTheta = angleFromPoint(p1Prime, cPrime) - M_PI;
    endTheta = angleFromPoint(p2Prime, cPrime) - M_PI;

    double angleStep = M_PI / 180; //Is this arbitary, or meaningful?

    if(sweep > 0)
    {
        if(endTheta < startTheta)
            endTheta += 2*M_PI;
    }
    else
    {
        if(endTheta > startTheta)
            endTheta -= 2*M_PI;
        angleStep *= -1;
    }

    double radToDeg = 180 / M_PI;
    //qDebug() << "Start Angle " << startTheta * radToDeg << " End Angle " << endTheta * radToDeg << " step " << angleStep * radToDeg;

    double currentAngle = startTheta;
    Point2D nextPoint;
    headDown(mGCode);
    while((angleStep < 0 && currentAngle < endTheta) || (angleStep > 0 && currentAngle < endTheta))
    {
        nextPoint.x = (rx * qCos(currentAngle)) + centerPoint.x;
        nextPoint.y = (ry * qSin(currentAngle)) + centerPoint.y;

        double thisAngle = angleFromPoint(centerPoint, nextPoint) + theta;
        double thisDist  = pointDistance(centerPoint, nextPoint);

        nextPoint.x = (thisDist * qCos(thisAngle)) + centerPoint.x;
        nextPoint.y = (thisDist * qSin(thisAngle)) + centerPoint.y;

        //THIS IS IT, DRAW THIS GODDAMNED LINE!
        GCodeLine line(nextPoint.x, nextPoint.y);
        line.setTransformationStack(*mTrans);
        *mGCode << line.toString();

        currentAngle += angleStep;
    }
}


double SVGPath_Elliptical_Arc_Abs::angleFromPoint(Point2D centre, Point2D target)
{
    double theta;
    double slope;
    if(centre.x == target.x)
    {
        theta = target.y > centre.y ? M_PI / 2 : -M_PI / 2;
    }
    else
    {
        slope = (target.y - centre.y) / (target.x - centre.x);
        theta = qAtan(slope);
        if(target.x > centre.x)
            theta += M_PI;
    }

    if(theta < 0)
        theta += 2*M_PI;

    return theta;
}

//Direct port from https://github.com/mythagel/nc_tools/blob/master/src/nc_svgpath/svgpath.cpp
double SVGPath_Elliptical_Arc_Abs::deltaAngle(Point2D u, Point2D v)
{
    double sign = u.x * v.y - u.y * v.x;
    double num = (u.x * v.x) + (u.y * v.y);
    double normu = qSqrt((u.x * u.x) + (u.y * u.y));
    double normv = qSqrt((v.x * v.x) + (v.y * v.y));
    double den = normu * normv;

    double cosStep = qAcos(num/den);
    //Remove the sign from cosStep
    cosStep = qAbs(cosStep);

    //And add it again if necessary
    if(sign < 0)
        cosStep *= -1;

    return cosStep;
}

double SVGPath_Elliptical_Arc_Abs::pointDistance(Point2D p1, Point2D p2)
{
    double xsqr = qAbs(p2.x - p1.x);
    xsqr *= xsqr;
    double ysqr = qAbs(p2.y - p1.y);
    ysqr *= ysqr;

    return qSqrt(xsqr + ysqr);
}

