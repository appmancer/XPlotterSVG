# XPlotterSVG
XPlotterSVG converts SVG files from Inkscape (or other sources) into GCode that can be interpreted by the XPlotter.

XPlotterSVG will convert SVG path elements, as well as simple shapes; rect, ellipse, circle, line, polygon and polyline.  It also applies all SVG transformations.

XPlotterSVG takes a second input file, a material definition file.  This is a simple XML document that details feedrates, power and repetitions of the path.  You can create XPlotter GCode files for the pen tool and the laser from the same SVG.

By specifying different stroke colours in the SVG, and matching those values to the material file, you can create different depths of cut.  In one GCode you can specify light, medium, dark and full cuts through the material.
