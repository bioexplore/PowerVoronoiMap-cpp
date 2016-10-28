#ifndef GEOMETRY_H
#define GEOMETRY_H
namespace voronoi {
/**
 * Helper class for sum small computations.
 * @author Nocaj
 *
 */
class Geometry
{
public:
    static double crossProduct(final Point2D p1,final Point2D p2);

    /**
     * * Shortest Distance from a Point p3 to a line segment p1-p2
     * http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/

     * @param x1 x value of first segment point
     * @param y1 y value of first segment point
     * @param x2 x value of second segment point
     * @param y2 y value of second segment point
     * @param x3 x value of point we are interested in
     * @param y3 y value of point we are interested in
     * @return
     */
    static double distancePointToSegment(final double x1,final double y1,final double x2,final double y2,final double x3, final double y3);
};
}
#endif // GEOMETRY_H
