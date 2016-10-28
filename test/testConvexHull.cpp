#include "../convexhull.h"
#include "../face.h"
#include <vector>
#include <iostream>
using namespace voronoi;
int main()
{ 
    int i=12;
    ConvexHull hull;
    hull.addPoint(273.0,237.0,130668.0);
    hull.addPoint(302.0,507.0,348223.0);
    hull.addPoint(296.0,394.0,242822.0);
    hull.addPoint(-700.0,-700.0,979999.999999);
    hull.addPoint(1550.0,-700.0,2892500.0);
    hull.addPoint(1550.0,1550.0,4805000.0);
    hull.addPoint(-700.0,1550.0,2892500.0);
    std::vector<Face*>* res=hull.compute();
    for(int i=0;i<res->size();++i)
    {
        std::cout<<res->at(i)->toString()<<std::endl;
    }
    std::cout<<"The end!"<<std::endl;
    return 0;
}
