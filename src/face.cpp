#include "face.h"
#include "conflictlist.h"
#include "plane3d.h"
#include <iostream>

voronoi::Face::Face(Vertex* a, Vertex* b,Vertex* c):
    v0_(a),v1_(b),v2_(c),e0_(NULL),e1_(NULL),e2_(NULL), 
   list_(NULL),normal_(NULL),marked_(false),index_(-1)
{
    list_=new ConflictList(true);
    Vertex aminusb=a->subtract(b);
    Vertex bminusc=b->subtract(c);
    Vertex t=aminusb.crossProduct(&bminusc);
    normal_=new Vector(-t.x,-t.y,-t.z);
    normal_->normalize();
    createEdges();
}
voronoi::Face::Face(Vertex* a,Vertex* b, Vertex* c, Vertex* orient_):
   v0_(a),v1_(b),v2_(c),e0_(NULL),e1_(NULL),e2_(NULL), 
   list_(NULL),normal_(NULL),marked_(false),index_(-1)
{
    list_=new ConflictList(true);
    Vertex aminusb=a->subtract(b);
    Vertex bminusc=b->subtract(c);
    Vertex t=aminusb.crossProduct(&bminusc);
    normal_=new Vector(-t.x,-t.y,-t.z);
    normal_->normalize();
    createEdges();
    orient(orient_);
}

voronoi::Face::~Face()
{
    if(list_) delete list_;
    if(normal_) delete normal_;
    if(e0_) delete e0_;
    if(e1_) delete e1_;
    if(e2_) delete e2_;
}

void voronoi::Face::orient(Vertex* orient)
{
    if(!behind(orient))
    {
        Vertex* temp = v1_;
        v1_=v2_;
        v2_=temp;
        normal_->negate();
        createEdges();
    }
}

void voronoi::Face::createEdges()
{
    if(e0_) delete e0_;
    if(e1_) delete e1_;
    if(e2_) delete e2_;
    e0_=new HullEdge(v0_,v1_,this);
    e1_=new HullEdge(v1_,v2_,this);
    e2_=new HullEdge(v2_,v0_,this);
    e0_->setNext(e1_);
    e0_->setPrev(e2_);
    e1_->setNext(e2_);
    e1_->setPrev(e0_);
    e2_->setNext(e0_);
    e2_->setPrev(e1_);
}
char* voronoi::Face::toString()
{
    char* buf=0;
    buf=new char[25];
    std::sprintf(buf,"Face:[%f,/%f,/%f]",v0_,v1_,v2_);
    return buf;
}
void voronoi::Face::link(Face* f0,Vertex* v0,Vertex* v1)
{
    HullEdge *twin=f0->getEdge(v0,v1);
    if(twin==NULL)
        //TODO: need exception capture code!!
        //throw new runtimeException("Twin of Edge is NULL, can't connect edges!");
        std::cerr<<"Twin of Edge is NULL, can't connect edges!\n";
    HullEdge* edge=getEdge(v0,v1);
    twin->setTwin(edge);
    edge->setTwin(twin);
}
voronoi::Vertex* voronoi::Face::getVertex(int i)            
{
    switch(i)
    {
        case 0:
            return v0_;
        case 1:
            return v1_;
        case 2:
            return v2_;
        default:
            return NULL;
    }
}

void voronoi::Face::setList(ConflictList* list) 
{
    if(list_) delete list_;
    list_=list;
}

bool voronoi::Face::behind( Vertex *p)
{ 
    return (normal_->dot(p) < normal_->dot(v0_));
}

bool voronoi::Face::conflict( Vertex* p)
{
    return (normal_->dot(p) > (normal_->dot(v0_)+epsilon_));
}

void voronoi::Face::link(HullEdge* e)
{
    HullEdge* edge=getEdge(e->getStart(),e->getEnd());
    e->setTwin(edge);
    edge->setTwin(e);
}

voronoi::HullEdge* voronoi::Face::getEdge(int i)
{
    switch(i)
    {
        case 0:
            return e0_;
        case 1:
            return e1_;
        case 2:
            return e2_;
        default:
            return NULL;
    }
}
        
voronoi::HullEdge* voronoi::Face::getEdge(Vertex* v0,Vertex* v1)
{
    if(e0_->isEqual(v0,v1))
        return e0_;
    if(e1_->isEqual(v0,v1))
        return e1_;
    if(e2_->isEqual(v0,v1))
        return e2_;
    return NULL;
}

voronoi::Point2D voronoi::Face::getDualPoint()
{
    Point3D pointv0(*(v0_));
    Point3D pointv1(*(v1_));
    Point3D pointv2(*(v2_));
    Plane3D plane3d(pointv0,pointv1,pointv2);
    return plane3d.getDualPointMappedToPlane();
}

voronoi::HullEdge* voronoi::Face::getHorizon()
{
    if(e0_->getTwin()!=NULL && e0_->getTwin()->isHorizon())
        return e0_;
    if(e1_->getTwin()!=NULL && e1_->getTwin()->isHorizon())
        return e1_;
    if(e2_->getTwin()!=NULL && e2_->getTwin()->isHorizon())
        return e2_;
    return NULL;
}
void voronoi::Face::removeConflict()
{
    list_->removeAll();
}
