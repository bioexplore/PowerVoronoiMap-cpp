#ifndef TUPLE3ID_HEADER
#define TUPLE3ID_HEADER
namespace voronoi
{
class Tuple3ID
{
public:
    int     id;
    double  valueX;
    double  valueY;

    Tuple3ID(int i,double x,double y):
        id(i),valueX(x),valueY(y)
    {}
};
}
#endif
