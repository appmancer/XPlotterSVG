QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += src/main.cpp \
    src/svgparser.cpp \
    src/gcodecommand.cpp \
    src/gcodemove.cpp \
    src/gcodefeed.cpp \
    src/gcodelaseron.cpp \
    src/gcodelaseroff.cpp \
    src/gcodependown.cpp \
    src/gcodepenup.cpp \
    src/material.cpp \
    src/svgelement.cpp \
    src/svgrect.cpp \
    src/gcoderect.cpp \
    src/tool.cpp \
    src/gcodeline.cpp \
    src/svgcircle.cpp \
    src/gcodecircle.cpp \
    src/gcodepath.cpp \
    src/gcodecomment.cpp \
    src/svgellipse.cpp \
    src/svgpath.cpp \
    src/transformationstack.cpp \
    src/plotterstate.cpp \
    src/svgpath_command.cpp \
    src/svgpath_move_abs.cpp \
    src/svgpath_move_rel.cpp \
    src/svgpath_closepath.cpp \
    src/svgpath_line_abs.cpp \
    src/svgpath_line_rel.cpp \
    src/svgpath_horizontal_line_abs.cpp \
    src/svgpath_horizontal_line_rel.cpp \
    src/svgpath_vertical_line_abs.cpp \
    src/svgpath_vertical_line_rel.cpp \
    src/svgpath_elliptical_arc_abs.cpp \
    src/svgpath_elliptical_arc_rel.cpp \
    src/svgpath_curve_abs.cpp \
    src/svgpath_curve_rel.cpp \
    src/svgpath_smoothto_abs.cpp \
    src/svgpath_smoothto_rel.cpp \
    src/svgpath_quadratic_curve_abs.cpp \
    src/svgpath_quadratic_curve_rel.cpp \
    src/svgpath_quadratic_smoothto_abs.cpp \
    src/svgpath_quadratic_smoothto_rel.cpp \
    src/svgline.cpp \
    src/gcodeellipse.cpp \
    src/svgpolygon.cpp \
    src/svgpolyline.cpp \
    src/svgdesc.cpp \
    src/transformationparser.cpp \
    src/svggroup.cpp

HEADERS += \
    src/svgparser.h \
    src/gcodecommand.h \
    src/gcodemove.h \
    src/gcodefeed.h \
    src/gcodelaseron.h \
    src/gcodelaseroff.h \
    src/gcodependown.h \
    src/gcodepenup.h \
    src/material.h \
    src/svgelement.h \
    src/svgrect.h \
    src/gcoderect.h \
    src/tool.h \
    src/gcodeline.h \
    src/svgcircle.h \
    src/gcodecircle.h \
    src/gcodepath.h \
    src/gcodecomment.h \
    src/svgellipse.h \
    src/svgpath.h \
    src/transformationstack.h \
    src/plotterstate.h \
    src/point2d.h \
    src/svgpath_command.h \
    src/svgpath_move_abs.h \
    src/svgpath_move_rel.h \
    src/svgpath_closepath.h \
    src/svgpath_line_rel.h \
    src/svgpath_line_abs.h \
    src/svgpath_horizontal_line_abs.h \
    src/svgpath_horizontal_line_rel.h \
    src/svgpath_vertical_line_abs.h \
    src/svgpath_vertical_line_rel.h \
    src/svgpath_elliptical_arc_abs.h \
    src/svgpath_elliptical_arc_rel.h \
    src/svgpath_curve_abs.h \
    src/svgpath_curve_rel.h \
    src/svgpath_smoothto_abs.h \
    src/svgpath_smoothto_rel.h \
    src/svgpath_quadratic_curve_abs.h \
    src/svgpath_quadratic_curve_rel.h \
    src/svgpath_quadratic_smoothto_abs.h \
    src/svgpath_quadratic_smoothto_rel.h \
    src/svgline.h \
    src/gcodeellipse.h \
    src/svgpolygon.h \
    src/svgpolyline.h \
    src/svgdesc.h \
    src/transformationparser.h \
    src/svggroup.h
