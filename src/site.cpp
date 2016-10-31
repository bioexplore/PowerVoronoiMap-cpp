#include "site.h"
using namespace voronoi;

voronoi::Site::Site(double x, double y):AbstractSite(x,y)
{}

voronoi::Site::Site(double x, double y, double weight):AbstractSite(x,y,weight)
{}

voronoi::Site::Site(double x, double y, double weight, double percentage):
    AbstractSite(x,y,weight,percentage)
{}

voronoi::VoroCellObject* voronoi::Site::getData()
{
    return data_;
}

voronoi::Site voronoi::Site::clone()
{
    Site site(x, y, weight_);
    site.isDummy=isDummy;
    //site.originalObject_=originalObject_;
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

void voronoi::Site::setData(VoroCellObject* child)
{
   data_=child;
}

void voronoi::Site::setLastIncrease(double increase)
{
   lastIncrease_=increase;
}

/*
void voronoi::Site::paintLastIncrease(Graphics2D g, double radSize)
{
   int radius = (int) ((lastIncrease-1)*radSize);

   if(radius>=0)
   g.setColor(new Color(0,0,128,128));
   else g.setColor(new Color(0,255,0,128));
   radius=Math.abs(radius);
   radius=Math.min((int)radSize, radius);
   g.fillOval((int)getX()-radius, (int)getY()-radius, 2*radius, 2*radius);
   g.setColor(Color.BLUE);
   g.drawOval((int)getX()-radius, (int)getY()-radius, 2*radius, 2*radius);
   g.setColor(Color.black);
   int width=1;
   g.fillRect((int)getX()-width, (int)getY()-width, 2*width,2*width );
}*/

double voronoi::Site::getLastIncrease()
{
   return lastIncrease_;
}
