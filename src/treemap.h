#ifndef TREEMAP_H
#define TREEMAP_H
#include <unordered_map>
#include <vector>
#include "treedata.h"
#include "voronode.h"
#include "vorosettings.h"
#include "polygonsimple.h"
#include "statusobject.h"

namespace voronoi {
class Tuple3ID;
class VoronoiTreemap
{
    friend class VoroNode;
public:
    VoronoiTreemap();
    VoronoiTreemap(StatusObject* statusObject, bool multiThreaded);
    VoronoiTreemap(StatusObject* statusObject);
    ~VoronoiTreemap();
    
    /** when a node is finished the status object is notified. **/
    std::unordered_map<int,VoroNode*>* getIdToNode();
    void        compute();
    void        computeLocked();
    void        setReferenceMap(std::vector<Tuple3ID*>* relativePositions);
    bool        getShowLeafs();
    //@Override
    //Iterator<VoroNode*> iterator();
    //@Override
    void        finished();//synchronized
    //void drawTreemap(Graphics2D g);
    void        setShrinkPercentage(double shrinkPercentage);
    double      getShrinkPercentage();
    void        setUseBorder(bool useBorder);
    bool        getUseBorder();
    void        setNumberMaxIterations(int numberMaxIterations);
    int         getNumberMaxIterations();
    void        setCancelOnThreshold(bool cancelOnThreshold);
    bool        getCancelOnThreshold();
    void        setCancelOnMaxIteration(bool cancelOnMaxIterat);
    bool        getCancelOnMaxIteration();
    void        setRootPolygon(PolygonSimple* rootPolygon);
    void        setRootRectangle(double x, double y, double width, double height);
    /**
     * Sets the root rectangle in which the treemap is computed.
     *
     * @param rectangle
     */
    void        setRootRectangle(Rectangle2D* rectangle);
    void        setNumberThreads(int numberThreads);
    PolygonSimple* getRootPolygon();
    /**
     * @return the numberThreads
     */
    int         getNumberThreads();
    void        setStatusObject(StatusObject* statusObject);
    StatusObject* getStatusObject();
    //@Override
    void        finishedNode(int Node, int layer, int* children, PolygonSimple* polygons);
    void        setTree(std::vector<std::vector<int> >& treeStructure);
    void        clear();
    void        setRandomSeed(long seed);
    long        getRandomSeed();
    void        setErrorAreaThreshold(double d);
    void        setTreeData(TreeData* treeData);
    void        readEdgeList(std::string file);
    bool        getUniFormWeights();
    void        setUniformWeights(bool considerWeights);
    void        setNumberIterationsLevel(int levelsMaxIteration[]);
    VoroSettings* getCoreSettings();
protected:
    VoroNode*   root_;//   Pointer created             [Retrieve]
    void        recalculatePercentage();
    void        setRootCell(VoroNode* cell);
    VoroNode*   getRootCell();
    void        init();
    void        initVoroNodes();
    void        setSettingsToVoroNode(VoroNode* node);
    //final
    void        addChildren(std::unordered_map<int,VoroNode*>* idToNode,std::vector<std::vector<int> >& adjLists, int currentPos);
    void        setAmountNodes(int amountNodes);
    int         getAmountNodes();
    //void drawTreemapWithComponents(Graphics2D g);
    //Graphics2D getGraphics();

private:
    int         amountAllNodes_;// = 0;
    bool        initialized_; //= false;
    int         numberThreads_;// = 1;
    int         rootIndex_;
    bool        showLeafs_;// = false;
    double      shrinkPercentage_;// = 1;
    long        timeStart_;
    long        timeEnd_;
    bool        uniformWeights_;// = false;
    bool        useBorder_;// = false;
    //Set<VoroCPU> runningThreads;

    int*                                    levelsMaxIteration_;
    std::vector<VoroNode*>*                 cellQueue_;  //    Pointer created              [Retrieve]
    VoroSettings*                           coreSettings_;//   Pointer created              [Retrieve]
    std::unordered_map<int,VoroNode*>*      idToNode_; //     Pointer created               [Retrieve]
    TreeData*                               treeData_;//      Pointer reference             [No Retrieve]
    PolygonSimple*                          rootPolygon_; //  Pointer reference             [No Retrieve]
    std::vector<StatusObject*>*             statusObjects_;// Pointer reference             [No Retrieve]
    std::vector<Tuple3ID*>*                 relativePositions_;// Pointer reference         [No Retrieve]

    void        startComputeThreads();
    void        setRelativePositions(std::vector<Tuple3ID*>* relativePositions);
    void        setShowLeafs(bool showLeafs);
};
}
#endif // TREEMAP_H
