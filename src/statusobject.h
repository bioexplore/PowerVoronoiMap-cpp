#ifndef STATUSOBJECT_HEADER
#define STATUSOBJECT_HEADER
namespace voronoi
{
class StatusObject
{
public:
    virtual void finishedNode(int node,int layer,int* children,PolygonSimple* polygons)=0;
    virtual void finished()=0;
};
}
#endif
