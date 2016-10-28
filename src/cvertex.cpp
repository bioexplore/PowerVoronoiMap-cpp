#include "cvertex.h"
//using namespace voronoi;

voronoi::cVertex::cVertex(double x, double y):
    prev(NULL),next(NULL)
{
     v = Point2D(x,y);
}
voronoi::cVertex::cVertex():
    v(NULL),prev(NULL),next(NULL)
{}
voronoi::cVertex::cVertex(Point2D& erg):
    prev(NULL),next(NULL)
{
    v=Point2D(erg.x, erg.y);
}
const char* voronoi::cVertex::toString()
{
     if(v != NULL){
         return v->toString();
     }
     return "";
}
