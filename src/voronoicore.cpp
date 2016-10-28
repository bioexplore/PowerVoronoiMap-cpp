#include "voronoicore.h"

VoronoiCore::VoronoiCore()
{
    sites = new OpenList();
    init();
}

VoronoiCore::VoronoiCore(Rectangle2D rectangle)
{
    this();
    setClipPolygon(rectangle);
}

VoronoiCore::VoronoiCore(PolygonSimple clipPolygon)
{
    this();
    setClipPolygon(clipPolygon);
}

VoronoiCore::VoronoiCore(OpenList sites, PolygonSimple clipPolygon)
{
    this();
    this.sites = sites;
    setClipPolygon(clipPolygon);
}

OpenList VoronoiCore::getSiteList()
{
    return sites;
}

static void VoronoiCore::setDebugMode()
{
    debugMode = true;
    BufferedImage image = new BufferedImage(2000, 2000,
                                            BufferedImage.TYPE_INT_RGB);

    frame = new ImageFrame(image);
    frame.setVisible(true);
    frame.setBounds(20, 20, 1600, 800);
    graphics = image.createGraphics();
}

/**
     * The resulting Voronoi cells are clipped with this polygon
     *
     * @param polygon
     *            clipping polygon
     */
void VoronoiCore::setClipPolygon(PolygonSimple polygon)
{
    clipPolygon = polygon;
    if (diagram != null)
        diagram.setClipPoly(polygon);
}

/**
     * Returns clipping polygon
     *
     * @return
     */
PolygonSimple VoronoiCore::getClipPolyogon()
{
    return clipPolygon;
}

/**
     * Sets a rectangle as clipping polygon.
     *
     * @param rectangle
     */
void VoronoiCore::setClipPolygon(Rectangle2D rectangle)
{
    PolygonSimple poly = new PolygonSimple();
    poly.add(rectangle.getMinX(), rectangle.getMinY());
    poly.add(rectangle.getMaxX(), rectangle.getMinY());
    poly.add(rectangle.getMaxX(), rectangle.getMaxY());
    poly.add(rectangle.getMinX(), rectangle.getMaxY());
    setClipPolygon(poly);
}

/**
     * Adds a site to the Voronoi diagram.
     *
     * @param site
     */
void VoronoiCore::addSite(Site site)
{
    sites.add(site);
}

void VoronoiCore::iterateSimple()
{
    // if(currentIteration<=settings.maxIterat){
    moveSites(sites);
    checkPointsInPolygon(sites);
    // }

    // voroDiagram();//does not seem to be necessary
    // fixNoPolygonSites();

    // adapt weights
    adaptWeightsSimple(sites);
    voroDiagram();

    // fixNoPolygonSites();
    // fixWeightsIfDominated(sites);
    if (frame != null)
        frame.repaintWithWait(4);
    currentAreaError = computeAreaError(sites);
    currentErrorMax = computeMaxError(sites);
    currentIteration++;
}
bool VoronoiCore::checkBadResult(OpenList sites)
{
    for (Site a : sites) {
        if (a.getPolygon() == null)
            return true;
        // assert clipPolygon.contains(a.getPolygon().getCentroid());
    }
    return false;
}

/**
     * Computes the diagram and sets the results
     */
synchronized void VoronoiCore::voroDiagram()
{
    bool worked = false;
    while (!worked) {
        try {
            PowerDiagram diagram = new PowerDiagram();
            diagram.setSites(sites);
            diagram.setClipPoly(clipPolygon);
            diagram.computeDiagram();
            worked = true;
        } catch (Exception e) {

            System.out
                    .println("Error on computing power diagram, fixing by randomization");
            // e.printStackTrace();

            randomizePoints(sites);
            adjustWeightsToBePositive(sites);
            fixWeightsIfDominated(sites);
        }
    }
}

void VoronoiCore::printCoreCode()
{
    printPolygonCode(clipPolygon);

    System.out.println("OpenList list=new OpenList(" + sites.size + ");");
    for (int i = 0; i < sites.size; i++) {
        Site s = sites.array[i];
        String line = "list.add(new Site(" + s.x + "," + s.y + ","
                + s.getWeight() + "));";
        System.out.println(line);
    }
}

static void VoronoiCore::printPolygonCode(PolygonSimple poly)
{
    double[] x = poly.getXPoints();
    double[] y = poly.getYPoints();
    System.out.println("PolygonSimple poly=new PolygonSimple();");

    for (int i = 0; i < poly.getNumPoints(); i++) {
        String line = "poly.add(" + x[i] + "," + y[i] + ");";
        System.out.println(line);
    }
}

/**
     * Computes the ordinary diagram and sets the results
     */
void VoronoiCore::voroOrdinaryDiagram(OpenList sites)
{
    diagram.setSites(sites);
    diagram.setClipPoly(clipPolygon);
    try {
        diagram.computeDiagram();
    } catch (Exception e) {
        System.out.println("Error on computing power diagram");
        e.printStackTrace();
    }
}

void VoronoiCore::doIterate() {
    if (sites.size <= 1) {
        sites.array[0].setPolygon(clipPolygon.clone());
        return;
    }

    shiftAndScaleZeroCenter();

    if (frame != null)
        frame.setVoroCore(this);// debug mode

    // solveDuplicates(this.sites);
    currentIteration = 0;
    currentAreaError = 1.0;

    checkPointsInPolygon(sites);
    if (firstIteration){
        firstIteration=false;
        voroDiagram();
    }

    bool badResult = true;
    while (true) {
        iterateSimple();
        badResult = checkBadResult(sites);

        if (!badResult) {
            if (settings.cancelAreaError
                    && currentAreaError < settings.errorThreshold
                    && (!settings.cancelOnLocalError || currentErrorMax < settings.errorThreshold))
                break;

            if (settings.cancelMaxIterat
                    && currentIteration > settings.maxIterat)
                break;
        }

        // System.out.println("Iter: " + currentIteration
        // + "\t AreaError: \t" + lastAreaError);
        if (frame != null)
            frame.repaintWithWait(4);
    }

    transformBackFromZero();
    transform = null;


    System.out.println("Iteration: " + currentIteration
                       + "\t AreaError: \t" + currentAreaError);
    System.out.println("Iteration: " + currentIteration + "\t MaxError: \t"
                       + currentErrorMax);

    // now its finished so give the cells a hint
    for (Site site : sites) {
        PolygonSimple poly = site.getPolygon();
        if (site.cellObject != null) {
            site.cellObject.setVoroPolygon(poly);
            site.cellObject.doFinalWork();
        }
    }
}

/**
     * For debugging only
     *
     * @param isLast
     */
void VoronoiCore::drawCurrentState(bool isLast) {
    if (graphics != null) {
        drawState(graphics, false);
        frame.repaint();
        // frame.resize(frame.getSize());
        // frame.validate();
    }
    // if (outPutMode){
    // drawState(svgGraphics, false);
    // // svgGraphics.setBackground(Color.black);
    // // svgGraphics.clearRect(bb.x,bb.y,bb.width,bb.height);
    // // Finally, stream out SVG to the standard output using
    // // UTF-8 encoding.
    // bool useCSS = true; // we want to use CSS style attributes
    // String filename="treemapIter-"+outCounter++ +".svg";
    // try {
    //
    // Writer out = new BufferedWriter(new OutputStreamWriter(
    //
    // new FileOutputStream(filename), "UTF8"));
    // svgGraphics.stream(out, useCSS);
    // out.flush();
    // out.close();
    // }catch(Exception e){
    // e.printStackTrace();
    // }
    // createPDF(filename, svgGraphics);
    // }
}

Color VoronoiCore::getFillColorScaled(Site s) {
    double increase = s.getLastIncrease();

    // double area=s.getPolygon().getArea();
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

synchronized void VoronoiCore::drawState(Graphics2D g, bool isLast) {
    try {
        if (transform != null)
            g.setTransform(transform.createInverse());
    } catch (NoninvertibleTransformException e) {
        // TODO Auto-generated catch block
        e.printStackTrace();
    }

    // g.translate(center.x, center.y);
    // g.scale(1/scale,1/scale);

    // g.clearRect(-2000, -2000, 5000, 5000);
    // g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
    // RenderingHints.VALUE_ANTIALIAS_ON);

    // g.setColor(Colors.circleBorder);
    // g.draw(clipPolygon);

    // Site[] array = sites.array;
    // int size = sites.size;
    //
    // for (int z = 0; z < size; z++) {
    // Site s = array[z];
    // g.setColor(Colors.circleFill);
    // s.paint(g);
    // s.paintLastIncrease(g, 15);
    //
    // // // write the number of the site down
    // // g.setColor(Color.black);
    // // g.setFont(g.getFont().deriveFont(7F));
    // // g.drawString(new Integer(z + 1).toString(), (int) s.getX() + 1,
    // // (int) s.getY() - 1);
    //
    // }

    for (Site s : sites) {
        // if(isLast){
        g.setColor(Colors.circleFill);
        s.paint(g);
        // s.paintLastIncrease(g, 15);
        // }
        PolygonSimple poly = s.getPolygon();
        if (poly != null) {
            // poly.shrinkForBorder(0.1);
            g.setColor(getFillColorScaled(s));
            g.fill(poly);
            // poly.shrinkForBorder(0.9);
            g.setColor(Color.red);
            g.draw(poly);
            // Double centroid = poly.getCentroid();
            // g.drawRoundRect(centroid.x-, y, width, height, arcWidth,
            // arcHeight)
        }

        // // write the number of the site down
        // g.setColor(Color.black);
        // g.setFont(g.getFont().deriveFont(7F));
        // g.drawString(new Integer(z + 1).toString(), (int) s.getX() + 1,
        // (int) s.getY() - 1);

        g.drawString("AreaError: " + computeAreaError(sites), 30, 80);
        g.drawString("Iteration: " + currentIteration, 30, 110);
    }

}

void VoronoiCore::setSites(OpenList sites) {
    this.sites = sites;
}

OpenList VoronoiCore::getSites() {
    return sites;
}

static void VoronoiCore::main(String[] args) {

    VoronoiCore.setDebugMode();
    VoronoiCore core = new VoronoiCore();
    OpenList sites = new OpenList();
    Random rand=new Random(200);
    int amount=500;
    PolygonSimple rootPolygon = new PolygonSimple();
    int width=1000;
    int height=1000;

    for (int i=0;i<amount;i++){
        Site site = new Site(rand.nextDouble()*width, rand.nextDouble()*height);
        site.setPercentage(rand.nextFloat());
        sites.add(site);
    }
    sites.get(0).setPercentage(50);
    sites.get(1).setPercentage(50);
    // sites.get(6).setWeight(2000);
    // sites.get(6).setXY(270, 320);
    // System.out.println(sites.get(6));
    // width=600;
    // height=600;

    rootPolygon.add(0, 0);
    rootPolygon.add(width, 0);
    rootPolygon.add(width, height);
    rootPolygon.add(0, height);


    core.setDebugMode();
    core.normalizeSites(sites);

    core.setSites(sites);
    core.setClipPolygon(rootPolygon);
    core.doIterate();

    // }
}

static void VoronoiCore::normalizeSites(OpenList sites) {
    double sum = 0;
    Site[] array = sites.array;
    int size = sites.size;
    for (int z = 0; z < size; z++) {
        Site s = array[z];
        sum += s.getPercentage();
    }
    for (int z = 0; z < size; z++) {
        Site s = array[z];
        s.setPercentage(s.getPercentage() / sum);
    }

}

void VoronoiCore::setLevel(int height) {
    this.level = height;
}

void VoronoiCore::setSettings(VoroSettings coreSettings) {
    this.settings = coreSettings;
}

void VoronoiCore::init() {
       diagram = new PowerDiagram();
}
void VoronoiCore::randomizePoints(OpenList sites) {

       // double dist=getGlobalAvgNeighbourDistance(sites);
       for (int i = 0; i < sites.size; i++) {
               Site point = sites.array[i];
               if (!clipPolygon.contains(point.x, point.y)) {
                       Point2D p = clipPolygon.getInnerPoint();
                       point.setXY(p.x, p.y);
                       continue;
               }
               // double x=0;
               // double y=0;
               // do{
               // x=rand.nextDouble()-dist*1E-6;
               // y=rand.nextDouble()-dist*1E-6;
               // }while(!clipPolygon.contains(point.x+x,point.y+y));
               // point.setXY(x, y);

       }
}
void VoronoiCore::fixNoPolygonSites() {
       for (Site a : sites) {
               if (a.getPolygon() == null) {
                       fixWeightsIfDominated(sites);
                       voroDiagram();
                       break;
               }
       }
}



void VoronoiCore::checkPointsInPolygon(OpenList sites) {
       bool outside = false;
       for (int i = 0; i < sites.size; i++) {
               Site point = sites.array[i];
               if (!clipPolygon.contains(point.x, point.y)) {
                       outside = true;
                       Point2D p = clipPolygon.getInnerPoint();
                       point.setXY(p.x, p.y);
               }
       }
       if (outside)
               fixWeightsIfDominated(sites);
}

double VoronoiCore::computeAreaError(OpenList sites) {
       double completeArea = clipPolygon.getArea();
       double errorArea = 0;
       for (int z = 0; z < sites.size; z++) {
               Site point = sites.array[z];
               PolygonSimple poly = point.getPolygon();
               double currentArea = (poly == null) ? 0.0 : poly.getArea();
               double wantedArea = completeArea * point.getPercentage();
               errorArea += Math.abs(wantedArea - currentArea)
                               / (completeArea * 2.0);
       }
       return errorArea;
}

double VoronoiCore::computeMaxError(OpenList sites2) {
       double completeArea = clipPolygon.getArea();
       double maxError = 0;
       for (int z = 0; z < sites.size; z++) {
               Site point = sites.array[z];
               PolygonSimple poly = point.getPolygon();
               double currentArea = (poly == null) ? 0.0 : poly.getArea();
               double wantedArea = completeArea * point.getPercentage();
               double error = Math.abs(wantedArea - currentArea) / (wantedArea);
               maxError = Math.max(error, maxError);
       }
       return maxError;
}

void VoronoiCore::moveSites(OpenList sites) {
       for (Site point : sites) {
               PolygonSimple poly = point.getPolygon();
               if (poly != null) {
                       Point2D centroid = poly.getCentroid();
                       double centroidX = centroid.getX();
                       double centroidY = centroid.getY();
                       if (clipPolygon.contains(centroidX, centroidY))
                               point.setXY(centroidX, centroidY);
               }
       }
}

void VoronoiCore::adjustWeightsToBePositive(OpenList sites) {
       double minWeight = 0;
       for (int z = 0; z < sites.size; z++) {
               Site s = sites.array[z];
               if (s.getWeight() < minWeight)
                       minWeight = s.getWeight();
       }

       for (int z = 0; z < sites.size; z++) {
               Site s = sites.array[z];
               double w = s.getWeight();
               if (Double.isNaN(w))
                       w = 0.0001;
               w -= minWeight;
               if (w < 0.0001)
                       w = 0.0001;
               s.setWeight(w);
       }

}

void VoronoiCore::adaptWeightsSimple(OpenList sites) {
       Site[] array = sites.array;
       int size = sites.size;
       Random rand = new Random(5);
       double averageDistance = getGlobalAvgNeighbourDistance(sites);
       // double averageWeight=getAvgWeight(sites);
       // averageDistance+=averageWeight;
       double error = computeAreaError(sites);
       for (int z = 0; z < size; z++) {
               Site point = array[z];
               PolygonSimple poly = point.getPolygon();

               // if(poly==null)
               // System.out.println(point.getWeight()+"\t"+error);
               double completeArea = clipPolygon.getArea();
               double currentArea = (poly == null) ? 0.0 : poly.getArea();
               double wantedArea = completeArea * point.getPercentage();

               double increase = wantedArea / currentArea;
               if (currentArea == 0.0)
                       increase = 2.0;

               double weight = point.getWeight();

               double step = 0;
               double errorTransform = (-(error - 1) * (error - 1) + 1);

               // errorTransform=error;
               // errorTransform=Math.max(errorTransform, settings.errorThreshold);
               // if(currentIteration>settings.maxIterat)
               // errorTransform*=rand.nextDouble();

               step = 1.0 * averageDistance * errorTransform;
               // step=2*averageDistance*error;
               double epsilon = 0.01;
               if (increase < (1.0 - epsilon))
                       weight -= step;
               else if (increase > (1.0 + epsilon))
                       weight += step;
               point.setWeight(weight);

               // debug purpose
               point.setLastIncrease(increase);

       }
}

void VoronoiCore::fixWeightsIfDominated(OpenList sites) {

       for (Site s : sites) {
               double weight = s.getWeight();
               if (Double.isNaN(weight)) {
                       s.setWeight(0.00000000001);
               }
       }

       for (Site s : sites) {
               for (Site q : sites) {
                       if (s != q) {
                               double distance = s.distance(q) * nearlyOne;
                               if (Math.sqrt(s.getWeight()) >= distance) {
                                       double weight = distance * distance;
                                       q.setWeight(weight);
                               }
                       }
               }
       }
}

void VoronoiCore::fixWeightsIfDominated2(OpenList sites) {

       // get all nearest neighbors
       OpenList copy = sites.cloneWithZeroWeights();
       for (Site s : sites)
               if (Double.isNaN(s.getWeight()))
                       System.out.println(s);
       PowerDiagram diagram = new PowerDiagram(sites,
                       sites.getBoundsPolygon(20));
       diagram.computeDiagram();

       // set pointer to original site
       for (int z = 0; z < sites.size; z++)
               copy.array[z].setData(sites.array[z]);

       for (int z = 0; z < copy.size; z++) {
               Site pointCopy = copy.array[z];
               Site point = sites.array[z];
               if (pointCopy.getNeighbours() != null)
                       for (Site neighbor : pointCopy.getNeighbours()) {
                               Site original = (Site) neighbor.getData();
                               if (original.getPolygon() == null) {
                                       double dist = pointCopy.distance(neighbor) * nearlyOne;
                                       if (Math.sqrt(pointCopy.getWeight()) > dist) {
                                               double weight = dist * dist;
                                               point.setWeight(weight);
                                       }
                               }
                       }

       }

}

/**
* Computes the minimal distance to the voronoi Diagram neighbours
*/
double VoronoiCore::getMinNeighbourDistance(Site point)
{
       double minDistance = Double.MAX_VALUE;
       for (Site neighbour : point.getNeighbours()) {
               double distance = neighbour.distance(point);
               if (distance < minDistance) {
                       minDistance = distance;
               }
       }
       return minDistance;
}

double VoronoiCore::getAvgNeighbourDistance(Site point) {
       double avg = 0;
       for (Site neighbour : point.getNeighbours()) {
               double distance = neighbour.distance(point);
               avg += distance;
       }
       avg /= point.getNeighbours().size();
       return avg;
}

double VoronoiCore::getAvgWeight(OpenList sites) {
       double avg = 0;
       int num = sites.size;
       for (Site point : sites)
               avg += point.getWeight();
       avg /= num;
       return avg;
}

double VoronoiCore::getGlobalAvgNeighbourDistance(OpenList sites) {
       double avg = 0;
       int num = 0;
       for (Site point : sites)
               if (point.getNeighbours() != null)
                       for (Site neighbour : point.getNeighbours()) {
                               double distance = neighbour.distance(point);
                               avg += distance;
                               num++;
                       }
       avg /= num;
       return avg;
}

double VoronoiCore::getMinNeighbourDistanceOld(Site point) {
       double minDistance = Double.MAX_VALUE;

       for (Site neighbour : point.getOldNeighbors()) {
               double distance = neighbour.distance(point);
               if (distance < minDistance) {
                       minDistance = distance;
               }
       }
       return minDistance;
}
/**
* Scaling and Shifting allows for higher geometry precision
*/
void VoronoiCore::shiftAndScaleZeroCenter() {

  PolygonSimple poly = clipPolygon;
  this.center = poly.getCentroid();
  Rectangle2D bounds = poly.getBounds2D();
  double width = Math.max(bounds.getWidth(), bounds.getHeight());
  double goalWidth = 500.0;

  this.scale = goalWidth / width;

  this.transform = new AffineTransform();

  transform.scale(scale, scale);
  transform.translate(-center.x, -center.y);

  poly.translate(-center.x, -center.y);
  poly.scale(scale);

  PolygonSimple copy = poly.getOriginalPolygon();
  if (copy != null) {
          copy.translate(-center.x, -center.y);
          copy.scale(scale);
  }

  setClipPolygon(poly);

  for (Site s : sites) {
          double a = s.getX();
          double b = s.getY();

          a -= center.x;
          b -= center.y;

          a *= scale;
          b *= scale;
          s.setX(a);
          s.setY(b);
  }
}

void VoronoiCore::transformBackFromZero() {

  clipPolygon.scale(1 / scale);
  clipPolygon.translate(center.x, center.y);

  for (Site s : sites) {
          double a = s.getX();
          double b = s.getY();
          a /= scale;
          b /= scale;

          a += center.x;
          b += center.y;

          s.setX(a);
          s.setY(b);

          PolygonSimple poly = s.getPolygon();
          poly.scale(1 / scale);
          poly.translate(center.x, center.y);
          s.setPolygon(poly);

          PolygonSimple copy = poly.getOriginalPolygon();
          if (copy != null) {
                  copy.scale(1 / scale);
                  copy.translate(center.x, center.y);
          }

          s.setWeight(s.getWeight() / (scale * scale));
  }

  scale = 1.0;
  center.x = 0;
  center.y = 0;
}
