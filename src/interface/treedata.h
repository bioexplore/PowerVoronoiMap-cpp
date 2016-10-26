#ifndef TREEDATA_H
#define TREEDATA_H
namespace voronoi {
/**
 * Class for storing tree data (or hierarchy) with a root.
 * @author Arlind Nocaj
 *
 */
class TreeData
{
public:
    //adjacency list of the tree structure
    ArrayList<ArrayList<Integer>> tree;
    // index of the root node
    Integer rootIndex;
    //map pointing to the node attributes of a specific id
    HashMap<Integer, Node> nodeAtt;

    static class Node
    {
    public:
        Node(){}

        // id of the parent node
        Integer parentId;
        // id of this node
        Integer nodeId;
        // level of this node
        Integer level;
        //name of this node
        String name;
        // weight of this node
        double weight=1;

        // relative coordinate of this node
        Point2D.Double relativeCoord;
    };

    void setRoot(String name);
    void setWeight(String name,double weight);
    void addLink(String childName, String parentName);

private:
    HashMap<String, Integer> nodeNameToId_;

    void init();
    Node createNode(String name);
};
}
#endif // TREEDATA_H
