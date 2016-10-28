#include "asite.h"
using namespace voronoi;

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
static double ASite::projectZ(double x,double y,double weight)
{
    return x*x+y*y-weight;
}

/* (non-Javadoc)
* @see j2d.iSite#getPolygon()
*/
PolygonSimple ASite::getPolygon()
{
    return polygon;
}

const char* ASite::toString()
{
    return "("+ getX()+"\t,"+getY()+","+weight+","+percentage+")+z: "+z;
}

/* (non-Javadoc)
* @see j2d.iSite#setPolygon(j2d.NPoly)
*/
void ASite::setPolygon(PolygonSimple poly)
{
    polygon=poly;
}

/* (non-Javadoc)
* @see j2d.iSite#setNeighbours(java.util.ArrayList)
*/
void ASite::setNeighbours(ArrayList<Site> list)
{
    this.setOldNeighbors(neighbours);
    neighbours=list;
}

/* (non-Javadoc)
* @see j2d.iSite#getNeighbours()
*/
ArrayList<Site> ASite::getNeighbours()
{
    return neighbours;
}

/**
*
* @param x x-coordinate
* @param y y-coordinate
*/
ASite::ASite(double x, double y)
{
    super(x,y,projectZ(x, y, nearlyZero));
    this.weight=nearlyZero;
}

/**
*
* @param x x-coordinate
* @param y y-coordinate
* @param weight the weight of the site, where weight=radius*radius which is used for weighting
*/
ASite::ASite(double x, double y, double weight)
{
    super(x,y,projectZ(x, y, weight));
    this.weight=weight;
}

ASite::ASite(double x, double y, double weight, double percentage){
    super(x,y,projectZ(x, y, weight));
    this.weight=weight;
    this.percentage=percentage;

}

/* (non-Javadoc)
* @see j2d.iSite#getWeight()
*/
double ASite::getWeight()
{
    return weight;
}

/* (non-Javadoc)
* @see j2d.iSite#setWeight(double)
*/
void ASite::setWeight(double weight)
{
    this.weight=weight;
    project();
}

/* (non-Javadoc)
* @see j2d.iSite#setXY(double, double)
*/
void ASite::setXY(double x, double y)
{
    this.x=x;
    this.y=y;
    project();
}

/* (non-Javadoc)
* @see j2d.iSite#setXYW(double, double, double)
*/
void ASite::setXYW(double x,double y, double weight)
{
    this.x=x;
    this.y=y;
    this.weight=weight;
    z=projectZ(x, y, weight);
}
private void ASite::project()
{
    z=projectZ(x, y, weight);
}

/* (non-Javadoc)
* @see j2d.iSite#setX(double)
*/
void ASite::setX(double x)
{
    this.x = x;
    project();
}
/* (non-Javadoc)
 * @see j2d.iSite#setY(double)
 */
void ASite::setY(double y)
{
    this.y = y;
    project();
}

/**
* The sites are first ordered according to the x-axis and then according to the y axis
*/
@Override
int ASite::compareTo(Site b)
{
    if (this.x<b.x) return -1;
    if (this.x>b.x) return 1;
    if (this.x==b.x)
    {
        if (this.y<b.y) return -1;
        if (this.y>b.y) return 1;
        if (this.y==b.y)return 0;
    }
    return 0;
}

void ASite::paint(Graphics2D g)
{
    int radius = (int)Math.sqrt(this.getWeight());
    //	g.fillOval((int)this.getX()-radius, (int)this.getY()-radius, 2*radius, 2*radius);
    g.setColor(Colors.circleBorder);
    g.drawOval((int)this.getX()-radius, (int)this.getY()-radius, 2*radius, 2*radius);
    g.setColor(Color.yellow);
    int width=1;
    g.fillRect((int)this.getX()-width, (int)this.getY()-width, 2*width,2*width );
}



/* (non-Javadoc)
* @see j2d.iSite#setPercentage(double)
*/
void ASite::setPercentage(double percentage)
{
    this.percentage = percentage;
}
/* (non-Javadoc)
* @see j2d.iSite#getPercentage()
*/
double ASite::getPercentage()
{
    return percentage;
}

/* (non-Javadoc)
* @see j2d.iSite#getPoint()
*/
Point2D ASite::getPoint()
{
    return new Point2D(getX(),getY());
}

/**
* Euclid
* @param point
* @return
*/
double ASite::distance(Site point)
{
    double dx = x-point.getX();
    double dy = y-point.getY();
    return Math.sqrt(dx*dx+dy*dy);
}

double ASite::distanceCircles(Site point)
{
    double dx = x-point.getX();
    double dy = y-point.getY();
    double radius1 = Math.sqrt(weight);
    double radius2=Math.sqrt(point.weight);
    return Math.sqrt(dx*dx+dy*dy)-radius1-radius2;
}


ArrayList<Site> ASite::getOldNeighbors()
{
    return oldNeighbors;
}
void ASite::setOldNeighbors(ArrayList<Site> oldNeighbors)
{
    this.oldNeighbors = oldNeighbors;
}
