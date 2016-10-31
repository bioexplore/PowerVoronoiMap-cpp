#ifndef VORONOI_POINT2D_H
#define VORONOI_POINT2D_H
#include <cmath>
#include <cstdio>
#ifdef JYXIA
#include <iostream>
#endif
/**
 * Two-dimentional point, which also has functions as 2D vectors
 * @author Jianye Xia at Chalmers University of Technology
 */
namespace voronoi{
class Point2D
{
public:
    double x;
    double y;

    Point2D():x(0),y(0){}
    Point2D(double a,double b):x(b),y(b){}
    /**
     * @brief distance Computes the euclidean distance between (x1,y1) and (x2,y2)
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @return elucidean distance between (x1,y1) and (x2,y2)
     */
    inline static double distance(double x1,double y1,double x2,double y2)
    {
        x1-=x2;y1-=y2;return std::sqrt(x1*x1+y1*y1);
    }
    inline double   distance(double px,double py)
    {
        px-=x;py-=y;return std::sqrt(px*px+py*py);
    }
    inline double   distance(Point2D p)         {return distance(p.x,p.y);}
    inline double   getX()                      {return x;}
    inline double   getY()                      {return y;}
    inline bool     negative()                  {return x<0 ||y<0;}
    inline char*    toString()
    {
        char* buf=0;
        buf=new char[25];
        std::sprintf(buf,"(%f,%f)",x,y);
        return buf;
    }
    inline void     scale(double d)             { x*=d; y*=d; }
    inline void     translate(double dx,double dy) {x+=dx;y+=dy;}
    /**
     * @brief dot computes dot-product of the given vector with position vector of the current point
     * @param p
     * @return dot-product
     */
    inline double   dot(Point2D* p)             {return x*(p->x)+y*(p->y);}
    inline double   cross(Point2D* p)           {return x*(p->y)-y*(p->x);}
    inline double   length()                    {return std::sqrt(x*x+y*y);}
    inline double   lengthSquared()             { return x*x + y*y;}
    /**
     * @brief normalize scales the coordinates with 1/(length of given vector)
     * @param p
     */
    inline void     normalize(Point2D p)
    {
        double d=1.0/std::sqrt(p.x*p.x+p.y*p.y);
        x = p.x*d;
        y = p.y*d;
    }
    inline void     normalize() 
    {
        if(length()==0)
            return;
        x/=length();
        y/=length();
    }
    /**
     * @brief angle computes the angle between the given vector and this vector
     * @param p
     * @return angle
     */
    inline double   angle(Point2D* p)
    {
        double d=dot(p)/(length()*(p->length()));
        if(d<-1.0)
           d=-1.0;
        if(d>1.0)
            d=1.0;
        return std::acos(d);
    }
    inline void            setLocation(double a,double b)  {x=a;y=b;}

#ifdef JYXIA
    inline void print()
    {
        std::cout<<"["<<x<<","<<y<<"]"<<std::endl;
    }
#endif
};
}
#endif // POINT2D_H

