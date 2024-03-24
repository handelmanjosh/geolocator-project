//
//  tour_generator.cpp
//  
//
//  Created by Tiffany Orian on 3/14/24.
//

#include "tour_generator.h"


string TourGenerator::get_move_direction(const GeoPoint& gp1, const GeoPoint& gp2) const
{
    double angle = angle_of_line(gp1, gp2);
    if (angle < 22.5) {
        return "east";
    } else if (angle < 67.5) {
        return "northeast";
    } else if (angle < 112.5) {
        return "north";
    } else if (angle < 157.5) {
        return "northwest";
    } else if (angle < 202.5) {
        return "west";
    } else if (angle < 247.5) {
        return "southwest";
    } else if (angle < 292.5) {
        return "south";
    } else if (angle < 337.5) {
        return "southeast";
    } else {
        return "east";
    }
}


string TourGenerator::get_turn_direction(const GeoPoint& gp1, const GeoPoint& gp2, const GeoPoint& gp3) const
{
    double angle = angle_of_turn(gp1, gp2, gp3);
    if (angle > 1 && angle < 180) {
        return "left";
    } else if (angle >= 180 && angle <= 359) {
        return "right";
    } else {
        return "";
    }
}


vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const 
{
    vector<vector<GeoPoint>> paths;
    vector<pair<string, string>> data;
    for (int i = 0; i < stops.size(); i++) { // we assume stops will be greater than 3
        string start_poi, start_talking_points;
        stops.get_poi_data(i, start_poi, start_talking_points);
        data.push_back(make_pair(start_poi, start_talking_points));
        string dest_poi, dest_talking_points;
        if (i != stops.size() - 1) {
            stops.get_poi_data(i + 1, dest_poi, dest_talking_points);
        } else {
            stops.get_poi_data(0, dest_poi, dest_talking_points);
        }
        GeoPoint start_gp, end_gp;
        geodb.get_poi_location(start_poi, start_gp);
        geodb.get_poi_location(dest_poi, end_gp);
        vector<GeoPoint> route = router.route(start_gp, end_gp);
        paths.push_back(route);
    }
    cout << data.size() << endl;
    vector<TourCommand> result;
    for (int i = 0; i < paths.size(); i++) {
        TourCommand location;
        location.init_commentary(data[i].first, data[i].second);
        result.push_back(location);
        for (int j = 0; j < paths[i].size() - 1; j++) {
            string street_name = geodb.get_street_name(paths[i][j], paths[i][j+1]);
            TourCommand proceed;
            string direction = get_move_direction(paths[i][j], paths[i][j+1]);
            double distance = distance_earth_miles(paths[i][j], paths[i][j+1]);
            proceed.init_proceed(direction, street_name, distance, paths[i][j], paths[i][j+1]);
            result.push_back(proceed);
            if (j < paths[i].size() - 2) {
                string street_2 = geodb.get_street_name(paths[i][j+1], paths[i][j+2]);
                if (street_2 != street_name) {
                    TourCommand turn;
                    string turn_direction = get_turn_direction(paths[i][j], paths[i][j+1], paths[i][j+2]);
                    if (turn_direction != "") {
                        turn.init_turn(turn_direction, street_2);
                        result.push_back(turn);
                    }
                }
            }
        }
    }
    return result;
}


