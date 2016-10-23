#ifndef VORONOI_PLANE3D_H
#define VORONOI_PLANE3D_H
#include "point2d.h"
#include "vertex.h"
#ifdef JYXIA
#include <iostream>
#endif
/**
 * Class Plane3D store a plane in 3 dimention which can be expressed by
 * a*x+b*y+c*z+d=0;
 * Class NormalPlane store a plane that expressed with equation like
 * z=a*x+b*y+c
 * Class Point3D is a helper class for dealing with points in 3D
 * @author Jianye Xia at Chalmers University of Technology
 */
namespace voronoi
{
class NormalPlane
{
public:
    double a;
    double b;
    double c;
    NormalPlane():a(0),b(0),c(0){}
#ifdef JYXIA
    void print()
    {
        std::cout<<"Plane(z=a*x+b*y+c):a="<<a<<",\tb="<<b<<",\tc="<<c<<std::endl;
    }
#endif
};
class Point3D
{
public:
    double x;
    double y;
    double z;
    Point3D():x(0),y(0),z(0){}
    Point3D(int a,int b, int c):x(a),y(b),z(c){}
    Point3D(double a,double b,double c):x(a),y(b),z(c){}
    Point3D(Vertex& v):x(v.x),y(v.y),z(v.z){}
#ifdef JYXIA
    void print()
    {
        std::cout<<"Point in 3D:["<<x<<","<<y<<","<<z<<"]"<<std::endl;
    }
#endif
};
class Plane3D
{
public:
    double a;
    double b;
    double c;
    double d;

    Plane3D(Point3D& p1,Point3D& p2,Point3D& p3)
    {
        a=p1.y*(p2.z-p3.z)+p2.y*(p3.z-p1.z)+p3.y*(p1.z-p2.z);
        b=p1.z*(p2.x-p3.x)+p2.z*(p3.x-p1.x)+p3.z*(p1.x-p2.x);
        c=p1.x*(p2.y-p3.y)+p2.x*(p3.y-p1.y)+p3.x*(p1.y-p2.y);
        d=-1*(p1.x*(p2.y*p3.z-p3.y*p2.z)+p2.x*(p3.y*p1.z-p1.y*p3.z)+p3.x*(p1.y*p2.z-p2.y*p1.z));
    }
    
    inline Point2D getDualPointMappedToPlane()
    {
        NormalPlane nPlane = getNormZPlane();
        Point2D dualPoint(nPlane.a/2,nPlane.b/2);
        return dualPoint;
    }
#ifdef JYXIA
    void print()
    {
        std::cout<<"Plane(a*x+b*y+c*z+d=0):a="<<a<<"\tb="<<b<<"\tc="<<c<<"\td="<<d<<std::endl;
    }
#endif
protected:
    inline NormalPlane getNormZPlane()
    {
         NormalPlane nPlane;
         nPlane.a=-1*(a/c);
         nPlane.b=-1*(b/c);
         nPlane.c=-1*(d/c);
         return nPlane;
    } 
private:
    Plane3D();

};
}
#endif

