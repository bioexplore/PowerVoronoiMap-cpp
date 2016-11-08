#ifndef VORONOI_VERTEX_H
#define VORONOI_VERTEX_H
#include "point2d.h"
/**
 * 3-dimentional point with some extra fields for ConvexHull computation.
 *
 * @author Jianye Xia at Chalmers University of Technology
 */
namespace voronoi {
    class ConflictList;
class Vertex
{
public:
    double      x;
    double      y;
    double      z;

    Vertex(double a,double b,double c);
    virtual ~Vertex();

     ConflictList*        getList();
     void                 setList(ConflictList* list);
     int                  getIndex();
     void                 setIndex(int idex);
     bool                 equals(Vertex* v);

    /**
     * @brief linearDependent
     * @param v Vertex compared to
     * @return Whether give vertex and the object are linearly dependent
     */
     bool                 linearDependent(Vertex* v);

     void                 negate();
     Vertex               subtract(Vertex* v);

     Vertex               crossProduct(Vertex* v);
     char*                toString();

     void                 setHandled(bool b);
     bool                 isHandled();       

    //Point3d toPoint3D(){
    //       return new Point3d(x,y,z);
    //}
     double           getX();
     double           getY();
     void             clear();

private:
    constexpr static double epsilon = 1e-10;
    ConflictList*   list_;
    int             index_;
    bool            handled_;
    Vertex();
};
}
#endif
