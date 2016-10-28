#include "voronode.h"

voronoi::VoroNode::VoroNode(int nodeID)
{
    this.nodeID = nodeID;
    relativeVector=new Point2D();
}

voronoi::VoroNode::VoroNode(int nodeID, int numberChildren){
    this(nodeID);
    children=new ArrayList<VoroNode>(numberChildren);
}

void voronoi::VoroNode::calculateWeights() {
    treemap.amountAllNodes++;
    if (children == null || children.size() == 0) {
        return;
    }
    double sum = 0;
    for (VoroNode child : children) {
        child.calculateWeights();
        sum += child.getWeight();
    }

    for (VoroNode child : children) {
        child.setWantedPercentage(child.getWeight() / sum);
    }
    this.weight = sum;

}

void voronoi::VoroNode::setSpiralRelativeCoordinates(){
    if (children == null || children.size() == 0) {
        return;
    }
    ArrayList<VoroNode> nodes=children;
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
    for (VoroNode voroNode : nodes) {
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
    for (VoroNode child : children)
        child.setSpiralRelativeCoordinates();
}
void voronoi::VoroNode::setNodeID(int nodeID) {
    this.nodeID = nodeID;
}

int voronoi::VoroNode::getNodeID() {
    return nodeID;
}

void voronoi::VoroNode::setParent(VoroNode parent) {
    this.parent = parent;
}

VoroNode voronoi::VoroNode::getParent() {
    return parent;
}

void voronoi::VoroNode::setChildren(ArrayList<VoroNode> children) {
    this.children = children;
}

ArrayList<VoroNode> voronoi::VoroNode::getChildren() {
    return children;
}

void voronoi::VoroNode::setPolygon(PolygonSimple polygon) {
    this.polygon = polygon;
}

PolygonSimple voronoi::VoroNode::getPolygon() {
    return polygon;
}

void voronoi::VoroNode::setToConsider(boolean toConsider) {
    this.toConsider = toConsider;
}

boolean voronoi::VoroNode::isToConsider() {
    return toConsider;
}

void voronoi::VoroNode::setWantedPercentage(double percentage) {
    this.weight=percentage;
    this.wantedPercentage = percentage;
    if (site != null)
        site.setPercentage(percentage);
}

double voronoi::VoroNode::getWantedPercentage() {
    return wantedPercentage;
}

void voronoi::VoroNode::addChild(VoroNode child) {
    if (children == null) {
        children = new ArrayList<VoroNode>();
    }
    children.add(child);
    child.parent = this;
    child.treemap = treemap;
    child.height = height + 1;
}

void voronoi::VoroNode::iterate()
{
    if (children == null || children.size() == 0)
        return;
    if (site != null)
        polygon=this.site.getPolygon();

    scaleRelativeVectors();
    if (this.core == null) {
        core = new VoronoiCore(this.polygon);
        setSettingsToCore();
        // add each children as a site
        for (VoroNode child : children) {
            Point2D p = polygon.getRelativePosition(child.relativeVector);

            Site s = new Site(p.getX(), p.getY());

            s.setPercentage(child.wantedPercentage);
            s.setData(child);
            core.addSite(s);
            child.setSite(s);
            s.cellObject = child;
        }
    } else {
        // move my children so that they are in my polygon
        // use their relative Vector for that
        core = new VoronoiCore(polygon);
        setSettingsToCore();
        for (VoroNode child : children) {

            if (child.getWantedPercentage() > 0) {
                Point2D pos = null;
                // if (getParent() != null) {
                if (child.relativeVector != null) {
                    pos = polygon.getRelativePosition(child.relativeVector);
                } else {
                    pos = polygon.getInnerPoint();
                }
                child.site.setXY(pos.getX(), pos.getY());

                core.addSite(child.site);
            }
        }

    }


    core.doIterate();

    if (treemap.getUseBorder()) {
        double shrinkPercentage=treemap.getShrinkPercentage();
        int length = core.getSites().size;
        Site[] sites = core.getSites().array;
        for (int i = 0; i < length; i++) {
            Site s = sites[i];
            s.getPolygon().shrinkForBorder(shrinkPercentage);
        }
    }
}

/**
 * Scales the relative vectors of the child nodes to the size of our current
 * polygon. Doing this helps to preserve the mental map.
 */
void voronoi::VoroNode::scaleRelativeVectors() {
    if (getChildren() == null)
        return;

    if(polygon==null && getParent().getChildren().size()==1){
        //clone from daddy
        polygon=getParent().getPolygon().clone();
    }

    if (getChildren().size() == 1) {
        VoroNode child = getChildren().get(0);
        child.setRelativeVector(polygon.getInnerPoint());
        return;
    }

    Rectangle bounds = polygon.getBounds();

    double minX = Double.MAX_VALUE;
    double minY = Double.MAX_VALUE;
    double maxX = Double.MIN_VALUE;
    double maxY = Double.MIN_VALUE;

    double localCenterX = 0;
    double localCenterY = 0;
    for (VoroNode child : getChildren()) {
        Point2D pos = child.getRelativeVector();
        // TODO
        if (pos == null) {
            pos=this.polygon.getInnerPoint();
        }

        localCenterX += pos.getX();
        localCenterY += pos.getY();

        if (pos.getX() < minX) {
            minX = pos.getX();
        }
        if (pos.getX() > maxX) {
            maxX = pos.getX();
        }
        if (pos.getY() < minY) {
            minY = pos.getY();
        }
        if (pos.getY() > maxY) {
            maxY = pos.getY();
        }
    }
    localCenterX = localCenterX / getChildren().size();
    localCenterY = localCenterY / getChildren().size();

    double scaleX = (bounds.getWidth() / (maxX - minX)) * 0.9;
    double scaleY = (bounds.getHeight() / (maxY - minY)) * 0.9;
    double centerX = bounds.getCenterX();
    double centerY = bounds.getCenterY();

    for (VoroNode child : getChildren()) {
        Point2D pos = child.getRelativeVector();
        pos.setLocation((pos.getX() - localCenterX) * scaleX,
                        (pos.getY() - localCenterY) * scaleY);
    }
}


void voronoi::VoroNode::increasePercentageDirectly() {
    weight = weight * 1.5;
    // site.percent=site.percent*1.5;
}

void voronoi::VoroNode::decreasePercentage() {
    site.setPercentage(site.getPercentage() * 0.4);
}

void voronoi::VoroNode::setHeight(int height) {
    this.height = height;
}

int voronoi::VoroNode::getHeight() {
    return height;
}

void voronoi::VoroNode::setWeight(double weight) {
    this.weight = weight;
}

double voronoi::VoroNode::getWeight() {
    return weight;
}

void voronoi::VoroNode::setSite(Site s) {
    this.site = s;
}

Site voronoi::VoroNode::getSite() {
    return site;
}
void voronoi::VoroNode::setTreemap(VoronoiTreemap treemap) {
    this.treemap = treemap;
}

VoronoiTreemap voronoi::VoroNode::getTreemap() {
    return treemap;
}

void voronoi::VoroNode::setRelativeVector(Point2D relativeVector) {
    this.relativeVector = relativeVector;
}

Point2D voronoi::VoroNode::getRelativeVector() {
    return relativeVector;
}


@Override
void voronoi::VoroNode::doFinalWork() {
    // TODO Auto-generated method stub

}

@Override
void voronoi::VoroNode::setVoroPolygon(PolygonSimple polygon) {
    this.polygon=polygon;
}

int[] voronoi::VoroNode::getChildrenIDs(){
    int length;
    if (children==null || (length=children.size())==0){
        return null;
    }
    int[] ids = new int[length];
    int i=0;
    for (VoroNode node:children){
        ids[i]=node.getNodeID();
        i++;
    }
    return ids;
}

PolygonSimple[] voronoi::VoroNode::getChildrenPolygons(){
    int length;
    if (children==null || (length=children.size())==0){
        return null;
    }
    PolygonSimple[] polygons=new PolygonSimple[length];
    int i=0;
    for (VoroNode node:children){
        polygons[i]=node.getSite().getPolygon();
        i++;
    }
    return polygons;
}

void voronoi::VoroNode::setName(String name) {
    this.name=name;
}

std::string voronoi::VoroNode::getName() {
    if(getParent()!=null && getParent().getChildren().size()==1){
        return getParent().getName()+"."+this.name;
    }
    return this.name;
}

//--------Private member funcs---------//
void voronoi::VoroNode::setSettingsToCore()
{
    core.setLevel(height);
    core.setSettings(treemap.coreSettings);
}
