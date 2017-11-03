#ifndef SVGPATH_QUADRATIC_SMOOTHTO_REL_H
#define SVGPATH_QUADRATIC_SMOOTHTO_REL_H

#include "svgpath_quadratic_smoothto_abs.h"

class SVGPath_Quadratic_SmoothTo_Rel : public SVGPath_Quadratic_SmoothTo_Abs
{
    public:
        SVGPath_Quadratic_SmoothTo_Rel();
        void process(QQueue<QString>* commandQueue, QTextStream* gcode, TransformationStack trans);
};

#endif // SVGPATH_QUADRATIC_SMOOTHTO_REL_H
