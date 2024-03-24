//
//  geodb.cpp
//  
//
//  Created by Tiffany Orian on 3/14/24.
//

#include "geodb.h"


bool GeoDatabase::load(const string& map_data_file)
{
        ifstream file(map_data_file);
        string name, coords, poi_string, poi_data;
        double start_lat, start_long, end_lat, end_long, poi;
        if (file.is_open()) {
            for (;;) {
                if (!getline(file, name) || !getline(file, coords) || !getline(file, poi_string)) break;
                istringstream coords_iss(coords);
                coords_iss >> start_lat >> start_long >> end_lat >> end_long;
                istringstream poi_string_iss(poi_string);
                poi_string_iss >> poi;
                // add count to name
                GeoPoint s_geopoint = GeoPoint(to_string(start_lat), to_string(start_long));
                Data start = { name, s_geopoint, set<string>() };
                GeoPoint e_geopoint = GeoPoint(to_string(end_lat), to_string(end_long));
                Data end = { name, e_geopoint, set<string>() };
                if (poi > 0) {
                    GeoPoint m_geopoint = midpoint(s_geopoint, e_geopoint);
                    Data midpoint_data = { name, m_geopoint, set<string>() };
                    connect(start, midpoint_data);
                    connect(end, midpoint_data);
                    for (int i = 0; i < poi; i++) {
                        getline(file, poi_data);
                        size_t delimiterPos = poi_data.find('|');
                        string poi_name = poi_data.substr(0, delimiterPos);
                        string coordinates = poi_data.substr(delimiterPos + 1);
                        double latitude, longitude;
                        istringstream poi_coord_iss(coordinates);
                        poi_coord_iss >> latitude >> longitude;
                        GeoPoint poi_geopoint = GeoPoint(to_string(latitude), to_string(longitude));
                        Data poi_data = { poi_name, poi_geopoint, set<string>() };
                        connect(poi_data, midpoint_data);
                        pois.insert(poi_name, poi_geopoint);
                        checked_insert(poi_data);
                        add_street(midpoint_data.gp, poi_data.gp, "a path");
                    }
                    checked_insert(midpoint_data);
                    add_street(start.gp, midpoint_data.gp, name);
                    add_street(midpoint_data.gp, end.gp, name);
                }
                connect(start, end);
                checked_insert(start);
                checked_insert(end);
                add_street(start.gp, end.gp, name);
            }
            file.close();
            return true;
        } else {
            cout << "Unable to open file";
            return false;
        }
}


bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const
{
    /*
        This method takes in the name of a point of interest, such as Ackerman Union, and then
        returns the GeoPoint location of that PoI. It returns true if the PoI was found, and sets its
        second parameter to the GeoPoint of the PoI. Otherwise, it must return false and make no
        changes to the second GeoPoint parameter. This function must run in O(1) (i.e., constant)
        time. Here's an example of its usage
    */
    GeoPoint* gp = pois.find(poi);
    if (gp != nullptr) {
        point = *gp;
        return true;
    } else {
        return false;
    }
}

vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const
{
    /*
        This method takes in a GeoPoint and returns a vector of all GeoPoints that are directly
        connected to that point via street segment(s). If the function finds no points connected to the
        searched-for point, then the function returns an empty vector. This function must run in O(p)
        time where p is the number of GeoPoints that are directly connected to the searched-for point.
    */
    Data* d = geopoints.find(pt.to_string());
    if (d != nullptr) {
        vector<GeoPoint> result;
        for (string s : d->connections) {
            result.push_back(*string_to_geopoint(s));
        }
        return result;
    } else {
        vector<GeoPoint> v;
        return v;
    }
}

string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const 
{
    /*
        A call to get_street_name() returns the name of the street segment connected by the two
        GeoPoints. If the two GeoPoints are not directly connected, then the function must return the
        empty string. This function must run in O(1) time
    */
    string combo = pt1.to_string() + pt2.to_string();
    string* name = streets.find(combo);
    return name == nullptr ? "" : *name;
}
