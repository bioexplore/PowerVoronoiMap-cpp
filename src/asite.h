#ifndef ASITE_H
#define ASITE_H

/**
 * A site is a 2 dimensional point which has a weight.
 *
 * @author Arlind Nocaj
 *
 */
namespace voronoi {
class ASite extends JVertex implements Comparable<Site>, iSite
{
public:
    PolygonSimple nonClippedPolyon;

    /**
     * Preflow Extrapolation
     */
    Point2D preflowVector;

    /**
      *
      * @param x x-coordinate
      * @param y y-coordinate
      */
    ASite(double x, double y);
    /**
      *
      * @param x x-coordinate
      * @param y y-coordinate
      * @param weight the weight of the site, where weight=radius*radius which is used for weighting
      */
    ASite(double x, double y, double weight);
    ASite(double x, double y, double weight, double percentage);

    /* (non-Javadoc)
     * @see j2d.iSite#getPolygon()
     */
    PolygonSimple getPolygon();

    const char* toString();
    /* (non-Javadoc)
     * @see j2d.iSite#setPolygon(j2d.NPoly)
     */
    void setPolygon(PolygonSimple poly);
    /* (non-Javadoc)
     * @see j2d.iSite#setNeighbours(java.util.ArrayList)
     */
    void setNeighbours(ArrayList<Site> list);
    /* (non-Javadoc)
     * @see j2d.iSite#getNeighbours()
     */
    ArrayList<Site> getNeighbours();

    /* (non-Javadoc)
     * @see j2d.iSite#getWeight()
     */
    double getWeight();
    /* (non-Javadoc)
     * @see j2d.iSite#setWeight(double)
     */
    void setWeight(double weight);
    /* (non-Javadoc)
     * @see j2d.iSite#setXY(double, double)
     */
    void setXY(double x, double y);
    /* (non-Javadoc)
     * @see j2d.iSite#setXYW(double, double, double)
     */
    void setXYW(double x,double y, double weight);
    void project();
    /* (non-Javadoc)
     * @see j2d.iSite#setX(double)
     */
    void setX(double x);
    /* (non-Javadoc)
     * @see j2d.iSite#setY(double)
     */
    void setY(double y);
    /**
     * The sites are first ordered according to the x-axis and then according to the y axis
     */
    int compareTo(Site b);
    void paint(Graphics2D g);
    /* (non-Javadoc)
     * @see j2d.iSite#setPercentage(double)
     */
    void setPercentage(double percentage);
    /* (non-Javadoc)
     * @see j2d.iSite#getPercentage()
     */
    double getPercentage();

    /* (non-Javadoc)
     * @see j2d.iSite#getPoint()
     */
    Point2D getPoint();
    /**
     * Euclid
     * @param point
     * @return
     */
    double distance(Site point);
    double distanceCircles(Site point);
    ArrayList<Site> getOldNeighbors();

protected:
    const static double nearlyZero = 1E-10;
    double weight=nearlyZero;
    double percentage=nearlyZero;

    PolygonSimple polygon;
    ArrayList<Site> neighbours;

    VoroCellObject cellObject;
private:
    ArrayList<Site> oldNeighbors_;
    void setOldNeighbors(ArrayList<Site> oldNeighbors);
    /**
     * return the dual point of the plane which is projected, see Aurenhammer,1987, Power Diagrams
     * @param x
     * @param y
     * @param weight
     * @return
     */
    static double projectZ(double x,double y,double weight);
};
}
#endif // ASITE_H
