#ifndef RECTANGLE2D_H
#define RECTANGLE2D_H
namespace voronoi {
class Rectangle2D
{
public:
    Rectangle2D();
    Rectangle2D(double x,double y,double w,double h);
    bool contain(double x,double y);
    bool contain(Rectangle& rec);
    double getX();
    double getY();
    double getWidth();
    double getHeight();
    void setX(double x);
    void setY(double y);
    void setWidth(double w);
    void setHeight(double h);

private:
    double xmin_;
    double ymin_;
    double width_;
    double height_;
};
}

#endif // RECTANGLE2D_H
