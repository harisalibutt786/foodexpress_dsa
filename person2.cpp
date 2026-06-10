// ================================================================
//  person2.cpp  —  PERSON 2
//  Module 3: Rider Dispatch & Assignment  (Dynamic Array)
//  Module 4: Routing & Infrastructure     (Adjacency List + Dijkstra)
// ================================================================

#include "person2.h"
#include "person3.h"   // for HistoryTracker full definition

// ================================================================
//  CityMap — Adjacency List + Dijkstra
// ================================================================

CityMap::CityMap() {
    capacity = 10;
    count = 0;
    locations = new LocationNode[capacity];
}

CityMap::~CityMap() {
    for (int i = 0; i < count; i++) {
        EdgeNode* cur = locations[i].head;
        while (cur) { EdgeNode* t = cur; cur = cur->next; delete t; }
    }
    delete[] locations;
}

void CityMap::resize() {
    capacity *= 2;
    LocationNode* temp = new LocationNode[capacity];
    for (int i = 0; i < count; i++) {
        temp[i].name = locations[i].name;
        temp[i].head = locations[i].head;
    }
    delete[] locations;
    locations = temp;
}

int CityMap::getCount() { return count; }
string CityMap::getName(int i) { return locations[i].name; }

int CityMap::getIndex(string name) {
    for (int i = 0; i < count; i++)
        if (locations[i].name == name) return i;
    return -1;
}

void CityMap::addLocation(string name) {
    if (getIndex(name) == -1) {
        if (count == capacity) resize();
        locations[count++].name = name;
    }
}

void CityMap::addRoute(string src, string dest, int distance) {
    int s = getIndex(src), d = getIndex(dest);
    if (s == -1 || d == -1) return;
    EdgeNode* n1 = new EdgeNode(d, distance);
    n1->next = locations[s].head; locations[s].head = n1;
    EdgeNode* n2 = new EdgeNode(s, distance);
    n2->next = locations[d].head; locations[d].head = n2;
}

void CityMap::blockRoad(string src, string dest) {
    int s = getIndex(src), d = getIndex(dest);
    if (s == -1 || d == -1) { cout << "  [ERROR] Location not found.\n"; return; }
    for (EdgeNode* e = locations[s].head; e; e = e->next)
        if (e->destination == d) { e->isBlocked = true; break; }
    for (EdgeNode* e = locations[d].head; e; e = e->next)
        if (e->destination == s) { e->isBlocked = true; break; }
    cout << "  [MAP ALERT] Road blocked between " << src << " and " << dest << ". Rerouting enabled.\n";
}

void CityMap::unblockRoad(string src, string dest) {
    int s = getIndex(src), d = getIndex(dest);
    if (s == -1 || d == -1) return;
    for (EdgeNode* e = locations[s].head; e; e = e->next)
        if (e->destination == d) { e->isBlocked = false; break; }
    for (EdgeNode* e = locations[d].head; e; e = e->next)
        if (e->destination == s) { e->isBlocked = false; break; }
    cout << "  [MAP] Road between " << src << " and " << dest << " is now open.\n";
}

int CityMap::getShortestDistance(int src, int dest) {
    if (src == -1 || dest == -1 || src == dest) return (src == dest) ? 0 : INF;
    int* dist = new int[count];
    bool* visited = new bool[count];
    for (int i = 0; i < count; i++) { dist[i] = INF; visited[i] = false; }
    dist[src] = 0;
    for (int iter = 0; iter < count; iter++) {
        int u = -1;
        for (int j = 0; j < count; j++)
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) u = j;
        if (u == -1 || dist[u] == INF) break;
        visited[u] = true;
        for (EdgeNode* e = locations[u].head; e; e = e->next) {
            if (!e->isBlocked && dist[u] + e->distance < dist[e->destination])
                dist[e->destination] = dist[u] + e->distance;
        }
    }
    int result = dist[dest];
    delete[] dist; delete[] visited;
    return result;
}

void CityMap::optimizeRoute(string start, string end) {
    int d = getShortestDistance(getIndex(start), getIndex(end));
    if (d == INF)
        cout << "  [ROUTING] No valid route found. All paths blocked.\n";
    else {
        cout << "  [ROUTING] Optimal route: " << start << " -> " << end << " | Distance: " << d << " mins\n";
        cout << "  [BILLING] Estimated Delivery Fee: Rs. " << (50 + d * 5) << "\n";
    }
}

void CityMap::compareRoutes(string a1, string a2, string dest) {
    int d1 = getShortestDistance(getIndex(a1), getIndex(dest));
    int d2 = getShortestDistance(getIndex(a2), getIndex(dest));
    cout << "  [COMPARE] " << a1 << " -> " << dest << ": " << (d1 == INF ? -1 : d1) << " mins\n";
    cout << "  [COMPARE] " << a2 << " -> " << dest << ": " << (d2 == INF ? -1 : d2) << " mins\n";
    if (d1 <= d2) cout << "  [RESULT]  Recommended start: " << a1 << "\n";
    else cout << "  [RESULT]  Recommended start: " << a2 << "\n";
}

void CityMap::displayMap() {
    cout << "  --- City Map Adjacency List ---\n";
    for (int i = 0; i < count; i++) {
        cout << "  " << locations[i].name << " -> ";
        for (EdgeNode* e = locations[i].head; e; e = e->next)
            cout << locations[e->destination].name << "(" << e->distance << (e->isBlocked ? ",BLOCKED" : "") << ") ";
        cout << "\n";
    }
}

// ================================================================
//  RiderManager — Dynamic Array Implementation
// ================================================================

RiderManager::RiderManager() {
    capacity = 5;
    count = 0;
    riders = new Rider[capacity];
}

RiderManager::~RiderManager() { delete[] riders; }

void RiderManager::resize() {
    capacity *= 2;
    Rider* temp = new Rider[capacity];
    for (int i = 0; i < count; i++) temp[i] = riders[i];
    delete[] riders;
    riders = temp;
}

void RiderManager::addRider(string name, int zone, int cap) {
    if (count == capacity) resize();
    riders[count++] = { name, true, zone, 0, cap, -1 };
}

void RiderManager::assignRider(int orderId, int restaurantZone, HistoryTracker& history, CityMap& city) {
    for (int i = 0; i < count; i++) {
        if (riders[i].assignedOrderId == orderId) {
            cout << "  [DISPATCH] Order #" << orderId << " is already assigned to Rider " << riders[i].name << ". Cannot assign again.\n";
            return;
        }
    }
    int bestRider = -1, minScore = INF;
    for (int i = 0; i < count; i++) {
        if (!riders[i].available) continue;
        if (riders[i].currentLoad >= riders[i].capacity) continue;
        int dist = city.getShortestDistance(riders[i].currentZone, restaurantZone);
        int score = dist + (riders[i].currentLoad * 10);
        if (score < minScore) { minScore = score; bestRider = i; }
    }
    if (bestRider != -1) {
        riders[bestRider].currentLoad++;
        riders[bestRider].assignedOrderId = orderId;
        if (riders[bestRider].currentLoad >= riders[bestRider].capacity)
            riders[bestRider].available = false;
        cout << "  [DISPATCH] Rider " << riders[bestRider].name << " assigned to Order #" << orderId << " | Score: " << minScore << " | Load: " << riders[bestRider].currentLoad << "/" << riders[bestRider].capacity << "\n";
        history.logAction("Rider " + riders[bestRider].name + " assigned to Order #" + to_string(orderId), "", to_string(orderId));
    } else {
        cout << "  [DISPATCH] System Overload: No riders available. Order queued.\n";
    }
}

void RiderManager::releaseRider(string name) {
    for (int i = 0; i < count; i++) {
        if (riders[i].name == name && riders[i].currentLoad > 0) {
            riders[i].currentLoad--;
            riders[i].assignedOrderId = -1;
            riders[i].available = true;
            cout << "  [RIDER] " << name << " delivery completed. Now available.\n";
            return;
        }
    }
    cout << "  [ERROR] Rider not found or has no active deliveries.\n";
}

void RiderManager::displayRiders() {
    cout << "  --- Rider Status ---\n";
    for (int i = 0; i < count; i++) {
        cout << "    " << riders[i].name << " | Zone: " << riders[i].currentZone << " | Load: " << riders[i].currentLoad << "/" << riders[i].capacity << " | Status: " << (riders[i].available ? "Available" : "Full") << "\n";
    }
}

int RiderManager::getCount() { return count; }
