#include "hulledge.h"
using namespace voronoi;

HullEdge::HullEdge(const Vertex& start,const Vertex& end,const Face& iface):
    start_(start),end_(end),iFace_(iface),twin_(NULL),next_(NULL),prev_(NULL)
{}

bool HullEdge::isEqual(const Vertex& start,const Vertex& end)
{
    bool equal=false;
    equal = start_.equals(start) && end_.equals(end);
    equal = equal || (start_.equals(end) && end_.equals(start));
    return equal;
}

bool HullEdge::isHorizon()
{
    bool horizon=false;
    horizon= (twin_!=NULL) && (twin_.getiFace().isMarked()) && (!iFace_.isMarked());
    return horizon;
}

void HullEdge::setEnd(const Vertex& end)
{
    end_=end;
}
void HullEdge::setStart(const Vertex& start)
{
    start_=start;
}
void HullEdge::setTwin(const HullEdge& edge)
{
    twin_=edge;
}
void HullEdge::setiFace(const Face& face)
{
    iFace_=face;
}
void HullEdge::setNext(const HullEdge& nextEdge)
{
    next_=nextEdge;
}
void HullEdge::setPrev(const HullEdge& prevEdge)
{
    prev_=prevEdge;
}

const Vertex& HullEdge::getStart()
{
    return start_;
}
const Vertex& HullEdge::getEnd()
{
    return end_;
}
const HullEdge& HullEdge::getTwin()
{
    return twin_;
}
const Face& HullEdge::getFace()
{
    return iFace_;
}
cconst HullEdge& HullEdge::getNext()
{
    return next_;
}
onst HullEdge& HullEdge::getPrev()
{
    return prev_;
}

