#include "../vector.h"
#include <iostream>
#include <cstdio>

using namespace voronoi;
int main()
{
    Vector v0;
    Vector v1(1,0,0);
    Vector v2(0,1,0);
    Vector v3(0,0,1);
    double V1dV2=v1.dot(&v2);
    Vector V1cV2=v1.cross(&v2);
    Vector V1sV2=v1.subtract(&v2);
    
    std::cout<<"[v0]:"<<v0.toString()<<std::endl
             <<"[v1]:"<<v1.toString()<<std::endl
             <<"[v2]:"<<v2.toString()<<std::endl
             <<"[v3]:"<<v3.toString()<<std::endl
             <<"\t[v1.*v2]:"<<V1dV2<<std::endl
             <<"\t[v1*v2]:"<<V1cV2.toString()<<std::endl
             <<"\t[v1-v2]:"<<V1sV2.toString()<<std::endl;
    V1sV2.normalize();
    std::cout<<"\t[v1-v2].normalize:"<<V1sV2.toString()<<std::endl;
    return 0;
}
