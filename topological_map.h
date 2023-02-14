/**
 * @file topological_map.h
 * @brief Defines classes and functions for a topological map.
 */
#include <vector>

using namespace std;

/// A node represents a point in the map.
class Node {
public:
    /// Constructor.
    Node(int id);

    /// Returns the ID of the node.
    int GetId() const;

    /// Adds an edge to the node.
    /// \param node The node to connect to.
    /// \param distance The distance to the connected node.
    void AddEdge(Node node, double distance);

    /// Returns a list of edges connected to the node.
    vector<pair<Node, double>> GetEdges();

    /// Sets the number of orders assigned to the node.
    /// \param orders The number of orders assigned to the node.
    void SetNumOrders(int orders);

    /// Returns the number of orders assigned to the node.
    int GetNumOrders() const;

private:
    /// A list of edges connected to the node.
    vector<pair<Node, double>> _edges;

    /// The number of orders assigned to the node.
    int _num_orders = 0;

    /// The ID of the node.
    int _id;
};


/// A graph represents the entire map.
class Graph {
public:
    /// Constructor.
    /// \param dist_matrix A distance matrix representing the map.
    Graph(vector<vector<double>> dist_matrix);

    /// Computes the shortest path between two nodes using Dijkstra algorithm.
    /// \param nodeIndex1 The index of the first node.
    /// \param nodeIndex2 The index of the second node.
    /// \param verbose Determines the display to console of path information.
    double ShortestPath(int nodeIndex1, int nodeIndex2, int verbose);

    /// Updates the number of orders assigned to each node.
    /// \param seed A random seed to use for generating the number of orders.
    void UpdateOrders(int seed);

    /// Returns the number of nodes in the graph.
    int NumNodes();

    /// Returns a list of pairs of node ids and number of orders for each node in the graph.
    vector<pair<int, int>> GetOrderList();

private:
    /// A list of nodes in the graph.
    vector<Node> _nodes;
};