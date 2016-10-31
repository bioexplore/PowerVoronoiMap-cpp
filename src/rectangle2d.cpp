#include "rectangle2d.h"

voronoi::Rectangle2D::Rectangle2D():
    xmin_(0),ymin_(0),width_(0),height_(0)
{}
voronoi::Rectangle2D::Rectangle2D(double x,double y,double w,double h):
    xmin_(x),ymin_(y),width_(w),height_(h)
{}
bool voronoi::Rectangle2D::contains(double x,double y)
{
    if(x<=xmin_+width_ && x>=xmin_ && y<=ymin_+height_ && y>=ymin_)
        return true;
    return false;
}
bool voronoi::Rectangle2D::contains(double xmin,double ymin,double width,double height)
{
    if( xmin>xmin_ && (xmin+width) < (xmin_+width_))
        if(ymin>ymin_ && (ymin+height) < (ymin_+height_))
            return true;
    return false;
}
bool voronoi::Rectangle2D::contains(Rectangle2D& rec)
{
    if(rec.width_>width_ || rec.height_>height_)
        return false;
    else if(rec.xmin_>=xmin_ && rec.xmin_ <= xmin_+width_-rec.width_ && rec.ymin_>=ymin_ && rec.ymin_ <= ymin_+height_-rec.height_)
        return true;
    else
        return false;
}
bool voronoi::Rectangle2D::intersects(Rectangle2D& r)
{
    double xmin=r.xmin_,ymin=r.ymin_;
    double xmax=r.xmin_+r.width_,ymax=r.ymin_+r.height_;
    if(contains(xmin,ymin) ||contains(xmin,ymax)||contains(xmax,ymin)||contains(xmax,ymax))
        return true;
    return false;
}
bool voronoi::Rectangle2D::intersects(double xmin,double ymin,double width,double height)
{
    Rectangle2D r(xmin,ymin,width,height);
    return intersects(r);
}
double voronoi::Rectangle2D::getX()
{
    return xmin_;
}
double voronoi::Rectangle2D::getY()
{
    return ymin_;
}
double voronoi::Rectangle2D::getCenterX()
{
    return xmin_ + width_/2.0;
}
double voronoi::Rectangle2D::getCenterY()
{
    return ymin_ + height_/2.0;
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
    width_=w;
}
void voronoi::Rectangle2D::setHeight(double h)
{
    height_=h;
}

