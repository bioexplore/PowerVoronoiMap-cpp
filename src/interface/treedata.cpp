#include "treedata.h"

void voronoi::TreeData::setRoot(String name)
{
    if(!nodeNameToId.containsKey(name)) return;
    rootIndex=nodeNameToId.get(name);
}

void voronoi::TreeData::setWeight(String name,double weight)
{
    if(weight<=0) return;
    if(!nodeNameToId.containsKey(name)) return;
    nodeAtt.get(nodeNameToId.get(name)).weight=weight;
}

void voronoi::TreeData::addLink(String childName, String parentName)
{
    if(nodeNameToId==null)
        init();

    Node childNode=null;
    Node parentNode=null;

    if(!nodeNameToId.containsKey(parentName))
        createNode(parentName);

    if(!nodeNameToId.containsKey(childName))
        createNode(childName);

    childNode=nodeAtt.get(nodeNameToId.get(childName));
    parentNode=nodeAtt.get(nodeNameToId.get(parentName));

    childNode.parentId=parentNode.nodeId;
    tree.get(parentNode.nodeId).add(childNode.nodeId);
}

//============PRIVATE MEMBER FUNCS===========\\
void voronoi::TreeData::init()
{
    tree=new ArrayList<ArrayList<Integer>>();
    rootIndex=0;
    nodeAtt=new HashMap<Integer, TreeData.Node>();
    nodeNameToId=new HashMap<String, Integer>();
}
voronoi::TreeData::Node voronoi::TreeData::createNode(String name)
{
    int index=tree.size();
    Node node=new TreeData.Node();
    node.nodeId=index;
    node.parentId=index;
    node.name=name;
    ArrayList<Integer> adjList=new ArrayList<Integer>();
    adjList.add(index);
    tree.add(adjList);
    nodeAtt.put(index, node);
    nodeNameToId.put(name, index);
    return node;
}
