#include "treemap.h"

//===========PUBLIC MEMBER FUNCS============\\
voronoi::VoronoiTreemap::VoronoiTreemap()
{
    init();
}

voronoi::VoronoiTreemap::VoronoiTreemap(StatusObject statusObject, bool multiThreaded)
{
    this(statusObject);

    if (multiThreaded)
        setNumberThreads(Runtime.getRuntime().availableProcessors());
}

voronoi::VoronoiTreemap::VoronoiTreemap(StatusObject statusObject)
{
    this();
    this.statusObject.add(statusObject);
}

/** when a node is finished the status object is notified. **/

HashMap<Integer, VoroNode> voronoi::VoronoiTreemap::getIdToNode()
{
    return idToNode;
}

void voronoi::VoronoiTreemap::compute()
{
    if (rootPolygon == null)
        throw new RuntimeException("Root Polygon not set.");
    timeStart = System.currentTimeMillis();
    initVoroNodes();
    cellQueue.add(root);
    startComputeThreads();
}

void voronoi::VoronoiTreemap::computeLocked()
{
    try {
        lock.acquire();
    } catch (InterruptedException e) {
        e.printStackTrace();
    }
    compute();
    try {
        lock.acquire();
        lock.release();
    } catch (InterruptedException e) {
        e.printStackTrace();
    }
}
void voronoi::VoronoiTreemap::setReferenceMap(ArrayList<Tuple3ID> relativePositions)
{
    for (Tuple3ID tuple : relativePositions)
    {
        VoroNode voroNode = null;
        voroNode = idToNode.get(tuple.id);
        if (voroNode != null)
        {

            voroNode.setRelativeVector(new Point2D(tuple.valueX,
                                                   tuple.valueY));
        } else
            System.out.println("node id could not be found for setting reference position: "
                             + tuple.id);
    }
}

bool voronoi::VoronoiTreemap::getShowLeafs()
{
    return showLeafs;
}
//@Override
Iterator<VoroNode> voronoi::VoronoiTreemap::iterator()
{
    return new NodeIterator(root);
}

//@Override
//synchronized
void voronoi::VoronoiTreemap::finished()
{
    for (StatusObject statusObject : this.statusObject)
        statusObject.finished();
    lock.release();
}

void voronoi::VoronoiTreemap::drawTreemap(Graphics2D g)
{
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                       RenderingHints.VALUE_ANTIALIAS_ON);
    int lastNodes = 0;
    int amountPolygons = 0;
    for (VoroNode node : this)
    {
        int height = node.getHeight();
        if (node.getChildren() == null)
        {
            lastNodes++;
        }
        Site site = node.getSite();
        if (site != null)
        {
            PolygonSimple poly = site.getPolygon();
            if (poly != null)
            {
                // poly.shrinkForBorder(0.95);
                amountPolygons++;
                g.draw(poly);
            }
        }

    }
}

void voronoi::VoronoiTreemap::setShrinkPercentage(double shrinkPercentage)
{
    this.shrinkPercentage = shrinkPercentage;
}


double voronoi::VoronoiTreemap::getShrinkPercentage()
{
    return shrinkPercentage;
}

void voronoi::VoronoiTreemap::setUseBorder(bool useBorder)
{
    this.useBorder = useBorder;
}


bool voronoi::VoronoiTreemap::getUseBorder()
{
    return useBorder;
}

void voronoi::VoronoiTreemap::setNumberMaxIterations(int numberMaxIterations)
{
    coreSettings.maxIterat = numberMaxIterations;
}

int voronoi::VoronoiTreemap::getNumberMaxIterations()
{
    return coreSettings.maxIterat;
}

void voronoi::VoronoiTreemap::setCancelOnThreshold(bool cancelOnThreshold)
{
    this.coreSettings.cancelAreaError = cancelOnThreshold;
}

bool voronoi::VoronoiTreemap::getCancelOnThreshold()
{
    return coreSettings.cancelAreaError;
}

void voronoi::VoronoiTreemap::setCancelOnMaxIteration(bool cancelOnMaxIterat)
{
    coreSettings.cancelMaxIterat = cancelOnMaxIterat;
}


bool voronoi::VoronoiTreemap::getCancelOnMaxIteration()
{
    return coreSettings.cancelMaxIterat;
}

void voronoi::VoronoiTreemap::setRootPolygon(PolygonSimple rootPolygon)
{
    this.rootPolygon = rootPolygon;
    if (root != null)
        root.setPolygon(rootPolygon);
}


void voronoi::VoronoiTreemap::setRootRectangle(double x, double y, double width, double height)
{
    rootPolygon = new PolygonSimple();
    rootPolygon.add(x, y);
    rootPolygon.add(x + width, y);
    rootPolygon.add(x + width, y + height);
    rootPolygon.add(x, y + height);
    root.setPolygon(rootPolygon);
}

/**
 * Sets the root rectangle in which the treemap is computed.
 *
 * @param rectangle
 */
void voronoi::VoronoiTreemap::setRootRectangle(Rectangle2D.Double rectangle)
{
    setRootRectangle(rectangle.getX(), rectangle.getY(),
                     rectangle.getWidth(), rectangle.getHeight());
}

PolygonSimple voronoi::VoronoiTreemap::getRootPolygon()
{
    return rootPolygon;
}


void voronoi::VoronoiTreemap::setNumberThreads(int numberThreads)
{
    if (numberThreads >= 1)
    {
        this.numberThreads = numberThreads;
    } else
    {
        this.numberThreads = 1;
    }

}

/**
 * @return the numberThreads
 */
int voronoi::VoronoiTreemap::getNumberThreads()
{
    return numberThreads;
}

void voronoi::VoronoiTreemap::setStatusObject(StatusObject statusObject)
{
    this.statusObject.add(statusObject);
}

StatusObject voronoi::VoronoiTreemap::getStatusObject()
{
    return statusObject.get(0);
}

//@Override
void voronoi::VoronoiTreemap::finishedNode(int Node, int layer, int[] children,
                  PolygonSimple[] polygons)
{
    for (StatusObject statusObject : this.statusObject)
        statusObject.finishedNode(Node, layer, children, polygons);
}

void voronoi::VoronoiTreemap::setTree(ArrayList<ArrayList<Integer>> treeStructure)
{
    idToNode = new HashMap<Integer, VoroNode>();

    for (int i = 0; i < treeStructure.size(); i++)
    {
        ArrayList<Integer> adj = treeStructure.get(i);
        VoroNode node = new VoroNode(i, adj.size() - 1);
        idToNode.put(i, node);
        node.setTreemap(this);
    }

    root = idToNode.get(rootIndex);

    addChildren(idToNode, treeStructure, rootIndex);

    for (VoroNode voroNode : idToNode.values())
    {
        double x = rand.nextDouble();
        double y = rand.nextDouble();
        voroNode.setRelativeVector(new Point2D(x, y));
    }

    root.setVoroPolygon(rootPolygon);
}

void voronoi::VoronoiTreemap::clear()
{
    init();
}

void voronoi::VoronoiTreemap::setRandomSeed(long seed)
{
    randomSeed = seed;
    rand.setSeed(seed);
}

long voronoi::VoronoiTreemap::getRandomSeed()
{
    return randomSeed;
}

void voronoi::VoronoiTreemap::setErrorAreaThreshold(double d)
{
    coreSettings.errorThreshold = d;

}

void voronoi::VoronoiTreemap::setTreeData(TreeData treeData)
{
    this.treeData = treeData;
    rootIndex = treeData.rootIndex;

    setTree(treeData.tree);
    root.setVoroPolygon(rootPolygon);

    // set names and weights
    if (treeData != null && treeData.nodeAtt != null)
    {
        for (Integer id : idToNode.keySet())
        {
            VoroNode voroNode = idToNode.get(id);
            Node node = treeData.nodeAtt.get(id);
            if(node==null) continue;
            if (!getUniFormWeights())
                voroNode.setWeight(node.weight);
            voroNode.setName(node.name);
        }
    }

}

void voronoi::VoronoiTreemap::readEdgeList(String file)
{
    try
    {
        TreeData data = IO.readEdgeList(file);
        setTreeData(data);
    } catch (Exception e)
    {
        e.printStackTrace();
    }
}

bool voronoi::VoronoiTreemap::getUniFormWeights()
{
    return uniformWeights;
}

void voronoi::VoronoiTreemap::setUniformWeights(bool considerWeights)
{
    this.uniformWeights = considerWeights;
    if (uniformWeights)
        for (VoroNode node : idToNode.values())
            node.setWeight(1.0);
}

void voronoi::VoronoiTreemap::setNumberIterationsLevel(int[] levelsMaxIteration)
{
    this.levelsMaxIteration = levelsMaxIteration;
}

//==============PROTECTED MEMBER FUNCTIONS=============\\
void voronoi::VoronoiTreemap::recalculatePercentage()
{
    amountAllNodes = 0;
    alreadyDoneNodes = 0;
    root.calculateWeights();
}

void voronoi::VoronoiTreemap::setRootCell(VoroNode cell)
{
    this.root = cell;
    root.setHeight(1);
    root.setWantedPercentage(0);
}

VoroNode voronoi::VoronoiTreemap::getRootCell()
{
    return root;
}

void voronoi::VoronoiTreemap::init()
{
    initialized = false;
    useBorder = false;
    shrinkPercentage = 1;
    showLeafs = false;
    numberThreads = 1;
    root = null;
    rootPolygon = null;
    if (cellQueue != null)
        cellQueue.clear();
    statusObject = new ArrayList<StatusObject>();
    rand = new Random(randomSeed);
    if (idToNode != null)
        idToNode.clear();
    lock = new Semaphore(1);
}

void voronoi::VoronoiTreemap::initVoroNodes()
{
    if (!initialized && root != null)
    {
        initialized = true;
        cellQueue.clear();
        root.calculateWeights();
        setRelativePositions(relativePositions);
    }
}
void voronoi::VoronoiTreemap::setSettingsToVoroNode(VoroNode node)
{
    node.setTreemap(this);
}
//final
void voronoi::VoronoiTreemap::addChildren(HashMap<Integer, VoroNode> idToNode,
                       final ArrayList<ArrayList<Integer>> adjLists, int currentPos)
{
    ArrayList<Integer> childList = adjLists.get(currentPos);
    if (childList == null || childList.size() == 1)
        return;

    Integer parentId = childList.get(0);
    VoroNode voroParent = idToNode.get(parentId);

    // add children to parent

    for (int i = 1; i < childList.size(); i++)
    {
        Integer childId = childList.get(i);
        VoroNode voroChild = idToNode.get(childId);
        voroParent.addChild(voroChild);
        voroChild.setParent(voroParent);
    }
    // add children to children
    for (int i = 1; i < childList.size(); i++)
    {
        Integer childId = childList.get(i);
        addChildren(idToNode, adjLists, childId);
    }

}
void voronoi::VoronoiTreemap::setAmountNodes(int amountNodes)
{
    this.amountAllNodes = amountNodes;
}

int voronoi::VoronoiTreemap::getAmountNodes()
{
    return amountAllNodes;
}

void voronoi::VoronoiTreemap::drawTreemapWithComponents(Graphics2D g)
{
    for (VoroNode child : this)
    {
        JPolygon jp = new JPolygon(child.getNodeID(), new Integer(
                                       child.getNodeID()).toString());
    }
}
Graphics2D voronoi::VoronoiTreemap::getGraphics()
{
    return graphics;
}

//===========PRIVATE MEMBER FUNCS============\\
void startComputeThreads()
{
    this.runningThreads = Collections.newSetFromMap(new ConcurrentHashMap<VoroCPU, bool>());
    for (int i = 0; i < getNumberThreads(); i++)
        new VoroCPU(cellQueue, this, runningThreads).start();
}

 void setRelativePositions(ArrayList<Tuple3ID> relativePositions)
 {
    if (relativePositions == null)
    {
        for (VoroNode voroNode : idToNode.values())
        {
            double x = rand.nextDouble();
            double y = rand.nextDouble();
            voroNode.setRelativeVector(new Point2D(x, y));
        }
        return;
    }

    setReferenceMap(relativePositions);

}
void setShowLeafs(bool showLeafs)
{
    this.showLeafs = showLeafs;
}

void setGraphics(Graphics2D graphics)
{
    this.graphics = graphics;
}

voronoi::VoronoiTreemap::NodeIterator::NodeIterator(VoroNode root)
{
    stack = new Stack<VoroNode>();
    stack.addAll(root.getChildren());

}

//@Override
bool voronoi::VoronoiTreemap::NodeIterator::hasNext()
{
    if (stack.size() > 0)
    {
        return true;
    } else
        return false;
}

//@Override
VoroNode voronoi::VoronoiTreemap::NodeIterator::next()
{
    VoroNode t = stack.pop();
    if (t != null && t.getChildren() != null
            && t.getChildren().size() > 0)
    {
        stack.addAll(t.getChildren());
    }
    return t;
}

//@Override
void voronoi::VoronoiTreemap::NodeIterator::remove()
{

}
