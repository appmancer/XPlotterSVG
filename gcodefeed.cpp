#include "gcodefeed.h"
#include <QTextStream>

GCodeFeed::GCodeFeed(int feed) : mFeed(feed)
{

}

QString GCodeFeed::toString()
{
    QString gcode;
    QTextStream stream(&gcode);

    stream << "G1 F" << mFeed << endl;

    return gcode;
}
