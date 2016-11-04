#ifndef VOROCPU_H
#define VOROCPU_H
namespace voronoi {
/**
 * Uses a CPU completely(all kernels) for calculation of the voronoi cells. As soon as the
 * queue is not empty it starts calculating.
 *
 * @author Arlind Nocaj
 *
 */
class VoroCPU //extends Thread
{
public:
    Set<VoroCPU> runningThreads;

    VoroCPU(BlockingQueue<VoroNode> queue, StatusObject tellEnd, Set<VoroCPU> runningThreads)
    {
        this.tellEnd = tellEnd;
        this.cellQueue = queue;
        this.runningThreads=runningThreads;
    }

    //@Override
    void run()
    {
        while (true)
        {
            try
            {
                VoroNode voroNode = cellQueue.poll();
                if (voroNode == null)
                {
                    // notify that we are finished
                    if (runningThreads.size() == 0 && cellQueue.size() == 0)
                    {
                        break;
                    }else
                    {
                        voroNode = cellQueue.poll(100, TimeUnit.MILLISECONDS);
                        if (voroNode == null)
                        {
                            continue;
                        }
                    }
                }
                runningThreads.add(this);
                voroNode.iterate();
                tellEnd.finishedNode(voroNode.getNodeID(), voroNode.getHeight(),voroNode.getChildrenIDs(),voroNode.getChildrenPolygons());
                ArrayList<VoroNode> children = voroNode.getChildren();
                if (children!=null)
                {
                    for (VoroNode node : children)
                    {
                        cellQueue.add(node);
                    }
                }
                runningThreads.remove(this);
                if (runningThreads.size() == 0 && cellQueue.size() == 0 && (voroNode.getChildren()==null || voroNode.getChildren().size()==0 )) {
                    tellEnd.finished();
                }
            } catch (Exception e)
            {
                e.printStackTrace();
                cellQueue.clear();
                runningThreads.remove(this);
                System.out.println("VoroCPU is stopped.");;
            }
        }
    }

    synchronized void stopThreads()
    {
        for (VoroCPU v : runningThreads)
        {
            v.interrupt();
        }
    }
protected:
    int allNodes=0;
private:
    BlockingQueue<VoroNode> cellQueue;
    StatusObject tellEnd;
};
}

#endif // VOROCPU_H
