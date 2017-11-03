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

SOURCES += main.cpp \
    svgparser.cpp \
    gcodecommand.cpp \
    gcodemove.cpp \
    gcodefeed.cpp \
    gcodelaseron.cpp \
    gcodelaseroff.cpp \
    gcodependown.cpp \
    gcodepenup.cpp \
    material.cpp \
    svgelement.cpp \
    svgrect.cpp \
    gcoderect.cpp \
    tool.cpp \
    gcodeline.cpp \
    svgcircle.cpp \
    gcodecircle.cpp \
    gcodepath.cpp \
    gcodecomment.cpp \
    svgellipse.cpp \
    svgpath.cpp \
    transformationstack.cpp \
    plotterstate.cpp \
    svgpath_command.cpp \
    svgpath_move_abs.cpp \
    svgpath_move_rel.cpp \
    svgpath_closepath.cpp \
    svgpath_line_abs.cpp \
    svgpath_line_rel.cpp \
    svgpath_horizontal_line_abs.cpp \
    svgpath_horizontal_line_rel.cpp \
    svgpath_vertical_line_abs.cpp \
    svgpath_vertical_line_rel.cpp \
    svgpath_elliptical_arc_abs.cpp \
    svgpath_elliptical_arc_rel.cpp \
    svgpath_curve_abs.cpp \
    svgpath_curve_rel.cpp \
    svgpath_smoothto_abs.cpp \
    svgpath_smoothto_rel.cpp \
    svgpath_quadratic_curve_abs.cpp \
    svgpath_quadratic_curve_rel.cpp \
    svgpath_quadratic_smoothto_abs.cpp \
    svgpath_quadratic_smoothto_rel.cpp \
    svgline.cpp \
    gcodeellipse.cpp \
    svgpolygon.cpp \
    svgpolyline.cpp \
    svgdesc.cpp \
    transformationparser.cpp \
    svggroup.cpp

HEADERS += \
    svgparser.h \
    gcodecommand.h \
    gcodemove.h \
    gcodefeed.h \
    gcodelaseron.h \
    gcodelaseroff.h \
    gcodependown.h \
    gcodepenup.h \
    material.h \
    svgelement.h \
    svgrect.h \
    gcoderect.h \
    tool.h \
    gcodeline.h \
    svgcircle.h \
    gcodecircle.h \
    gcodepath.h \
    gcodecomment.h \
    svgellipse.h \
    svgpath.h \
    transformationstack.h \
    plotterstate.h \
    point2d.h \
    svgpath_command.h \
    svgpath_move_abs.h \
    svgpath_move_rel.h \
    svgpath_closepath.h \
    svgpath_line_rel.h \
    svgpath_line_abs.h \
    svgpath_horizontal_line_abs.h \
    svgpath_horizontal_line_rel.h \
    svgpath_vertical_line_abs.h \
    svgpath_vertical_line_rel.h \
    svgpath_elliptical_arc_abs.h \
    svgpath_elliptical_arc_rel.h \
    svgpath_curve_abs.h \
    svgpath_curve_rel.h \
    svgpath_smoothto_abs.h \
    svgpath_smoothto_rel.h \
    svgpath_quadratic_curve_abs.h \
    svgpath_quadratic_curve_rel.h \
    svgpath_quadratic_smoothto_abs.h \
    svgpath_quadratic_smoothto_rel.h \
    svgline.h \
    gcodeellipse.h \
    svgpolygon.h \
    svgpolyline.h \
    svgdesc.h \
    transformationparser.h \
    svggroup.h
