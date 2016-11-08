#ifndef SVG_STATUSOBJECT_HEADER
#define SVG_STATUSOBJECT_HEADER
#include "statusobject.h"
namespace voronoi
{
class PolygonSimple;
class SVGStatusObject:public StatusObject
{
public:
    virtual void finishedNode(int node,int layer,int* children,PolygonSimple* polygons);
    virtual void finished();
};
}
#endif
