//
//  router.cpp
//  
//
//  Created by Tiffany Orian on 3/14/24.
//


#include "router.h"
Router::Router(GeoDatabaseBase& geo_db) : geo_db(geo_db) { }
vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const 
{
        stack<vector<GeoPoint>> paths;
        map<string, bool> visited;
        vector<vector<GeoPoint>> found_paths;
        // Start the path with the first point
        paths.push({ pt1 });
        visited[pt1.to_string()] = true;

        while (!paths.empty()) {
            vector<GeoPoint> currentPath = paths.top();
            paths.pop();

            GeoPoint currentPoint = currentPath.back();

            // Check if the current point is the destination
            if (currentPoint.latitude == pt2.latitude && currentPoint.longitude == pt2.longitude) {
                // Found the path to destination
                found_paths.push_back(currentPath);
            }

            // Explore connected points
            vector<GeoPoint> connected = geo_db.get_connected_points(currentPoint);
            sort(connected.begin(), connected.end(), [&pt2](const GeoPoint& g1, const GeoPoint& g2) {
                return distance_earth_miles(g1, pt2) > distance_earth_miles(g2, pt2);
            });
            for (GeoPoint& gp : connected) {
                if (!visited[gp.to_string()]) {
                    visited[gp.to_string()] = true;  // Mark as visited before pushing onto the stack
                    // Create a new path with the connected point and push it on the stack
                    vector<GeoPoint> newPath(currentPath);
                    newPath.push_back(gp);
                    paths.push(newPath);
                }
            }
        }
        int min = 1000000;
        int min_index = -1;
        //cout << "Size: " << found_paths.size() << endl;

        for (int i = 0; i < found_paths.size(); i++) {
            //cout << "Path Size: " << found_paths[i].size() << endl;
            if (found_paths[i].size() < min) {
                min = found_paths[i].size();
                min_index = i;
            }
        }
        // If no path is found, return an empty vector
        if (min_index == -1) {
            return {};
        } else {
            return found_paths[min_index];
        }
    }
