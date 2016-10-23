#include "face.h"
#include "conflictlist.h"
#include "plane3d.h"
#include <iostream>
using namespace voronoi;

Face::Face(Vertex* a, Vertex* b,Vertex* c):
    marked_(false),index_(-1)
{
    v_[0]=a;v_[1]=b;v_[2]=c;
    e_[0]=e_[1]=e_[2]=NULL;
    list_=new ConflictList(true);
    Vertex aminusb=a->subtract(b);
    Vertex bminusc=b->subtract(c);
    Vertex t=aminusb.crossProduct(&bminusc);
    normal_=new Vector(-t.x,-t.y,-t.z);
    normal_->normalize();
    createEdges();
}
Face::Face(Vertex* a,Vertex* b, Vertex* c, Vertex* orient_):
    marked_(false),index_(-1)
{
    v_[0]=a;v_[1]=b;v_[2]=c;
    e_[0]=e_[1]=e_[2]=NULL;
    list_=new ConflictList(true);
    Vertex aminusb=a->subtract(b);
    Vertex bminusc=b->subtract(c);
    Vertex t=aminusb.crossProduct(&bminusc);
    normal_=new Vector(-t.x,-t.y,-t.z);
    normal_->normalize();
    createEdges();
    orient(orient_);
}

Face::~Face()
{
    if(list_) delete list_;
    if(normal_) delete normal_;
    for(int i=0;i<3;i++)
    {
        if(e_[i]) delete e_[i];
    } 
}

void Face::orient(Vertex* orient)
{
    if(!behind(orient))
    {
        Vertex* temp = v_[1];
        v_[1]=v_[2];
        v_[2]=temp;
        normal_->negate();
        createEdges();
    }
}

void Face::createEdges()
{
    e_[0]=new HullEdge(v_[0],v_[1],this);
    e_[1]=new HullEdge(v_[1],v_[2],this);
    e_[2]=new HullEdge(v_[2],v_[0],this);
    e_[0]->setNext(e_[1]);
    e_[0]->setPrev(e_[2]);
    e_[1]->setNext(e_[2]);
    e_[1]->setPrev(e_[0]);
    e_[2]->setNext(e_[0]);
    e_[2]->setPrev(e_[1]);
}
char* Face::toString()
{
    char* buf=0;
    buf=new char[25];
    std::sprintf(buf,"Face:[%f,/%f,/%f]",v_[0],v_[1],v_[2]);
    return buf;
}
void Face::link(Face* f0,Vertex* v0,Vertex* v1)
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

void Face::link(HullEdge* e)
{
    HullEdge* edge=getEdge(e->getStartPtr(),e->getEndPtr());
    e->setTwin(edge);
    edge->setTwin(e);
}
HullEdge* Face::getEdge(Vertex* v0,Vertex* v1)
{
    for(int i=0; i<3;i++)
    {
        if(e_[i]->isEqual(v0,v1))//isEqual does not take consideration of edge direaction
            return e_[i];
    }
    return NULL;
}
Point2D Face::getDualPoint()
{
    Point3D pointv0(*(v_[0]));
    Point3D pointv1(*(v_[1]));
    Point3D pointv2(*(v_[2]));
    Plane3D plane3d(pointv0,pointv1,pointv2);
    return plane3d.getDualPointMappedToPlane();
}
HullEdge* Face::getHorizon()
{
    for(int i=0;i<3;i++)
    {
        if(e_[i]->getTwin()!=NULL && e_[i]->getTwin()->isHorizon())//original code
        //if(e_[i]->getTwin()!=NULL && e_[i]->isHorizon())//modified by Jianye Xia @20161022
            return e_[i];
    }
    return NULL;
}
void Face::removeConflict()
{
    list_->removeAll();
}
