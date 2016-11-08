#include "site.h"

voronoi::AbstractSite::AbstractSite(double x, double y):
    Vertex(x,y,projectZ(x,y,nearlyZero_)),weight_(nearlyZero_),percentage_(nearlyZero_),cellObject_(NULL),
    neighbours_(NULL),oldNeighbours_(NULL),polygon_(NULL),nonClippedPolygon(NULL),preflowVector()
{}
voronoi::AbstractSite::AbstractSite(double x, double y, double weight):
    Vertex(x,y,projectZ(x,y,weight)),weight_(weight),percentage_(nearlyZero_),cellObject_(NULL),
    neighbours_(NULL),oldNeighbours_(NULL),polygon_(NULL),nonClippedPolygon(NULL),preflowVector()
{}
voronoi::AbstractSite::AbstractSite(double x, double y, double weight, double percentage):
    Vertex(x,y,projectZ(x,y,weight)),weight_(weight),percentage_(percentage),cellObject_(NULL),
    neighbours_(NULL),oldNeighbours_(NULL),polygon_(NULL),nonClippedPolygon(NULL),preflowVector()
{}

voronoi::AbstractSite::~AbstractSite()
{
    //if(nonClippedPolygon) delete nonClippedPolygon;
    if(polygon_) delete polygon_;
    if(neighbours_) delete neighbours_;
    if(oldNeighbours_) delete oldNeighbours_;
}
/*
* return the dual point of the plane which is projected, see Aurenhammer,1987, Power Diagrams
*/
//static private func
double voronoi::AbstractSite::projectZ(double x,double y,double weight)
{
    return x*x+y*y-weight;
}

voronoi::PolygonSimple* voronoi::AbstractSite::getPolygon()
{
    return polygon_;
}

const char* voronoi::AbstractSite::toString()
{
    char* buf=0;
    buf=new char[120];
    std::sprintf(buf,"(%f,%f,%f)z:%f",x,y,weight_,z);
    return buf;
}

void voronoi::AbstractSite::setPolygon(voronoi::PolygonSimple* poly)
{
    polygon_=poly;
}
//public func
std::vector<voronoi::Site*>* voronoi::AbstractSite::getOldNeighbours()
{
    return oldNeighbours_;
}
//private func
void voronoi::AbstractSite::setOldNeighbors(std::vector<voronoi::Site*>* oldNeighbors)
{
    if(oldNeighbours_) delete oldNeighbours_;
    oldNeighbours_ = oldNeighbors;
}

void voronoi::AbstractSite::setNeighbours(std::vector<voronoi::Site*>* list)
{
    setOldNeighbors(neighbours_);
    neighbours_=list;
}

std::vector<voronoi::Site*>* voronoi::AbstractSite::getNeighbours()
{
    return neighbours_;
}

double voronoi::AbstractSite::getWeight()
{
    return weight_;
}

void voronoi::AbstractSite::setWeight(double weight)
{
    weight_=weight;
    project();
}

void voronoi::AbstractSite::setXY(double x, double y)
{
    this->x=x;
    this->y=y;
    project();
}

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

void voronoi::AbstractSite::setX(double x)
{
    this->x = x;
    project();
}

void voronoi::AbstractSite::setY(double y)
{
    this->y = y;
    project();
}

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

void voronoi::AbstractSite::setPercentage(double percentage)
{
    percentage_ = percentage;
}

double voronoi::AbstractSite::getPercentage()
{
    return percentage_;
}

voronoi::Point2D voronoi::AbstractSite::getPoint()
{
    return Point2D(getX(),getY());
}

double voronoi::AbstractSite::distance(voronoi::Site* point)
{
    double dx = x-point->getX();
    double dy = y-point->getY();
    return std::sqrt(dx*dx+dy*dy);
}

voronoi::VoroCellObject* voronoi::AbstractSite::getCellObject()
{
    return cellObject_;
}

voronoi::VoroCellObject* voronoi::AbstractSite::getData()
{
    return cellObject_;
}

void voronoi::AbstractSite::setData(VoroCellObject* obj)
{
    cellObject_=obj;
}

double voronoi::AbstractSite::distanceCircles(voronoi::Site* point)
{
    double dx = x-point->getX();
    double dy = y-point->getY();
    double radius1 = std::sqrt(weight_);
    double radius2=std::sqrt(point->weight_);
    return std::sqrt(dx*dx+dy*dy)-radius1-radius2;
}
