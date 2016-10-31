#ifndef VORONOICORE_H
#define VORONOICORE_H
#include <vector>
#include "rectangle2d.h"
#include "polygonsimple.h"
#include "vorosettings.h"
#include "powerdiagram.h"

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
    VoronoiCore();
    VoronoiCore(Rectangle2D& rectangle);
    VoronoiCore(PolygonSimple* clipPolygon);
    VoronoiCore(std::vector<Site*>* sites, PolygonSimple* clipPolygon);
    ~VoronoiCore();

    std::vector<Site*>* getSiteList();

    /**
     * The resulting Voronoi cells are clipped with this polygon
     *
     * @param polygon
     *            clipping polygon
     */
    void setClipPolygon(PolygonSimple* polygon);
    /**
     * Returns clipping polygon
     *
     * @return
     */
    PolygonSimple* getClipPolyogon();
    /**
     * Sets a rectangle as clipping polygon.
     *
     * @param rectangle
     */
    void setClipPolygon(Rectangle2D& rectangle);
    /**
     * Adds a site to the Voronoi diagram.
     *
     * @param site
     */
    void addSite(Site* site);
    void iterateSimple();
    bool checkBadResult(std::vector<Site*>* sites);
    /**
     * Computes the diagram and sets the results
     */
    //synchronized 
    void voroDiagram();
    void printCoreCode();
    static void printPolygonCode(PolygonSimple* poly);
    void doIterate();
    /**
     * For debugging only
     *
     * @param isLast
     */
    //void drawCurrentState(bool isLast);
    //Color getFillColorScaled(Site s);
    //synchronized 
    //void drawState(Graphics2D g, bool isLast);
    void setSites(std::vector<Site*>* sites);
    std::vector<Site*>* getSites();
    static void normalizeSites(std::vector<Site*>* sites);
    void setLevel(int height);
    void setSettings(VoroSettings* coreSettings);
protected:
    PolygonSimple* clipPolygon_;
    std::vector<Site*>* sites_;
    PowerDiagram* diagram_;
    double currentAreaError_;// = 1.0;
    /**
     * Computes the ordinary diagram and sets the results
     */
    //synchronized 
    void voroOrdinaryDiagram(std::vector<Site*>* sites);
private:
    constexpr static double nearlyOne_ = 0.999;
    // private SVGGraphics2D svgGraphics;
    //static Random rand = new Random(99);
    /** If this mode is true, svg files are created in each iteration **/
    bool outputMode_;
    /** Counter for creating the output files. **/
    int outCounter_;// = 1;

    /** If true, means this obj's clipPoly is construced by Rectangle2D
     * and memories of clipPolygon_ should be retrieved when destruction
     */
    bool rectanglePoly_;

    /**
     * core variables
     */
    bool firstIteration_;// = true;

    /**
     * Settings for the Core
     */
    VoroSettings* settings_;// = new VoroSettings();

    int currentIteration_;
    // level in the hierarchy, level=0 is the first layer
    int level_;
    Point2D center_;
    double scale_;
    //AffineTransform transform;
    double currentErrorMax_;

    void init();
    void randomizePoints(std::vector<Site*>* sites);
    void fixNoPolygonSites();
    void checkPointsInPolygon(std::vector<Site*>* sites);
    double computeAreaError(std::vector<Site*>* sites);
    double computeMaxError(std::vector<Site*>* sites2);
    void moveSites(std::vector<Site*>* sites);
    void adjustWeightsToBePositive(std::vector<Site*>* sites);
    void adaptWeightsSimple(std::vector<Site*>* sites);
    void fixWeightsIfDominated(std::vector<Site*>* sites);
    void fixWeightsIfDominated2(std::vector<Site*>* sites);
    /**
     * Computes the minimal distance to the voronoi Diagram neighbours
     */
    double getMinNeighbourDistance(Site* point);
    double getAvgNeighbourDistance(Site* point);
    double getAvgWeight(std::vector<Site*>* sites);
    double getGlobalAvgNeighbourDistance(std::vector<Site*>* sites);
    double getMinNeighbourDistanceOld(Site* point);
    /**
     * Scaling and Shifting allows for higher geometry precision
     */
    void shiftAndScaleZeroCenter();
    void transformBackFromZero();
};
}
#endif // VORONOICORE_H
