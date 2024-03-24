#ifndef TOUR_GENERATOR_H
#define TOUR_GENERATOR_H

#include "base_classes.h"
#include <vector>
#include <utility>
#include "stops.h"
#include "router.h"
#include "geodb.h"

using namespace std;

class TourGenerator : public TourGeneratorBase 
{
    private:
    const GeoDatabaseBase& geodb;
    const RouterBase& router;
    string get_move_direction(const GeoPoint& gp1, const GeoPoint& gp2) const;
    string get_turn_direction(const GeoPoint& gp1, const GeoPoint& gp2, const GeoPoint& gp3) const;
    public:
    TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router) : geodb(geodb), router(router) {}
    vector<TourCommand> generate_tour(const Stops& stops) const;
};

#endif
