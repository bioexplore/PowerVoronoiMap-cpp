#include <limits>
#include <cmath>
#include "powerdiagram.h"
#include "face.h"
#include "convexhull.h"

voronoi::PowerDiagram::PowerDiagram():
   amountPolygons_(0),bb_(),s1_(NULL),s2_(NULL),s3_(NULL),s4_(NULL),
   hull_(NULL),sites_(NULL),facets_(NULL),clipPoly_(NULL)
{}

voronoi::PowerDiagram::PowerDiagram(std::vector<voronoi::Site*>* sites, voronoi::PolygonSimple* clipPoly):
   amountPolygons_(0),bb_(),s1_(NULL),s2_(NULL),s3_(NULL),s4_(NULL),
   hull_(NULL),sites_(sites),facets_(NULL),clipPoly_(clipPoly)
{
}

//Do not need to consider sites_ and clipPoly_, as they are passed through
//by setSites and setClipyPoly funciton
voronoi::PowerDiagram::~PowerDiagram()
{
    if(s1_) delete s1_;
    if(s2_) delete s2_;
    if(s3_) delete s3_;
    if(s4_) delete s4_;
    if(hull_) delete hull_;
}
void voronoi::PowerDiagram::setSites(std::vector<voronoi::Site*>* sites)
{
    sites_ = sites;
    if(hull_) delete hull_;//Retrieve memory
    hull_=NULL;
}

void voronoi::PowerDiagram::setClipPoly(voronoi::PolygonSimple* polygon)
{
    clipPoly_ = polygon;
    bb_ = *(polygon->getBounds2D());
    // create sites on a rectangle which is big enough to not create
    // bisectors which intersect the clippingPolygon
    double minX = bb_.getX();
    double minY = bb_.getY();

    double width = bb_.getWidth();
    double height = bb_.getHeight();

    if(s1_) delete s1_;
    if(s2_) delete s2_;
    if(s3_) delete s3_;
    if(s4_) delete s4_;

    s1_ = new Site(minX - width, minY - height);
    s2_ = new Site(minX + 2 * width, minY - height);
    s3_ = new Site(minX + 2 * width, minY + 2 * height);
    s4_ = new Site(minX - width, minY + 2 * height);

    s1_->setAsDummy();
    s2_->setAsDummy();
    s3_->setAsDummy();
    s4_->setAsDummy();
}

voronoi::PolygonSimple* voronoi::PowerDiagram::getClipPoly()
{
    return clipPoly_;
}

void voronoi::PowerDiagram::computeDiagram()
{
    if (sites_->size() > 0)
    {
        //sites_->permutate();
        hull_ = new ConvexHull();
        int size = sites_->size();
        for (int z = 0; z < size; z++)
        {
            Site* s = sites_->at(z);
            hull_->addPoint(s);
        }
        // reset the border sites, otherwise they could have old data
        // cached.
        s1_->clear();
        s2_->clear();
        s3_->clear();
        s4_->clear();

        hull_->addPoint(s1_);
        hull_->addPoint(s2_);
        hull_->addPoint(s3_);
        hull_->addPoint(s4_);

        facets_ = hull_->compute();
        computeData();
    }
}

//TODO: Need improve
void voronoi::PowerDiagram::writeHullTestCodeOut(voronoi::Site* s)
{
   /*
    System.out.println("hull_->addPoint(" + s.x + "," + s.y + "," + s.z
                       + ");");
                       */
}

void voronoi::PowerDiagram::setAmountPolygons(int amountPolygons)
{
    amountPolygons_ = amountPolygons;
}

int voronoi::PowerDiagram::getAmountPolygons()
{
    return amountPolygons_;
}

void voronoi::PowerDiagram::initDebug()
{
    /*
    BufferedImage image = new BufferedImage(2000, 2000,
                                            BufferedImage.TYPE_INT_RGB);
    frame = new ImageFrame(image);
    frame.setVisible(true);
    frame.setBounds(20, 20, 1600, 800);
    graphics = image.createGraphics();
    graphics.translate(200, 200);
    */
}

void voronoi::PowerDiagram::showDiagram()
{
   /*
    initDebug();
    graphics.clearRect(0, 0, 1600, 800);
    graphics.setColor(Color.blue);

    Site[] array = sites_->array;
    int size = sites_->size;
    for (int z = 0; z < size; z++)
    {
        Site s = array[z];
        s.paint(graphics);
        PolygonSimple poly = s.getPolygon();
        if (poly != null) {
            graphics.draw(poly);
        } else {
            System.out.println("Poly null of:" + s);
        }
    }
    frame.repaint();
    */
}

void voronoi::PowerDiagram::draw(voronoi::Site s)
{
    /*
    s.paint(graphics);
    PolygonSimple poly = s.getPolygon();
    if (poly != null)
    {
        graphics.draw(poly);
    } else
    {
        System.out.println("Poly null of:" + s);
    }
    */
}

//----------Private Member Functions------------//
void voronoi::PowerDiagram::computeData()
{
    // make all vertices visible. When we finished working on one we make
    // invisible to not do it several times
    int vertexCount = hull_->getVertexCount();
    std::vector<bool> verticesVisited(vertexCount,false);

    int facetCount = facets_->size();
    for (int i = 0; i < facetCount; i++)
    {
        Face* facet = facets_->at(i);
        if (facet->isVisibleFromBelow())
        {
            for (int e = 0; e < 3; e++)
            {
                // got through the edges and start to build the polygon by
                // going through the double connected edge list
                HullEdge* edge = facet->getEdge(e);
                Vertex* destVertex = edge->getEnd();
                Site* site = (Site*)(destVertex);
                if (!verticesVisited[destVertex->getIndex()])
                {
                    verticesVisited[destVertex->getIndex()] = true;
                    if (site->isDummy)
                    {
                        continue;
                    }
                    // faces around the vertices which correspond to the
                    // polygon corner points
                    std::vector<Face*>* faces = getFacesOfDestVertex(edge);
                    PolygonSimple* poly = new PolygonSimple();
                    //ref http://en.cppreference.com/w/cpp/types/numeric_limits/quiet_NaN
                    //need climit header and cmath
                    double lastX = std::numeric_limits<double>::quiet_NaN();
                    double lastY = std::numeric_limits<double>::quiet_NaN();
                    double dx = 1;
                    double dy = 1;
                    for (Face* face : *faces)
                    {
                        Point2D point = face->getDualPoint();
                        double x1 = point.getX();
                        double y1 = point.getY();
                        if (!std::isnan(lastX))
                        {
                            dx = lastX - x1;
                            dy = lastY - y1;
                            if (dx < 0) {
                                dx = -dx;
                            }
                            if (dy < 0) {
                                dy = -dy;
                            }
                        }
                        if (dx > numericError_ || dy > numericError_)
                        {
                            poly->add(x1, y1);
                            lastX = x1;
                            lastY = y1;
                        }
                    }
                    if(site->nonClippedPolygon) delete site->nonClippedPolygon;
                    site->nonClippedPolygon = poly;
                    if (!site->isDummy)
                    {
                        site->setPolygon(clipPoly_->convexClip(poly));
                    }
                    if(faces) delete faces;
                }
            }
        }
    }
}
//TODO:need to take care of faces returned,which contains new applied memory
std::vector<voronoi::Face*>* voronoi::PowerDiagram::getFacesOfDestVertex(voronoi::HullEdge* edge)
{
    std::vector<Face*>* faces = new std::vector<Face*>();
    HullEdge* previous = edge;
    Vertex* first = edge->getEnd();

    Site* site = (Site*)(first);
    std::vector<Site*>* neighbours = new std::vector<Site*>();
    do
    {
        previous = previous->getTwin()->getPrev();
        // add neighbour to the neighbourlist
        Site* siteOrigin = (Site*)(previous->getStart());
        if (!siteOrigin->isDummy) {
            neighbours->push_back(siteOrigin);
        }
        Face* iFace = previous->getiFace();
        if (iFace->isVisibleFromBelow()) {
            faces->push_back(iFace);
        }
    } while (previous != edge);
    site->setNeighbours(neighbours);
    return faces;
}

