#ifndef CVERTEXLIST_H
#define CVERTEXLIST_H

namespace voronoi
{
class cVertexList
{
public:
    int n;
    cVertex* head;
    void InsertBeforeHead(cVertex* vertex);
    cVertexList* copyList();
    const char* toString();
    void ReverseList();
private:
    const static double epsilon = 0.00000001;
    double abs(double a, double b);
    bool checkForIdentity(cVertex& vertex);
};
}

#endif // CVERTEXLIST_H
