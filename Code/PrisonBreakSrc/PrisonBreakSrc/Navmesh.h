#pragma once
#include <vector>
#include <list>
#include <utility>      // std::pair, std::make_pair

class Node {
public:
    int x;
    int y;
    int id;
    Node(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class Navmesh {

private:
    
    //A list of nodes to draw edges between. Index of node in list is its ID
    std::vector<Node*> nodes;

    //For a weighted graph, an edge contains a vertex and a weight
    std::list<std::pair<int, int>>* adj = nullptr;

public:

    //Constructor. Each room needs its own separate navmesh upon being loaded in by roomManager  
    Navmesh();

    //Destructor
    ~Navmesh();
    
    //Weight (distance) between two nodes
    int weight(Node* a, Node* b);

    //add a node to the nodes vector
    void addNode(int x, int y);

    //find closest node to a pair of coordinates
    Node* closestNode(int x, int y);

    //random node from list
    Node* randomNode();

    //create an adjacency graph using the list of nodes, edges are made from nodes that are within a certain radius of another
    void generateGraph(int radius);

    //returns a list of nodes in order from a src node to a dest node
    std::vector<Node*> shortestPath(int src, int dest);

    //recursive function that generates a given path vector from a src to a dest using a completed parents vector. used by shortestPath()
    void _shortestPath(int currentVertex, std::vector<int> parents, std::vector<Node*> *path);

    //dijkstra's algorithm which populates a parents vector. used by shortestPath()
    void dijkstra(int src, std::vector<int> *parents);


};
