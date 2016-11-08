#include "vertex.h"
#include "conflictlist.h"

voronoi::Vertex::Vertex(double a,double b,double c):
   x(a),y(b),z(c),handled_(false),index_(-1)
{
   list_=new ConflictList(false);
}

voronoi::Vertex::~Vertex()
{
   if(list_)    delete list_; 
}

voronoi::ConflictList* voronoi::Vertex::getList()                   
{ 
    return list_;
}

void  voronoi::Vertex::setList(ConflictList* list)     
{
    list_=list;
}

int   voronoi::Vertex::getIndex()                  
{   
    return index_;
}

void  voronoi::Vertex::setIndex(int idex)          
{
    index_=idex;
}

bool  voronoi::Vertex::equals(Vertex* v)      
{
    if(v->x==x && v->y==y && v->z==z) 
        return true; 
    return false;
}

/**
 * @brief linearDependent
 * @param v Vertex compared to
 * @return Whether give vertex and the object are linearly dependent
 */
bool voronoi::Vertex::linearDependent(Vertex* v)
{
    if(x == 0 && v->x == 0)
    {
       if(y == 0 && v->y == 0)
       {
          if(z == 0 && v->z == 0) 
              return true;
          if(z == 0 || v->z == 0) 
              return false;
                
          return true;
       }
            
       if(y == 0 || v->y == 0) 
           return false;
            
       if(z/y >= v->z/v->y -epsilon  && z/y <= v->z/v->y +epsilon)
           return true;
       else
           return false;
        
    }
        
    if(x == 0 || v->x == 0) 
        return false;
        
    if(y/x <= v->y/v->x+epsilon && y/x >= v->y/v->x-epsilon &&  z/x >= v->y/v->x -epsilon  && z/x <= v->z/v->x +epsilon)
        return true;
    else
        return false;
}

void  voronoi::Vertex::negate()        
{
    x*=-1;
    y*=-1;
    z*=-1;
}

voronoi::Vertex  voronoi::Vertex::subtract(Vertex* v)     
{
    return Vertex(x-v->x,y-v->y,z-v->z);
}

voronoi::Vertex  voronoi::Vertex::crossProduct( Vertex* v)
{
    return Vertex(y*(v->z)-z*(v->y),z*(v->x)-x*(v->z),x*(v->y)-y*(v->x));
}

char*   voronoi::Vertex::toString()
{
   char* buf=0;
   buf=new char[25];
   std::sprintf(buf,"%f/%f/%f",x,y,z);
   return buf;
}

void  voronoi::Vertex::setHandled(bool b)      
{
    handled_=b;
}

bool  voronoi::Vertex::isHandled()             
{
    return handled_;
}

double  voronoi::Vertex::getX()      
{
    return x;
}

double  voronoi::Vertex::getY()      
{
    return y;
}

void    voronoi::Vertex::clear()
{
    list_->removeAll();
    index_=-1;
    handled_=false;
}

