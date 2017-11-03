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

#include <QCoreApplication>
#include <QtDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QStringList>
#include "gcodecomment.h"
#include "svgparser.h"

inline QTextStream& qStdout()
{
    static QTextStream r{stdout};
    return r;
}

void printUsage()
{
    printf("\nUsage: XPlotterSVG <sourcefile> <materialfile> <outputfile>\n");
}

bool checkArgs(int argc, char* argv[])
{
    if(argc < 3)
    {
        printUsage();
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qStdout() << "XPlotterSVG Copyright (c) 2017 Samuel Pickard" << endl;
    qStdout() << "This program comes with ABSOLUTELY NO WARRANTY." << endl;
    qStdout() << "This is free software, and you are welcome to redistribute it" << endl;
    qStdout() << "under certain conditions" << endl;

    if(checkArgs(argc, argv))
    {
        //Load the SVG file into the parser

        QString inputFilePath(QCoreApplication::arguments().at(1));
        QString materialsFilePath(QCoreApplication::arguments().at(2));
        QString outputFilePath("XPLOTTER.G");
        if(argc > 3)
        {
            outputFilePath = QCoreApplication::arguments().at(3);
        }

        QFile materialsFile(materialsFilePath);

        if (!QFile::exists(materialsFilePath))
        {
            qStdout() << "Materials file not found" << endl;
            return 0;
        }
        else if (!materialsFile.open(QIODevice::ReadOnly))
        {
            qStdout() << "Cannot open materials file" << endl;
            return 0;
        }

        QXmlStreamReader materialXml(&materialsFile);
        Material material;
        material.load(&materialXml);

        QFile inputFile(inputFilePath);

        if (!QFile::exists(inputFilePath))
        {
            qStdout() << "SVG file not found" << endl;
            return 0;

        } else if (!inputFile.open(QIODevice::ReadOnly)) {
            qStdout() << "Cannot open SVG file" << endl;
            return 0;
        }

        QXmlStreamReader reader(&inputFile);

        //Open the output file for writing
        QFile gcodeFile(outputFilePath);
        if(!gcodeFile.open(QIODevice::ReadWrite|QIODevice::Truncate))
        {
            qStdout() << "Cannot open output file" << endl;
            return 0;
        }

        //Create an output stream
        QTextStream gcode(&gcodeFile);

        //Write a comment at the start of the file
        GCodeComment comment(inputFilePath);
        QString debug = comment.toString();
        gcode << comment.toString();

        //Parse!
        SVGParser parser;
        parser.process(&reader, &gcode, &material);


        qStdout() << "Parsing finished" << endl;
        //Close the output file
        gcodeFile.close();
    }

    return 0;
}
