#include "face.h"
using namespace voronoi;

Face::Face(Vertex& a, Vertex& b, Vertex& c):
    v_[0](a),v_[1](b),v_[2](c),marked_(false)
{
    list=new ConflictList(true);
    Vertex t((v_[0].subtract(v_[1])).crossProduct(v_[1].subtract(v_[2])));
    normal_=new Vector(-t.x,-t.y,-t.z);
    normal_.normalize();
    createEdges();
}
Face::Face(Vertex& a, Vertex& b, Vertex& c, Vertex& orient)
{
    Face(a,b,c);
    orient(orient);
}

void Face::orient(Vertex& orient)
{
    if(!behind(orient))
    {
        Vertex& temp = v_[1];
        v_[1]=v_[2];
        v_[2]=temp;
        normal.negate();
        createEdges();
    }
}

void Face::createEdges()
{
    e_[0]=new HullEdge(v_[0],v_[1],this);
    e_[1]=new HullEdge(v_[1],v_[2],this);
    e_[2]=new HullEdge(v_[2],v_[0],this);
    e_[0].setNext(e_[1]);
    e_[0].setPrev(e_[2]);
    e_[1].setNext(e_[2]);
    e_[1].setNext(e_[0]);
    e_[2].setNext(e_[0]);
    e_[2].setNext(e_[1]);
}
std::string Face::toString()
{
    std::string str=std::printf("[ %f,%f,%f ]\n",v_[0],v_[1],v_[2]);
    return str;
}
void Face::link(Face& f0,Vertex& v0,Vertex& v1)
{
    HullEdge& twin=f0.getEdge(v0,v1);
    if(twin==NULL)
        //TODO: need exception capture code!!
        //throw new runtimeException("Twin of Edge is NULL, can't connect edges!");
        std::cerr<<"Twin of Edge is NULL, can't connect edges!\n";
    HullEdge& edge=getEdge(v0,v1);
    twin.setTwin(edge);
    edge.setTwin(twin);
}
void Face::link(HullEdge& e)
{
    HullEge& edge=getEdge(e.getStart(),e.getEnd());
    e.setTwin(edge);
    edge.setTwin(e);
}
HullEdge& getEdge(const Vertex& v0,const Vertex& v1)
{
    for(int i=0; i<3;i++)
    {
        if(e_[i].isEqual(v0,v1))
            return e_[i];
    }
    return NULL;
}
Point2D& getDualPoint()
{
    if(dualPoint_==NULL)
    {
        Plane3D plane3d=new Plane3D(this);
        dualPoint_=plane3d.getDualPointMappedToPlane();
    }
    return dualPoint_;
}
HullEdge& getHorizon()
{
    for(int i=0;i<3;i++)
    {
        if(e_[i].getTwin()!=NULL && e_[i].getTwin().isHorizon())
            return e_[i];
    }
    return NULL;
}
