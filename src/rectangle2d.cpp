#include "rectangle2d.h"

voronoi::Rectangle2D::Rectangle2D():
    xmin_(0),ymin_(0),width_(0),h_(0)
{}
voronoi::Rectangle2D::Rectangle2D(double x,double y,double w,double h):
    xmin_(x),ymin_(y),width_(w),h_(h)
{}
bool voronoi::Rectangle2D::contain(double x,double y)
{
    if(x<=xmin_+width_ && x>=xmin_ && y<=ymin_+height_ && y>=ymin_)
        return true;
    return false;
}
bool voronoi::Rectangle2D::contain(Rectangle& rec)
{
    if(rec.width_>width_ || rec.height_>height_)
        return false;
    else if(rec.xmin_>=xmin_ && rec.xmin_ <= xmin_+width_-rec.width_ && rec.ymin_>=ymin_ && rec.ymin_ <= ymin_+height_-rec.height_)
        return true;
    else
        return false;
}
double voronoi::Rectangle2D::getX()
{
    return xmin_;
}
double voronoi::Rectangle2D::getY()
{
    return ymin_;
}
double voronoi::Rectangle2D::getWidth()
{
    return width_;
}
double voronoi::Rectangle2D::getHeight()
{
    return height_;
}
void voronoi::Rectangle2D::setX(double x)
{
    xmin_=x;
}
void voronoi::Rectangle2D::setY(double y)
{
    ymin_=y;
}
void voronoi::Rectangle2D::setWidth(double w)
{
    width_(w);
}
void voronoi::Rectangle2D::setHeight(double h)
{
    height_(h);
}

