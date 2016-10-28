#include "cvertexlist.h"
#include <cmath>
using namespace voronoi;
void cVertexList::InsertBeforeHead(cVertex* vertex)
{
    if(head == NULL)
    {
        head = vertex;
        vertex->next = vertex;
        vertex->prev = vertex;
        n = 1;
    }else
    {
        if(!checkForIdentity(*vertex))
        {
            head->prev->next = vertex;
            vertex->prev = head->prev;
            vertex->next = head;
            head->prev = vertex;
            ++n;
        }
    }
}
cVertexList* cVertexList::copyList()
{
    cVertex* temp1 = head, *temp2;
    cVertexList* erg = new cVertexList();
    do{
        temp2 = new cVertex();
        temp2->v  = temp1->v;
        erg->InsertBeforeHead(temp2);
        temp1 = temp1->next;
    }while(temp1 != head);
    return erg;
}
const char* cVertexList::toString()//TODO:need to change to std::string
{
    return "";
//    if (n == 0){
//        return "";
//    }
//    //String erg = "";
//    const char* erg=0;
//    cVertex* curr = head;
//    do{
//        erg += " " + curr->toString();
//        curr = curr.next;
//    }while(curr != head);
//    return erg;
}
void cVertexList::ReverseList()//TODO:seems not correct
{
    cVertexList* listcopy = copyList();
    cVertex *temp1, *temp2;
    head = NULL;
    n = 0;

    //Fill this list in proper order:
    temp1 = listcopy->head;
    do {
        temp2 = new cVertex();
        temp2->v = temp1->v;
        InsertBeforeHead( temp2 );
        temp1 = temp1->prev;
    } while (temp1 != listcopy->head );
    //  System.out.println("Reversing list...");
}

//========PRIVATE MEMBER FUNCS=========\\
double cVertexList::abs(double a, double b)
{
    return std::abs(b-a);
}
bool cVertexList::checkForIdentity(cVertex& vertex)
{
    Point2D& c = vertex.v;
    Point2D& comp = head->prev->v;
    if(abs(c.x,comp.x)< epsilon && abs(c.y,comp.y) < epsilon)
        return true;
    if(abs(c.x,(head->v).x)< epsilon && abs(c.y,(head->v).y) < epsilon)
        return true;
    return false;
}
