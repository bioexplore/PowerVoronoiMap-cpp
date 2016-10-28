#include "../treedata.h"
int main()
{
    voronoi::TreeData treeData;
    treeData.addLink("subProject1","Project1");
    treeData.addLink("sim1","subProject1");
    treeData.addLink("sim2","subProject1");
    return 0;
}
