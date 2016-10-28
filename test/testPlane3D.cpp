#include "../plane3d.h"
//#include "../point2d.h"
#include <iostream>
using namespace voronoi;
int main()
{
    Point3D p0;
    Point3D p1(0,1,1);
    Point3D p2(1,0,1);
    Plane3D plane(p0,p1,p2);
    Point2D dualPoint=plane.getDualPointMappedToPlane();
    std::cout<<"p0:";
    p0.print();
    std::cout<<"p1:";
    p1.print();
    std::cout<<"p2:";
    p2.print();
    std::cout<<"plane:";
    plane.print();
    std::cout<<"dual point:";
    dualPoint.print();
    return 0;
}
