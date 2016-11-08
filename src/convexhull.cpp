#include <vector>
#include <iostream>
#include "vertex.h"
#include "face.h"
#include "convexhull.h"
#include "conflictlist.h"
using namespace voronoi;
#define deleteList(objectListPointer,elementType) \
    if(objectListPointer)               \
    {                                   \
        for(std::vector<elementType*>::iterator it=objectListPointer->begin();it!=objectListPointer->end();++it)  \
        {                                   \
            if(*it)                         \
                delete (*it);               \
        }                                   \
        delete objectListPointer;           \
    }


//const static

ConvexHull::ConvexHull():
    currentIndex_(0),permutate_(false)
{
    points_ = new std::vector<Vertex*>();//Content of this vector is pointer reference 
    facets_ = new std::vector<Face*>();
    created_ = new std::vector<Face*>();
    horizon_ = new std::vector<HullEdge*>();
    visible_ = new std::vector<Face*>();
}

ConvexHull::~ConvexHull()
{
    if(points_) delete points_;
    deleteList(facets_,Face);
    //deleteList(created_,Face);
    //deleteList(horizon_,HullEdge);//all edges are deleted by their owned face, no need to retrieve!
    //deleteList(visible_,Face);//all content in visible_ are pointer reference, no need to retrieve!
    if(created_) delete created_;
    if(horizon_) delete horizon_;
    if(visible_) delete visible_;
}

void ConvexHull::addPoint(Vertex *v)
{
    //Vertex* tempVertex = new Vertex(v->x,v->y,v->z);
    //tempVertex->setIndex(points_->size());
    v->setIndex(points_->size());
    points_->push_back(v);
}

//Abandon this function,do not use it
void ConvexHull::addPoint(double x, double y, double z)
{
    std::cerr<<"File:"<<__FILE__<<",line"<<__LINE__<<std::endl
        <<"function:ConvexHull::addPoint(double,double,double) is abandoned, use addPoint(Vertex*)!!"<<std::endl;
    exit(1);
    /*Vertex* v = new Vertex(x,y,z);
    v->setIndex(points_->size());
    points_->push_back(v);
    */
}

std::vector<Face*>* ConvexHull::compute()
{
    prep();//Make the initial 4 facets that constitute a tetrahedron
    while(currentIndex_ < points_->size()){
        Vertex* nextP = points_->at(currentIndex_);
        if(nextP->getList()->empty())
        {
            currentIndex_++;
            continue;
        }
        created_->clear();//std::vector support clear, and content with pointer point to heap memory will not be deleted
        horizon_->clear();
        visible_->clear();
        //Mark all the visible faces to the "nextP" Vertex
        nextP->getList()->fill(visible_);//will append all the faces in the dlink table saved in nextP's conflictlist
        //Horizon edges are orderly added to the horizon list
        HullEdge* e;
        for(int i=0;i < visible_->size();i++)//loop over all visible faces to "nextP"
        {
            e=visible_->operator[](i)->getHorizon();
            if(e!=NULL)
            {
                e->findHorizon(horizon_);
                break;
            }
        }
        Face* last=NULL;
        Face* first=NULL;
        //Iterate over horizon edges and create new faces oriented with the marked face 3rd unused point
        for(int i=0;i < horizon_->size();i++)
        {
            HullEdge* tmpEdge=horizon_->operator[](i);
            Face* fn=new Face(nextP,tmpEdge->getStart(),tmpEdge->getEnd(),tmpEdge->getTwin()->getNext()->getEnd());
            fn->setList(new ConflictList(true));

            //Add to facet list
            addFacet(fn);//set index of the new created face, and push it to the facets_ vector
            created_->push_back(fn);

            //Add new conflicts to new created face fn's ConflictList based on ConflictList of old face
            addConflicts(tmpEdge->getiFace(),tmpEdge->getTwin()->getiFace(),fn);//The 3 faces have one mutual edge 

            //Link the new face with the horizon edge
            fn->link(tmpEdge);  //original code
            //fn->link(tmpEdge->getTwin());//After this original link between the two old faces will be broken
                                        //modified by Jianye Xia @20161022
            if(last!=NULL)
                fn->link(last,nextP,tmpEdge->getStart());//original code
                //fn->link(last,nextP,tmpEdge->getEnd());//modified by Jianye Xia @20161022
            last=fn;//Save the new created face to last for next loop useage
            if(first == NULL)
                first = fn;
        }
        //Links the first and the last created Face
        if(first!=NULL && last!=NULL)
            last->link(first,nextP,horizon_->at(0)->getStart());//original code
            //last->link(first,nextP,horizon_->at(0)->getEnd());//modified by Jianye Xia @20161022
        if(created_->size()!=0)
        {
            //Update conflict graph
            for(int i=0;i < visible_->size();i++) removeConflict(visible_->at(i));
            currentIndex_++;
            created_->clear();//Don't need to clean the memory allocated as all facets generated are appended to faces_
                              //and they will be taken care during the destruction
        }
    }
    return facets_;
}
/**
 * Function addConflicts will make ConflictList of the new created Face
 * fnew based on ConflictList of old Faces old1 and old2
 */
void ConvexHull::addConflicts(Face* old1,Face* old2, Face* fnew)
{
    //Adding the vertices
    std::vector<Vertex*> *plist1=new std::vector<Vertex*>();
    old1->getList()->getVertices(plist1);//get all "normal-direction-visible,NDV" vertices by face old1
    std::vector<Vertex*> *plist2=new std::vector<Vertex*>();
    old2->getList()->getVertices(plist2);//get all "normal-direction-visible,NDV" vertices by face old2
    std::vector<Vertex*>* nCL = new std::vector<Vertex*>();//nCL means new conflict list,it will collect all NDV vertices by face old1 and old2
    Vertex *v1, *v2;
    int i=0,l=0;
    //Fill the possible new conflict list
    while(i < plist1->size() || l < plist2->size())
    {
        if(i < plist1->size() && l < plist2->size())
        {
            v1=plist1->at(i);
            v2=plist2->at(l);
            //If the index is the same, its the same vertex and only 1 has to be added
            if(v1->getIndex()==v2->getIndex())
            {
                nCL->push_back(v1);
                i++;
                l++;
            }else if(v1->getIndex() > v2->getIndex())
            {
                nCL->push_back(v1);
                i++;
            }else
            {
                nCL->push_back(v2);
                l++;
            }
        }else if (i < plist1->size())
        {
            nCL->push_back(plist1->at(i++));
        }else
        {
            nCL->push_back(plist2->at(l++));
        }
    }
    //Check if the possible conflicts are real conflicts
    for(i=nCL->size()-1;i>=0;--i)
    {
        v1=nCL->at(i);
        if(fnew->conflict(v1))
            addConflict(fnew,v1);
    }
    //Clean allocated memories
    plist1->clear();
    plist2->clear();
    nCL->clear();
    delete plist1;
    delete plist2;
    delete nCL;
}
void    ConvexHull::removeConflict(Face* f)
{
    f->removeConflict();//clear face's ConflictList
    int index = f->getIndex();
    f->setIndex(-1);
    Face* tempF=NULL;
    if(index==facets_->size()-1)
    {
        tempF=facets_->back();
        facets_->pop_back();
        if(tempF) delete tempF;
        return;
    }
    if(index >= facets_->size() || index<0)
    {
        return;
    }
    tempF=facets_->back();
    tempF->setIndex(index);
    facets_->at(index)=tempF;
    facets_->pop_back();
    if(f) delete f;
    return;
}

void    ConvexHull::prep()
{
    if(points_->size()<=3)
    {
        std::cerr<<"No enough points!\n";//TODO: change to exception capture mechanism
        return;
    }
    if(permutate_)  permutation();//Randomize the vertices
    for(int i=0;i<points_->size();i++)
        points_->at(i)->setIndex(i);
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
        //if n.*v0 == n.*vi means vi locates on face f0
        if((f0->getNormal()->dot(f0->getVertex(0)))!=(f0->getNormal()->dot(points_->at(i))))
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

    addFacet(f0);//Push the face f0 to the vector facets_ and make index for the face
    addFacet(f1);
    addFacet(f2);
    addFacet(f3);

    //Connect facets
    f0->link(f1,v0,v2);//Link twin for edge v0->v2
    f0->link(f2,v0,v1);
    f0->link(f3,v1,v2);
    f1->link(f2,v0,v3);
    f1->link(f3,v2,v3);
    f2->link(f3,v3,v1);

    currentIndex_=4;
    //Fill conflict graph
    Vertex* v=NULL;
    //The following loop will generate double link table (ConflictList) for each point and f0-f4 
    //e.g. f0->list_ will contain a linked table which saves vertexs that can conflict with f0
    //     v4->list_ will contain a linked table which saves faces that are conflict with v4
    for(int i=currentIndex_;i<points_->size();i++)
    {
        v=points_->at(i);
        if(f0->conflict(v))  addConflict(f0,v);//add v to conflictlist of f0
        //!f1.behind(v)
        if(f1->conflict(v))  addConflict(f1,v);
        if(f2->conflict(v))  addConflict(f2,v);
        if(f3->conflict(v))  addConflict(f3,v);
    }
}

//Not implemented
void ConvexHull::permutation()
{
    /*
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
    */
}
void ConvexHull::addFacet(Face* f0)
{
    f0->setIndex(facets_->size());
    facets_->push_back(f0);
}
void ConvexHull::addConflict(Face* f0,Vertex* v)
{
    GraphEdge *e=new GraphEdge(f0,v);
    f0->getList()->add(e);
    v->getList()->add(e);
}
