#include "../vertex.h"
#include <iostream>
#include <cstdio>

using namespace voronoi;
int main()
{
    Vertex v1(1,0,0);
    Vertex v2(0,1,0);
    Vertex v3(0,0,1);
    Vertex v4(0,0,2);
    //double V1dV2=v1.dot(&v2);
    Vertex V1cV2=v1.crossProduct(v2);
    Vertex V1sV2=v1.subtract(v2);
    bool linearDependv1v2=v1.linearDependent(v2);
    bool linearDependv3v4=v3.linearDependent(v4);
    const char* p=linearDependv1v2?"is linear to":"is not linear to";
    const char* p2=linearDependv3v4?"is linear to":"is not linear to";
    std::cout<<"[v1]:"<<v1.toString()<<std::endl
             <<"[v2]:"<<v2.toString()<<std::endl
             <<"[v3]:"<<v3.toString()<<std::endl
             <<"[v4]:"<<v4.toString()<<std::endl
             //`<<"\t[v1.*v2]:"<<V1dV2<<std::endl
             <<"\t[v1*v2]:"<<V1cV2.toString()<<std::endl
             <<"\t[v1-v2]:"<<V1sV2.toString()<<std::endl
             <<"\tv1 "<<p<<" v2"<<std::endl
             <<"\tv3 "<<p2<<" v4"<<std::endl;
    //V1sV2.normalize();
    //std::cout<<"\t[v1-v2].normalize:"<<V1sV2.toString()<<std::endl;
    return 0;
}
