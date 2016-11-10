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
    InfoPoint(Point2D* erg, char code,bool needClearMem=false);
    InfoPoint(Point2D* erg, Point2D* snd, char code,bool needCelarMem=false);
    ~InfoPoint();
    const char* toString();
    char code();
    bool containNewlyCreatedPoint();
    Point2D erg();
    Point2D snd();

private:
    Point2D* erg_;
    Point2D* snd_;
    char code_;
    bool needClearMem_;//If true need delete erg_ and snd_; default is false
};
}

#endif // INFOPOINT_H
