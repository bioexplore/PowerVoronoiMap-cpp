#include "site.h"
using namespace voronoi;

voronoi::Site::Site(double x, double y):
    AbstractSite(x,y),alpha_(0.0),deltaX_(0.0),deltaY_(0.0),lastIncrease_(0.0),isDummy(false)
{}

voronoi::Site::Site(double x, double y, double weight):
    AbstractSite(x,y,weight),alpha_(0.0),deltaX_(0.0),deltaY_(0.0),lastIncrease_(0.0),isDummy(false)
{}

voronoi::Site::Site(double x, double y, double weight, double percentage):
    AbstractSite(x,y,weight,percentage),alpha_(0.0),deltaX_(0.0),deltaY_(0.0),lastIncrease_(0.0),isDummy(false)
{}

voronoi::Site voronoi::Site::clone()
{
    Site site(x, y, weight_);
    site.isDummy=isDummy;
    site.percentage_=percentage_;
    return site;
}

voronoi::Site voronoi::Site::cloneZeroWeight()
{
   Site site(x, y, 0);
   return site;
}

void voronoi::Site::setAsDummy()
{
   isDummy=true;
}

void voronoi::Site::setLastIncrease(double increase)
{
   lastIncrease_=increase;
}

double voronoi::Site::getLastIncrease()
{
   return lastIncrease_;
}
