#include "hulledge.h"
using namespace voronoi;

HullEdge::HullEdge( Vertex* start, Vertex* end,Face* iface):
    start_(start),end_(end),iFace_(iface),twin_(NULL),next_(NULL),prev_(NULL)
{}

bool HullEdge::isEqual( Vertex* start, Vertex* end)
{
    bool equal=false;
    equal = start_->equals(start) && end_->equals(end);
    equal = equal || (start_->equals(end) && end_->equals(start));
    return equal;
}
// The edge that whose neighbor face is marked but its owner face not marked is horizon 
bool HullEdge::isHorizon()
{
    bool horizon=false;
    horizon= (twin_!=NULL) && (twin_->getiFace()->isMarked()) && (!iFace_->isMarked());
    //horizon= (twin_!=NULL) && !(twin_->getiFace()->isMarked()) && (iFace_->isMarked());//Modified by Jianye Xia @20161022
    return horizon;
}
void HullEdge::findHorizon(std::vector<HullEdge*>* horizons)
{
    if(isHorizon())
    {
        if(horizons->size()>0 && this == horizons->at(0))
        {
            return;
        }else
        {
            horizons->push_back(this);
            next_->findHorizon(horizons);
        }
    }else
    {
        if(twin_!=NULL)
        {
            twin_->getNext()->findHorizon(horizons);
        }
    }
}

void HullEdge::setEnd(Vertex* end)
{
    end_=static_cast< Vertex*>(end);
}
void HullEdge::setStart(Vertex* start)
{
    start_=start;
}
void HullEdge::setTwin(HullEdge* edge)
{
    twin_=edge;
}
void HullEdge::setiFace(Face* face)
{
    iFace_=face;
}
void HullEdge::setNext(HullEdge* nextEdge)
{
    next_=nextEdge;
}
void HullEdge::setPrev(HullEdge* prevEdge)
{
    prev_=prevEdge;
}

 Vertex* HullEdge::getStartPtr()
{
    return start_;
}
 Vertex* HullEdge::getEndPtr()
{
    return end_;
}

