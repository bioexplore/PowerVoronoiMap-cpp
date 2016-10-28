#ifndef TREEMAP_H
#define TREEMAP_H
namespace oronoi {
class VoronoiTreemap
{
public:
    /**
     * DEBUGGING
     */
    // TODO remove debug Mode
    static bool debugMode = false;
    static ImageFrame frame;
    static Graphics2D graphics;
    // geVoroRenderer renderer;

    VoronoiTreemap();
    VoronoiTreemap(StatusObject statusObject, bool multiThreaded);
    VoronoiTreemap(StatusObject statusObject);

    /** when a node is finished the status object is notified. **/
    HashMap<Integer, VoroNode> getIdToNode();
    void compute();
    void computeLocked();
    void setReferenceMap(ArrayList<Tuple3ID> relativePositions);
    bool getShowLeafs();
    //@Override
    Iterator<VoroNode> iterator();
    //@Override
    void finished();//synchronized
    void drawTreemap(Graphics2D g);
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
    void setRootPolygon(PolygonSimple rootPolygon);
    void setRootRectangle(double x, double y, double width, double height);
    /**
     * Sets the root rectangle in which the treemap is computed.
     *
     * @param rectangle
     */
    void setRootRectangle(Rectangle2D.Double rectangle);
    PolygonSimple getRootPolygon();
    void setNumberThreads(int numberThreads);
    /**
     * @return the numberThreads
     */
    int getNumberThreads();
    void setStatusObject(StatusObject statusObject);
    StatusObject getStatusObject();
    //@Override
    void finishedNode(int Node, int layer, int[] children,
                      PolygonSimple[] polygons);
    void setTree(ArrayList<ArrayList<Integer>> treeStructure);
    void clear();
    void setRandomSeed(long seed);
    long getRandomSeed();
    void setErrorAreaThreshold(double d);
    void setTreeData(TreeData treeData);
    void readEdgeList(String file);
    bool getUniFormWeights();
    void setUniformWeights(bool considerWeights);
    void setNumberIterationsLevel(int[] levelsMaxIteration);
protected:
    VoroNode root;
    void voronoi::VoronoiTreemap::recalculatePercentage();
    void voronoi::VoronoiTreemap::setRootCell(VoroNode cell);
    VoroNode voronoi::VoronoiTreemap::getRootCell();
    void voronoi::VoronoiTreemap::init();
    void voronoi::VoronoiTreemap::initVoroNodes();
    void voronoi::VoronoiTreemap::setSettingsToVoroNode(VoroNode node);
    //final
    void voronoi::VoronoiTreemap::addChildren(HashMap<Integer, VoroNode> idToNode,
                                              final ArrayList<ArrayList<Integer>> adjLists, int currentPos);
    void voronoi::VoronoiTreemap::setAmountNodes(int amountNodes);
    int voronoi::VoronoiTreemap::getAmountNodes();
    void voronoi::VoronoiTreemap::drawTreemapWithComponents(Graphics2D g);
    Graphics2D voronoi::VoronoiTreemap::getGraphics();

private:
    /**
     * DEBUGGING
     */
    bool initialized = false;
    bool useBorder = false;
    bool uniformWeights = false;
    double shrinkPercentage = 1;
    bool showLeafs = false;
    int numberThreads = 1;
    PolygonSimple rootPolygon;
    int amountAllNodes = 0;
    int alreadyDoneNodes = 0;
    long timeStart;
    long timeEnd;
    Semaphore lock = new Semaphore(1);
    /**
     * This queue handles the voronoi cells which have to be calculated.
     */
    BlockingQueue<VoroNode> cellQueue = new LinkedBlockingQueue<VoroNode>();
    List<StatusObject> statusObject;
    // used for randomization
    long randomSeed = 21;
    Random rand = new Random(randomSeed);
    HashMap<Integer, VoroNode> idToNode;
    ArrayList<Tuple3ID> relativePositions;
    VoroSettings coreSettings = new VoroSettings();
    int[] levelsMaxIteration;
    Set<VoroCPU> runningThreads;
    int rootIndex;
    TreeData treeData;

    void startComputeThreads();
    void setRelativePositions(ArrayList<Tuple3ID> relativePositions);
    void setShowLeafs(bool showLeafs);

    /**
     * Iterator for going over the VoroNodes of this Treemap
     *
     * @author nocaj
     *
     */
    class NodeIterator implements Iterator<VoroNode>
    {
    public:
        Stack<VoroNode> stack;

        NodeIterator(VoroNode root);
        //@Override
        bool hasNext();
        //@Override
        VoroNode next();
        //@Override
        void remove();
    };
};
}
#endif // TREEMAP_H
