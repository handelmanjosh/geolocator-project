#ifndef GEO_DB_H
#define GEO_DB_H

#include <string>
#include <vector>
#include <iostream>
#include "geopoint.h"
#include <sstream>
#include "base_classes.h"
#include "HashMap.h"
#include "geotools.h"
#include <set>
#include <algorithm>

using namespace std;
class GeoDatabase: public GeoDatabaseBase 
{
    private:
    typedef struct Data 
    {
        string name;
        GeoPoint gp;
        set<string> connections;
    } 
    Data;
    HashMap<GeoPoint> pois;
    HashMap<Data> geopoints;
    HashMap<string> streets;
    inline void add_street(GeoPoint gp1, GeoPoint gp2, string name) {
        streets.insert(gp1.to_string() + gp2.to_string(), name);
        streets.insert(gp2.to_string() + gp1.to_string(), name);
    }   
    inline GeoPoint* string_to_geopoint(string s) const {
        // turns string in format "latitude longitude" to geopoint
        size_t delimiterPos = s.find(',');
        string latitude = s.substr(0, delimiterPos);
        string longitude = s.substr(delimiterPos + 1);
        GeoPoint* gp = new GeoPoint(latitude, longitude);
        return gp;
    }
    inline void resolve_connections(vector<Data>& connections, Data& data) {
        for (int i = 0; i < connections.size(); i++) {
            if (connections[i].gp.latitude == data.gp.latitude && connections[i].gp.longitude == data.gp.longitude) {
                connect(connections[i], data);
            }
        }
    }
    inline void connect(Data& d1, Data& d2) {
        d1.connections.insert(d2.gp.to_string());
        d2.connections.insert(d1.gp.to_string());
    }
    inline void checked_insert(Data& d) {
        Data* dp = geopoints.find(d.gp.to_string());
        if (dp != nullptr) {
            dp->connections.insert(d.connections.begin(), d.connections.end());
        } else {
            geopoints.insert(d.gp.to_string(), d);
        }
    }
    public:
    bool load(const string& map_data_file);
    bool get_poi_location(const std::string& poi, GeoPoint& point) const;
    vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
    string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;
};

#endif
