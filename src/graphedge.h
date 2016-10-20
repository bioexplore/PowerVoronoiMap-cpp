#ifndef VORONOI_GRAPHEDGE_H
#define VORONOI_GRAPHEDGE_H
#include "vertex.h"
#include "face.h"

/**
 * Edge for ConflictList
 * @author Jianye Xia at Chalmers University of Technology
 */
namespace voronoi {
class GraphEdge
{
public:
    GraphEdge(Face& face,Vertex& vertex):
        face_(face),vertex_(vertex),nextf_(NULL),nextv_(NULL),prevf_(NULL),prevv_(NULL)
    {}
protected:
    Face& face_;
    Vertex& vertex_;
    GraphEdge* nextf_;
    GraphEdge* prevf_;
    GraphEdge* nextv_;
    GraphEdge* prevv_;
};
}
#endif // GRAPHEDGE_H

