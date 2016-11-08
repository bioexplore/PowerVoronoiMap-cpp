#ifndef SITE_H
#define SITE_H
#include "asite.h"
#include "vorocellobject.h"
/**
 * Extended Version of a site, which has relative Positions
 * @author Arlind Nocaj
 *
 */
namespace voronoi {
class Site: public AbstractSite
{
public:
    friend class VoroNode;
    /**
     * This attribute is used to mark the sites, which are only added to bound the Voronoi diagram.
     */
    bool            isDummy;

    Site(double x, double y);
    Site(double x, double y, double weight);
    Site(double x, double y, double weight, double percentage);
    Site            clone();
    Site            cloneZeroWeight();

    //VoroCellObject* getData();//moved to AbstractSite
    const char*     toString();
    void            setAsDummy();
    //void            setData(VoroCellObject* child);//moved to AbstractSite
    void            setLastIncrease(double increase);
    //TODO:need to move to vtk
    //void paintLastIncrease(Graphics2D g, double radSize);
    double          getLastIncrease();
private:
    //VoroCellObject* data_;//merged with AbstractSite cellObject_
    double          lastIncrease_;
    double          deltaX_;
    double          deltaY_;
    double          alpha_;
};
}
#endif // SITE_H
