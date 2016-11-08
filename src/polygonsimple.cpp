#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "polygonsimple.h"

#ifdef JYXIA
void voronoi::PolygonSimple::printSelf()
{
    std::cout<<"Polygon info:"<<std::endl
            <<"\tSize:\t"<<size()<<std::endl
            <<"\tCentroid:\t["<<centroid_->x<<","<<centroid_->y<<"]"<<std::endl
            <<"\tArea:    \t"<<area_<<std::endl
            <<"\tBoding box:\t["<<bounds_->getX()<<","<<bounds_->getY()<<",w"<<bounds_->getWidth()<<",h"<<bounds_->getHeight()<<"]"
            <<std::endl;
            if(oldPolygon_)
                std::cout<<"\tOldPolygon:\t[Addr]"<<oldPolygon_<<std::endl;

}
#endif
voronoi::PolygonSimple::PolygonSimple():
   std::vector<Point2D>(), area_(-1),bounds_(NULL),centroid_(NULL),oldPolygon_(NULL)
{}

voronoi::PolygonSimple::PolygonSimple(int numberPoints):
    std::vector<Point2D>(),area_(-1),bounds_(NULL),centroid_(NULL),oldPolygon_(NULL)
{
    if (numberPoints > 2)
    {
        generateDefaultPolygon(numberPoints);
    } else
    {
        generateDefaultPolygon(16);
    }
}
// The user who invoke this constructor should guarentee that length 
// do not exceed the memory occupied by xPoints and yPoints
voronoi::PolygonSimple::PolygonSimple(double xPoints[], double yPoints[], int length):
    std::vector<Point2D>(),area_(-1),bounds_(NULL),centroid_(NULL),oldPolygon_(NULL)
{
    Point2D p;
    for(int i=0;i<length;++i)
    {
        p.x=xPoints[i];
        p.y=yPoints[i];
        push_back(p);
    }
}

voronoi::PolygonSimple::~PolygonSimple()
{
    if(bounds_) delete bounds_;
    if(centroid_) delete centroid_;
    if(oldPolygon_) delete oldPolygon_;
    clear();
}

bool voronoi::PolygonSimple::contains(double inX, double inY)
{
    bool contains = false;
    if (bounds_ == NULL)
        getBounds();
    if (!(bounds_->contains(inX, inY))) 
    {
        return false;
    }
    int length=size();
    // Take a horizontal ray from (inX,inY) to the right.
    // If ray across the polygon edges an odd # of times, the point is
    // inside.
    for (int i = 0, j = length - 1; i < length; j = i++) 
    {
        if (( ((at(i).y <= inY) && (inY < at(j).y)) || ( (at(j).y <= inY) && (inY < at(i).y) ) ) 
                    && ( inX < (at(j).x - at(i).x) * (inY - at(i).y) / (at(j).y - at(i).y) + at(i).x))
            contains = !contains;
    }
    return contains;
}

bool voronoi::PolygonSimple::contains(Rectangle2D r)
{
    return contains(r.getX(), r.getY(), r.getWidth(), r.getHeight());
}

bool voronoi::PolygonSimple::contains(Point2D p)
{
    return contains(p.getX(), p.getY());
}

bool voronoi::PolygonSimple::contains(double x, double y, double w, double h)
{
    if (bounds_ == NULL)
        getBounds2D();
    if (bounds_->contains(x, y, w, h)) 
    {
        if (contains(x, y) && contains(x + w, y) && contains(x, y + h) && contains(x + w, y + h))
            return true;
        return false;
    } else
        return false;
}

voronoi::Rectangle2D* voronoi::PolygonSimple::getBounds2D()
{
    if (bounds_ == NULL) {
        getBounds();
    }
    return bounds_;
}

bool voronoi::PolygonSimple::intersects(Rectangle2D& r)
{
    if (bounds_==NULL)
    {
        getBounds();
    }
    return bounds_->intersects(r);
}

bool voronoi::PolygonSimple::intersects(double x, double y, double w, double h)
{
    if (bounds_==NULL)
    {
        getBounds();
    }
    return bounds_->intersects(x, y, w, h);
}

voronoi::Rectangle2D* voronoi::PolygonSimple::getBounds()
{
    if (bounds_ == NULL)
    {
        double xmin = std::numeric_limits<double>::max();
        double ymin = std::numeric_limits<double>::max();
        double xmax = std::numeric_limits<double>::min();
        double ymax = std::numeric_limits<double>::min();

        for(std::vector<Point2D>::iterator iter=begin();iter!=end();++iter)
        {
            double x=iter->x;
            double y=iter->y;
            if (x < xmin)
                xmin = x;
            if (x > xmax)
                xmax = x;
            if (y < ymin)
                ymin = y;
            if (y > ymax)
                ymax = y;
        }
        bounds_ = new Rectangle2D(xmin, ymin, (xmax - xmin),(ymax - ymin));
    }
    return bounds_;
}

int voronoi::PolygonSimple::getNumPoints()
{
    return size();
}

void voronoi::PolygonSimple::add(double x, double y)
{
    Point2D tmpPoint;
    tmpPoint.x=x;
    tmpPoint.y=y;
    push_back(tmpPoint);
}

void voronoi::PolygonSimple::scale(double scalingFactor)
{
    for(std::vector<Point2D>::iterator iter=begin();iter!=end();++iter)
        iter->scale(scalingFactor);
}

void voronoi::PolygonSimple::translate(double tx, double ty)
{
    for(std::vector<Point2D>::iterator iter=begin();iter!=end();++iter)
        iter->translate(tx,ty);
    clearCacheOnly();
}

void voronoi::PolygonSimple::clearCacheOnly()
{
    if(centroid_) delete centroid_;
    centroid_=NULL;
    if(bounds_) delete bounds_;
    bounds_=NULL;
    area_=-1;
    if(oldPolygon_!=NULL)
        oldPolygon_->clearCacheOnly();
}

void voronoi::PolygonSimple::add(Point2D& p)
{
    add(p.x, p.y);
}

voronoi::PolygonSimple* voronoi::PolygonSimple::convexClip(PolygonSimple* poly)
{
    if(!bounds_) getBounds();
    Rectangle2D* polyBox=poly->getBounds2D();
    //bounding box have to match for intersection
    if (!bounds_->intersects(*polyBox))
        return NULL;
    //check if bounding box corners are in polygon: then poly is contained completely inside the outer polygon
    if (contains(*polyBox))
        return poly->clone();

    //TODO: Need to modify
    return NULL;
    /*
    //bounding boxes intersect
    // to vertexList
    cVertexList list1 = this.getVertexList();
    cVertexList list2 = poly.getVertexList();
    ConvexClip clipper = new ConvexClip();
    //		list1.PrintVertices();
    //		list2.PrintVertices();
    clipper.Start(list1, list2);
    PolygonSimple res = new PolygonSimple();
    if (clipper.inters != null && clipper.inters.n > 0)
    {
        cVertex node = clipper.inters.head;
        double firstX = node.v.x;
        double firstY = node.v.y;
        res.add(node.v.x, node.v.y);
        double lastX = node.v.x;
        double lastY = node.v.y;
        for (int i = 1; i < clipper.inters.n; i++)
        {
            node = node.next;

            if (lastX != node.v.x || lastY != node.v.y) // do not add point
            {
                // if its the same as before
                if (i != (clipper.inters.n - 1) || (node.v.x != firstX)
                        || node.v.y != firstY) // do not add if it is the
                {
                    // end point and the same as
                    // the first point
                    res.add(node.v.x, node.v.y);
                }
            }
        }
        return res;
    }
    //no intersection between the two polygons, so check if one is inside the other
    if(contains(poly.x[0],poly.y[0]))
        return poly;

    // no intersection between the polygons at all
    return null;
    */
}


voronoi::Point2D* voronoi::PolygonSimple::containsPoly(PolygonSimple* poly)
{
    for(std::vector<Point2D>::iterator p=poly->begin();p!=poly->end();++p)
    {
        if(!contains(*p))
        {
            //System.out.println(p);
            return static_cast<Point2D*>(&(*p));
        }
    }
    return NULL;
}
std::vector<voronoi::Point2D>* voronoi::PolygonSimple::getVertexList()
{
    return this;
    /*
    cVertexList list = new cVertexList();
    for (int i = length - 1; i >= 0; i--)
    {
        cVertex vertex = new cVertex(x[i], y[i]);
        list.InsertBeforeHead(vertex);
    }
    return list;
    */
}

double voronoi::PolygonSimple::getArea()
{
    if (area_ > 0)
        return area_;

    double area = 0;
    // we can implement it like this because the polygon is closed
    // (point2D.get(0) = point2D.get(length + 1)
    int size = (this->size()-1);
    double x0=0.0,x1=0.0;
    double y0=0.0,y1=0.0;
    for (int i = 0; i < size; i++)
    {
        x0=at(i).x;
        y0=at(i).y;
        x1=at(i+1).x;
        y1=at(i+1).y;
        area += (x0 * y1 - x1 * y0);
    }
    x0=at(0).x;
    y0=at(0).y;
    area += (x1 * y0 - x0 * y1);
    area_ = std::abs(area) * 0.5;
    return area_;
}

//TODO:to be implemented
double voronoi::PolygonSimple::getMinDistanceToBorder(double x, double y)
{
    Point2D p(x,y);
    double result=getDistance(0,p);
    for (int i=1;i<size();i++)
    {
        double distance=getDistance(i,p);
        if(distance<result)
            result=distance;
    }
    return result;
}

voronoi::Point2D* voronoi::PolygonSimple::getCentroid()
{
    if (centroid_ == NULL)
    {
        double xv = 0;
        double yv = 0;
        double areaQuotient = getArea() * 6;
        int length=this->size()-1;
        double x0=0.0,y0=0.0;
        double x1=0.0,y1=0.0;
        for (int i = 0; i < length; i++)
        {
            x0=at(i).x;
            y0=at(i).y;
            x1=at(i+1).x;
            y1=at(i+1).y;
            xv += (x0+x1)*(x0*y1-x1*y0);
            yv += (y0+y1)*(x0*y1-x1*y0);
        }
        x0=at(length).x;
        y0=at(length).y;
        x1=at(0).x;
        y1=at(0).y;
        xv += (x0+x1)*(x0*y1-x1*y0);
        yv += (y0+y1)*(x0*y1-x1*y0);
        xv /= areaQuotient;
        yv /= areaQuotient;
        centroid_ = new Point2D(xv, yv);
    }
    return centroid_;
}

//TODO: need to define
voronoi::PolygonSimple* voronoi::PolygonSimple::clone()
{
    PolygonSimple* p = new PolygonSimple;
    for(Point2D point:*this)
    {
       p->push_back(point); 
    }
    p->area_=area_;
    p->getBounds();
    p->getCentroid();
    p->oldPolygon_=oldPolygon_;
    return p;
}

void voronoi::PolygonSimple::shrinkForBorder(double percentage)
{
    oldPolygon_ = (PolygonSimple*)this->clone();
    getCentroid();
    double cx = centroid_->getX();
    double cy = centroid_->getY();
    for(int i=0;i<this->size();++i)
    {
        double deltaX = at(i).x - cx;
        double deltaY = at(i).y - cy;
        double xnew = cx + deltaX * percentage;
        double ynew = cy + deltaY * percentage;
        at(i).x = xnew;
        at(i).y = ynew;
    }
}

voronoi::Point2D voronoi::PolygonSimple::getRelativePosition(voronoi::Point2D& vector)
{
    getCentroid();

    double endPointX = centroid_->getX() + vector.getX();
    double endPointY = centroid_->getY() + vector.getY();
    Point2D endPoint(endPointX, endPointY);
    if (contains(endPointX, endPointY))
    {
        return Point2D(endPointX, endPointY);
    } else
    {
        double endPointX2 = centroid_->getX() + vector.getX() * 0.85;
        double endPointY2 = centroid_->getY() + vector.getY() * 0.85;
        if (contains(endPointX2, endPointY2))
        {
            return Point2D(endPointX2, endPointY2);
        }
    }
    Point2D p1;
    Point2D p2=at(0);
    Point2D result;
    int length=this->size();
    for (int i = 1; i <= length; i++)
    {
        p1 = p2;
        //TODO Keine Ahnung ob richtig
        if(i == length)
        {
            p2.x=0.0;
            p2.y=0.0;
        }else
        {
            p2 = at(i);
        }
        Point2D* intersection = getIntersection(p1, p2, *centroid_, endPoint);
        if (intersection != NULL)
        {
            double deltaX = intersection->getX() - centroid_->getX();
            double deltaY = intersection->getY() - centroid_->getY();
            double e = intersection->distance(*centroid_);
            double minimalDistanceToBorder = 10;
            double alpha = (e - minimalDistanceToBorder) / e;
            if (contains(*centroid_)) {
                // make vector smaller
                result = Point2D(centroid_->getX() + deltaX * 0.8,
                                     centroid_->getY() + deltaY * 0.8);
            } else {
                // make vector longer
                result = Point2D(centroid_->getX() + deltaX * 1.1,
                                     centroid_->getY() + deltaY * ((1 - alpha) + 1));
            }
            if (contains(result)) {
                return result;
            }
        }
    }
    if (contains(result))
        return result;
    else
    {
        // System.out.println("Innerpoint");
        return getInnerPoint();
    }
}


voronoi::Point2D voronoi::PolygonSimple::getRelativePosition(voronoi::Point2D vector, double alphaLine)
{
    double dx = vector.getX();
    double dy = vector.getY();

    getCentroid();
    double centroidX = centroid_->getX();
    double centroidY = centroid_->getY();

    double endPointX = centroidX + dx;
    double endPointY = centroidY + dy;
    Point2D endPoint(endPointX, endPointY);

    Point2D p1;
    Point2D p2 =at(0); 
    Point2D result;
    int length=this->size();
    for (int i = 1; i < length; i++)
    {
        p1 = p2;
        p2 = at(i);
        Point2D* intersection = getIntersectionOfSegmentAndLine(p1, p2, *centroid_, endPoint);
        if (intersection != NULL)
        {
            double deltaX = intersection->getX() - centroidX;
            double deltaY = intersection->getY() - centroidY;
            //double e = intersection->distance(*centroid_);
            double nX = centroidX + deltaX * alphaLine;
            double nY = centroidY + deltaY * alphaLine;
            return Point2D(nX, nY);
        }
    }
   //FIXME:need define return 
    //System.out.println("Problem, relative Placement did not go right.");
    //return null;
}

voronoi::Point2D voronoi::PolygonSimple::getInnerPoint()
{
    std::srand(time(NULL));//initialize randome generator
    double randNum=((double)rand()/RAND_MAX);
    Rectangle2D* b = getBounds();
    double x = -1;
    double y = -1;
    do {
        x = b->getX() + randNum*b->getWidth();
        y = b->getY() + randNum*b->getHeight();
    } while (!contains(x, y));

    return Point2D(x, y);
}
//this funciton will apply memory for the returned Point2D, who invoke this func should retrieve the memory
voronoi::Point2D* voronoi::PolygonSimple::getIntersectionWithPolygon(double inx, double iny, double outx, double outy)
{
    double t,s,denum;
    Point2D* intersections = new Point2D[4];
    int k = 0;
    int length=this->size();
    double x0=0.0,y0=0.0,x1=0.0,y1=0.0;
    for(int i = 0, j = length - 1; i < length; j = i++)
    {
        x0=at(i).x;
        y0=at(i).y;
        x1=at(j).x;
        y1=at(j).y;
        denum = ((x0 - x1)*(iny - outy) - (inx - outx) *(y0 - y1));
        if(denum == 0)
            continue;
        t = (outx*(y0 - y1) - outy * (x0 - x1) + y1 * (x0 - x1) - x1* (y0 - y1))/denum;
        s = (outy* (inx - outx) + x1 *(iny - outy)  - outx * (iny - outy) - y1 *(inx - outx))/-denum;
        if(t > 0 && t <= 1 && s > 0 && s <= 1)
        {
            intersections[k++] = Point2D(outx + t*(inx - outx), outy + t*(iny - outy));
        }
    }
    if(k > 1)
        return NULL;
    else
        return intersections;
}
//FIXME:this func will return a new replied memory, who invoke this should retrive the memory after usage!!
voronoi::Point2D* voronoi::PolygonSimple::getIntersection(voronoi::Point2D& p1, voronoi::Point2D& p2, voronoi::Point2D& p3,
                                voronoi::Point2D& p4)
{
    // Bounding Box test
    double x1 = 0;
    double x2 = 0;
    double y1 = 0;
    double y2 = 0;

    double x3 = 0;
    double x4 = 0;
    double y3 = 0;
    double y4 = 0;

    if (p1.getX() < p2.getX())
    {
        x1 = p1.getX();
        x2 = p2.getX();
    } else
    {
        x1 = p2.getX();
        x2 = p1.getX();
    }
    if (p1.getY() < p2.getY())
    {
        y1 = p1.getY();
        y2 = p2.getY();
    } else
    {
        y1 = p2.getY();
        y2 = p1.getY();
    }

    if (p3.getX() < p4.getX())
    {
        x3 = p3.getX();
        x4 = p4.getX();
    } else
    {
        x3 = p4.getX();
        x4 = p3.getX();
    }
    if (p3.getY() < p4.getY())
    {
        y3 = p3.getY();
        y4 = p4.getY();
    } else
    {
        y3 = p4.getY();
        y4 = p3.getY();
    }

    //FIXME bounding box intersection needs to be corrected
    if (!(x2 >= x3 && x4 >= x1 && y2 >= y3 && y4 >= y1))
    {
        return NULL;
    }

    Point2D* n1 = new Point2D(p3.getX() - p1.getX(), p3.getY()
                             - p1.getY());
    Point2D* n2 = new Point2D(p2.getX() - p1.getX(), p2.getY()
                             - p1.getY());
    Point2D* n3 = new Point2D(p4.getX() - p1.getX(), p4.getY()
                             - p1.getY());
    Point2D* n4 = new Point2D(p2.getX() - p1.getX(), p2.getY()
                             - p1.getY());

    if ( n1->cross(n2) * n3->cross(n4) >= 0)
    {
        return NULL;
    }

    double denominator = (p4.getY() - p3.getY()) * (p2.getX() - p1.getX())
            - (p4.getX() - p3.getX()) * (p2.getY() - p1.getY());
    if (denominator == 0)
    {
        return NULL;//FIXME:need change to exception!!
        // return null;
        //throw new RuntimeException("Lines are parallel");
    }
    double ua = (p4.getX() - p3.getX()) * (p1.getY() - p3.getY())
            - (p4.getY() - p3.getY()) * (p1.getX() - p3.getX());
    double ub = (p2.getX() - p1.getX()) * (p1.getY() - p3.getY())
            - (p2.getY() - p1.getY()) * (p1.getX() - p3.getX());
    ua = ua / denominator;
    ub = ub / denominator;

    if ((ua >= 0 && ua <= 1) && (ub >= 0 && ub <= 1))
    {
        return new Point2D(p1.getX() + ua * (p2.getX() - p1.getX()),
                           p1.getY() + ua * (p2.getY() - p1.getY()));
    } else
    {
        // no intersection of the two segments
        return NULL;
    }
}

voronoi::Point2D* voronoi::PolygonSimple::getIntersectionOfSegmentAndLine(voronoi::Point2D p1,
                                                       voronoi::Point2D p2, voronoi::Point2D p3, voronoi::Point2D p4)
{
    double denominator = (p4.getY() - p3.getY()) * (p2.getX() - p1.getX())
            - (p4.getX() - p3.getX()) * (p2.getY() - p1.getY());
    if (denominator == 0)
    {
        return NULL;//FIXME:need exception
        // return null;
        //throw new RuntimeException("Lines are parallel");
    }
    double ua = (p4.getX() - p3.getX()) * (p1.getY() - p3.getY())
            - (p4.getY() - p3.getY()) * (p1.getX() - p3.getX());
    double ub = (p2.getX() - p1.getX()) * (p1.getY() - p3.getY())
            - (p2.getY() - p1.getY()) * (p1.getX() - p3.getX());
    ua = ua / denominator;
    ub = ub / denominator;

    if ((ua >= 0 && ua <= 1) && ub >= 1)
    {
        return new Point2D(p1.getX() + ua * (p2.getX() - p1.getX()),
                           p1.getY() + ua * (p2.getY() - p1.getY()));
    } else
    {
        // no intersection of the two segments
        return NULL;
    }
}

voronoi::PolygonSimple* voronoi::PolygonSimple::getOriginalPolygon()
{
    return oldPolygon_;
}

void voronoi::PolygonSimple::generateDefaultPolygon(int numPoint)
{
    double width=500.0,height=300.0;
    double angle=2.0*std::atan(1.0)*4.0/numPoint;//Here: pi = std::atan(1.0) * 4
    Point2D tmpPoint;
    for (int i=0;i<numPoint;++i)
    {
        tmpPoint.x=width*std::cos(i*angle)+width;
        tmpPoint.y=height*std::sin(i*angle)+height;
        push_back(tmpPoint);
    }
}

// Note: who use this func should guarantee the given edge index is valid
double voronoi::PolygonSimple::getDistance(int edgeIndex,Point2D& p)
{
    Point2D& startP=at(edgeIndex);
    Point2D& endP=(edgeIndex==size()-1)?at(0):at(edgeIndex+1);
    double s_x=startP.x;
    double s_y=startP.y;
    double e_x=endP.x;
    double e_y=endP.y;
    double t_x=p.x;
    double t_y=p.y;
    double edgeLength=std::sqrt(std::pow(e_x-s_x,2.0)+std::pow(e_y-s_y,2.0));
    double area=std::abs((e_x-s_x)*(t_y-s_y)-(t_x-s_x)*(e_y-s_y));
    
    // WATCHME: risk of divide by 0!!
    double distance=area/edgeLength;
    return distance;
}
