#ifndef CVERTEX_H
#define CVERTEX_H

namespace voronoi {
/**
 * Container of the VertexList, with prev/next pointer and Point2D as information
 * @author Hildenbrand, Nocaj
 *
 */
class Point2D;
class cVertex
{
public:
       Point2D v;
       cVertex *prev, *next;

       cVertex();
       cVertex(double x, double y);
       cVertex(Point2D& erg);
};
}

#endif // CVERTEX_H
