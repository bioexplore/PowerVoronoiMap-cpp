#ifndef INTERPOLCOLOR_H
#define INTERPOLCOLOR_H

namespace voronoi {
/**
 * Color Interpolator, e.g. for Fading and Highlighting
 * @author Nocaj
 *
 */
public class InterpolColor
{
public:
    Color c;// = new Color(Color.HSBtoRGB((float) h, (float) s, (float) v));

    InterpolColor(double minValue, double maxValue, double leftH,
            double leftS, double leftV, double rightH, double rightS,
            double rightV);
    InterpolColor();
    void leftHSV(double leftH, double leftS, double leftV);
    void rightHSV(double rightH, double rightS, double rightV);
    void setMinValue(double minValue);
    void setMaxValue(double maxValue);
    Color getColorLinear(double key);

    /**
     * key value and the alpha value for this color, which is from 0 to 255
     * @param key
     * @param alpha
     * @return
     */
    Color getColorLinear(double key,int alpha);
    Color getColorLog(double key);
private:
    double minValue_;
    double maxValue_;
    double leftH_;
    double leftS_;
    double text_;
    double leftV_;
    double rightH_;
    double rightS_;
    double rightV_;
};
}

#endif // INTERPOLCOLOR_H
