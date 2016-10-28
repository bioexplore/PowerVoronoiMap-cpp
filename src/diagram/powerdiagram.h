#ifndef POWERDIAGRAM_H
#define POWERDIAGRAM_H

namespace voronoi {
/**
 * Computes the PowerDiagram by using the convex hull of the transformed half
 * planes of the sites.
 *
 * @author Arlind Nocaj
 *
 */
class PowerDiagram
{
    const static double numericError = 1E-10;
    int amountPolygons;
    Rectangle2D bb;

    // set of sites which forms a rectangle that is big enough to bound a
    // diagram with creating a bisector in the clipping polygon
    Site s1;
    Site s2;
    Site s3;
    Site s4;

    /**
     * For each site the corresponding polygon and the corresponding neighbours
     * are computed and stored in the site.
     */
    void computeData();
    /**
     * Return the faces which are visible from below
     *
     * @param edge
     * @return
     */
    ArrayList<JFace> getFacesOfDestVertex(HEdge edge);
protected:
    JConvexHull hull = null;
    OpenList sites;
    PolygonSimple clipPoly;
    List<JFace> facets = null;

public:
    static Random rand = new Random(99);
    static final int halfLineScalingFactor = 10000;

    static boolean debug = true;
    static ImageFrame frame;
    static Graphics2D graphics;

    PowerDiagram() ;

    PowerDiagram(OpenList sites, PolygonSimple clipPoly);
    /*
     * (non-Javadoc)
     *
     * @see diagram.iPowerDiagram#setSites(datastructure.OpenList)
     */
    void setSites(OpenList sites);
    void setClipPoly(PolygonSimple polygon);
    PolygonSimple getClipPoly();
    /*
     * (non-Javadoc)
     *
     * @see diagram.iPowerDiagram#computeDiagram()
     */
    void computeDiagram();
    void writeHullTestCodeOut(Site s);
    void setAmountPolygons(int amountPolygons);
    int getAmountPolygons();
    static void initDebug();
    static void main(String[] args) {

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
        for (int i = 0; i < 100; i++) {
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
        for (int i=0;i<sites.size;i++){
            Site site=sites.array[i];
            PolygonSimple polygon=site.getPolygon();
        }
    }

    void showDiagram();
    void draw(Site s);
};
}

#endif // POWERDIAGRAM_H
