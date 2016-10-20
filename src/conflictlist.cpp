#include "conflictlist.h"
using namespace voronoi;

void ConflictList::add(GraphEdge* e)
{
    if(head_==NULL) head_=e;
    else
    {
        if(face_)
        {
            head_->prevv_=e;
            e->nextv_=head_;
            head_=e;
        }
        else
        {
            head_->prevf_=e;
            e->nextf_=head_;
            head_=e;
        }
    }
}

void ConflictList::fill(std::list<Face &> visible)
{
    if(face_)
    {
        std::cout<<"Can't fill facet\n";
        return;
    }
    GraphEdge* curr=head_;
    do{
        visible.add(curr->face());
        curr->face().setMarked(true);
        curr=curr->nextf();
    }while(curr!=NULL);
}

void ConflictList::removeAll()
{
    if(face_)//remove all vertices from face
    {
        GraphEdge* curr=head_;
        do{
            if(curr->prevf()==NULL)
            {
                if(curr->nextf()==NULL) curr->vertex().getList().head_=NULL;
                else
                {
                    curr->nextf().prevf()=NULL;
                    curr->vertex().getList().head=curr.nextf();
                }
            }else
            {
                if(curr->nextf()!=NULL) curr->nextf().prevf()=curr->prevf();
                curr->prevf().nextf()=curr->nextf();
            }
            curr=curr->nextv();
            if(curr!=NULL)  curr->prevv()=NULL;
        }while(curr!=NULL);
    }else//remove all faces from vertex
    {
        GraphEdge* curr=head_;
        do{
            if(curr->prevv()==NULL)//Node is head
            {
                if(curr->nextv()==NULL) curr->face().getList().head_=NULL;
                else
                {
                    curr->nextv().prevv()=NULL;
                    curr->face().getList().head=curr.nextv();
                }
            }else
            {
                if(curr->nextv()!=NULL) curr->nextv().prevv()=curr->prevv();
                curr->prevv().nextv()=curr->nextv();
            }
            curr=curr->nextf();
            if(curr!=NULL)  curr->prevf()=NULL;
        }while(curr!=NULL);
    }
}

std::list<Vertex&> ConflictList::getVertices(std::list<Vertex&> l1)
{
    GraphEdge* curr=head_;
    while(curr!=NULL)
    {
        l1.add(curr->vertex());
        curr=curr->nextv();
    }
    return l1
}
