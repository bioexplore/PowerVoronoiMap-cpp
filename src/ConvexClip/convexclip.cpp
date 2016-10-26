#incldue "convexclip.h"
#include "point2d.h"
//using namespace voronoi;
static int voronoi::ConvexClip::AreaSign(Point2D& a, Point2D& b, Point2D& c)
{
    double area;
    area = (b.x-a.x)* (c.y-a.y) - (c.x - a.x) * (b.y - a.y);//cross product of vec(a->c) and vec(a->b)
    if( area > 0.000005) return 1;
    else if( area < -0.000005) return -1;
    else return 0; //means a, b, c are on the same line
}

void voronoi::ConvexClip::Start(VertexList& list1, VertexList& list2)
{
    VertexList p = list1.copyList();
    VertexList q = list2.copyList();
    if(!isConvex(p)){ //Check for convexity
        p.ReverseList(); //If list is not oriented counterclockwise
        if(!isConvex(p)){
            std::cerr<<"Polygons are not Convex..."<<std::endl;
            //throw new RuntimeException("Polygons are not Convex...");//TODO: add exception throw
        }
    }
    if(!isConvex(q)){ //Check for convexity
        q.ReverseList();//If list is not oriented counterclockwise
        if(!isConvex(q)){
            std::cerr<<"Polygons are not Convex..."<<std::endl;
            //throw new RuntimeException("Polygons are not Convex...");//TODO: add exception throw
        }
    }
    inters = new VertexList(); //result list
    ConvexIntersection(p,q,p.n,q.n);
}

InfoPoint voronoi::ConvexClip::intersect(Point2D& a, Point2D& b, Point2D& c, Point2D& d)
{
    double t,s,denum;
    denum = ((d.x-c.x)*(b.y-a.y) - (b.x-a.x)* (d.y-c.y));//cross product of vec(a->b) and vec(c->d)
    if(denum == 0)// means vec(a->b) is parallel to vec(c->d)
    {
        if(AreaSign(a,b,c) == 0){ //Collinear
            if(between(a,b,c) && between (a,b,d)){
                return new InfoPoint(c,d,'e');
            }
            if(between(c,d,a) && between (c,d,b)){
                return new InfoPoint(a,b,'e');
            }
            if(between(a,b,c) && between (c,d,b)){
                return new InfoPoint(c,b,'e');
            }
            if(between(a,b,c) && between (c,d,a)){
                return new InfoPoint(c,a,'e');
            }
            if(between(a,b,d) && between (c,d,b)){
                return new InfoPoint(d,b,'e');
            }
            if(between(a,b,d) && between (c,d,a)){
                return new InfoPoint(d,a,'e');
            }
        }
        return new InfoPoint(null,'n'); //no intersection, parallel  //TODO: define a nullpoint Point2D object that has nothing in it!
    }
    t = (a.x*(d.y -c.y) - a.y*(d.x-c.x) + c.y * (d.x-c.x) - c.x * (d.y-c.y))/denum;
    s = ((b.x-a.x) * a.y + c.x * (b.y - a.y) - a.x * (b.y -a.y) - c.y * (b.x - a.x))/-denum;
    if(t >= 0 && t <= 1 && s >= 0 && s <= 1){
        return new InfoPoint(new Point2D(a.x + t*(b.x-a.x),a.y + t*(b.y-a.y)),'1'); //Proper intersection
    }
    return new InfoPoint(null,'n');
}

//=============PRIVATE MEMBER FUNCS============\\

static bool voronoi::ConvexClip::between(Point2D& a, Point2D& b, Point2D& c)
{
    if(a.x != b.x)
        return (c.x >= a.x && c.x <= b.x) || (c.x <= a.x && c.x >= b.x);
    else
        return (c.y >= a.y && c.y <= b.y) || (c.y <= a.y && c.y >= b.y);
}

static bool voronoi::ConvexClip::isConvex(VertexList& p2)
{
    if(p2.n < 3){
        return false;
    }
    cVertex* curr = p2.head->next;

    while(curr != p2.head){
        if(AreaSign(curr->v, curr->next->v, curr->next->next->v) < 0){
            return false;
        }
        curr = curr->next;
    }
    return true;
}

void voronoi::ConvexClip::ConvexIntersection(VertexList& p, VertexList& q, int n, int m)
{
    cVertex *currp = p.head, *currq = q.head; //current vertex of both polygons
    InsideFlag flg = InsideFlag.UNKNOWN; //Information flag whether p or q is on the inside (or if it is unknown)
    int ap = 0, aq = 0;		// counter for the termination condition(ap = advance p,aq = advance q)
    Point2D nil; // (0,0) Vertex
    Point2D vQ,vP; //current directed edges of both polygons
    bool FirstPoint = true; //Flag whether first point or not
    do{
        InfoPoint c = intersect(currp->prev->v, currp->v, currq->prev->v, currq->v); //Intersection of two polygon edges
        vQ = Point2D(currq->v.x - currq->prev->v.x,currq->v.y - currq->prev->v.y);
        vP = Point2D(currp->v.x - currp->prev->v.x,currp->v.y - currp->prev->v.y);
        int cross = AreaSign(nil,vP,vQ); //sign of crossproduct of vP and vQ
        int pInQ  = AreaSign(currq->prev->v, currq->v, currp->v); // if currp is on the half plane of q
        int qInP  = AreaSign(currp->prev->v, currp->v, currq->v); // if currq is on the half plane of p
        if(c.code == '1'){ // Proper intersection
            if (flg == InsideFlag.UNKNOWN && FirstPoint) {
                FirstPoint = false;
                ap = aq = 0;
            }
            inters->InsertBeforeHead(new cVertex(c.erg)); //Adding the intersection to the result
            //Flag update
            if(pInQ > 0){
                flg = InsideFlag.PIN;
            }else if(qInP > 0){
                flg = InsideFlag.QIN;
            }
        }
        //Advance Rules:
        // vP and vQ overlap and oppositely oriented
        if(c.code == 'e' && dot(vP,vQ) < 0){ //Shared SEQUENCE
            inters.InsertBeforeHead(new cVertex(c.erg));
            inters.InsertBeforeHead(new cVertex(c.snd));
            return;

        }
        // vP and vQ are parallel and separated. p and q are disjoint!
        if(cross == 0 && pInQ <0 && qInP < 0){
            return;
            //vP and vQ are collinear
        }else if(cross == 0 && pInQ == 0 && qInP == 0){
            if(flg == InsideFlag.PIN){
                ++aq;
                currq = currq.next;
            }else{
                ++ap;
                currp = currp.next;
            }
            /* Generic cases
                 * cross  		halfplane condition   advance rule
                 *  >0				qInP == 1			 p
                 *  >0				qInP == 0			 q
                 *  <0				pInQ == 1			 q
                 *  <0				pInQ == 0			 p
                 *  if p is advanced and inside flag = p then add currp to result (equivalent to q)
                 */
        }else if(cross >= 0){
            if(qInP> 0){
                if(flg == InsideFlag.PIN)
                    inters.InsertBeforeHead(new cVertex(currp.v));
                ++ap;
                currp = currp.next;
            }else{
                if(flg == InsideFlag.QIN)
                    inters.InsertBeforeHead(new cVertex(currq.v));
                ++aq;
                currq = currq.next;
            }
        }else {//cross < 0
            if(pInQ > 0){
                if(flg == InsideFlag.QIN)
                    inters.InsertBeforeHead(new cVertex(currq.v));
                ++aq;
                currq = currq.next;
            }else{
                if(flg == InsideFlag.PIN)
                    inters.InsertBeforeHead(new cVertex(currp.v));
                ++ap;
                currp = currp.next;
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


