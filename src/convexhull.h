#ifndef CONVEXHULL_H
#define CONVEXHULL_H
#include <vector>

/**
 * Class for computing the covex hull of given vertices with a randomized
 * incremental algorithm in O(n*log(n)) after book of Mark de Berg/Mark Overmars
 * /Otfried Schwarzkopf, Computational geometry: algorithms and applications
 *
 * @author Jianye Xia at Chalmers University of Technology
 */
namespace voronoi {
    class Vertex;
    class Face;
    class HullEdge;
class ConvexHull
{
public:
    ConvexHull();
    ~ConvexHull();

    void                    addPoint(Vertex* v);
    void                    addPoint(double x,double y,double z);
    std::vector<Face*>*       compute();

    inline int              getVertexCount()        { return points_->size(); }
    inline Vertex*          getVertex(int i)        { return points_->operator[](i);}
    inline int              getFacetCount()         { return facets_->size(); }
    inline Face*            getFacet(int i)         { return facets_->operator[](i); }

protected:
    const static int rand_seed_=0;
private:
    std::vector<Vertex*>*         points_;
    std::vector<Face*>*           facets_;
    std::vector<Face*>*           created_;
    std::vector<HullEdge*>*       horizon_;
    std::vector<Face*>*           visible_;
    int                         currentIndex_;
    bool                        permutate_;

    /**
     * Conflicts of the new Face can be only the conflicts of the incident Faces of the horizon edge
     * @param old1 incident facet of the horizon edge
     * @param old2 other incident facet of the horizon edge
     * @param fn newly created facet, which conflicts are added
     */
    void            addConflicts(Face* old1, Face* old2, Face* fnew);
    
    void            removeConflict(Face* f);
    /**
     * Prepares the convex hull computation
     * Builds the tetrahedron, fills the conflict graph and builds permutation for the points list
     *
     */
    void            prep();

    /**
     * Creates the permutation of the vertices with the Fisher-Yates Shuffle.
     */
    void            permutation();

    void            addFacet(Face* f0);
    void            addConflict(Face* f0, Vertex* v);
    inline bool     isPermutate()       { return permutate_; }
    inline void     setPermutate(bool permutate)    { permutate_=permutate; }
};
}

#endif // CONVEXHULL_H

