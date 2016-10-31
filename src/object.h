#ifndef VORONOI_OBJECT_H
#define VORONOI_OBJECT_H
#include <string>
namespace voronoi
{
class Object
{
public:
   virtual double getWeight()=0;
   virtual std::string getName()=0;
};
}
#endif
