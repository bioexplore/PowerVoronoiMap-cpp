#ifndef POWERDIAGRAM_H
#define POWERDIAGRAM_H
#include <vector>
#include "rectangle2d.h"
#include "site.h"
#include "polygonsimple.h"
#include "convexhull.h"
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
    constexpr static double numericError_ = 1E-10;
    int             amountPolygons_;
    Rectangle2D     bb_;

    // set of sites which forms a rectangle that is big enough to bound a
    // diagram with creating a bisector in the clipping polygon
    Site*           s1_;
    Site*           s2_;
    Site*           s3_;
    Site*           s4_;

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
    std::vector<Face*>* getFacesOfDestVertex(HullEdge* edge);
protected:
    ConvexHull*             hull_;
    std::vector<Site*>*     sites_;
    PolygonSimple*          clipPoly_;
    std::vector<Face*>*     facets_;

public:
    //static Random rand = new Random(99);
    const static int halfLineScalingFactor = 10000;
    PowerDiagram();
    PowerDiagram(std::vector<Site*>* sites, PolygonSimple* clipPoly);
    ~PowerDiagram();
    void 		    setSites(std::vector<Site*>* sites);
    void 		    setClipPoly(PolygonSimple* polygon);
    PolygonSimple* 	getClipPoly();
    void 		    computeDiagram();
    void 		    writeHullTestCodeOut(Site* s);
    void 		    setAmountPolygons(int amountPolygons);
    int 		    getAmountPolygons();
    static void 	initDebug();
    void 		    showDiagram();
    void 		    draw(Site s);
};
}

#endif // POWERDIAGRAM_H
