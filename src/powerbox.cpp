#include "powerbox.h"
using namespace voronoi;

PowerBox::PowerBox()
{
    super();
    clipPoly.add(50,50);
    clipPoly.add(50,800);
    clipPoly.add(800,800);
    clipPoly.add(800,50);
    this.powerBox=this;
    getContentPane().setLayout(new BorderLayout());
    getContentPane().setBackground(Color.BLACK);
    getContentPane().add(panel);
    panel.setVisible(true);
    panel.setLayout(null);
    panel.addMouseListener(new MouseAdapter() {

        @Override
        public void mousePressed(MouseEvent e) {
            // TODO Auto-generated method stub
            super.mousePressed(e);

        }
        @Override
        synchronized public void mouseClicked(MouseEvent e) {

            Point p = e.getPoint();
            double weight=30;
            Site site = new Site(p.getX(), p.getY(),weight);
            sites.add(site);
            JSite jsite=new JSite(site);

            panel.add(jsite);
            jsite.setVisible(true);
            points.add(jsite);
            if (points.size()>1){
//					computeDiagram();
//					vertices = diagram.getVertices();
                //lines = diagram.getLines();
            }
            PowerBox.powerBox.computeDiagram();
            setPreferredSize(getSize());
            repaint();
        }
    });
    setVisible(true);
    setSize(500, 400);
}

void PowerBox::computeDiagram()
{
    PowerDiagram diagram = new PowerDiagram(sites, clipPoly);
    diagram.computeDiagram();
    for(JSite component:points){
        component.setPolygon(component.getSite().getPolygon());
    }
    setPreferredSize(getSize());
    pack();
    repaint();
}

/**
 * @param args
 */
static void PowerBox::main(String[] args)
{
    new PowerBox();
}

// @Override
void PowerBox::paint(Graphics g2)
{
    // TODO Auto-generated method stub
    super.paint(g2);

    Graphics2D g=(Graphics2D)g2;
    g2.translate(2,22);
    g.setColor(Color.red);
    Site[] array = sites.array;
    int size=sites.size;
    g2.setColor(Color.green);
    for (int z=0;z<size;z++){
    Site s = array[z];
    double posX=s.getX();
    double posY=s.getY();
        double radius = Math.sqrt(s.getWeight());
        g.drawOval((int)posX-(int)radius, (int)posY-(int)radius, (int)(2*radius), (int)(2*radius));
        int r2=5;
        Color normal = g.getColor();
        if(s.getPolygon()==null){
            g.setColor(Color.red);
        }
        g.drawRect((int)posX-r2, (int)posY-r2, 2*r2, 2*r2);
        g.setColor(normal);
    }
    int i=0;
    for (int z=0;z<size;z++){
        g.setColor(Color.GRAY.brighter());
    Site s = array[z];
            if (i!=-2){
            PolygonSimple poly = s.getPolygon();
            if (poly!=null){
            g.draw(poly);
            }
            s.paint(g);
            }
        i++;

    }

    for (int z=0;z<size;z++){
        Site s = array[z];
        double posX=s.getX();
        double posY=s.getY();
            double radius = Math.sqrt(s.getWeight());

            g.drawOval((int)posX-(int)radius, (int)posY-(int)radius, (int)(2*radius), (int)(2*radius));
            int r2=7;
            g.drawRect((int)posX-r2, (int)posY-r2, 2*r2, 2*r2);
            PolygonSimple poly = s.getPolygon();
            if (poly!=null){
            Point2D center = poly.getCentroid();
            r2=5;
            g.setColor(Color.blue);
            g.drawRect((int)center.getX()-r2, (int)center.getY()-r2, 2*r2, 2*r2);
            }
        }
    this.validate();
}
