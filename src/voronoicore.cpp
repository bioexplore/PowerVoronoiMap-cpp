#include <cmath>
#include <cfloat>
#include <limits>
#include <iostream>
#include "voronoicore.h"

voronoi::VoronoiCore::VoronoiCore():
    outputMode_(true),outCounter_(1),firstIteration_(true),
    settings_(NULL),currentIteration_(0),rectanglePoly_(false),createSites_(true),
    level_(0),center_(),scale_(1.0),currentErrorMax_(0.0),
    clipPolygon_(NULL),sites_(NULL),diagram_(NULL),currentAreaError_(1.0)
{
    settings_=new VoroSettings();
    diagram_=new PowerDiagram();
    sites_=new std::vector<Site*>();
}

voronoi::VoronoiCore::VoronoiCore(Rectangle2D& rectangle):
    outputMode_(true),outCounter_(1),firstIteration_(true),
    settings_(NULL),currentIteration_(0),rectanglePoly_(false),createSites_(true),
    level_(0),center_(),scale_(1.0),currentErrorMax_(0.0),
    clipPolygon_(NULL),sites_(NULL),diagram_(NULL),currentAreaError_(1.0)
{
    settings_=new VoroSettings();
    diagram_=new PowerDiagram();
    sites_=new std::vector<Site*>();
    setClipPolygon(rectangle);
}

voronoi::VoronoiCore::VoronoiCore(voronoi::PolygonSimple* clipPolygon):
    outputMode_(true),outCounter_(1),firstIteration_(true),
    settings_(NULL),currentIteration_(0),rectanglePoly_(false),createSites_(true),
    level_(0),center_(),scale_(1.0),currentErrorMax_(0.0),
    clipPolygon_(clipPolygon),sites_(NULL),diagram_(NULL),currentAreaError_(1.0)
{
    settings_=new VoroSettings();
    diagram_=new PowerDiagram();
    sites_=new std::vector<Site*>();
    diagram_->setClipPoly(clipPolygon);
}

voronoi::VoronoiCore::VoronoiCore(std::vector<voronoi::Site*>* sites, voronoi::PolygonSimple* clipPolygon):
    outputMode_(true),outCounter_(1),firstIteration_(true),
    settings_(NULL),currentIteration_(0),rectanglePoly_(false),createSites_(false),
    level_(0),center_(),scale_(1.0),currentErrorMax_(0.0),
    clipPolygon_(clipPolygon),sites_(sites),diagram_(NULL),currentAreaError_(1.0)
{
    settings_=new VoroSettings();
    diagram_=new PowerDiagram();
    diagram_->setClipPoly(clipPolygon);
}

voronoi::VoronoiCore::~VoronoiCore()
{
    if(settings_) delete settings_;
    if(diagram_) delete diagram_;
    if(rectanglePoly_ && clipPolygon_) delete clipPolygon_;
    if(createSites_ && sites_) delete sites_;
}

std::vector<voronoi::Site*>* voronoi::VoronoiCore::getSiteList()
{
    return sites_;
}

void voronoi::VoronoiCore::setClipPolygon(voronoi::PolygonSimple* polygon)
{
    clipPolygon_ = polygon;
    if (diagram_ != NULL)
        diagram_->setClipPoly(polygon);
}

voronoi::PolygonSimple* voronoi::VoronoiCore::getClipPolyogon()
{
    return clipPolygon_;
}

void voronoi::VoronoiCore::setClipPolygon(voronoi::Rectangle2D& rectangle)
{
    PolygonSimple* poly = new PolygonSimple();
    poly->add(rectangle.getX(), rectangle.getY());
    poly->add(rectangle.getX(), rectangle.getY());
    poly->add(rectangle.getX(), rectangle.getY());
    poly->add(rectangle.getX(), rectangle.getY());
    setClipPolygon(poly);
    rectanglePoly_=true;
}

void voronoi::VoronoiCore::addSite(voronoi::Site* site)
{
    sites_->push_back(site);
}

void voronoi::VoronoiCore::iterateSimple()
{
    moveSites(sites_);
    checkPointsInPolygon(sites_);

    // adapt weights
    adaptWeightsSimple(sites_);
    voroDiagram();

    currentAreaError_ = computeAreaError(sites_);
    currentErrorMax_ = computeMaxError(sites_);
    currentIteration_++;
}

bool voronoi::VoronoiCore::checkBadResult(std::vector<voronoi::Site*>* sites)
{
    for (Site* a: *sites)
    {
        if (a->getPolygon() == NULL)
            return true;
    }
    return false;
}

void voronoi::VoronoiCore::voroDiagram()
{
    diagram_->setSites(sites_);
    diagram_->setClipPoly(clipPolygon_);
    diagram_->computeDiagram();
}

//TODO:deal with later
void voronoi::VoronoiCore::printCoreCode()
{
    /*
    printPolygonCode(clipPolygon);

    System.out.println("std::vector<Site*> list=new std::vector<Site*>(" + sites_->size + ");");
    for (int i = 0; i < sites_->size; i++) {
        Site s = sites_->array[i];
        String line = "list.add(new Site(" + s.x + "," + s.y + ","
                + s.getWeight() + "));";
        System.out.println(line);
    }
    */
}

//TODO:deal with later
void voronoi::VoronoiCore::printPolygonCode(voronoi::PolygonSimple* poly)
{
    /*
    double[] x = poly.getXPoints();
    double[] y = poly.getYPoints();
    System.out.println("PolygonSimple poly=new PolygonSimple();");

    for (int i = 0; i < poly.getNumPoints(); i++) {
        String line = "poly.add(" + x[i] + "," + y[i] + ");";
        System.out.println(line);
    }
    */
}

//TODO:deal with later
void voronoi::VoronoiCore::voroOrdinaryDiagram(std::vector<voronoi::Site*>* sites)
{
    /*
    diagram.setSites(sites);
    diagram.setClipPoly(clipPolygon);
    try {
        diagram.computeDiagram();
    } catch (Exception e) {
        System.out.println("Error on computing power diagram");
        e.printStackTrace();
    }
    */
}

void voronoi::VoronoiCore::doIterate()
{
    if (sites_->size() <= 1)
    {
        sites_->at(0)->setPolygon(clipPolygon_);//FIXME:use clipPolygon_->clone()
        return;
    }

    shiftAndScaleZeroCenter();

    currentIteration_ = 0;
    currentAreaError_ = 1.0;

    checkPointsInPolygon(sites_);
    if (firstIteration_)
    {
        firstIteration_=false;
        voroDiagram();
    }

    bool badResult = true;
    while (true)
    {
        iterateSimple();
        badResult = checkBadResult(sites_);

        if (!badResult)
        {
            if (settings_->cancelAreaError
                    && currentAreaError_ < settings_->errorThreshold
                    && (!settings_->cancelOnLocalError || currentErrorMax_ < settings_->errorThreshold))
                break;

            if (settings_->cancelMaxIteration
                    && currentIteration_ > settings_->maxIteration)
                break;
        }
    }
    transformBackFromZero();
    //transform = NULL;
    std::cout<<"Iteration: "<<currentIteration_<<"\t AreaError: "<<currentAreaError_<<std::endl;
    std::cout<<"Iteration: "<<currentIteration_<<"\t MaxError: "<<currentErrorMax_<<std::endl;

    // now its finished so give the cells a hint
    for (Site* site : *sites_)
    {
        PolygonSimple* poly = site->getPolygon();
        if (site->getCellObject() != NULL)
        {
            site->getCellObject()->setVoroPolygon(poly);
            site->getCellObject()->doFinalWork();
        }
    }
}

/*
void voronoi::VoronoiCore::drawCurrentState(bool isLast) {
    if (graphics != NULL) {
        drawState(graphics, false);
        frame.repaint();
    }
}


Color voronoi::VoronoiCore::getFillColorScaled(Site s) {
    double increase = s.getLastIncrease();

    double completeArea = clipPolygon.getArea();
    double wantedArea = completeArea * s.getPercentage();
    double value = wantedArea / completeArea;
    InterpolColor interpolPos = new InterpolColor(1, 2, 0.6, 0.0, 0.8, 0.6,
                                                  1.0, 0.8);

    InterpolColor interpolNeg = new InterpolColor(0.5, 1.0, 0.0, 0.9, 0.8,
                                                  0.0, 0.0, 0.8);

    int alpha = (int) value * 256 + 50;
    if (increase < 1.0) {
        Math.max(increase, 0.5);
        return interpolNeg.getColorLinear(increase, alpha);

    }
    if (increase > 1.0) {
        increase = Math.min(increase, 2.0);
        return interpolPos.getColorLinear(increase, alpha);
    }
    return Color.white;
}

void voronoi::VoronoiCore::drawState(Graphics2D g, bool isLast) {
    try {
        if (transform != NULL)
            g.setTransform(transform.createInverse());
    } catch (NoninvertibleTransformException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
    }

    for (Site s : sites) {
        g.setColor(Colors.circleFill);
        s.paint(g);
        PolygonSimple poly = s.getPolygon();
        if (poly != NULL) {
            g.setColor(getFillColorScaled(s));
            g.fill(poly);
            g.setColor(Color.red);
            g.draw(poly);
        }

        g.drawString("AreaError: " + computeAreaError(sites), 30, 80);
        g.drawString("Iteration: " + currentIteration, 30, 110);
    }

}
*/

void voronoi::VoronoiCore::setSites(std::vector<voronoi::Site*>* sites)
{
    if(createSites_ && sites_) delete sites_;
    createSites_=false;
    sites_ = sites;
}

std::vector<voronoi::Site*>* voronoi::VoronoiCore::getSites()
{
    return sites_;
}

void voronoi::VoronoiCore::normalizeSites(std::vector<voronoi::Site*>* sites)
{
    double sum = 0;
    int size = sites->size();
    for (int z = 0; z < size; z++)
    {
        Site* s = sites->at(z);
        sum += s->getPercentage();
    }
    for (int z = 0; z < size; z++) {
        Site* s = sites->at(z);
        s->setPercentage(s->getPercentage() / sum);
    }

}

void voronoi::VoronoiCore::setLevel(int height) {
    level_ = height;
}

void voronoi::VoronoiCore::setSettings(voronoi::VoroSettings* coreSettings) {
    if(settings_) delete settings_;
    settings_ = coreSettings;
}

void voronoi::VoronoiCore::randomizePoints(std::vector<voronoi::Site*>* sites)
{
    for (int i = 0; i < sites->size(); i++)
    {
        Site* point = sites->at(i);
        if (!clipPolygon_->contains(point->x, point->y))
        {
            Point2D p = clipPolygon_->getInnerPoint();
            point->setXY(p.x, p.y);
            continue;
        }
    }
}

void voronoi::VoronoiCore::fixNoPolygonSites()
{
    for (Site* a : *sites_)
    {
        if (a->getPolygon() == NULL)
        {
            fixWeightsIfDominated(sites_);
            voroDiagram();
            break;
        }
    }
}

void voronoi::VoronoiCore::checkPointsInPolygon(std::vector<voronoi::Site*>* sites)
{
    bool outside = false;
    for (int i = 0; i < sites->size(); i++)
    {
        Site* point = sites->at(i);
        if (!clipPolygon_->contains(point->x, point->y))
        {
            outside = true;
            Point2D p = clipPolygon_->getInnerPoint();
            point->setXY(p.x, p.y);
        }
    }
    if (outside)
        fixWeightsIfDominated(sites);
}

double voronoi::VoronoiCore::computeAreaError(std::vector<voronoi::Site*>* sites)
{
    double completeArea = clipPolygon_->getArea();
    double errorArea = 0;
    for (int z = 0; z < sites->size(); z++)
    {
        Site* point = sites->at(z);
        PolygonSimple* poly = point->getPolygon();
        double currentArea = (poly == NULL) ? 0.0 : poly->getArea();
        double wantedArea = completeArea * (point->getPercentage());
        errorArea += std::abs(wantedArea - currentArea)
                / (completeArea * 2.0);
    }
    return errorArea;
}

double voronoi::VoronoiCore::computeMaxError(std::vector<voronoi::Site*>* sites2)
{
    double completeArea = clipPolygon_->getArea();
    double maxError = 0;
    for (int z = 0; z < sites2->size(); z++)
    {
        Site* point = sites2->at(z);
        PolygonSimple* poly = point->getPolygon();
        double currentArea = (poly == NULL) ? 0.0 : poly->getArea();
        double wantedArea = completeArea * (point->getPercentage());
        double error = std::abs(wantedArea - currentArea) / (wantedArea);
        maxError = std::max(error, maxError);
    }
    return maxError;
}

void voronoi::VoronoiCore::moveSites(std::vector<voronoi::Site*>* sites)
{
    for (Site* point : *sites)
    {
        PolygonSimple* poly = point->getPolygon();
        if (poly != NULL)
        {
            Point2D centroid = *(poly->getCentroid());
            double centroidX = centroid.getX();
            double centroidY = centroid.getY();
            if (clipPolygon_->contains(centroidX, centroidY))
                point->setXY(centroidX, centroidY);
        }
    }
}

void voronoi::VoronoiCore::adjustWeightsToBePositive(std::vector<voronoi::Site*>* sites)
{
    double minWeight = 0;
    for (int z = 0; z < sites->size(); z++)
    {
        Site* s = sites->at(z);
        if (s->getWeight() < minWeight)
            minWeight = s->getWeight();
    }

    for (int z = 0; z < sites->size(); z++)
    {
        Site* s = sites->at(z);
        double w = s->getWeight();
        if (std::isnan(w))
            w = 0.0001;
        w -= minWeight;
        if (w < 0.0001)
            w = 0.0001;
        s->setWeight(w);
    }
}

void voronoi::VoronoiCore::adaptWeightsSimple(std::vector<voronoi::Site*>* sites)
{
    int size = sites->size();
    double averageDistance = getGlobalAvgNeighbourDistance(sites);
    double error = computeAreaError(sites);
    for (int z = 0; z < size; z++)
    {
        Site* point = sites->at(z);
        PolygonSimple* poly = point->getPolygon();
        double completeArea = clipPolygon_->getArea();
        double currentArea = (poly == NULL) ? 0.0 : poly->getArea();
        double wantedArea = completeArea * point->getPercentage();
        double increase = 1.0;
        if(currentArea==0.0)
            increase = 2.0;
        else
            increase= wantedArea / currentArea;

        double weight = point->getWeight();

        double step = 0;
        double errorTransform = (-(error - 1) * (error - 1) + 1);

        step = 1.0 * averageDistance * errorTransform;
        double epsilon = 0.01;
        if (increase < (1.0 - epsilon))
            weight -= step;
        else if (increase > (1.0 + epsilon))
            weight += step;
        point->setWeight(weight);
        // debug purpose
        point->setLastIncrease(increase);
    }
}

void voronoi::VoronoiCore::fixWeightsIfDominated(std::vector<voronoi::Site*>* sites)
{
    for (Site* s : *sites)
    {
        double weight = s->getWeight();
        if (std::isnan(weight))
        {
            s->setWeight(0.00000000001);
        }
    }

    for (Site* s : *sites)
    {
        for (Site* q : *sites)
        {
            if (s != q)//WATCHME: only compare whether they point to the same point
            {
                double distance = s->distance(q) * nearlyOne_;
                if (std::sqrt(s->getWeight()) >= distance)
                {
                    double weight = distance * distance;
                    q->setWeight(weight);
                }
            }
        }
    }
}

//TODO: deal with later
void voronoi::VoronoiCore::fixWeightsIfDominated2(std::vector<voronoi::Site*>* sites)
{
    /*
    // get all nearest neighbors
    std::vector<Site*>* copy = sites_->cloneWithZeroWeights();
    for (const auto& s : *sites)
        if (std::isnan(s->getWeight()))
            System.out.println(s);
    PowerDiagram diagram = new PowerDiagram(sites,
                                            sites_->getBoundsPolygon(20));
    diagram.computeDiagram();

    // set pointer to original site
    for (int z = 0; z < sites_->size; z++)
        copy.array[z].setData(sites_->array[z]);

    for (int z = 0; z < copy.size; z++)
    {
        Site pointCopy = copy.array[z];
        Site point = sites_->array[z];
        if (pointCopy.getNeighbours() != NULL)
            for (Site neighbor : pointCopy.getNeighbours())
            {
                Site original = (Site) neighbor.getData();
                if (original.getPolygon() == NULL)
                {
                    double dist = pointCopy.distance(neighbor) * nearlyOne;
                    if (Math.sqrt(pointCopy.getWeight()) > dist)
                    {
                        double weight = dist * dist;
                        point.setWeight(weight);
                    }
                }
            }
    }
    */
}

double voronoi::VoronoiCore::getMinNeighbourDistance(voronoi::Site* point)
{
    double minDistance = std::numeric_limits<double>::max();
    std::vector<Site*>* neighbours=point->getNeighbours();
    for (Site* neighbour : *neighbours)
    {
        double distance = neighbour->distance(point);
        if (distance < minDistance)
        {
            minDistance = distance;
        }
    }
    return minDistance;
}

double voronoi::VoronoiCore::getAvgNeighbourDistance(voronoi::Site* point)
{
    double avg = 0;
    std::vector<Site*>* neighbours=point->getNeighbours();
    for (Site* neighbour : *neighbours)
    {
        double distance = neighbour->distance(point);
        avg += distance;
    }
    avg /= neighbours->size();
    return avg;
}

double voronoi::VoronoiCore::getAvgWeight(std::vector<voronoi::Site*>* sites)
{
    double avg = 0;
    int num = sites->size();
    for (Site* point : *sites)
        avg += point->getWeight();
    avg /= num;
    return avg;
}

double voronoi::VoronoiCore::getGlobalAvgNeighbourDistance(std::vector<voronoi::Site*>* sites)
{
    double avg = 0;
    int num = 0;
    std::vector<Site*>* neighbours=NULL;
    for (Site* point : *sites)
    {
        neighbours=point->getNeighbours();
        if (neighbours != NULL)
            for (Site* neighbour :*neighbours )
            {
                double distance = neighbour->distance(point);
                avg += distance;
                num++;
            }
    }
    avg /= num;
    return avg;
}

double voronoi::VoronoiCore::getMinNeighbourDistanceOld(voronoi::Site* point)
{
    double minDistance = std::numeric_limits<double>::max();
    std::vector<Site*>* neighbours = point->getNeighbours();
    for (Site* neighbour : *neighbours)
    {
        double distance = neighbour->distance(point);
        if (distance < minDistance)
        {
            minDistance = distance;
        }
    }
    return minDistance;
}

void voronoi::VoronoiCore::shiftAndScaleZeroCenter()
{
    PolygonSimple *poly = clipPolygon_;
    center_ = *(poly->getCentroid());
    Rectangle2D bounds = *(poly->getBounds2D());
    double width = std::max(bounds.getWidth(), bounds.getHeight());
    double goalWidth = 500.0;

    scale_ = goalWidth / width;

    //transform = new AffineTransform();
    //transform.scale(scale, scale);
    //transform.translate(-center.x, -center.y);

    poly->translate(-center_.x, -center_.y);
    poly->scale(scale_);

    PolygonSimple* copy = poly->getOriginalPolygon();
    if (copy != NULL)
    {
        copy->translate(-center_.x, -center_.y);
        copy->scale(scale_);
    }

    setClipPolygon(poly);

    for (Site* s : *sites_)
    {
        double a = s->getX();
        double b = s->getY();

        a -= center_.x;
        b -= center_.y;

        a *= scale_;
        b *= scale_;
        s->setX(a);
        s->setY(b);
    }
}

void voronoi::VoronoiCore::transformBackFromZero()
{
    clipPolygon_->scale(1 / scale_);
    clipPolygon_->translate(center_.x, center_.y);

    for (Site* s : *sites_)
    {
        double a = s->getX();
        double b = s->getY();
        a /= scale_;
        b /= scale_;

        a += center_.x;
        b += center_.y;

        s->setX(a);
        s->setY(b);

        PolygonSimple* poly = s->getPolygon();
        poly->scale(1 / scale_);
        poly->translate(center_.x, center_.y);
        s->setPolygon(poly);

        PolygonSimple* copy = poly->getOriginalPolygon();
        if (copy != NULL)
        {
            copy->scale(1 / scale_);
            copy->translate(center_.x, center_.y);
        }

        s->setWeight(s->getWeight() / (scale_ * scale_));
    }

    scale_ = 1.0;
    center_.x = 0;
    center_.y = 0;
}
