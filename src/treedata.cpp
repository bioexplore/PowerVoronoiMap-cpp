#ifdef JYXIA
#include <iostream>
#endif
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
    int id=findId->second;
    const auto& findNodeIter=nodeAtt_->find(id);
    Node* findNode=findNodeIter->second;
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
    
    findParent=nodeNameToId_->find(parentName);
    findChild=nodeNameToId_->find(childName);
    childNode=nodeAtt_->operator[](findChild->second);
    parentNode=nodeAtt_->operator[](findParent->second);

    childNode->parentId=parentNode->nodeId;
    tree_->at(parentNode->nodeId).push_back(childNode->nodeId);
}

int voronoi::TreeData::getRootIndex()
{
    return rootIndex_;
}
std::vector<std::vector<int> >* voronoi::TreeData::getTree()
{
    return tree_;
}
std::unordered_map<int, voronoi::TreeData::Node*>* voronoi::TreeData::getNodeAttDict()
{
    return nodeAtt_;
}
//============PRIVATE MEMBER FUNCS===========\\

void voronoi::TreeData::init()
{
    tree_=new std::vector<std::vector<int> >();
    rootIndex_=0;
    nodeAtt_=new std::unordered_map<int, TreeData::Node*>();
    nodeNameToId_=new std::unordered_map<std::string, int>();
}
voronoi::TreeData::Node* voronoi::TreeData::createNode(std::string name)
{
    int index=tree_->size();
    Node *node=new Node();
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

//=============Debug==============//
#ifdef JYXIA
void voronoi::TreeData::printSelf()
{
    std::cout<<"TreeData content:"<<std::endl
             <<"\tTotal node number:"<<tree_->size()<<std::endl;
    int size=tree_->size();
    for(int i=0;i<size;i++)
    {
        std::vector<int>& children=tree_->at(i);
        int id=children[0];
        std::cout<<"\tParent:("<<id<<","<<(nodeAtt_->operator[](id))->name<<")"<<std::endl;
        for(int j=1;j<children.size();j++)
        {
            std::cout<<((j==1)?("\t\tchildrens:"):("-"))<<"("<<children[j]<<","<<(nodeAtt_->operator[](children[j]))->name<<")"
                <<((j==children.size()-1)?("\n"):(""));
        }
    }
}
#endif
