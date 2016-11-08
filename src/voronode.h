#ifndef VORONODE_H
#define VORONODE_H
#include <string>
#include <vector>
#include "vorocellobject.h"
namespace voronoi {
/**
 * Represents a node in the Voronoi Treemap hierarchy. This node is put into the computation queue.
 *
 * @author Arlind Nocaj
 *
 */
class PolygonSimple;
class Site;
class VoronoiCore;
class Point2D;
class VoroNode;
class VoronoiTreemap;
class VoroNode:public VoroCellObject //implements VoroCellObject
{
private:
    std::vector<VoroNode*>*     children_; //A vector that contains all included children nodes. A pointer with allocated memory        [Retrieve]
    VoroNode*                   parent_;   //A pointer point to this node's parent.A pointer reference.                                 [No Retrieve]
    VoronoiTreemap*             treemap_;  //A pointer point to the treemap that manage this node. A pointer reference.                 [No Retrieve]
    PolygonSimple*              polygon_;  //A pointer point to the node's corresponding polygon. A pointer reference.                  [No Retrieve] ??
    VoronoiCore*                core_;     //A pointer that contains allocated voronoicore object. A pointer with allocated memory      [Retrieve] 
    Site*                       site_;     //A pointer that contains allocated site object. A pointer with allocated memory             [Retrieve]
    Point2D*                    relativeVector_; //A pointer that contains a llocated point2d object. A pointer with allocated memory.  [Retrieve]

    int                         nodeID_;            // Id for the node
    int                         height_;            // Node level in the tree structure
    bool                        toConsider_;        // Whether to compute this node
    double                      wantedPercentage_;  // Parameter used to update node
    double                      weight_;            // The weight for the node

    void setSettingsToCore();
public:
    std::string name;

    VoroNode(int nodeID);
    VoroNode(int nodeID, int numberChildren);
    ~VoroNode();

    void            addChild(VoroNode* child);
    void            calculateWeights();
    void            decreasePercentage();
    int             getNodeID();
    VoroNode*       getParent();
    std::vector<VoroNode*>*     getChildren();
    PolygonSimple*  getPolygon();
    bool            isToConsider();
    double          getWantedPercentage();
    void            iterate();
    /**
     * Scales the relative vectors of the child nodes to the size of our current
     * polygon. Doing this helps to preserve the mental map.
     */
    void            scaleRelativeVectors();

    void            setChildren(std::vector<VoroNode*>* children);
    void            setHeight(int height);
    void            setWeight(double weight);
    void            setName(std::string name);
    void            setNodeID(int nodeID);
    void            setParent(VoroNode* parent);
    void            setPolygon(PolygonSimple* polygon);
    void            setRelativeVector(Point2D* relativeVector);
    void            setRelativeVector(Point2D relativeVector);
    void            setSite(Site* s);
    void            setSpiralRelativeCoordinates();
    void            setToConsider(bool toConsider);
    void            setTreemap(VoronoiTreemap* treemap);
    void            setWantedPercentage(double percentage);

    void            increasePercentageDirectly();
    int             getChildrenIDs(int* list);
    int             getChildrenPolygons(PolygonSimple**);
    int             getHeight();
    std::string     getName();
    double          getWeight();
    Site*           getSite();
    VoronoiTreemap* getTreemap();
    Point2D*        getRelativeVector();

    // Virtual memeber functions impelement
    void            doFinalWork();
    void            setVoroPolygon(PolygonSimple* polygon);

};

}

#endif // VORONODE_H
