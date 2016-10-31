#include "site.h"

/**
*
* @param x
* @param y
* @param weight
* @return
*/

/*
* return the dual point of the plane which is projected, see Aurenhammer,1987, Power Diagrams
*/
double voronoi::AbstractSite::projectZ(double x,double y,double weight)
{
    return x*x+y*y-weight;
}

/* (non-Javadoc)
* @see j2d.iSite#getPolygon()
*/
voronoi::PolygonSimple* voronoi::AbstractSite::getPolygon()
{
    return polygon_;
}
//TODO:need modify
const char* voronoi::AbstractSite::toString()
{
    char* buf=0;
    buf=new char[120];
    std::sprintf(buf,"(%f,%f,%f)z:%f",x,y,weight_,z);
    return buf;
}

/* (non-Javadoc)
* @see j2d.iSite#setPolygon(j2d.NPoly)
*/
void voronoi::AbstractSite::setPolygon(voronoi::PolygonSimple* poly)
{
    polygon_=poly;
}

/* (non-Javadoc)
* @see j2d.iSite#setNeighbours(java.util.std::vector)
*/
void voronoi::AbstractSite::setNeighbours(std::vector<voronoi::Site*>* list)
{
    setOldNeighbors(neighbours_);
    neighbours_=list;
}

/* (non-Javadoc)
* @see j2d.iSite#getNeighbours()
*/
std::vector<voronoi::Site*>* voronoi::AbstractSite::getNeighbours()
{
    return neighbours_;
}

/**
*
* @param x x-coordinate
* @param y y-coordinate
*/
voronoi::AbstractSite::AbstractSite(double x, double y):
    Vertex(x,y,projectZ(x,y,nearlyZero_)),weight_(nearlyZero_),percentage_(nearlyZero_),cellObject_(NULL),
    neighbours_(NULL),oldNeighbours_(NULL),polygon_(NULL),nonClippedPolygon(NULL),preflowVector()
{}

/**
*
* @param x x-coordinate
* @param y y-coordinate
* @param weight the weight of the site, where weight=radius*radius which is used for weighting
*/
voronoi::AbstractSite::AbstractSite(double x, double y, double weight):
    Vertex(x,y,projectZ(x,y,weight)),weight_(weight),percentage_(nearlyZero_),cellObject_(NULL),
    neighbours_(NULL),oldNeighbours_(NULL),polygon_(NULL),nonClippedPolygon(NULL),preflowVector()
{}

voronoi::AbstractSite::AbstractSite(double x, double y, double weight, double percentage):
    Vertex(x,y,projectZ(x,y,weight)),weight_(weight),percentage_(percentage),cellObject_(NULL),
    neighbours_(NULL),oldNeighbours_(NULL),polygon_(NULL),nonClippedPolygon(NULL),preflowVector()
{}

/* (non-Javadoc)
* @see j2d.iSite#getWeight()
*/
double voronoi::AbstractSite::getWeight()
{
    return weight_;
}

/* (non-Javadoc)
* @see j2d.iSite#setWeight(double)
*/
void voronoi::AbstractSite::setWeight(double weight)
{
    weight_=weight;
    project();
}

/* (non-Javadoc)
* @see j2d.iSite#setXY(double, double)
*/
void voronoi::AbstractSite::setXY(double x, double y)
{
    this->x=x;
    this->y=y;
    project();
}

/* (non-Javadoc)
* @see j2d.iSite#setXYW(double, double, double)
*/
void voronoi::AbstractSite::setXYW(double x,double y, double weight)
{
    this->x=x;
    this->y=y;
    weight_=weight;
    z=projectZ(x, y, weight);
}
void voronoi::AbstractSite::project()
{
    z=projectZ(x, y, weight_);
}

/* (non-Javadoc)
* @see j2d.iSite#setX(double)
*/
void voronoi::AbstractSite::setX(double x)
{
    this->x = x;
    project();
}
/* (non-Javadoc)
 * @see j2d.iSite#setY(double)
 */
void voronoi::AbstractSite::setY(double y)
{
    this->y = y;
    project();
}

/**
* The sites are first ordered according to the x-axis and then according to the y axis
*/
//@Override
int voronoi::AbstractSite::compareTo(Site* b)
{
    if (x<b->x) return -1;
    if (x>b->x) return 1;
    if (x==b->x)
    {
        if (y<b->y) return -1;
        if (y>b->y) return 1;
        if (y==b->y)return 0;
    }
    return 0;
}

/* (non-Javadoc)
* @see j2d.iSite#setPercentage(double)
*/
void voronoi::AbstractSite::setPercentage(double percentage)
{
    percentage_ = percentage;
}
/* (non-Javadoc)
* @see j2d.iSite#getPercentage()
*/
double voronoi::AbstractSite::getPercentage()
{
    return percentage_;
}

/* (non-Javadoc)
* @see j2d.iSite#getPoint()
*/
voronoi::Point2D voronoi::AbstractSite::getPoint()
{
    return Point2D(getX(),getY());
}

/**
* Euclid
* @param point
* @return
*/
double voronoi::AbstractSite::distance(voronoi::Site* point)
{
    double dx = x-point->getX();
    double dy = y-point->getY();
    return std::sqrt(dx*dx+dy*dy);
}

//get cellobject
voronoi::VoroCellObject* getCellObject()
{
    return cellObject_;
}

double voronoi::AbstractSite::distanceCircles(voronoi::Site* point)
{
    double dx = x-point->getX();
    double dy = y-point->getY();
    double radius1 = std::sqrt(weight_);
    double radius2=std::sqrt(point->weight_);
    return std::sqrt(dx*dx+dy*dy)-radius1-radius2;
}


std::vector<voronoi::Site*>* voronoi::AbstractSite::getOldNeighbours()
{
    return oldNeighbours_;
}
//TODO: should delete oldNeighbors originally occupied memory
void voronoi::AbstractSite::setOldNeighbors(std::vector<voronoi::Site*>* oldNeighbors)
{
    if(oldNeighbours_) delete oldNeighbours_;
    oldNeighbours_ = oldNeighbors;
}
