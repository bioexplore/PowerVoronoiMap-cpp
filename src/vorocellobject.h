#ifndef VOROCELLOBJECT_H
#define VOROCELLOBJECT_H
#include <string>

namespace voronoi
{
class PolygonSimple;
class VoroCellObject
{
public:
    virtual void setVoroPolygon(PolygonSimple* polygon)=0;
    virtual void doFinalWork()=0;
    virtual double getWeight()=0;
    virtual std::string getName()=0;
};
}
#endif
