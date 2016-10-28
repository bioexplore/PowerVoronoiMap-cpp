#include "powerdiagram.h"

voronoi::PowerDiagram::PowerDiagram()
{
    sites = null;
    clipPoly = null;
}

voronoi::PowerDiagram::PowerDiagram(OpenList sites, PolygonSimple clipPoly)
{
    setSites(sites);
    setClipPoly(clipPoly);
}

void voronoi::PowerDiagram::setSites(OpenList sites)
{
    this.sites = sites;
    hull = null;
}

void voronoi::PowerDiagram::setClipPoly(PolygonSimple polygon)
{
    clipPoly = polygon;
    bb = polygon.getBounds2D();
    // create sites on a rectangle which is big enough to not create
    // bisectors which intersect the clippingPolygon
    double minX = bb.getMinX();
    double minY = bb.getMinY();

    double width = bb.getWidth();
    double height = bb.getHeight();

    s1 = new Site(minX - width, minY - height);
    s2 = new Site(minX + 2 * width, minY - height);
    s3 = new Site(minX + 2 * width, minY + 2 * height);
    s4 = new Site(minX - width, minY + 2 * height);

    s1.setAsDummy();
    s2.setAsDummy();
    s3.setAsDummy();
    s4.setAsDummy();

}

PolygonSimple voronoi::PowerDiagram::getClipPoly()
{
    return clipPoly;
}

void voronoi::PowerDiagram::computeDiagram()
{
    if (sites.size > 0)
    {
        sites.permutate();
        hull = new JConvexHull();
        Site[] array = sites.array;
        int size = sites.size;
        for (int z = 0; z < size; z++)
        {
            Site s = array[z];
            if (Double.isNaN(s.getWeight()))
            {
                 throw new RuntimeException("Weight of a Site may not be NaN.");
            }
            hull.addPoint(s);
        }
        // reset the border sites, otherwise they could have old data
        // cached.
        s1.clear();
        s2.clear();
        s3.clear();
        s4.clear();

        hull.addPoint(s1);
        hull.addPoint(s2);
        hull.addPoint(s3);
        hull.addPoint(s4);

        facets = hull.compute();
        computeData();
    }
}

void voronoi::PowerDiagram::writeHullTestCodeOut(Site s)
{
    System.out.println("hull.addPoint(" + s.x + "," + s.y + "," + s.z
                       + ");");
}

void voronoi::PowerDiagram::setAmountPolygons(int amountPolygons)
{
    this.amountPolygons = amountPolygons;
}

int voronoi::PowerDiagram::getAmountPolygons()
{
    return amountPolygons;
}

static void voronoi::PowerDiagram::initDebug()
{
    BufferedImage image = new BufferedImage(2000, 2000,
                                            BufferedImage.TYPE_INT_RGB);
    frame = new ImageFrame(image);
    frame.setVisible(true);
    frame.setBounds(20, 20, 1600, 800);
    graphics = image.createGraphics();
    graphics.translate(200, 200);
}

static void main(String[] args)
{
    PowerDiagram diagram = new PowerDiagram();
    //normal list based on an array
    OpenList sites = new OpenList();
    Random rand = new Random(100);
    //  create a root polygon which limits the voronoi diagram.
    //  here it is just a rectangle.
    PolygonSimple rootPolygon = new PolygonSimple();
    int width = 1000;
    int height = 1000;
    rootPolygon.add(0, 0);
    rootPolygon.add(width, 0);
    rootPolygon.add(width, height);
    rootPolygon.add(0, height);
    // create 100 points (sites) and set random positions in the rectangle defined above.
    for (int i = 0; i < 100; i++)
    {
        Site site = new Site(rand.nextInt(width), rand.nextInt(width));
        // we could also set a different weighting to some sites
        // site.setWeight(30)
        sites.add(site);
    }
    // set the list of points (sites), necessary for the power diagram
    diagram.setSites(sites);
    // set the clipping polygon, which limits the power voronoi diagram
    diagram.setClipPoly(rootPolygon);
    // do the computation
    diagram.computeDiagram();
    // for each site we can no get the resulting polygon of its cell.
    // note that the cell can also be empty, in this case there is no polygon for the corresponding site.
    for (int i=0;i<sites.size;i++)
    {
        Site site=sites.array[i];
        PolygonSimple polygon=site.getPolygon();
    }
}

void voronoi::PowerDiagram::showDiagram()
{
    initDebug();
    graphics.clearRect(0, 0, 1600, 800);
    graphics.setColor(Color.blue);

    Site[] array = sites.array;
    int size = sites.size;
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
}

void voronoi::PowerDiagram::draw(Site s)
{
    s.paint(graphics);
    PolygonSimple poly = s.getPolygon();
    if (poly != null)
    {
        graphics.draw(poly);
    } else
    {
        System.out.println("Poly null of:" + s);
    }
}

//----------Private Member Functions------------//
void voronoi::PowerDiagram::computeData()
{
    // make all vertices visible. When we finished working on one we make
    // invisible to not do it several times
    int vertexCount = hull.getVertexCount();
    boolean[] verticesVisited = new boolean[vertexCount];

    int facetCount = facets.size();
    for (int i = 0; i < facetCount; i++)
    {
        JFace facet = facets.get(i);
        if (facet.isVisibleFromBelow())
        {
            for (int e = 0; e < 3; e++)
            {
                // got through the edges and start to build the polygon by
                // going through the double connected edge list
                HEdge edge = facet.getEdge(e);
                JVertex destVertex = edge.getDest();
                Site site = (Site) destVertex.originalObject;
                if (!verticesVisited[destVertex.getIndex()])
                {
                    verticesVisited[destVertex.getIndex()] = true;
                    if (site.isDummy)
                    {
                        continue;
                    }
                    // faces around the vertices which correspond to the
                    // polygon corner points
                    ArrayList<JFace> faces = getFacesOfDestVertex(edge);
                    PolygonSimple poly = new PolygonSimple();
                    double lastX = Double.NaN;
                    double lastY = Double.NaN;
                    double dx = 1;
                    double dy = 1;
                    for (JFace face : faces)
                    {
                        Point2D point = face.getDualPoint();
                        double x1 = point.getX();
                        double y1 = point.getY();
                        if (!Double.isNaN(lastX))
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
                        if (dx > numericError || dy > numericError)
                        {
                            poly.add(x1, y1);
                            lastX = x1;
                            lastY = y1;
                        }
                    }
                    site.nonClippedPolyon = poly;
                    if (!site.isDummy)
                    {
                        site.setPolygon(clipPoly.convexClip(poly));
                    }

                }
            }
        }

    }
}


ArrayList<JFace> voronoi::PowerDiagram::getFacesOfDestVertex(HEdge edge)
{
    ArrayList<JFace> faces = new ArrayList<JFace>();
    HEdge previous = edge;
    JVertex first = edge.getDest();

    Site site = (Site) first.originalObject;
    ArrayList<Site> neighbours = new ArrayList<Site>();
    do
    {
        previous = previous.getTwin().getPrev();
        // add neighbour to the neighbourlist
        Site siteOrigin = (Site) previous.getOrigin().originalObject;
        if (!siteOrigin.isDummy) {
            neighbours.add(siteOrigin);
        }
        JFace iFace = previous.getiFace();
        if (iFace.isVisibleFromBelow()) {
            faces.add(iFace);
        }
    } while (previous != edge);
    site.setNeighbours(neighbours);
    return faces;
}

