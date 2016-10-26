#ifndef SITE_H
#define SITE_H

/**
 * Extended Version of a site, which has relative Positions
 * @author Arlind Nocaj
 *
 */
namespace voronoi {
class Site: public ASite
{
public:
    /**
     * This attribute is used to mark the sites, which are only added to bound the Voronoi diagram.
     */
    bool isDummy;

    /**
     *
     * @param x x-coordinate
     * @param y y-coordinate
     */
    Site(double x, double y);
    /**
     *
     * @param x x-coordinate
     * @param y y-coordinate
     * @param weight the weight of the site, where weight=radius*radius which is used for weighting
     */
    Site(double x, double y, double weight);
    Site(double x, double y, double weight, double percentage);
    Site clone();
    Site cloneZeroWeight();

    Object getData();
    const char* toString();
    void setAsDummy();
    void setData(Object child);
    void setLastIncrease(double increase);
    void paintLastIncrease(Graphics2D g, double radSize);
    double getLastIncrease();
private:
    Object data_;
    double lastIncrease_;
    double deltaX_;
    double deltaY_;
    double alpha_;
};
}
#endif // SITE_H
