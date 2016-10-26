#include "interpolcolor.h"
using namespace voronoi;

InterpolColor::InterpolColor(double minValue, double maxValue, double leftH,
                             double leftS, double leftV, double rightH, double rightS,
                             double rightV)
{
    this.minValue = minValue;
    this.maxValue = maxValue;
    this.leftH = leftH;
    this.leftS = leftS;
    this.leftV = leftV;
    this.rightH = rightH;
    this.rightS = rightS;
    this.rightV = rightV;
}

InterpolColor::InterpolColor()
{
    this(0.0f, 100.0f, 0.0f, 1.0f, 0.001f, 0f, 1f, 1f);
}

void InterpolColor::leftHSV(double leftH, double leftS, double leftV)
{
    this.leftH = leftH;
    this.leftS = leftS;
    this.leftV = leftV;
}

void InterpolColor::rightHSV(double rightH, double rightS, double rightV)
{
    this.rightH = rightH;
    this.rightS = rightS;
    this.rightV = rightV;
}

void InterpolColor::setMinValue(double minValue)
{
    this.minValue = minValue;
}

void InterpolColor::setMaxValue(double maxValue)
{
    this.maxValue = maxValue;
}

Color InterpolColor::getColorLinear(double key)
{
    const double anteil = (key - minValue) / (maxValue - minValue);

    double divH = rightH - leftH;
    double divS = rightS - leftS;
    double divV = rightV - leftV;
    double h = leftH + anteil * divH;
    double s = leftS + anteil * divS;
    double v = leftV + anteil * divV;

    return new Color(Color.HSBtoRGB((float) h, (float) s, (float) v));
}

/**
 * key value and the alpha value for this color, which is from 0 to 255
 * @param key
 * @param alpha
 * @return
 */
Color getColorLinear(double key,int alpha)
{
    double anteil = (key - minValue) / (maxValue - minValue);
    double divH = rightH - leftH;
    double divS = rightS - leftS;
    double divV = rightV - leftV;
    double h = leftH + anteil * divH;
    double s = leftS + anteil * divS;
    double v = leftV + anteil * divV;
    Color c = new Color(Color.HSBtoRGB((float) h, (float) s, (float) v));
    return new Color(c.getRed(),c.getGreen(),c.getBlue(),alpha);
}

Color InterpolColor::getColorLog(double key)
{
    double anteil = (double) ((Math.log(key - minValue)) / (Math
                                                            .log(maxValue - minValue)));
    double divH = rightH - leftH;
    double divS = rightS - leftS;
    double divV = rightV - leftV;
    double h = leftH + anteil * divH;
    double s = leftS + anteil * divS;
    double v = leftV + anteil * divV;
    return new Color(Color.HSBtoRGB((float) h, (float) s, (float) v));
}
