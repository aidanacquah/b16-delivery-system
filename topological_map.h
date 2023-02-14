#include <vector>

using namespace std;

// A node represents a point in the map
class Node {
    public:
        Node(vector<pair<Node *, double>> edges): _edges(edges) {};
        void SetOrder(int order);
        int GetOrder();
    private:
        const vector<pair<Node *, double>> _edges;
        int _order=0;
};

// A graph represents the entire map
class Graph {
    public:
        Graph(int num_nodes, vector<vector<int>> adj_matrix);
        void DisplayPath();
        void UpdateOrders();

    private:
        const vector<Node> _nodes;
};