#include <ctime>
#include <cstdlib>
#include <iostream>
#include "treemap.h"
#include "tuple3id.h"

//===========PUBLIC MEMBER FUNCS============//
voronoi::VoronoiTreemap::VoronoiTreemap():
    alreadyDoneNodes_(0),amountAllNodes_(0),cellQueue_(NULL),coreSettings_(NULL),idToNode_(NULL),initialized_(false),levelsMaxIteration_(NULL),
    numberThreads_(1),randomSeed_(21),relativePositions_(NULL),root_(NULL),rootIndex_(-1),rootPolygon_(NULL),showLeafs_(false),shrinkPercentage_(1),
    statusObjects_(NULL),timeStart_(-1),timeEnd_(-1),treeData_(NULL),uniformWeights_(false),useBorder_(false)
{
    init();
}

voronoi::VoronoiTreemap::VoronoiTreemap(voronoi::StatusObject* statusObject, bool multiThreaded):
    alreadyDoneNodes_(0),amountAllNodes_(0),cellQueue_(NULL),coreSettings_(NULL),idToNode_(NULL),initialized_(false),levelsMaxIteration_(NULL),
    numberThreads_(1),randomSeed_(21),relativePositions_(NULL),root_(NULL),rootIndex_(-1),rootPolygon_(NULL),showLeafs_(false),shrinkPercentage_(1),
    statusObjects_(NULL),timeStart_(-1),timeEnd_(-1),treeData_(NULL),uniformWeights_(false),useBorder_(false)
{
    init();
    statusObjects_->push_back(statusObject);
}

voronoi::VoronoiTreemap::VoronoiTreemap(voronoi::StatusObject* statusObject):
    alreadyDoneNodes_(0),amountAllNodes_(0),cellQueue_(NULL),coreSettings_(NULL),idToNode_(NULL),initialized_(false),levelsMaxIteration_(NULL),
    numberThreads_(1),randomSeed_(21),relativePositions_(NULL),root_(NULL),rootIndex_(-1),rootPolygon_(NULL),showLeafs_(false),shrinkPercentage_(1),
    statusObjects_(NULL),timeStart_(-1),timeEnd_(-1),treeData_(NULL),uniformWeights_(false),useBorder_(false)
{
    init();
    statusObjects_->push_back(statusObject);
}

/** when a node is finished the status object is notified. **/

std::unordered_map<int,voronoi::VoroNode*>* voronoi::VoronoiTreemap::getIdToNode()
{
    return idToNode_;
}

void voronoi::VoronoiTreemap::compute()
{
    if (rootPolygon_ == NULL){
        //TODO:need using exception
        return;
        //throw new RuntimeException("Root Polygon not set.");
    }
    timeStart_ = std::time(0);
    initVoroNodes();
    cellQueue_->push_back(root_);
    startComputeThreads();
}

//FIXME:to add threads
void voronoi::VoronoiTreemap::computeLocked()
{
    /*try {
        lock.acquire();
    } catch (InterruptedException e) {
        e.printStackTrace();
    }*/
    compute();
    /*try {
        lock.acquire();
        lock.release();
    } catch (InterruptedException e) {
        e.printStackTrace();
    }*/
}
void voronoi::VoronoiTreemap::setReferenceMap(std::vector<voronoi::Tuple3ID*>* relativePositions)
{
    for(std::vector<Tuple3ID*>::iterator tuple=relativePositions->begin();tuple!=relativePositions->end();++tuple)
    {
        int id=(*tuple)->id;
        double x=(*tuple)->valueX;
        double y=(*tuple)->valueY;
        VoroNode* voroNode = idToNode_->find(id)->second;
        if (voroNode != NULL)
            voroNode->setRelativeVector(Point2D(x,y));
        else
            std::cout<<"Node id could not be found for setting reference position: "<<id<<std::endl;
    }
}

bool voronoi::VoronoiTreemap::getShowLeafs()
{
    return showLeafs_;
}
//@Override
/*Iterator<VoroNode> voronoi::VoronoiTreemap::iterator()
{
    return new NodeIterator(root);
}*/

//@Override
//synchronized
void voronoi::VoronoiTreemap::finished()
{
    for(std::vector<StatusObject*>::iterator statusObject=statusObjects_->begin();statusObject!=statusObjects_->end();++statusObject)
       (*statusObject)->finished();
    //lock.release();
}
/*
void voronoi::VoronoiTreemap::drawTreemap(Graphics2D g)
{
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                       RenderingHints.VALUE_ANTIALIAS_ON);
    int lastNodes = 0;
    int amountPolygons = 0;
    for (VoroNode node : this)
    {
        int height = node.getHeight();
        if (node.getChildren() == NULL)
        {
            lastNodes++;
        }
        Site site = node.getSite();
        if (site != NULL)
        {
            PolygonSimple poly = site.getPolygon();
            if (poly != NULL)
            {
                // poly.shrinkForBorder(0.95);
                amountPolygons++;
                g.draw(poly);
            }
        }

    }
}
*/

void voronoi::VoronoiTreemap::setShrinkPercentage(double shrinkPercentage)
{
    shrinkPercentage_ = shrinkPercentage;
}


double voronoi::VoronoiTreemap::getShrinkPercentage()
{
    return shrinkPercentage_;
}

void voronoi::VoronoiTreemap::setUseBorder(bool useBorder)
{
    useBorder_ = useBorder;
}


bool voronoi::VoronoiTreemap::getUseBorder()
{
    return useBorder_;
}

void voronoi::VoronoiTreemap::setNumberMaxIterations(int numberMaxIterations)
{
    coreSettings_->maxIteration = numberMaxIterations;
}

int voronoi::VoronoiTreemap::getNumberMaxIterations()
{
    return coreSettings_->maxIteration;
}

void voronoi::VoronoiTreemap::setCancelOnThreshold(bool cancelOnThreshold)
{
    coreSettings_->cancelAreaError = cancelOnThreshold;
}

bool voronoi::VoronoiTreemap::getCancelOnThreshold()
{
    return coreSettings_->cancelAreaError;
}

void voronoi::VoronoiTreemap::setCancelOnMaxIteration(bool cancelOnMaxIterat)
{
    coreSettings_->cancelMaxIteration = cancelOnMaxIterat;
}


bool voronoi::VoronoiTreemap::getCancelOnMaxIteration()
{
    return coreSettings_->cancelMaxIteration;
}

void voronoi::VoronoiTreemap::setRootPolygon(voronoi::PolygonSimple* rootPolygon)
{
    rootPolygon_ = rootPolygon;
    if (root_ != NULL)
        root_->setPolygon(rootPolygon);
}


void voronoi::VoronoiTreemap::setRootRectangle(double x, double y, double width, double height)
{
    rootPolygon_ = new PolygonSimple();
    rootPolygon_->add(x, y);
    rootPolygon_->add(x + width, y);
    rootPolygon_->add(x + width, y + height);
    rootPolygon_->add(x, y + height);
    root_->setPolygon(rootPolygon_);
}

/**
 * Sets the root rectangle in which the treemap is computed.
 *
 * @param rectangle
 */
void voronoi::VoronoiTreemap::setRootRectangle(voronoi::Rectangle2D* rectangle)
{
    setRootRectangle(rectangle->getX(), rectangle->getY(),
                     rectangle->getWidth(), rectangle->getHeight());
}

voronoi::PolygonSimple* voronoi::VoronoiTreemap::getRootPolygon()
{
    return rootPolygon_;
}


void voronoi::VoronoiTreemap::setNumberThreads(int numberThreads)
{
    if (numberThreads >= 1)
    {
        numberThreads_ = numberThreads;
    } else
    {
        numberThreads_ = 1;
    }
}

/**
 * @return the numberThreads
 */
int voronoi::VoronoiTreemap::getNumberThreads()
{
    return numberThreads_;
}

void voronoi::VoronoiTreemap::setStatusObject(voronoi::StatusObject* statusObject)
{
    statusObjects_->push_back(statusObject);
}

voronoi::StatusObject* voronoi::VoronoiTreemap::getStatusObject()
{
    StatusObject* statusObj=statusObjects_->at(0);
    statusObjects_->erase(statusObjects_->begin());
    return statusObj;
}

//@Override
void voronoi::VoronoiTreemap::finishedNode(int Node, int layer, int* children, voronoi::PolygonSimple* polygons)
{
    for(std::vector<StatusObject*>::iterator statusObj=statusObjects_->begin();statusObj!=statusObjects_->end();++statusObj)
        (*statusObj)->finishedNode(Node,layer,children,polygons);
}

void voronoi::VoronoiTreemap::setTree(std::vector<std::vector<int> >& treeStructure)
{
    if (idToNode_==NULL)
        idToNode_ = new std::unordered_map<int, VoroNode*>();
    for (int i = 0; i < treeStructure.size(); i++)
    {
        std::vector<int>& adj = treeStructure.at(i);
        VoroNode* node = new VoroNode(i, adj.size() - 1);
        idToNode_->insert({i, node});
        node->setTreemap(this);
    }
    const auto& nodePair1=idToNode_->find(rootIndex_);
    root_ =nodePair1->second; 
    addChildren(idToNode_, treeStructure, rootIndex_);
    for(const auto& nodePair:*idToNode_)
    {
        double randx = ((double)std::rand()/(RAND_MAX));
        double randy = ((double)std::rand()/(RAND_MAX));
        VoroNode* node=nodePair.second;
        node->setRelativeVector(Point2D(randx, randy));
    }
    root_->setVoroPolygon(rootPolygon_);
}

void voronoi::VoronoiTreemap::clear()
{
    alreadyDoneNodes_=0;
    amountAllNodes_=0;
    if(cellQueue_) 
        delete cellQueue_;
    cellQueue_=NULL;
    if(coreSettings_) 
        delete coreSettings_;
    coreSettings_=NULL;
    if(idToNode_)
        delete idToNode_;
    idToNode_=NULL;
    initialized_=false;
    if(levelsMaxIteration_)
        delete levelsMaxIteration_;
    levelsMaxIteration_=NULL;
    numberThreads_=1;
    randomSeed_=21;
    if(relativePositions_)
        delete relativePositions_;
    relativePositions_=NULL;
    if(root_)
        delete root_;
    root_=NULL;
    rootIndex_=-1;
    if(rootPolygon_)
        delete rootPolygon_;
    rootPolygon_=NULL;
    showLeafs_=false;
    shrinkPercentage_=1;
    if(statusObjects_)
        delete statusObjects_;
    statusObjects_=NULL;
    timeStart_=-1;
    timeEnd_=-1;
    if(treeData_)
        delete treeData_;
    treeData_=NULL;
    uniformWeights_=false;
    useBorder_=false;
    init();
}

void voronoi::VoronoiTreemap::setErrorAreaThreshold(double d)
{
    coreSettings_->errorThreshold = d;

}

void voronoi::VoronoiTreemap::setTreeData(voronoi::TreeData* treeData)
{
    treeData_ = treeData;
    rootIndex_ = treeData->getRootIndex();

    setTree(*(treeData->getTree()));
    root_->setVoroPolygon(rootPolygon_);

    // set names and weights
    if (treeData != NULL && treeData->getNodeAttDict() != NULL)
    {
        for(auto& nodePair:*idToNode_)
        {
            VoroNode* voroNode = nodePair.second;
            int id=nodePair.first;
            const auto& tmp = treeData->getNodeAttDict()->find(id);
            if(tmp==treeData->getNodeAttDict()->end()) continue;
            TreeData::Node* node=tmp->second;
            if (!getUniFormWeights())
                voroNode->setWeight(node->weight);
            voroNode->setName(node->name);
        }
    }

}

//FIXME: redefine this func
void voronoi::VoronoiTreemap::readEdgeList(std::string file)
{
    /*
    try
    {
        TreeData data = IO.readEdgeList(file);
        setTreeData(data);
    } catch (Exception e)
    {
        e.printStackTrace();
    }
    */
}

bool voronoi::VoronoiTreemap::getUniFormWeights()
{
    return uniformWeights_;
}

void voronoi::VoronoiTreemap::setUniformWeights(bool considerWeights)
{
    uniformWeights_ = considerWeights;
    if (uniformWeights_)
        for (auto& nodePair : *idToNode_)
           nodePair.second->setWeight(1.0);
}

void voronoi::VoronoiTreemap::setNumberIterationsLevel(int* levelsMaxIteration)
{
    levelsMaxIteration_ = levelsMaxIteration;
}

//==============PROTECTED MEMBER FUNCTIONS=============//
void voronoi::VoronoiTreemap::recalculatePercentage()
{
    amountAllNodes_ = 0;
    alreadyDoneNodes_ = 0;
    root_->calculateWeights();
}

void voronoi::VoronoiTreemap::setRootCell(voronoi::VoroNode* cell)
{
    root_ = cell;
    root_->setHeight(1);
    root_->setWantedPercentage(0);
}

voronoi::VoroNode* voronoi::VoronoiTreemap::getRootCell()
{
    return root_;
}

void voronoi::VoronoiTreemap::init()
{
    cellQueue_=new std::vector<VoroNode*>;
    coreSettings_=new VoroSettings;
    idToNode_=new std::unordered_map<int,VoroNode*>;
    statusObjects_=new std::vector<StatusObject*>;
    rootPolygon_=new PolygonSimple;
}

void voronoi::VoronoiTreemap::initVoroNodes()
{
    if (!initialized_ && root_ != NULL)
    {
        initialized_ = true;
        cellQueue_->clear();
        root_->calculateWeights();
        setRelativePositions(relativePositions_);
    }
}
void voronoi::VoronoiTreemap::setSettingsToVoroNode(voronoi::VoroNode* node)
{
    node->setTreemap(this);
}
//final
void voronoi::VoronoiTreemap::addChildren(std::unordered_map<int,voronoi::VoroNode*>* idToNode,std::vector<std::vector<int> >& adjLists, int currentPos)
{
    std::vector<int> childList = adjLists.at(currentPos);
    if ( childList.size() == 1)
        return;

    int parentId = childList.at(0);
    VoroNode* voroParent = idToNode->find(parentId)->second;

    // add children to parent

    for (int i = 1; i < childList.size(); i++)
    {
        int childId = childList.at(i);
        VoroNode *voroChild = idToNode->find(childId)->second;
        voroParent->addChild(voroChild);
        voroChild->setParent(voroParent);
    }
    // add children to children
    for (int i = 1; i < childList.size(); i++)
    {
        int childId = childList.at(i);
        addChildren(idToNode, adjLists, childId);
    }
}
void voronoi::VoronoiTreemap::setAmountNodes(int amountNodes)
{
    amountAllNodes_ = amountNodes;
}

int voronoi::VoronoiTreemap::getAmountNodes()
{
    return amountAllNodes_;
}

/*
void voronoi::VoronoiTreemap::drawTreemapWithComponents(Graphics2D g)
{
    for (VoroNode child : this)
    {
        JPolygon jp = new JPolygon(child.getNodeID(), new int(
                                       child.getNodeID()).toString());
    }
}
Graphics2D voronoi::VoronoiTreemap::getGraphics()
{
    return graphics;
}*/

//===========PRIVATE MEMBER FUNCS============//
void voronoi::VoronoiTreemap::startComputeThreads()
{
    //runningThreads = Collections.newSetFromMap(new Concurrentstd::unordered_map<VoroCPU, bool>());
    //for (int i = 0; i < getNumberThreads(); i++)
        //new VoroCPU(cellQueue, this, runningThreads).start();
   if(cellQueue_->size()==0) return;
   while(true)
   {
       VoroNode* node=cellQueue_->at(0);
       node->iterate();
   }
   
}

void voronoi::VoronoiTreemap::setRelativePositions(std::vector<voronoi::Tuple3ID*>* relativePositions)
{
    if (relativePositions == NULL)
    {
        for (const auto& voroNodePair : *idToNode_)
        {
            double x = ((double)std::rand()/RAND_MAX);
            double y = ((double)std::rand()/RAND_MAX);
            VoroNode* voroNode=voroNodePair.second;
            voroNode->setRelativeVector(Point2D(x, y));
        }
        return;
    }
    setReferenceMap(relativePositions);
}
void voronoi::VoronoiTreemap::setShowLeafs(bool showLeafs)
{
    showLeafs_ = showLeafs;
}

/*
void setGraphics(Graphics2D graphics)
{
    graphics = graphics;
}
*/
/*
voronoi::VoronoiTreemap::NodeIterator::NodeIterator(VoroNode root)
{
    stack = new Stack<VoroNode>();
    stack.addAll(root_->getChildren());

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
    if (t != NULL && t.getChildren() != NULL && t.getChildren().size() > 0)
    {
        stack.addAll(t.getChildren());
    }
    return t;
}

//@Override
void voronoi::VoronoiTreemap::NodeIterator::remove()
{

}
*/
