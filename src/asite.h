#ifndef ASITE_H
#define ASITE_H
#include "vertex.h"
#include "polygonsimple.h"
#include "vorocellobject.h"
/**
 * A site is a 2 dimensional point which has a weight.
 *
 * @author Arlind Nocaj
 *
 */
namespace voronoi {
class Site;
class AbstractSite:public Vertex
{
public:
    /**
     * Preflow Extrapolation
     */
    Point2D                 preflowVector;
    PolygonSimple*          nonClippedPolygon;  //

    AbstractSite(double x, double y);
    AbstractSite(double x, double y, double weight);
    AbstractSite(double x, double y, double weight, double percentage);
    ~AbstractSite();

    const char*             toString();

    PolygonSimple*          getPolygon();
    void                    setPolygon(PolygonSimple* poly);
    void                    setNeighbours(std::vector<Site*>* list);
    std::vector<Site*>*     getNeighbours();

    double          getWeight();
    void            setWeight(double weight);
    void            setXY(double x, double y);
    void            setXYW(double x,double y, double weight);
    void            project();
    void            setX(double x);
    void            setY(double y);
    /**
     * The sites are first ordered according to the x-axis and then according to the y axis
     */
    int             compareTo(Site* b);
    void            setPercentage(double percentage);
    double          getPercentage();

    //get cellobject pointer
    VoroCellObject*     getCellObject();
    VoroCellObject*     getData();
    void                setData(VoroCellObject*);

    Point2D             getPoint();
    double              distance(Site* point);
    double              distanceCircles(Site* point);
    std::vector<Site*>* getOldNeighbours();

protected:
    constexpr static double nearlyZero_ = 1E-10;
    double          weight_;//=nearlyZero;
    double          percentage_;//=nearlyZero;

    PolygonSimple*          polygon_;           //created           [Retrieve]  
    std::vector<Site*>*     neighbours_;        //create            [Retrieve]
    VoroCellObject*         cellObject_;        //passed by reference [No Retrieve]
private:
    std::vector<Site*>*     oldNeighbours_;     // no create but    [Retrieve]

    void                    setOldNeighbors(std::vector<Site*>* oldNeighbours);
    /**
     * return the dual point of the plane which is projected, see Aurenhammer,1987, Power Diagrams
     * @param x
     * @param y
     * @param weight
     * @return
     */
    static double           projectZ(double x,double y,double weight);
};
}
#endif // ASITE_H
