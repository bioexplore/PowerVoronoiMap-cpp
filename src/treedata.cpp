#include "treedata.h"

voronoi::TreeData::TreeData():
    tree_(NULL),nodeNameToId_(NULL),rootIndex_(-1),nodeAtt_(NULL)
{
    init();
}
voronoi::TreeData::~TreeData()
{
    if(tree_) delete tree_;
    if(nodeAtt_)
    {
        for(const auto& e:*nodeAtt_)
            if(e.second) delete e.second;
        delete nodeAtt_;
    }
    if(nodeNameToId_) delete nodeNameToId_;
}

void voronoi::TreeData::setRoot(std::string name)
{
    const auto& findId=nodeNameToId_->find(name);
    if(findId==nodeNameToId_->end()) return;
    rootIndex_=findId->second;
}

void voronoi::TreeData::setWeight(std::string name,double weight)
{
    if(weight<=0) return;
    const auto& findId=nodeNameToId_->find(name);
    if(findId==nodeNameToId_->end()) return;
    int id=(*findId).second;
    const auto& findNodeIter=nodeAtt_->find(id);
    Node* findNode=(*findNodeIter).second;
    findNode->weight=weight;
}

void voronoi::TreeData::addLink(std::string childName,std::string parentName)
{
    if(nodeNameToId_==NULL)
        init();
    Node *childNode=NULL;
    Node *parentNode=NULL;
    
    auto findParent=nodeNameToId_->find(parentName);
    if(findParent==nodeNameToId_->end())
        createNode(parentName);
    auto findChild=nodeNameToId_->find(childName);
    if(findChild==nodeNameToId_->end())
        createNode(childName);
    
    const auto& findParent1=nodeNameToId_->find(parentName);
    const auto& findChild1=nodeNameToId_->find(childName);
    childNode=nodeAtt_->operator[](findChild1->second);
    parentNode=nodeAtt_->operator[](findParent1->second);

    childNode->parentId=parentNode->nodeId;
    tree_->at(parentNode->nodeId).push_back(childNode->nodeId);
}

//============PRIVATE MEMBER FUNCS===========\\

void voronoi::TreeData::init()
{
    tree_=new std::vector<std::vector<int>>();
    rootIndex_=0;
    nodeAtt_=new std::unordered_map<int, TreeData::Node*>();
    nodeNameToId_=new std::unordered_map<std::string, int>();
}
voronoi::TreeData::Node* voronoi::TreeData::createNode(std::string name)
{
    int index=tree_->size();
    Node *node=new TreeData::Node();
    node->nodeId=index;
    node->parentId=index;
    node->name=name;
    std::vector<int> adjList;
    adjList.push_back(index);
    tree_->push_back(adjList);
    nodeAtt_->insert({index, node});
    nodeNameToId_->insert({name, index});
    return node;
}
