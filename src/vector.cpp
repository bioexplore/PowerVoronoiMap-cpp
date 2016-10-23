#include "vector.h"
#include "vertex.h"
using namespace voronoi;
double Vector::dot(Vertex& b)
{
    double res=x*(b.x)+y*(b.y)+z*(b.z); 
    return res;
}
double Vector::dot(Vertex* b)
{
    double res;
    res=x*(b->x)+y*(b->y)+z*(b->z); 
    return res;
}
