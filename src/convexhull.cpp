#include "convexhull.h"
using namespace voronoi;
#define deleteList(objectListPointer,elementType) \
    if(objectListPointer)               \
    {                                   \
        for(std::vector<elementType*>::iterator it=objectListPointer.begin();it!=objectListPointer.end();++it)  \
        {                                   \
            if(*it)                         \
                delete (*it);               \
        }                                   \
        delete objectListPointer;           \
    }


//const static

ConvexHull::ConvexHull():
    current_(0),permutate_(false)
{
    points_ = new std::vector<Vertex*>();
    facets_ = new std::vector<Face*>();
    created_ = new std::vector<Face*>();'
    horizon_ = new std::vector<HullEdge*>();
    visible_ = new std::vector<Face*>();
}

ConvexHull::~ConvexHull()
{
    deleteList(points_,Vertex);
    deleteList(facets_,Face);
    deleteList(created_,Face);
    deleteList(horizon_,HullEdge);
    deleteList(visible_,Face);
}

void ConvexHull::addPoint(Vertex &v)
{
    Vertex* tempVertex = new Vertex(v.x,v.y,v.z);
    tempVertex.originalObject = v;
    tempVertex.setIndex(points_->size());
    points_->push_back(tempVertex);
}
void ConvexHull::addPoint(double x, double y, double z)
{
    Vertex* v = new Vertex(x,y,z);
    v.setIndex(points_->size());
    points_->push_back(v);
}

std::vector<Face*>* compute()
{
    prep();
    while(current_<points_->size()){
        Vertex& next = points_->at(current);
        if(next.getList().empty())
        {
            current_++;
            continue;
        }
        created_->clear();
        horizon_->clear();
        visible_->clear();
        //The visible faces are also marked
        next.getList().fill(visible_);
        //Horizon edges are orderly added to the horizon list
        HullEdge& e;
        for(int i=0;i<visible_->size();i++)
        {
            e=visible_[i].getHorizon();
            if(e!=NULL)
            {
                e.findHorizon(horizon_);
                break;
            }
        }
        Face& last=NULL;
        Face& first=NULL;
        //Iterate over horizon edges and create new faces oriented with the marked face 3rd unused point
        for(int i=0;i<horizon_->size();i++)
        {
            HullEdge& tmpEdge=horizon_[i];
            Face& fn=new Face(next,tmpEdge.getStart(),tmpEdge.getEnd(),tmpEdge.getTwin().getNext().getEnd());
            fn.setList(new ConflictList(true));

            //Add to facet list
            addFacet(fn);
            created_->push_back(fn);

            //Add new conflicts
            addConflicts(tmpEdge.getiFace(),tmpEdge.getTwin().getiFace(),fn);

            //Link the new face with the horizon edge
            fn.link(tmpEdge);
            if(last!=NULL)
                fn.link(last,next,tmpEdge.getStart());
            last=fn;
            if(first == NULL)
                first = fn;
        }
        //Links the first and the last created Face
        if(first!=NULL && last!=NULL)
            last.link(first,next,horizon_->at(0).getOrigin());
        if(created_->size()!=0)
        {
            //Update conflict graph
            for(int i=0;i<visible_->size();i++) removeConflict(visible_[i]);
            current_++;
            created_->clear();
        }
    }
    return facets_;
}

void ConvexHull::addConflicts(Face &old1, Face &old2, Face fnew)
{
    //Adding the vertices
    std::vector<Vertex&> *plist1=new std::vector<Vertex&>();
    old1.getList().getVertices(plist1);
    std::vector<Vertex&> *plist2=new std::vector<Vertex&>();
    old2.getList().getVertices(plist2);
    std::vector<Vertex&> nCL = new std::vector<Vertex&>();
    Vertex &v1, &v2;
    int i=l=0;
    //Fill the possible new conflict list
    while(i<plist1.size()||l<plist2.size())
    {
        if(i<plist1.size()&&l<plist2.size())
        {
            v1=plist1->at(i);
            v2=plist2->at(l);
            //If the index is the same, its the same vertex and only 1 has to be added
            if(v1.getIndex()==v2.getIndex())
            {
                nCL.add(v1);
                i++;
                l++;
            }else if(v1.getIndex()>v2.getIndex())
            {
                nCL.add(v1);
                i++;
            }else
            {
                nCL.add(v2);
                l++;
            }
        }else if (i<plist1.size())
        {
            nCL.add(plist1->at(i++));
        }else
        {
            nCL.add(plist2->at(l++));
        }
    }
    //Check if the possible conflicts are real conflicts
    for(i=nCL.size()-1;i>=0;--i)
    {
        v1=nCL.at(i);
        if(fn.conflict(v1))
            addConflict(fn.v1);
    }
}
void    removeConflict(Face& f)
{
    f.removeConflict();
    int index = f.getIndex();
    f.setIndex(-1);
    if(index==facets.size()-1)
    {
        facets_->remove(facets_->size()-1);
        return;
    }
    if(index>=facets_->size()||index<0)
    {
        return;
    }
    Face& last=facets_->remove(facets_->size()-1);
    last.setIndex(index);
    facets_->set(index,last);
}

void    prep()
{
    if(points_->size()<=3)
    {
        std::cerr<<"No enough points!\n";//TODO: change to exception capture mechanism
        return;
    }
    if(permutate_)  permuation();//Randomize the vertices
    for(int i=0;i<points_->size();i++)
        points_->at(i).setIndex(i);
    Vertex *v0,*v1,*v2,*v3;
    Face *f1,*f2,*f3,*f0;
    v0=points_->at(0);
    v1=points_->at(1);
    v2=v3=NULL;
    for(int i=2;i<points_->size();i++)
    {
        if(!(v0->linearDependent(points_->at(i)) && v1->linearDependent(points_->at(i))))
        {
            v2 =points_->at(i);
            v2->setIndex(2);
            points_->at(2)->setIndex(i);
            points_->operator[](i)=points_->at(2);
            points_->operator[](2)=v2;
            break;
        }
    }
    if(v2==NULL)
    {
        std::cerr<<"Not enough non-planar points!\n"; //TODO: change to exception
    }
    //Create first Face object
    f0=new Face(v0,v1,v2);
    for(int i=3;i<points_->size();i++)
    {
        if((f0->getNormal().doct(f0->getVertex(0)))!=(f0->getNormal().dot(points_->at(i))))
        {
            v3=points_->at(i);
            v3->setIndex(3);
            points_->at(3)->setIndex(i);
            points_->operator[](i)=points_->at(3);
            points_->operator[](3)=v3;
            break;
        }
    }
    if(v3==NULL)
    {
        std::cerr<<"Not enough non-planar points!\n"; //TODO: change to exception
    }
    f0->orient(v3);
    f1=new Face(v0,v2,v3,v1);
    f2=new Face(v0,v1,v3,v2);
    f3=new Face(v1,v2,v3,v0);

    addFacet(f0);
    addFacet(f1);
    addFacet(f2);
    addFacet(f3);

    //Connect facets
    f0.link(f1,v0,v2);
    f0.link(f2,v0,v1);
    f0.link(f3,v1,v2);
    f1.link(f2,v0,v3);
    f1.link(f3,v2,v3);
    f2.link(f3,v3,v1);

    current_=4;
    //Fill conflict graph
    Vertex* v=NULL;
    for(int i=current_;i<points_->size();i++)
    {
        v=points_->at(i);
        if(f0.conflict(v))  addConflict(f0,v);
        //!f1.behind(v)
        if(f1.conflict(v))  addConflict(f1,v);
        if(f2.conflict(v))  addConflict(f2.v);
        if(f3.conflict(v))  addConflict(f3.v);
    }
}

void ConvexHull::permutation()
{
    int pointCount=points_->size();
    for(int i=pointCount-1;i>0;i--) //TODO: need to be optimized, no need to loop all points
    {
        int ra = rand.nextInt(i);
        Vertex* tmp=points_->at(ra);
        tmp->setIndex(i);
        Vertex* currentItem = points_->at(i);
        currentItem->setIndex(ra);
        points_->set(ra.currentItem);
        points_->set(i,tmp);
    }
}
void addFacet(Face& f0)
{
    f0.setIndex(facets_->size());
    facets_->push_back(f0);
}
void addConflict(Face& f0,Vertex& v)
{
    GraphEdge *e=new GraphEdge(f0,v);
    f0.getList().add(e);
    v.getList().add(e);
}
