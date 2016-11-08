#ifndef TREEDATA_H
#define TREEDATA_H
#include <unordered_map>
#include <string>
#include <vector>
namespace voronoi {
/**
 * Class for storing tree data (or hierarchy) with a root.
 * @author Arlind Nocaj
 *
 */
class TreeData
{
public:
    TreeData();
    ~TreeData();

    class Node
    {
    public:
        Node():parentId(-1),nodeId(-1),level(-1),name(""),weight(1){}

        int         nodeId;// id of this node
        int         parentId;// id of the parent node
        int         level;// level of this node
        double      weight;// weight of this node
        std::string name;//name of this node

        // relative coordinate of this node
        //Point2D relativeCoord;
    };

    void        setRoot(std::string name);
    void        setWeight(std::string name,double weight);
    void        addLink(std::string childName, std::string parentName);
    int         getRootIndex();
    std::vector<std::vector<int> >*         getTree();
    std::unordered_map<int,Node*>*          getNodeAttDict();

#ifdef JYXIA
    void        printSelf();
#endif
private:
    std::unordered_map<std::string, int>*       nodeNameToId_;//A pointer that contains name-id pair list,allocated memory      [Retrieve] 
    std::vector<std::vector<int> >*             tree_;        //A pointer that contains the tree structure, allocated memory    [Retrieve]
    std::unordered_map<int, Node*>*             nodeAtt_;     //A pointer that contains id-node pair list, allocated memory     [Retrieve]

    // index of the root node
    int         rootIndex_;

    void        init();
    Node*       createNode(std::string name);
};
}
#endif // TREEDATA_H
