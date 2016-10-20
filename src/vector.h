#ifndef VORONOI_VECTOR_H
#define VORONOI_VECTOR_H
#include <cmath>
#include <cstdio>
/**
 * 3-dimentional Vector with some helpfull functions, e.g., normalizing ...
 *
 * @author Jianye Xia at Chalmers University of Technology
 */
namespace voronoi{
class Vector
{
    public:
        double x;
        double y;
        double z;
        Vector():x(0.0),y(0.0),z(0.0){}
        Vector(double a,double b,double c):x(a),y(b),z(c){}
        Vector(const Vector& a) {x=a.x;y=a.y;z=a.z;}
        
        // TODO: consider to change these 3 functions to operators
        inline Vector   subtract(const Vector& b)   {return Vector(x-b.x,y-b.y,z-b.z);}
        inline Vector   cross(const Vector& b)      {return Vector(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);}
        inline double   dot(const Vector& b)        {return x*b.x+y*b.y+z*b.z;}

        inline void     negate()                    {x*=-1;y*=-1;z*=-1;}
        inline double   length()                    {return std::sqrt(x*x+y*y+z*z);}
        inline char*    toString()                  
        {
            char* buf=0;
	    buf=new char[25];
            std::sprintf(buf,"X: %f\tY: %f\tZ:%f",x,y,z);
            return buf;
        }
        inline void     normalize()                 
        {
            double len=length();
            if(len>0.0)
            {
                x/=len;y/=len;z/=len;
            }
        }
};
}
#endif
