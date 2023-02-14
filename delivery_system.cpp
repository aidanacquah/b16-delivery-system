#include <cstdio>
#include <iostream>
#include <limits>
#include <algorithm>
#include <queue>
#include "topological_map.h"

using namespace std;

// Global parameters
const double epsilon = 0.000001;

// Implementation of Node class
Node::Node(int id): _id(id) {};

int Node::GetId() { return _id; }

void Node::AddEdge(Node node, double distance) {
    _edges.push_back(make_pair(node, distance));
}

vector<pair<Node, double>> Node::GetEdges() {
    return _edges;
}

void Node::SetNumOrders(int orders) { _num_orders = orders; }
int Node::GetNumOrders() const { return _num_orders; }

bool operator==(Node &lhs, Node &rhs) {
	return lhs.GetId() == rhs.GetId();
}


/// Implementation of Graph class
Graph::Graph(vector<vector<double>> dist_matrix) {
    const int num_nodes = dist_matrix.size();
    double dist;
    vector<Node> nodes;

    for (int i = 0; i < num_nodes; i++) {
        nodes.push_back(Node(i));
    }

    for (int i=0; i<num_nodes; i++) {
        for (int j=0; j<num_nodes; j++) {
            dist = dist_matrix[i][j];
            if (dist > epsilon) {
                nodes[i].AddEdge(nodes[j], dist);
            }
        }
    }

    _nodes = nodes;
}

int Graph::NumNodes() {
    return _nodes.size();
}

void Graph::UpdateOrders(int seed) {
    srand(seed);
    // Exclude the first node that is the store
    for (int i=1; i<NumNodes(); i++) {
        int order = rand() % 3; // one can order up to 2 things
        _nodes[i].SetNumOrders(order);
    }
}

vector<int> Graph::GetOrderList() {
    vector<int> orders;

    // Exclude the first node that is the store
    for (int i=1; i<NumNodes(); i++) {
        orders.push_back(_nodes[i].GetNumOrders());
    } 

    return orders;
}

// Produced using ChatGPT
double Graph::ShortestPath(int nodeIndex1, int nodeIndex2, bool verbose=false) {
    const double infinity = numeric_limits<double>::infinity();
    vector<double> dist(NumNodes(), infinity);
    vector<int> prev(NumNodes(), -1);

    // Set the distance of the start node to 0
    dist[nodeIndex1] = 0;

    // Define a lambda function for the priority queue comparison function
    auto cmp = [&dist](int left, int right) { return dist[left] > dist[right]; };

    // Create a priority queue with the comparison function
    priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);

    // Add the start node to the priority queue
    pq.push(nodeIndex1);

    // Loop until the priority queue is empty or the destination node is reached
    while (!pq.empty()) {
        int u = pq.top(); // Get the node with the smallest distance
        pq.pop();
        if (u == nodeIndex2) {
            break;
        }

        // Iterate over all edges of the current node
        for (auto& edge : _nodes[u].GetEdges()) {
            int v = edge.first.GetId();
            double alt = dist[u] + edge.second;

            if (alt < dist[v]) {
                dist[v] = alt;
                prev[v] = u;
                pq.push(v);
            }
        }
        
    }

    if (prev[nodeIndex2] == -1) {
        // If the destination node is not reachable from the start node, return
        return -1;
    }

    // Reconstruct the shortest path
    vector<int> path;
    int u = nodeIndex2;
    while (u != -1) {
        path.push_back(u);
        u = prev[u];
    }
    reverse(path.begin(), path.end()); // Reverse the order of the nodes in the path

    if (verbose) {
        // Print the nodes in the shortest path
        for (int i = 0; i < path.size(); i++) {
            cout << path[i];

            if (i < path.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    }

    // Return the shortest distance between the input nodes
    return dist[nodeIndex2];
}


vector<vector<double>> generate_dist_matrix(int size, double connectivity=0.0, int seed=0) {
    srand(seed);
    vector<vector<double>> matrix(size, vector<double>(size, 0.0));
    double p, dist;

    // Connect all nodes to the adjacent node to ensure connectivity
    for (int i = 0; i < size; i++) {
        int j = (i + 1) % size;
        dist = (float)rand()/RAND_MAX;
        matrix[i][j] = dist;
        matrix[j][i] = dist;
    }

    // Make random connections between nodes
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            p = (float)rand()/RAND_MAX;
            if ((p < connectivity) & (i != j)) {
                dist = (float)rand()/RAND_MAX;
                matrix[i][j] = dist;
                matrix[j][i] = dist;
            }
        }
    }

    return matrix;
}

template <typename T>
std::ostream & operator<<(std::ostream &os, vector<T> v) {
    for (int j=0; j<v.size(); j++) {
        os << v[j] << " ";
    }
    os << endl;

    return os;
}

std::ostream & operator<<(std::ostream &os, vector<vector<double>> matrix) {
    os << "Matrix:" << endl;
    for (int i=0; i<matrix.size(); i++) {
        for (int j=0; j<matrix.size(); j++) {
            printf(" %.2f ", matrix[i][j]);
        }
        os << endl;
    }

    return os;
}


int main ()
{
    const int num_nodes = 11; // 10 houses + 1 store
    const double connectivity = 0.1;
    vector<vector<double>> dist_mat = generate_dist_matrix(num_nodes, connectivity, 0);
    cout << dist_mat;
    Graph graph(dist_mat);

    double u = graph.ShortestPath(1, 2, true);

    cout << "Path distance: " << u;

    return 0;
}