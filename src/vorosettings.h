#ifndef VOROSETTINGS_H
#define VOROSETTINGS_H
namespace voronoi
{
class VoroSettings
{
public:
    bool    cancelAreaError;
    double  errorThreshold;
    bool    cancelMaxIteration;
    int     maxIteration;
    bool    cancelOnLocalError;
    double  boostConvergence;
    VoroSettings():cancelAreaError(true),errorThreshold(0.03),
                   cancelMaxIteration(true),maxIteration(800),
                   cancelOnLocalError(true),boostConvergence(1.0)
    {}
};
}
#endif
