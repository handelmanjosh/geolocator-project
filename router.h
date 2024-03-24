#ifndef ROUTER_H
#define ROUTER_H

#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <stack>
#include <queue>
#include "base_classes.h"
#include "geodb.h"
#include "geotools.h"

using namespace std;
class Router: public RouterBase {
    private:
    GeoDatabaseBase& geo_db;
    public:
    Router(GeoDatabaseBase& geo_db);
    vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;
        
};

#endif
