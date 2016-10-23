#include "conflictlist.h"
#include <iostream>
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

void ConflictList::fill(std::vector<Face*>* visible)
{
    if(face_)
    {
        std::cout<<"Can't fill facet\n";
        return;
    }
    GraphEdge* curr=head_;
    do{
        visible->push_back(curr->face_);
        curr->face_->setMarked(true);
        curr=curr->nextf_;
    }while(curr!=NULL);
}

void ConflictList::removeAll()
{
    if(face_)//remove all vertices from face
    {
        GraphEdge* curr=head_;
        while(curr!=NULL)
        {
            if(curr->prevf_==NULL)
            {
                if(curr->nextf_==NULL) curr->vertex_->getList()->head_=NULL;
                else
                {
                    curr->nextf_->prevf_=NULL;
                    curr->vertex_->getList()->head_=curr->nextf_;
                }
            }else
            {
                if(curr->nextf_!=NULL) curr->nextf_->prevf_=curr->prevf_;
                curr->prevf_->nextf_=curr->nextf_;
            }
            GraphEdge* tmp=curr; 
            curr=curr->nextv_;
            delete tmp;//Clean generate heap memory
            if(curr!=NULL)  curr->prevv_=NULL;
        }
    }else//remove all faces from vertex
    {
        GraphEdge* curr=head_;
        while(curr!=NULL)
        {
            if(curr->prevv_==NULL)//Node is head
            {
                if(curr->nextv_==NULL) curr->face_->getList()->head_=NULL;
                else
                {
                    curr->nextv_->prevv_=NULL;
                    curr->face_->getList()->head_=curr->nextv_;
                }
            }else
            {
                if(curr->nextv_!=NULL) curr->nextv_->prevv_=curr->prevv_;
                curr->prevv_->nextv_=curr->nextv_;
            }
            GraphEdge* tmp=curr;
            curr=curr->nextf_;
            delete tmp;
            if(curr!=NULL)  curr->prevf_=NULL;
        }
    }
}

void ConflictList::getVertices(std::vector<Vertex*>* l1)
{
    GraphEdge* curr=head_;
    while(curr!=NULL)
    {
        l1->push_back(curr->vertex_);
        curr=curr->nextv_;
    }
}
