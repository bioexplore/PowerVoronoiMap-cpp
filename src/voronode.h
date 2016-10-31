#ifndef VORONODE_H
#define VORONODE_H
#include <string>
#include <vector>
#include "vorocellobject.h"
#include "polygonsimple.h"
#include "site.h"
#include "voronoicore.h"
#include "point2d.h"

namespace voronoi {
/**
 * Represents a node in the Voronoi Treemap hierarchy. This node is put into the computation queue.
 *
 * @author Arlind Nocaj
 *
 */
    class VoroNode;
typedef std::vector<VoroNode*> ChildList;
    class VoronoiTreemap;
class VoroNode:public VoroCellObject //implements VoroCellObject
{
private:
    //pointers to higher levels of the hierarchy
    /**
     * tree information
     */
    VoronoiTreemap* treemap_;
    VoroNode* parent_;
    ChildList* children_;

    int nodeID_;
    int height_;//=0;
    PolygonSimple* polygon_;
    bool toConsider_;// = true;

    double weight_;//=1;
    VoronoiCore* core_;
    Site* site_;
    double wantedPercentage_;//=0;
    Point2D relativeVector_;
    void setSettingsToCore();

public:
    std::string name;

    VoroNode(int nodeID);
    VoroNode(int nodeID, int numberChildren);
    ~VoroNode();
    void calculateWeights();
    void setSpiralRelativeCoordinates();
    void setNodeID(int nodeID);
    int getNodeID();
    void setParent(VoroNode* parent);
    VoroNode* getParent();
    void setChildren(std::vector<VoroNode*>* children);
    std::vector<VoroNode*>* getChildren();
    void setPolygon(PolygonSimple* polygon);
    PolygonSimple* getPolygon();
    void setToConsider(bool toConsider);
    bool isToConsider();
    void setWantedPercentage(double percentage);
    double getWantedPercentage();
    void addChild(VoroNode* child);
    void iterate();
    /**
     * Scales the relative vectors of the child nodes to the size of our current
     * polygon. Doing this helps to preserve the mental map.
     */
    void scaleRelativeVectors();

    void increasePercentageDirectly();
    void decreasePercentage();
    void setHeight(int height);
    int getHeight();
    void setWeight(double weight);
    double getWeight();
    void setSite(Site* s);
    Site* getSite();
    void setTreemap(VoronoiTreemap* treemap);
    VoronoiTreemap* getTreemap();
    void setRelativeVector(Point2D relativeVector);
    Point2D getRelativeVector();

    //@Override
    void doFinalWork();
    //@Override
    void setVoroPolygon(PolygonSimple* polygon);
    int  getChildrenIDs(int* list);
    int  getChildrenPolygons(PolygonSimple** );
    void setName(std::string name);
    std::string getName();
};

}

#endif // VORONODE_H
