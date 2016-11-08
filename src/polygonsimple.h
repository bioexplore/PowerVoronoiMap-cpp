#ifndef POLYGONSIMPLE_H
#define POLYGONSIMPLE_H
#include <vector>
#include <iostream>
#include "rectangle2d.h"
#include "point2d.h"
namespace voronoi {
/**
 * Implements a simple polygon with one continous region.
 * @author Jianye Xia 
 *
 */
class PolygonSimple:public std::vector<Point2D>
{
public:
    PolygonSimple();
    PolygonSimple(int numberPoints);
    PolygonSimple(double xPoints[], double yPoints[], int length);
    ~PolygonSimple();

    bool            contains(double inX, double inY);
    bool            contains(Rectangle2D r);
    bool            contains(Point2D p);
    bool            contains(double x, double y, double w, double h);
    bool            intersects(Rectangle2D& r);
    bool            intersects(double x, double y, double w, double h);

    int             length();
    int             getNumPoints();
    double          getArea();
    double          getMinDistanceToBorder(double x, double y);
    Point2D*        getCentroid();
    Point2D         getRelativePosition(Point2D& vector);
    Point2D         getInnerPoint();
    Rectangle2D*    getBounds();
    Rectangle2D*    getBounds2D();
    PolygonSimple*  getOriginalPolygon();

    std::vector<double> getXPoints();
    std::vector<double> getYPoints();
    
    void            add(double x, double y);
    void            add(Point2D& p);
    void            scale(double scalingFactor);
    void            translate(double tx, double ty);
    void            clearCacheOnly();
    void            shrinkForBorder(double percentage);
    PolygonSimple*  clone();
    PolygonSimple*  convexClip(PolygonSimple* poly);

#ifdef JYXIA
    void            printSelf();
#endif

protected:
    double 	        area_;
    Point2D* 	    centroid_;
    Rectangle2D*    bounds_;

    PolygonSimple*  oldPolygon_;

private:
    //int[] getPointsClosed(std::vector<double> values);
    int*            getPointsClosed(std::vector<double> values);

    /**
     * Debugging only
     * @param poly
     */
    Point2D*        containsPoly(PolygonSimple* poly);

    //VertexList getVertexList();
    std::vector<Point2D>*   getVertexList();

    /**
     * Computes the intersection point iff (inx,iny) is within the polygon and (outx,outy) is outside of the polygon.
     * Returns null if the is no intersection or the line crosses the complete polygon.
     * @param inx x-coordinate of the presumably inner point
     * @param iny y-coordiante of the presumably inner point
     * @param outx x-coordinate of the presumably outer point
     * @param outy y-coordinate of the presumably outer point
     * @return Intersection Point
     */
    Point2D*        getIntersectionWithPolygon(double inx, double iny, double outx, double outy);

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
    Point2D*        getIntersection(Point2D& p1, Point2D& p2,Point2D& p3, Point2D& p4);
    
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
    Point2D         getRelativePosition(Point2D vector, double alphaLine);

    void            generateDefaultPolygon(int);
    double          getDistance(int edegIndex,Point2D& p);//Helper function for calculating distance from point p to the edgeIndex edge fragment

};
}

#endif // POLYGONSIMPLE_H
