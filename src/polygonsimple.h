#ifndef POLYGONSIMPLE_H
#define POLYGONSIMPLE_H
#include <vector>
#include "rectangle2d.h"
#include "point2d.h"
#include "vertex.h"
namespace voronoi {
/**
 * Implements a simple polygon with one continous region, by using two double arrays.
 * @author Arlind Nocaj
 *
 */
class PolygonSimple:public std::vector<Point2D>
{
public:
    PolygonSimple();
    int length();

    /**
     *
     * @param numberPoints initial array size, default initial array size is 16.
     */
    PolygonSimple(int numberPoints);

    /**
     *
     * @param xPoints x-coordinate of the polygon points
     * @param yPoints y-coordinate of the polygon points
     * @param length number of elements which should be considered from the given arrays
     */
    PolygonSimple(double xPoints[], double yPoints[], int length );

    /**
     * tests whether the given point is contained in the polygon (linear time).
     */
    bool contains(double inX, double inY);

    /**
     * {@link #contains(double, double, double, double)}
     */
    bool contains(Rectangle2D r);

    /**
     * {@link #contains(double, double)}
     */
    bool contains(Point2D p);

    /**
     * tests whether each corner point of the given rectangle is contained in the polygon.
     */
    //@Override
    bool contains(double x, double y, double w, double h);

    //@Override
    Rectangle2D* getBounds2D();

    /**
     * tests whether the given rectangle will intersect to the bounds of the polygon.
     */
    //@Override
    bool intersects(Rectangle2D& r);

    /**
     * {@link #intersects(Rectangle2D)}
     */
    //@Override
    bool intersects(double x, double y, double w, double h);

    /**
     * Returns the bounding rectangle of this polygon.
     */
    Rectangle2D* getBounds();

    /**
     * Return the number of points in this polygon.
     * @return integer number of points
     */
    int getNumPoints();

    /**
     * Adds a point to the polygon. Extends the corresponding array if necessary.
     */
    void add(double x, double y);

    /**
     * Scales all points by multiplying with the scalingFactor
     *
     * @param scalingFactor
     */
    void scale(double scalingFactor);

    /**
     * Translates all points of the polygon by adding the values
     * @param tx translation on x
     * @param ty translation on y
     */
    void translate(double tx, double ty);

    void clearCacheOnly();

    /**
     * {@link #add(double, double)}
     */
    void add(Point2D p);


    /**
     * Uses the linear time algorithm of O'Rourke to compute the intersection of
     * two convex polygons.
     *
     * @param poly
     * @return
     */
    PolygonSimple* convexClip(PolygonSimple* poly);

    /**
     * Returns the area of the polygon.
     */
    double getArea();

    /**
     * For the given point, the minimal distance to the segments of the polygon
     * is computed.
     *
     * @param x
     * @param y
     * @return
     */
    double getMinDistanceToBorder(double x, double y);

    /**
     * Computes the centroid of a polygon.
     *
     * @return centroid point
     */
    Point2D* getCentroid();

    //@Override
    PolygonSimple* clone();

    /**
     * Default percentage can be 0.96
     *
     * @param percentage
     */
    void shrinkForBorder(double percentage);

    /**
     * We get a vector which describes where the point should be relative to the
     * center. We change the length of the vector so that the point fits in the
     * polygon. (reimplementation needed here)
     *
     * @return Point which is contained by this polygon and has same direction
     *         as the given vector point
     */
    Point2D getRelativePosition(Point2D& vector);

        /**
     * Returns a random point in the polygon.
     * @return
     */
    Point2D getInnerPoint();


    /**
     * Array with x-values of the polygon points.
     * @return
     */
    std::vector<double> getXPoints();

    /**
     * Array with y-values of the polygon points.
     * @return
     */
    std::vector<double> getYPoints();

    /**
     * If the polygon is modified by e.g. shrinking, this method returns the original polygon. If the polyogn was not modified, it can return null.
     * @return
     */
    PolygonSimple* getOriginalPolygon();
    //@Override
    //Iterator<Point2D> iterator();
    //int[] getXpointsClosed();
    //int[] getYpointsClosed();    
protected:
    /**
     * Used for generation of a random point in a polygon.
     */
    //Random seed = new Random(5);

    /**
     * centroid of the polygon is stored for faster access, once it is computed
     */
    Point2D* 	    centroid_;
    double 	        area_;
    Rectangle2D*    bounds_;

    /**
     * Stores the orginal polygon result, without shrinking
     */
    PolygonSimple*  oldPolygon_;

private:
    //int[] getPointsClosed(std::vector<double> values);
    int* getPointsClosed(std::vector<double> values);

    /**
     * Debugging only
     * @param poly
     */
    Point2D* containsPoly(PolygonSimple* poly);

    //VertexList getVertexList();
    std::vector<Point2D>* getVertexList();

    /**
     * Computes the intersection point iff (inx,iny) is within the polygon and (outx,outy) is outside of the polygon.
     * Returns null if the is no intersection or the line crosses the complete polygon.
     * @param inx x-coordinate of the presumably inner point
     * @param iny y-coordiante of the presumably inner point
     * @param outx x-coordinate of the presumably outer point
     * @param outy y-coordinate of the presumably outer point
     * @return Intersection Point
     */
    Point2D* getIntersectionWithPolygon(double inx, double iny, double outx, double outy);

    /**
     * intersection of two lines formed by the given points:
     * http://paulbourke.net/geometry/lineline2d/
     *
     * @param p1
     * @param p2
     * @param p3
     * @param p4
     * @return
     */
    Point2D* getIntersection(Point2D& p1, Point2D& p2,Point2D& p3, Point2D& p4);
    
    /**
     * Return the intersection of the segment given bei p1 and p2 and the line
     * given by p3 and p4. intersection:
     * http://paulbourke.net/geometry/lineline2d/
     *
     * @param p1
     * @param p2
     * @param p3
     * @param p4
     * @return
     */
    static Point2D* getIntersectionOfSegmentAndLine(Point2D p1, Point2D p2,Point2D p3, Point2D p4);
    
    /**
     * We get a vector which describes where the point should be relative to the
     * center. We change the length of the vector so that the point fits in the
     * polygon. alpha is the percentage of the point when considering the line
     * to the border.
     *
     * @return Point which is contained by this polygon and has same direction
     *         as the given vector point
     */
    Point2D getRelativePosition(Point2D vector, double alphaLine);

    void generateDefaultPolygon(int);

};
}

#endif // POLYGONSIMPLE_H
