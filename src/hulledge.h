#ifndef HULL_EDGE
#define HULL_EDGE
#include "convexhull.h"
#include "vertex.h"
#include "face.h"
#include <string>
#include <list>

/**
 * The class HullEdge is used for representing an edge on the ConvexHul!l
 * Adapted from Arlind Nocaj's github repository Java version
 * URL: https://gitub.com/ArlindNocaj/Voronoi_Treemap_Library
 * Reference:
 *  Arlind Nocaj, Ulrik Brandes, "Computing Voronoi Treemaps: Faster, 
 *      Simpler, and Resolution-independent", Computer Graphics Forum,
 *      vol.31, no.3, June 2012, pp.855-864.
 *
 * @author Jianye Xia, at Chalmers University of Technology
 */
namespace voronoi
{
class HullEdge{
    public:
        HullEdge(Vertex* start,Vertex* end,Face* iFace);
        bool isEqual(Vertex* start,Vertex* end);
        bool isHorizon() ;

        void setEnd(Vertex* end);
        void setStart(Vertex* start);
        void setTwin(HullEdge* edge);
        void setiFace(Face* face);
        void setNext(HullEdge* edge);
        void setPrev(HullEdge* edge);

        Vertex*           getStart() ;
        Vertex*           getEnd() ;
        HullEdge*               getTwin()               {return twin_;}
        Face*             getiFace()                    {return iFace_;}
        HullEdge*               getNext()               {return next_;} 
        HullEdge*               getPrev()               {return prev_;}
        const char*             toString() const;

        
        void                    findHorizon(std::vector<HullEdge*>* horizon);

    private:
        Vertex*      start_;
        Vertex*      end_;
        Face*        iFace_;
        HullEdge*    twin_;
        HullEdge*    next_;
        HullEdge*    prev_;
        HullEdge();//disable default constructor
        HullEdge(const HullEdge&);//disable copy constructor
};
}
#endif
