#include "application.h"

#include <iostream>
#include <limits>
#include <map>
#include <queue> // priority_queue
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "dist.h"
#include "graph.h"

using namespace std;

double INF = numeric_limits<double>::max();

#include "json.hpp"
using json = nlohmann::json;

void buildGraph(istream &input, graph<long long, double> &g,
                vector<BuildingInfo> &buildings,
                unordered_map<long long, Coordinates> &coords) {
  json j;
  input >> j;

  unordered_set<long long> waypointIDs;

  for (auto &wp : j["waypoints"]) {
    long long id = wp["id"];
    Coordinates c(wp["lat"], wp["lon"]);

    coords[id] = c;
    g.addVertex(id);
    waypointIDs.insert(id);
  }

  for (auto &b : j["buildings"]) {
    long long id = b["id"];
    Coordinates c(b["lat"], b["lon"]);

    buildings.push_back(BuildingInfo(id, c, b["name"], b["abbr"]));
    g.addVertex(id);
  }

  for (auto &fw : j["footways"]) {
    for (size_t i = 0; i + 1 < fw.size(); i++) {
      long long id1 = fw[i];
      long long id2 = fw[i + 1];

      double dist = distBetween2Points(coords[id1], coords[id2]);

      g.addEdge(id1, id2, dist);
      g.addEdge(id2, id1, dist);
    }
  }

const double threshold = 0.036;

for (const auto &b : buildings) {
  for (long long wpID : waypointIDs) {
    double dist = distBetween2Points(b.location, coords[wpID]);

    if (dist <= threshold) {
      g.addEdge(b.id, wpID, dist);
      g.addEdge(wpID, b.id, dist);
    }
  }
}
}


BuildingInfo getBuildingInfo(const vector<BuildingInfo> &buildings,
                             const string &query) {
  for (const BuildingInfo &building : buildings) {
    if (building.abbr == query) {
      return building;
    } else if (building.name.find(query) != string::npos) {
      return building;
    }
  }
  BuildingInfo fail;
  fail.id = -1;
  return fail;
}

BuildingInfo getClosestBuilding(const vector<BuildingInfo> &buildings,
                                Coordinates c) {
  double minDestDist = INF;
  BuildingInfo ret = buildings.at(0);
  for (const BuildingInfo &building : buildings) {
    double dist = distBetween2Points(building.location, c);
    if (dist < minDestDist) {
      minDestDist = dist;
      ret = building;
    }
  }
  return ret;
}

class prioritize {
public:
  bool operator()(const pair<long long, double>& p1,
                  const pair<long long, double>& p2) const {
    return p1.second > p2.second;
  }
};

vector<long long> dijkstra(const graph<long long, double> &G, long long start,
                           long long target,
                           const set<long long> &ignoreNodes) {
  if (start == target) return {start};
  
  unordered_map<long long, double> distances;
  unordered_map<long long, long long> predecessors;
  for (long long v : G.getVertices()) {
    distances[v] = INF;
  }
  
  priority_queue<pair<long long, double>, 
                 vector<pair<long long, double>>, 
                 prioritize> worklist;

  distances[start] = 0;
  worklist.push({start, 0.0});

  while (!worklist.empty()) {
    long long currV = worklist.top().first;
    double currDist = worklist.top().second;
    worklist.pop();

    if (currDist > distances[currV]) continue;
    
    if (currV == target) break;

    for (long long neighbor : G.neighbors(currV)) {
      if (ignoreNodes.count(neighbor) && neighbor != target && neighbor != start) {
        continue;
      }

      double weight;
      G.getWeight(currV, neighbor, weight);
      double alternativeDist = distances[currV] + weight;

      if (alternativeDist < distances[neighbor]) {
        distances[neighbor] = alternativeDist;
        predecessors[neighbor] = currV;
        worklist.push({neighbor, alternativeDist});
      }
    }
  }

  vector<long long> path;
  if (distances[target] == INF) return path;

  for (long long curr = target; curr != start; curr = predecessors[curr]) {
    path.push_back(curr);
    if (predecessors.find(curr) == predecessors.end()) 
    return {}; 
  }
  path.push_back(start);
  
  reverse(path.begin(), path.end());
  return path;
}

double pathLength(const graph<long long, double> &G,
                  const vector<long long> &path) {
  double length = 0.0;
  double weight;
  for (size_t i = 0; i + 1 < path.size(); i++) {
    bool res = G.getWeight(path.at(i), path.at(i + 1), weight);
    if (!res) {
      return -1;
    }
    length += weight;
  }
  return length;
}

void outputPath(const vector<long long> &path) {
  for (size_t i = 0; i < path.size(); i++) {
    cout << path.at(i);
    if (i != path.size() - 1) {
      cout << "->";
    }
  }
  cout << endl;
}

// Honestly this function is just a holdover from an old version of the project
void application(const vector<BuildingInfo> &buildings,
                 const graph<long long, double> &G) {
  string person1Building, person2Building;

  set<long long> buildingNodes;
  for (const auto &building : buildings) {
    buildingNodes.insert(building.id);
  }

  cout << endl;
  cout << "Enter person 1's building (partial name or abbreviation), or #> ";
  getline(cin, person1Building);

  while (person1Building != "#") {
    cout << "Enter person 2's building (partial name or abbreviation)> ";
    getline(cin, person2Building);

    // Look up buildings by query
    BuildingInfo p1 = getBuildingInfo(buildings, person1Building);
    BuildingInfo p2 = getBuildingInfo(buildings, person2Building);
    Coordinates P1Coords, P2Coords;
    string P1Name, P2Name;

    if (p1.id == -1) {
      cout << "Person 1's building not found" << endl;
    } else if (p2.id == -1) {
      cout << "Person 2's building not found" << endl;
    } else {
      cout << endl;
      cout << "Person 1's point:" << endl;
      cout << " " << p1.name << endl;
      cout << " " << p1.id << endl;
      cout << " (" << p1.location.lat << ", " << p1.location.lon << ")" << endl;
      cout << "Person 2's point:" << endl;
      cout << " " << p2.name << endl;
      cout << " " << p2.id << endl;
      cout << " (" << p2.location.lon << ", " << p2.location.lon << ")" << endl;

      Coordinates centerCoords = centerBetween2Points(p1.location, p2.location);
      BuildingInfo dest = getClosestBuilding(buildings, centerCoords);

      cout << "Destination Building:" << endl;
      cout << " " << dest.name << endl;
      cout << " " << dest.id << endl;
      cout << " (" << dest.location.lat << ", " << dest.location.lon << ")"
           << endl;

      vector<long long> P1Path = dijkstra(G, p1.id, dest.id, buildingNodes);
      vector<long long> P2Path = dijkstra(G, p2.id, dest.id, buildingNodes);

      // This should NEVER happen with how the graph is built
      if (P1Path.empty() || P2Path.empty()) {
        cout << endl;
        cout << "At least one person was unable to reach the destination "
                "building. Is an edge missing?"
             << endl;
        cout << endl;
      } else {
        cout << endl;
        cout << "Person 1's distance to dest: " << pathLength(G, P1Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P1Path);
        cout << endl;
        cout << "Person 2's distance to dest: " << pathLength(G, P2Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P2Path);
      }
    }

    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);
  }
}
