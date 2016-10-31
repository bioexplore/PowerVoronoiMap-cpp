#include <iostream>
#include <limits>
#include <vector>
#include "voronode.h"
#include "treemap.h"

voronoi::VoroNode::VoroNode(int nodeID):
    children_(NULL),core_(NULL),height_(0),name(""),nodeID_(nodeID),parent_(NULL),
    polygon_(NULL),relativeVector_(-1,-1),site_(NULL),toConsider_(false),treemap_(NULL),
    wantedPercentage_(0),weight_(0)
{}

voronoi::VoroNode::VoroNode(int nodeID, int numberChildren):
    children_(NULL),core_(NULL),height_(0),name(""),nodeID_(nodeID),parent_(NULL),
    polygon_(NULL),relativeVector_(-1,-1),site_(NULL),toConsider_(false),treemap_(NULL),
    wantedPercentage_(0),weight_(0)
{
    children_=new std::vector<VoroNode*>(numberChildren);
}

voronoi::VoroNode::~VoroNode()
{
    if(children_) delete children_;
    children_=NULL;
}

void voronoi::VoroNode::calculateWeights() 
{
    if(!treemap_)
    {
        std::cerr<<"TreeMap member is not set for VoroNode"<<std::endl;
        return;
    }
    treemap_->amountAllNodes_++;//this can be invoked because Voronode is friend class of VoronoiTreemap
    if (children_ == NULL || children_->size() == 0) 
    {
        return;
    }
    double sum = 0;
    for (VoroNode* child : *children_) 
    {
        child->calculateWeights();
        sum += child->getWeight();
    }
    for (VoroNode* child : *children_) 
    {
        child->setWantedPercentage(child->getWeight() / sum);
    }
    weight_ = sum;
}

//TODO:need to redefine
void voronoi::VoroNode::setSpiralRelativeCoordinates()
{
    if (children_ == NULL || children_->size() == 0) 
    {
        return;
    }
    /*
    std::vector<VoroNode*>* nodes=children_;
    Collections.sort(nodes,new Comparator<VoroNode>()
    {
         //@Override
         int compare(VoroNode o1, VoroNode o2)
         {
             // TODO Auto-generated method stub
             return -Double.compare(o1.getWantedPercentage(),o2.getWantedPercentage());
         }

    });

    int i=0;
    Random rand=new Random();
    for (VoroNode voroNode : nodes) 
    {
        double angle=Math.PI*2.0*(i*1.0/nodes.size());
        double b=1;
        double a=1;
        //			double radius=b*Math.exp(a*angle);
        double radius=1-i*1.0/nodes.size()+(rand.nextFloat()-0.5)*0.2;
        double x =Math.cos(angle);
        double y =Math.sin(angle);
        x*=radius;
        y*=radius;
        voroNode.setRelativeVector(new Point2D(x, y));
        i++;
    }
    for (VoroNode child : children_)
        child.setSpiralRelativeCoordinates();
    */
}

void voronoi::VoroNode::setNodeID(int nodeID) 
{
    nodeID_ = nodeID;
}

int voronoi::VoroNode::getNodeID() 
{
    return nodeID_;
}

void voronoi::VoroNode::setParent(voronoi::VoroNode* parent) 
{
    parent_ = parent;
}

voronoi::VoroNode* voronoi::VoroNode::getParent()
{
    return parent_;
}

void voronoi::VoroNode::setChildren(std::vector<voronoi::VoroNode*>* children_) 
{
    children_ = children_;
}

std::vector<voronoi::VoroNode*>* voronoi::VoroNode::getChildren() 
{
    return children_;
}

void voronoi::VoroNode::setPolygon(voronoi::PolygonSimple* polygon) 
{
    polygon_ = polygon;
}

voronoi::PolygonSimple* voronoi::VoroNode::getPolygon() 
{
    return polygon_;
}

void voronoi::VoroNode::setToConsider(bool toConsider)
{
    toConsider = toConsider;
}

bool voronoi::VoroNode::isToConsider()
{
    return toConsider_;
}

void voronoi::VoroNode::setWantedPercentage(double percentage)
{
    weight_=percentage;
    wantedPercentage_ = percentage;
    if (site_ != NULL)
        site_->setPercentage(percentage);
}

double voronoi::VoroNode::getWantedPercentage()
{
    return wantedPercentage_;
}

void voronoi::VoroNode::addChild(voronoi::VoroNode* child)
{
    if (children_ == NULL)
    {
        children_ = new std::vector<VoroNode*>();
    }
    children_->push_back(child);
    child->parent_ = this;
    child->treemap_ = treemap_;
    child->height_ = height_ + 1;
}

void voronoi::VoroNode::iterate()
{
    if (children_ == NULL || children_->size() == 0)
        return;
    if (site_ != NULL)
        polygon_=site_->getPolygon();

    scaleRelativeVectors();
    if (core_ == NULL) 
    {
        core_ = new VoronoiCore(polygon_);
        setSettingsToCore();
        // add each children_ as a site
        for (VoroNode* child : *children_)
        {
            Point2D p = polygon_->getRelativePosition(child->relativeVector_);
            Site* s = new Site(p.getX(), p.getY());
            s->setPercentage(child->wantedPercentage_);
            s->setData(child);
            core_->addSite(s);
            child->setSite(s);
            s->cellObject_ = child;//Voronode is friend class of Site
        }
    } else 
    {
        // move my children_ so that they are in my polygon
        // use their relative Vector for that
        core_ = new VoronoiCore(polygon_);
        setSettingsToCore();
        for (VoroNode* child : *children_) 
        {
            if (child->getWantedPercentage() > 0) 
            {
                //FIXME: need to fix
                Point2D pos;
                if (child->relativeVector_.x !=-1 && child->relativeVector_.y !=-1 ) 
                {
                    pos = polygon_->getRelativePosition(child->relativeVector_);
                } else 
                {
                    pos = polygon_->getInnerPoint();
                }
                child->site_->setXY(pos.getX(), pos.getY());
                core_->addSite(child->site_);
            }
        }
    }
    core_->doIterate();
    if (treemap_->getUseBorder()) 
    {
        double shrinkPercentage=treemap_->getShrinkPercentage();
        std::vector<Site*>* siteList=core_->getSites();
        int length = siteList->size();
        for(Site* site:*siteList)
        {
            site->getPolygon()->shrinkForBorder(shrinkPercentage);
        }
    }
}

/**
 * Scales the relative vectors of the child nodes to the size of our current
 * polygon. Doing this helps to preserve the mental map.
 */
void voronoi::VoroNode::scaleRelativeVectors() 
{
    if (children_ == NULL)
        return;

    if(polygon_==NULL && parent_->children_->size()==1)
    {
        //TODO:need to define clone function of PolygonSimple
        //clone from daddy
        polygon_=parent_->polygon_->clone();
    }

    if (children_->size() == 1) 
    {
        VoroNode* child = children_->at(0);
        child->setRelativeVector(polygon_->getInnerPoint());
        return;
    }

    Rectangle2D* bounds = polygon_->getBounds();
    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::min();
    double maxY = std::numeric_limits<double>::min();
    double localCenterX = 0;
    double localCenterY = 0;
    for (VoroNode* child : *children_) 
    {
        Point2D pos = child->getRelativeVector();
        // TODO:negative means outsiede polygon
        if (pos.negative()) 
        {
            pos=polygon_->getInnerPoint();
        }
        localCenterX += pos.getX();
        localCenterY += pos.getY();
        if (pos.getX() < minX) 
        {
            minX = pos.getX();
        }
        if (pos.getX() > maxX) 
        {
            maxX = pos.getX();
        }
        if (pos.getY() < minY) 
        {
            minY = pos.getY();
        }
        if (pos.getY() > maxY) 
        {
            maxY = pos.getY();
        }
    }
    localCenterX = localCenterX / children_->size();
    localCenterY = localCenterY / children_->size();

    double scaleX = (bounds->getWidth() / (maxX - minX)) * 0.9;
    double scaleY = (bounds->getHeight() / (maxY - minY)) * 0.9;
    double centerX = bounds->getCenterX();
    double centerY = bounds->getCenterY();

    for (VoroNode* child : *children_) 
    {
        Point2D pos = child->getRelativeVector();
        pos.setLocation((pos.getX() - localCenterX) * scaleX,
                        (pos.getY() - localCenterY) * scaleY);
    }
}


void voronoi::VoroNode::increasePercentageDirectly() 
{
    weight_ *= 1.5;
    // site.percent=site.percent*1.5;
}

void voronoi::VoroNode::decreasePercentage() 
{
    site_->setPercentage(site_->getPercentage() * 0.4);
}

void voronoi::VoroNode::setHeight(int height) 
{
    height_ = height;
}

int voronoi::VoroNode::getHeight() 
{
    return height_;
}

void voronoi::VoroNode::setWeight(double weight) 
{
    weight_ = weight;
}

double voronoi::VoroNode::getWeight() 
{
    return weight_;
}

void voronoi::VoroNode::setSite(voronoi::Site* s) 
{
    site_ = s;
}

voronoi::Site* voronoi::VoroNode::getSite() 
{
    return site_;
}
void voronoi::VoroNode::setTreemap(voronoi::VoronoiTreemap* treemap) 
{
    treemap_ = treemap;
}

voronoi::VoronoiTreemap* voronoi::VoroNode::getTreemap() 
{
    return treemap_;
}

void voronoi::VoroNode::setRelativeVector(voronoi::Point2D relativeVector) 
{
    relativeVector_ = relativeVector;
}

voronoi::Point2D voronoi::VoroNode::getRelativeVector() 
{
    return relativeVector_;
}


//@Override
void voronoi::VoroNode::doFinalWork() 
{
    // TODO Auto-generated method stub

}

//@Override
void voronoi::VoroNode::setVoroPolygon(PolygonSimple* polygon) 
{
    polygon_=polygon;
}

/**
 * Who use this member func should retrieve the memory
 * that allocated for the returned int array
 * @author Jianye Xia
 */
int voronoi::VoroNode::getChildrenIDs(int* idList)
{
    int length;
    if (children_==NULL || (length=children_->size())==0)
    {
        return 0;
    }
    if(idList) delete idList;
    idList=new int[length];
    int i=0;
    for (VoroNode* node:*children_)
    {
        idList[i]=node->getNodeID();
        i++;
    }
    return length;
}

int voronoi::VoroNode::getChildrenPolygons(voronoi::PolygonSimple* childrenPolygonList[])
{
    int length;
    if (children_==NULL || (length=children_->size())==0)
    {
        return 0;
    }
    if(childrenPolygonList) delete childrenPolygonList;
    childrenPolygonList=new PolygonSimple*[length];
    int i=0;
    for (VoroNode* node: *children_)
    {
        childrenPolygonList[i]=node->getSite()->getPolygon();
        i++;
    }
    return length;
}

void voronoi::VoroNode::setName(std::string name) 
{
    this->name=name;
}

std::string voronoi::VoroNode::getName() 
{
    if(parent_!=NULL && parent_->children_->size()==1)
    {
        return parent_->getName()+"."+name;
    }
    return name;
}

//--------Private member funcs---------//
void voronoi::VoroNode::setSettingsToCore()
{
    core_->setLevel(height_);
    core_->setSettings(treemap_->coreSettings_);
}
