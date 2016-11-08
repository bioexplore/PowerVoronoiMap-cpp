#ifndef CONVEXCLIP_H
#define CONVEXCLIP_H
#include <vector>
/**
 *
 * @author Hildenbrand, Nocaj
 * ConvexClip used for computing intersection of two convex polygons in O(n+m)
 *
 */
namespace voronoi
{
class Vertex;
class Point2D;
class InfoPoint;
class ConvexClip
{
public:
    /**
     * Vertex List of the final intersection with the result of the computation
     */
    std::vector<Vertex*>* inters;

    /**
     * Computes the sign of the area of the triangle a,b,c
     * @param a
     * @param b
     * @param c
     * @return returns sign of the are {0,1,-1}
     */
    static int AreaSign(Point2D& a, Point2D& b, Point2D& c);
    static int AreaSign(Vertex* a,Vertex* b,Vertex* c);

    /**
     *
     * @param list1 first polygon for the intersection
     * @param list2	second polygon for the intersection
     * @throws RuntimeException  if one of the two polygons is not convex
     */
    void Start(std::vector<Vertex*>& list1, std::vector<Vertex*>& list2);

    /**
     * computes the intersection of the egdes between ab and cd
     * @param a
     * @param b
     * @param c
     * @param d
     * @return Infopoint with the flag of the intersection type and the point of the intersection
     *
     */
    InfoPoint intersect(Point2D& a, Point2D& b, Point2D& c, Point2D& d);
private:
    /**
     * Checks whether p2 is convex and counterclockwise oriented
     * @param p2 VertexList
     * @return returns whether p2 is convex and counterclockwise oriented
     */
    static bool isConvex(std::vector<Vertex*>& p2);
    static bool isReverseConvex(std::vector<Vertex*>& p2);

    /**
     * Checks if point c is between a and b or not
     * @param a
     * @param b
     * @param c
     * @return
     */
    static bool between(Point2D& a, Point2D& b, Point2D& c);
    /**
     * Using two counterclockwise oriented VertexLists to compute the intersection of the corresponding polygons
     * @param p first counterclockwise oriented polygon
     * @param q second counterclockwise oriented polygon
     * @param n number of vertices of the first polygon
     * @param m number of vertices of the second polygon
     */
    void ConvexIntersection(std::vector<Vertex*>& p, std::vector<Vertex*>& q, int n, int m);
    /**
     * computes the dot Product
     * @param vP
     * @param vQ
     * @return dotproduct of vP and vQ
     */
    double dot(Point2D& vP, Point2D& vQ);

};
}

#endif // CONVEXCLIP_H

