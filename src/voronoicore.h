#ifndef VORONOICORE_H
#define VORONOICORE_H

namespace voronoi {
/**
 * Core class for generating Voronoi Treemaps. position and weight of sites is
 * changed on each iteration to get the wanted area for a cell.
 *
 * @author Arlind Nocaj
 */
class VoronoiCore
{
public:
    // TODO remove when finished
    static bool debugMode;// = false;
    static ImageFrame frame;
    static Graphics2D graphics;


    VoronoiCore();
    VoronoiCore(Rectangle2D rectangle);
    VoronoiCore(PolygonSimple clipPolygon);
    VoronoiCore(OpenList sites, PolygonSimple clipPolygon);
    OpenList getSiteList();
    static void setDebugMode();

    /**
     * The resulting Voronoi cells are clipped with this polygon
     *
     * @param polygon
     *            clipping polygon
     */
    void setClipPolygon(PolygonSimple polygon);
    /**
     * Returns clipping polygon
     *
     * @return
     */
    PolygonSimple getClipPolyogon();
    /**
     * Sets a rectangle as clipping polygon.
     *
     * @param rectangle
     */
    void setClipPolygon(Rectangle2D rectangle);
    /**
     * Adds a site to the Voronoi diagram.
     *
     * @param site
     */
    void addSite(Site site);
    void iterateSimple();
    bool checkBadResult(OpenList sites);
    /**
     * Computes the diagram and sets the results
     */
    //synchronized 
    void voroDiagram();
    void printCoreCode();
    static void printPolygonCode(PolygonSimple poly);
    void doIterate();
    /**
     * For debugging only
     *
     * @param isLast
     */
    void drawCurrentState(bool isLast);
    Color getFillColorScaled(Site s);
    //synchronized 
    void drawState(Graphics2D g, bool isLast);
    void setSites(OpenList sites);
    OpenList getSites();
    static void main(String[] args);
    static void normalizeSites(OpenList sites);
    void setLevel(int height);
    void setSettings(VoroSettings coreSettings);
protected:
    PolygonSimple clipPolygon;
    OpenList sites;
    PowerDiagram diagram;
    double currentAreaError = 1.0;
    /**
     * Computes the ordinary diagram and sets the results
     */
    //synchronized 
    void voroOrdinaryDiagram(OpenList sites);
private:
    // private SVGGraphics2D svgGraphics;
    static Random rand = new Random(99);
    /** If this mode is true, svg files are created in each iteration **/
    bool outPutMode;
    /** Counter for creating the output files. **/
    int outCounter = 1;

    /**
     * core variables
     */
    bool firstIteration = true;
    static final double nearlyOne = 0.999;

    /**
     * Settings for the Core
     */
    VoroSettings settings = new VoroSettings();

    int currentIteration;
    // level in the hierarchy, level=0 is the first layer
    int level;
    Point2D center;
    double scale;
    AffineTransform transform;
    double currentErrorMax;

    void init();
    void randomizePoints(OpenList sites);
    void fixNoPolygonSites();
    void checkPointsInPolygon(OpenList sites);
    double computeAreaError(OpenList sites);
    double computeMaxError(OpenList sites2);
    void moveSites(OpenList sites);
    void adjustWeightsToBePositive(OpenList sites);
    void adaptWeightsSimple(OpenList sites);
    void fixWeightsIfDominated(OpenList sites);
    void fixWeightsIfDominated2(OpenList sites);
    /**
     * Computes the minimal distance to the voronoi Diagram neighbours
     */
    double getMinNeighbourDistance(Site point);
    double getAvgNeighbourDistance(Site point);
    double getAvgWeight(OpenList sites);
    double getGlobalAvgNeighbourDistance(OpenList sites);
    double getMinNeighbourDistanceOld(Site point);
    /**
     * Scaling and Shifting allows for higher geometry precision
     */
    void shiftAndScaleZeroCenter();
    void transformBackFromZero();
};
}
#endif // VORONOICORE_H
