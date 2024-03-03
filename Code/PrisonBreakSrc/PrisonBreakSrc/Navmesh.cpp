#include "Navmesh.h"
#include <cmath>
#include <queue>

Navmesh::Navmesh() {
    nodes = std::vector<Node*>();
}

Navmesh::~Navmesh() {

    for (Node* n : nodes) {
        delete(n);
    }
    nodes.clear();

    //delete(adj);
}

int Navmesh::weight(Node* a, Node* b) {
	return (int) sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}

void Navmesh::addNode(int x, int y) {
	nodes.push_back(new Node(x, y));
    nodes.back()->id = nodes.size() - 1;
}

Node* Navmesh::closestNode(int x, int y) {
    
    Node* tempNode = new Node(x, y);
    Node* retNode = nullptr;
    int retDistance = 10000;
    
    for (Node* currNode : nodes) {
        int currDistance = weight(tempNode, currNode);
        if (retDistance > currDistance) {
            retDistance = currDistance;
            retNode = currNode;
        }
    }

    return retNode;
}

Node* Navmesh::randomNode() {
    int r = rand() % nodes.size();
    return nodes.at(r);
}

void Navmesh::generateGraph(int radius) {

    if(nodes.size() > 0)
    adj = new std::list<std::pair<int, int>>[nodes.size()];

	for (int i = 0; i < nodes.size(); i++) {
		for (int j = i + 1; j < nodes.size(); j++) {
			int w = weight(nodes[i], nodes[j]);
			if (w <= radius) {
				adj[i].push_back(std::make_pair(j, w));
				adj[j].push_back(std::make_pair(i, w));
			}
		}
	}
}

std::vector<Node*> Navmesh::shortestPath(int src, int dest) {
	
    //use dijkstra's to set up a list of parent nodes for each node in the shortest path to the src
    std::vector<int> parents = std::vector<int>(nodes.size());
    dijkstra(src, &parents);

    //contruct a vector of nodes from the src to the dest using the parents vector
    std::vector<Node*> path = std::vector<Node*>();
    _shortestPath(dest, parents, &path);

    return path;
}

void Navmesh::_shortestPath(int currentVertex, std::vector<int> parents, std::vector<Node*>* path) {
    if (currentVertex == -1) {
        return;
    }

    _shortestPath(parents[currentVertex], parents, path);
    path->push_back(nodes.at(currentVertex));
}

//Algorithm adapted from https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
void Navmesh::dijkstra(int src, std::vector<int>* parents) {
    
    // Initialize new parents vector
    parents->clear();
    // Create a priority queue to store vertices that
    // are being preprocessed. This is weird syntax in C++.
    // Refer below link for details of this syntax
    // https://www.geeksforgeeks.org/implement-min-heap-using-stl/
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

    // Create a vector for distances and initialize all
    // distances as infinite (INF)
    std::vector<int> dist(nodes.size(), 10000);

    // Insert source itself in priority queue and initialize
    // its distance as 0.
    pq.push(std::make_pair(0, src));
    dist[src] = 0;

    for (int i = 0; i < nodes.size(); i++) {
        parents->push_back(-1);
    }

    /* Looping till priority queue becomes empty (or all
    distances are not finalized) */
    while (!pq.empty()) {
        // The first vertex in pair is the minimum distance
        // vertex, extract it from priority queue.
        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = pq.top().second;
        pq.pop();

        // 'i' is used to get all adjacent vertices of a
        // vertex
        std::list<std::pair<int, int>>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i) {
            // Get vertex label and weight of current
            // adjacent of u.
            int v = (*i).first;
            int weight = (*i).second;

            // If there is shorted path to v through u.
            if (dist[v] > dist[u] + weight) {
                // Updating distance of v
                dist[v] = dist[u] + weight;
                pq.push(std::make_pair(dist[v], v));

                //u is parent of v
                parents->at(v) = u;
            }
        }
    }
}


