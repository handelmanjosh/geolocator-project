#include <cstdio>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "geodb.h"
#include "router.h"
#include "stops.h"
#include "tourcmd.h"
#include "tour_generator.h"

using namespace std;

void print_tour(vector<TourCommand>& tcs)
{
    double total_dist = 0;
    string direction;
    double street_distance = 0;

    cout << "Starting tour...\n";

    for (size_t i = 0; i < tcs.size(); ++i)
    {
        if (tcs[i].get_command_type() == TourCommand::commentary)
        {
            cout << "Welcome to " << tcs[i].get_poi() << "!\n";
            cout << tcs[i].get_commentary() << "\n";
        }
        else if (tcs[i].get_command_type() == TourCommand::turn)
        {
            cout << "Take a " << tcs[i].get_direction() << " turn on " << tcs[i].get_street() << endl;
        }
        else if (tcs[i].get_command_type() == TourCommand::proceed)
        {
            total_dist += tcs[i].get_distance();
            if (direction.empty())
                direction = tcs[i].get_direction();
            street_distance += tcs[i].get_distance();
            if (i+1 < tcs.size() && tcs[i+1].get_command_type() == TourCommand::proceed 
                && tcs[i+1].get_street() == tcs[i].get_street() && tcs[i].get_street() != "a path")
            {
                continue;
            }

            cout << "Proceed " << std::fixed << std::setprecision(3) << street_distance << " miles " << direction << " on " << tcs[i].get_street() << endl;
            street_distance = 0;
            direction.clear();
        }
    }

    cout << "Your tour has finished!\n";
    cout << "Total tour distance: " << std::fixed << std::setprecision(3) << total_dist << " miles\n";
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "usage: BruinTour mapdata.txt stops.txt\n";
        return 1;
    }

    GeoDatabase geodb;
    if (!geodb.load(argv[1]))
    {
        cout << "Unable to load map data: " << argv[1] << endl;
        return 1;
    }
    cout << "Loaded" << endl;
    Router router(geodb);
    TourGenerator tg(geodb, router);

    Stops stops;
    if (!stops.load(argv[2]))
    {
        cout << "Unable to load tour data: " << argv[2] << endl;
        return 1;
    }
    // string start_poi, start_points, end_poi, end_points;
    // GeoPoint start, end;
    // stops.get_poi_data(0, start_poi, start_points);
    // stops.get_poi_data(stops.size() - 2, end_poi, end_points);
    // for (int i = 0; i < stops.size(); i++) {
    //     string poi, description;
    //     stops.get_poi_data(i, poi, description);
    //     GeoPoint gp;
    //     geodb.get_poi_location(poi, gp);
    //     cout << i << " " << poi << " " << description << " " << gp.to_string() << endl;
    // }
    // geodb.get_poi_location(start_poi, start);
    // geodb.get_poi_location(end_poi, end);
    // cout << "Start " << start_poi << " " << start.to_string() << " End " << end_poi << " " << end.to_string() << endl;
    // cout << "Routing...\n\n";
    // vector<GeoPoint> result = router.route(start, end);
    // cout << "Found path? " << (result.size() > 0) << " " << result.size() << endl;
    // cout << result[0].to_string() << " " << result[result.size() - 1].to_string() << endl;
    // for (int i = 0; i < result.size(); i++) {
    //     cout << result[i].to_string() << endl;
    // }
    vector<TourCommand> tcs = tg.generate_tour(stops);
    if (tcs.empty())
        cout << "Unable to generate tour!\n";
    else
        print_tour(tcs);
}
