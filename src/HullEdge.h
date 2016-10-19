#ifndef HULL_EDGE
#define HULL_EDGE
#include "ConvexHull.h"
#include "Vertex.h"
#include "Face.h"
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
        HullEdge(const Vertex& start,const Vertex& end,const Face& iFace);
        bool isEqual(const Vertex& start, const Vertex& end);
        bool isHorizon();

        void setEnd(const Vertex& end);
        void setStart(const Vertex& start);
        void setTwin(const HullEdge& edge);
        void setiFace(const Face& face);
        void setNext(const HullEdge& edge);
        void setPrev(const HullEdge& edge);

        const Vertex& getStart();
        const Vertex& getEnd();
        const HullEdge& getTwin();
        const Face& getiFace();
        const HullEdge& getNext();
        const HullEdge& getPrev();
        std::string toString();
        
        void findHorizon(std::list<HullEdge&> horizon);

    private:
        const Vertex&      start_;
        const Vertex&      end_;
        const Face&        iFace_;
        const HullEdge&    twin_;
        const HullEdge&    next_;
        const HullEdge&    prev_;
};
}
#endif
