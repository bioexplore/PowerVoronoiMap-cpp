#include "site.h"
using namespace voronoi;

Site::Site(double x, double y):ASite(x,y)
{}

Site::Site(double x, double y, double weight):ASite(x,y,weight)
{}

Site::Site(double x, double y, double weight, double percentage)
{
    super(x,y,weight,percentage);
}

Object Site::getData()
{
    return data_;
}

Site Site::clone()
{
    Site site=new Site(x, y, weight);
    site.isDummy=this.isDummy;
    site.originalObject=this.originalObject;
    site.percentage=this.percentage;
    return site;
}

Site Site::cloneZeroWeight()
{
   Site site=new Site(x, y, 0);
   return site;
}

String Site::toString()
{
   // TODO Auto-generated method stub
   return super.toString();
}

void Site::setAsDummy()
{
   this.isDummy=true;
}

void Site::setData(Object child)
{
   this.data=child;
}

void Site::setLastIncrease(double increase)
{
   this.lastIncrease=increase;
}

void Site::paintLastIncrease(Graphics2D g, double radSize)
{
   int radius = (int) ((lastIncrease-1)*radSize);

   if(radius>=0)
   g.setColor(new Color(0,0,128,128));
   else g.setColor(new Color(0,255,0,128));
   radius=Math.abs(radius);
   radius=Math.min((int)radSize, radius);
   g.fillOval((int)this.getX()-radius, (int)this.getY()-radius, 2*radius, 2*radius);
   g.setColor(Color.BLUE);
   g.drawOval((int)this.getX()-radius, (int)this.getY()-radius, 2*radius, 2*radius);
   g.setColor(Color.black);
   int width=1;
   g.fillRect((int)this.getX()-width, (int)this.getY()-width, 2*width,2*width );
}

double Site::getLastIncrease()
{
   return lastIncrease;
}
