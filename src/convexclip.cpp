#include <iostream>
#include "convexclip.h"
#include "point2d.h"
#include "vertex.h"
#include "infopoint.h"
//using namespace voronoi;

voronoi::ConvexClip::ConvexClip():inters(NULL){}
voronoi::ConvexClip::~ConvexClip()
{
    if (inters) delete inters;
}

int voronoi::ConvexClip::AreaSign(Point2D& a, Point2D& b, Point2D& c)
{
    double area;
    area = (b.x-a.x)* (c.y-a.y) - (c.x - a.x) * (b.y - a.y);//cross product of vec(a->b) and vec(a->c)
    if( area > 0.000005) return 1;
    else if( area < -0.000005) return -1;
    else return 0; //means a, b, c are on the same line
}

void voronoi::ConvexClip::Start(std::vector<Point2D>& list1, std::vector<Point2D>& list2)
{
    std::vector<Point2D>& p = list1;//.copyList();
    std::vector<Point2D>& q = list2;//.copyList();
    if(!isConvex(p)||!isReverseConvex(p))
    { //Check for convexity
        std::cerr<<"Polygons are not Convex..."<<std::endl;
            //throw new RuntimeException("Polygons are not Convex...");//TODO: add exception throw
    }
    if(!isConvex(q)||!isReverseConvex(p))
    { //Check for convexity
        std::cerr<<"Polygons are not Convex..."<<std::endl;
            //throw new RuntimeException("Polygons are not Convex...");//TODO: add exception throw
    }
    if(inters) delete inters;
    inters = new std::vector<Point2D>(); //result list
    ConvexIntersection(p,q,p.size(),q.size());
}

voronoi::InfoPoint voronoi::ConvexClip::intersect(Point2D& a, Point2D& b, Point2D& c, Point2D& d)
{
    double t,s,denum;
    denum = ((b.x-a.x)*(d.y-c.y) - (d.x-c.x)* (b.y-a.y));//cross product of vec(c->d) and vec(a->b)
    if(denum == 0)// means vec(a->b) is parallel to vec(c->d)
    {
        if(AreaSign(a,b,c) == 0)
        { //Collinear
            if(between(a,b,c) && between (a,b,d))// Condition like ( *a-----*c----------*d-------*b)
            {
                return InfoPoint(&c,&d,'e');
            }
            if(between(c,d,a) && between (c,d,b))// Condition like ( *c-----*a--------*b-------*d)
            {
                return InfoPoint(&a,&b,'e');
            }
            if(between(a,b,c) && between (c,d,b))// Condition like ( *a-----*c-----*b------*d)
            {
                return InfoPoint(&c,&b,'e');
            }
            if(between(a,b,c) && between (c,d,a))// Condition like ( *d-------*a-------*c---*b)
            {
                return InfoPoint(&c,&a,'e');
            }
            if(between(a,b,d) && between (c,d,b))// Condition like ( *a----*d-----*b------*c)
            {
                return InfoPoint(&d,&b,'e');
            }
            if(between(a,b,d) && between (c,d,a))// Condition like ( *c----*a----*d----*b)
            {
                return InfoPoint(&d,&a,'e');
            }
        }
        return InfoPoint(NULL,'n'); //no intersection, parallel  //TODO: define a nullpoint Point2D object that has nothing in it!
    }
    t = ((c.x-a.x)*(d.y -c.y) - (c.y-a.y)*(d.x-c.x))/denum;
    s = ((a.x-c.x)*(b.y-a.y)-(a.y-c.y)*(b.x-a.x))/-denum;
    if(t >= 0 && t <= 1 && s >= 0 && s <= 1)
    {
        return InfoPoint(new Point2D(a.x + t*(b.x-a.x),a.y + t*(b.y-a.y)),'1',true); //Proper intersection
    }
    return InfoPoint(NULL,'n');
}

//=============PRIVATE MEMBER FUNCS============\\

bool voronoi::ConvexClip::between(Point2D& a, Point2D& b, Point2D& c)
{
    if(a.x != b.x)
        return (c.x >= a.x && c.x <= b.x) || (c.x <= a.x && c.x >= b.x);
    else
        return (c.y >= a.y && c.y <= b.y) || (c.y <= a.y && c.y >= b.y);
}

bool voronoi::ConvexClip::isConvex(std::vector<Point2D>& p2)
{
    if(p2.size() < 3)
    {
        return false;
    }
    Point2D& v1=p2[0];
    Point2D& v2=p2[1];
    Point2D& v3=p2[2]; 
    int i=0;
    do
    {
       v1=p2[i];
       v2=p2[i+1];
       v3=p2[i+2];
       if(AreaSign(v1,v2,v3)<0)
           return false;
       ++i;
    }while(i<(p2.size()-2));
    v1=v2;
    v2=v3;
    v3=p2[0];
    if(AreaSign(v1,v2,v3)<0)
        return false;
    return true;
}

bool voronoi::ConvexClip::isReverseConvex(std::vector<Point2D>& p2)
{
    if(p2.size() < 3)
    {
        return false;
    }
    Point2D& v1=p2[0];
    Point2D& v2=p2[1];
    Point2D& v3=p2[2]; 
    int i=p2.size()-1;
    do
    {
       v1=p2[i];
       v2=p2[i-1];
       v3=p2[i-2];
       if(AreaSign(v1,v2,v3)<0)
           return false;
       ++i;
    }while(i>1);
    v1=v2;
    v2=v3;
    v3=p2[p2.size()-1];
    if(AreaSign(v1,v2,v3)<0)
        return false;
    return true;
}

//Who invoke this function should make sure p and q is not empty, and n <=p.size() m<=q.size();
void voronoi::ConvexClip::ConvexIntersection(std::vector<Point2D>& p, std::vector<Point2D>& q, int n, int m)
{
    std::vector<Point2D>::iterator iterP=p.begin();
    std::vector<Point2D>::iterator iterQ=q.begin();
    Point2D& currP = p[0];
    Point2D& prevP = p[p.size()-1];
    Point2D& currQ = q[0]; //current vertex of both polygons
    Point2D& prevQ = q[q.size()-1];
    InsideFlag flg = UNKNOWN; //Information flag whether p or q is on the inside (or if it is unknown)
    int ap = 0, aq = 0;		// counter for the termination condition(ap = advance p,aq = advance q)
    Point2D nil; // (0,0) Vertex
    Point2D vQ,vP; //current directed edges of both polygons
    bool FirstPoint = true; //Flag whether first point or not
    do
    {
        InfoPoint c = intersect(prevP, currP, prevQ, currQ); //Intersection of two polygon edges
        vQ = Point2D(currQ.x - prevQ.x, currQ.y - prevQ.y);
        vP = Point2D(currP.x - prevP.x, currP.y - prevP.y);
        int cross = AreaSign(nil,vP,vQ); //sign of crossproduct of vP and vQ
        int pInQ  = AreaSign(prevQ, currQ, currP); // if currp is on the half plane of q
        int qInP  = AreaSign(prevP, currP, currQ); // if currq is on the half plane of p
        if(c.code() == '1')
        { // Proper intersection
            if (flg == UNKNOWN && FirstPoint) 
            {
                FirstPoint = false;
                ap = aq = 0;
            }

            inters->push_back(c.erg()); //Adding the intersection to the result
            //Flag update
            if(pInQ > 0)
            {
                flg = PIN;
            }else if(qInP > 0)
            {
                flg = QIN;
            }
        }
        //Advance Rules:
        // vP and vQ overlap and oppositely oriented
        if(c.code() == 'e' && dot(vP,vQ) < 0)
        { //Shared SEQUENCE
            inters->push_back(c.erg());
            inters->push_back(c.snd());
            return;
        }
        // vP and vQ are parallel and separated. p and q are disjoint!
        if(cross == 0 && pInQ <0 && qInP < 0)
        {
            return;
            //vP and vQ are collinear
        }else if(cross == 0 && pInQ == 0 && qInP == 0)
        {
            if(flg == PIN)
            {
                ++aq;
                prevQ=currQ;
                currQ=*(++iterQ);
            }else
            {
                ++ap;
                prevP=currP;
                currP=*(++iterP);
            }
            /* Generic cases
                 * cross  		halfplane condition   advance rule
                 *  >0				qInP == 1			 p
                 *  >0				qInP == 0			 q
                 *  <0				pInQ == 1			 q
                 *  <0				pInQ == 0			 p
                 *  if p is advanced and inside flag = p then add currp to result (equivalent to q)
                 */
        }else if(cross >= 0)
        {
            if(qInP> 0)
            {
                if(flg == PIN)
                    inters->push_back(currP);
                ++ap;
                prevP=currP;
                currP =*(++iterP);
            }else
            {
                if(flg == QIN)
                    inters->push_back(currQ);
                ++aq;
                prevQ=currQ;
                currQ=*(++iterQ);
            }
        }else 
        {//cross < 0
            if(pInQ > 0)
            {
                if(flg == QIN)
                    inters->push_back(currQ);
                ++aq;
                prevQ=currQ;
                currQ=*(++iterQ);
            }else
            {
                if(flg == PIN)
                    inters->push_back(currP);
                ++ap;
                prevP=currP;
                currP=*(++iterP);
            }
        }
        /*
         * Termination condition: if ap >= n and aq >=m then both polygons traversed
         * if ap >= 2*n or aq >= 2*m then p or q cycled twice and there will not be another intersection.
         */
    }while(((ap < n) || (aq < m)) && (ap < 2*n) && (aq < 2*m));
}

double voronoi::ConvexClip::dot(Point2D& vP, Point2D& vQ)
{
    return vP.x*vQ.x + vP.y*vQ.y;
}


