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

#ifndef SVGPATH_ELLIPTICAL_ARC_ABS_H
#define SVGPATH_ELLIPTICAL_ARC_ABS_H

#include "svgpath_command.h"

class SVGPath_Elliptical_Arc_Abs : public SVGPath_Command
{
    protected:
        void drawArc(double rx, double ry, double xAxisRotation, bool largeArc, bool sweep, double x, double y);
        double angleFromPoint(Point2D p1, Point2D p2);
        double pointDistance(Point2D p1, Point2D p2);

        QTextStream* mGCode;
        TransformationStack* mTrans;

        void drawArc_avwuff(Point2D p1Prime, Point2D p2Prime, Point2D cPrime, Point2D centerPoint, double rx, double ry, double theta, int sweep);
        void drawArc_nc(Point2D p1Prime, Point2D centerPrime, Point2D center, double rx, double ry, bool sweep, double phi);

        double deltaAngle(Point2D u, Point2D v);

    public:
        SVGPath_Elliptical_Arc_Abs();
        void process(QQueue<QString>* commandQueue, QTextStream* gcode, TransformationStack trans);
};

#endif // SVGPATH_ELLIPTICAL_ARC_ABS_H
