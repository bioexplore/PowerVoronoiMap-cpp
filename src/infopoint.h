#ifndef INFOPOINT_H
#define INFOPOINT_H

namespace voronoi {
/**
 * Result wrapper for intersection with information (= code) of intersection type
 * @author Hildenbrand, Nocaj
 *
 */
class Point2D;
class InfoPoint
{
public:
    InfoPoint(Point2D* erg, char code);
    InfoPoint(Point2D* erg, Point2D* snd, char code);
    const char* toString();
private:
    Point2D* erg_;
    Point2D* snd_;
    char code_;
};
}

#endif // INFOPOINT_H
