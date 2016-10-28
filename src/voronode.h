#ifndef VORONODE_H
#define VORONODE_H

namespace voronoi {
/**
 * Represents a node in the Voronoi Treemap hierarchy. This node is put into the computation queue.
 *
 * @author Arlind Nocaj
 *
 */
class VoroNode //implements VoroCellObject
{
private:
    //pointers to higher levels of the hierarchy
    /**
     * tree information
     */
    VoronoiTreemap treemap;
    VoroNode parent;
    ArrayList<VoroNode> children;

    Integer nodeID;
    int height=0;
    PolygonSimple polygon;
    boolean toConsider = true;

    double weight=1;
    VoronoiCore core;
    Site site;
    double wantedPercentage=0;
    Point2D relativeVector;
    void setSettingsToCore();

public:
    std::string name;

    VoroNode(int nodeID);
    VoroNode(int nodeID, int numberChildren);
    void calculateWeights();
    void setSpiralRelativeCoordinates();
    void setNodeID(int nodeID);
    int getNodeID();
    void setParent(VoroNode parent);
    VoroNode getParent();
    void setChildren(ArrayList<VoroNode> children);
    ArrayList<VoroNode> getChildren();
    void setPolygon(PolygonSimple polygon);
    PolygonSimple getPolygon();
    void setToConsider(boolean toConsider);
    boolean isToConsider();
    void setWantedPercentage(double percentage);
    double getWantedPercentage();
    void addChild(VoroNode child);
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
    void setSite(Site s);
    Site getSite();
    void setTreemap(VoronoiTreemap treemap);
    VoronoiTreemap getTreemap();
    void setRelativeVector(Point2D relativeVector);
    Point2D getRelativeVector();

    //@Override
    void doFinalWork();
    //@Override
    void setVoroPolygon(PolygonSimple polygon);
    int[] getChildrenIDs();
    PolygonSimple[] getChildrenPolygons();
    void setName(String name);
    std::string getName();
};

}

#endif // VORONODE_H
