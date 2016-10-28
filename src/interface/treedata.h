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

        // id of the parent node
        int parentId;
        // id of this node
        int nodeId;
        // level of this node
        int level;
        //name of this node
        std::string name;
        // weight of this node
        double weight;

        // relative coordinate of this node
        //Point2D relativeCoord;
    };

    void setRoot(std::string name);
    void setWeight(std::string name,double weight);
    void addLink(std::string childName, std::string parentName);

private:
    std::unordered_map<std::string, int>* nodeNameToId_;
    //adjacency list of the tree structure
    std::vector<std::vector<int> >* tree_;
    // index of the root node
    int rootIndex_;
    //map pointing to the node attributes of a specific id
    std::unordered_map<int, Node*>* nodeAtt_;


    void init();
    Node* createNode(std::string name);
};
}
#endif // TREEDATA_H
