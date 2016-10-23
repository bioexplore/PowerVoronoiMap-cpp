#ifndef VORONOI_CONFLICTLIST_HEADER
#define VORONOI_CONFLICTLIST_HEADER
#include "graphedge.h"
#include <vector>
/**
 * Data structure to save the conflict graph of vertices and faces.
 * For every vertex is saved, which faces are in conflict and for every
 * Face is saved, which vertices are in conflict.
 * The ConflictList supports constant-time and remove methods.
 *
 * @author Jianye Xia at Chalmers University of Technology
 */
namespace voronoi
{
class ConflictList
{
    public:
        inline ConflictList(bool face):face_(face),head_(NULL){}

        inline  bool            empty()     {return head_==NULL;}

        void                    add(GraphEdge *e);
        void                    fill(std::vector<Face*>* visible);
        void                    removeAll();
        void                    getVertices(std::vector<Vertex*>* l1);

    protected:
        GraphEdge* head_;
    private:
        bool face_;//Determine this is a face conflictList or a vertex 
                   //conflictList! true for face conflictlist, 
                   //and false for vertex conflictList!
        ConflictList();
};
}
#endif
