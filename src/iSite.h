#ifndef ISITE_H
#define ISITE_H
namespace voronoi
{
class iSite
{
public:
    PolygonSimple getPolygon()=0;
    void setPolygon(PolygonSimple poly)=0;
    void setNeighbours(ArrayList<Site> list)=0;
    ArrayList<Site> getNeighbours()=0;
    double getWeight()=0;
    void setWeight(double weight)=0;
    void setXY(double x, double y)=0;
    void setXYW(double x, double y, double weight)=0;
    void setX(double x)=0;
    void setY(double y)=0;
    void setPercentage(double percentage)=0;
    double getPercentage()=0;
    Point2D getPoint()=0;
};
}

#endif // ISITE_H
