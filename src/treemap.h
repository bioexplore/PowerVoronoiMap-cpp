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
    /**
     * DEBUGGING
     */
    // TODO remove debug Mode
   // static bool debugMode = false;
    //static ImageFrame frame;
    //static Graphics2D graphics;
    // geVoroRenderer renderer;

    VoronoiTreemap();
    VoronoiTreemap(StatusObject* statusObject, bool multiThreaded);
    VoronoiTreemap(StatusObject* statusObject);

    /** when a node is finished the status object is notified. **/
    std::unordered_map<int,VoroNode*>* getIdToNode();
    void compute();
    void computeLocked();
    void setReferenceMap(std::vector<Tuple3ID*>* relativePositions);
    bool getShowLeafs();
    //@Override
    //Iterator<VoroNode*> iterator();
    //@Override
    void finished();//synchronized
    //void drawTreemap(Graphics2D g);
    void setShrinkPercentage(double shrinkPercentage);
    double getShrinkPercentage();
    void setUseBorder(bool useBorder);
    bool getUseBorder();
    void setNumberMaxIterations(int numberMaxIterations);
    int getNumberMaxIterations();
    void setCancelOnThreshold(bool cancelOnThreshold);
    bool getCancelOnThreshold();
    void setCancelOnMaxIteration(bool cancelOnMaxIterat);
    bool getCancelOnMaxIteration();
    void setRootPolygon(PolygonSimple* rootPolygon);
    void setRootRectangle(double x, double y, double width, double height);
    /**
     * Sets the root rectangle in which the treemap is computed.
     *
     * @param rectangle
     */
    void setRootRectangle(Rectangle2D* rectangle);
    PolygonSimple* getRootPolygon();
    void setNumberThreads(int numberThreads);
    /**
     * @return the numberThreads
     */
    int getNumberThreads();
    void setStatusObject(StatusObject* statusObject);
    StatusObject* getStatusObject();
    //@Override
    void finishedNode(int Node, int layer, int* children, PolygonSimple* polygons);
    void setTree(std::vector<std::vector<int> >& treeStructure);
    void clear();
    void setRandomSeed(long seed);
    long getRandomSeed();
    void setErrorAreaThreshold(double d);
    void setTreeData(TreeData* treeData);
    void readEdgeList(std::string file);
    bool getUniFormWeights();
    void setUniformWeights(bool considerWeights);
    void setNumberIterationsLevel(int levelsMaxIteration[]);
protected:
    VoroNode* root_;
    void recalculatePercentage();
    void setRootCell(VoroNode* cell);
    VoroNode* getRootCell();
    void init();
    void initVoroNodes();
    void setSettingsToVoroNode(VoroNode* node);
    //final
    void addChildren(std::unordered_map<int,VoroNode*>* idToNode,std::vector<std::vector<int> >& adjLists, int currentPos);
    void setAmountNodes(int amountNodes);
    int getAmountNodes();
    //void drawTreemapWithComponents(Graphics2D g);
    //Graphics2D getGraphics();

private:
    /**
     * DEBUGGING
     */
    bool        initialized_; //= false;
    bool        useBorder_;// = false;
    bool        uniformWeights_;// = false;
    double      shrinkPercentage_;// = 1;
    bool        showLeafs_;// = false;
    int         numberThreads_;// = 1;
    int         amountAllNodes_;// = 0;
    int         alreadyDoneNodes_;// = 0;
    long        timeStart_;
    long        timeEnd_;
    long        randomSeed_;// = 21;
    //Set<VoroCPU> runningThreads;
    int         rootIndex_;
    TreeData*   treeData_;

    //Semaphore lock = new Semaphore(1);
    /**
     * This queue handles the voronoi cells which have to be calculated.
     */
    //BlockingQueue<VoroNode*> cellQueue = new LinkedBlockingQueue<VoroNode*>();
    std::vector<VoroNode*>*                 cellQueue_;
    std::vector<StatusObject*>*             statusObjects_;
    PolygonSimple*                          rootPolygon_;
    std::unordered_map<int,VoroNode*>*      idToNode_;
    std::vector<Tuple3ID*>*                 relativePositions_;
    VoroSettings*                           coreSettings_;// = new VoroSettings();
    int*                                    levelsMaxIteration_;
    void startComputeThreads();
    void setRelativePositions(std::vector<Tuple3ID*>* relativePositions);
    void setShowLeafs(bool showLeafs);

    /**
     * Iterator for going over the VoroNode*s of this Treemap
     *
     * @author nocaj
     *
     */
    /*
    class NodeIterator implements Iterator<VoroNode*>
    {
    public:
        Stack<VoroNode*> stack;

        NodeIterator(VoroNode* root);
        //@Override
        bool hasNext();
        //@Override
        VoroNode* next();
        //@Override
        void remove();
    };
    */
};
}
#endif // TREEMAP_H
