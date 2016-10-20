#ifndef VORONOI_VERTEX_H
#define VORONOI_VERTEX_H
#include "point2d.h"
/**
 * 3-dimentional point with some extra fields for ConvexHull computation.
 *
 * @author Jianye Xia at Chalmers University of Technology
 */
namespace voronoi {
class Vertex
{
public:
    double x;
    double y;
    double z;
    Point2D& originalObject; //TODO:consider change to originalPoint

    inline Vertex(double a,double b,double c):
        x(a),y(b),z(c),originalObject(NULL),handled_(false)
    {
        list_=new ConflictList(false);
        index_=-1;
    }

    inline ConflictList&        getList()       { return *list_;}
    inline void                 setList(ConflictList& list)     {list_=&list;}
    inline int                  getIndex()      {   return index_;}
    inline void                 setIndex(int idex)      {index_=idex;}
    inline bool                 equals(Vertex& v) {if(v.x==x && v.y==y && v.z==z) return true; return false;}

    /**
     * @brief linearDependent
     * @param v Vertex compared to
     * @return Whether give vertex and the object are linearly dependent
     */
    inline bool                 linearDependent(Vertex& v)
    {
        if(x == 0 && v.x == 0)
        {
            if(y == 0 && v.y == 0){
                if(z == 0 && v.z == 0) return true;
                if(z == 0 || v.y == 0) return false;
                return true;
            }
            if(y == 0 || v.y == 0) return false;
            if(z/y >= v.z/v.y -epsilon  && z/y <= v.z/v.y +epsilon)
                return true;
            else
                return false;
        }
        if(x == 0 || v.x == 0) return false;
        if(y/x <= v.y/v.x+epsilon && y/x >= v.y/v.x-epsilon &&  z/x >= v.y/v.x -epsilon  && z/x <= v.z/v.x +epsilon)
                return true;
        else
                return false;
    }

    inline void                 negate()        {x*=-1;y*=-1;z*=-1;}
    inline Vertex*              subtract(Vertex& v)     {return new Vertex(x-v.x,y-v.y,z-v.z);}
    inline Vertex*              crossProduct(Vertex& v)
                                {return new Vertex(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.z);}
    inline char*                toString()
    {
        char* buf=0;
        buf=new char[25];
        std::sprintf(buf,"%f/%f/%f",x,y,z);
        return buf;
    }

    inline void                 setHandled(bool b)      {handled_=b;}
    inline bool                 isHandled()             {return handled_;}

    //Point3d toPoint3D(){
    //       return new Point3d(x,y,z);
    //}
    inline double           getX()      {return x;}
    inline double           getY()      {return y;}
    inline void             clear()
    {
        list_.clear();//TODO: add this function to ConflictList class
        index_=-1;
        handled_=false;
    }

private:
    const static double epsilon = 1e-10;
    ConflictList*   list_;
    int             index_;
    bool            handled_;

};
}
#endif
