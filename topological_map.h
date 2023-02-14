#include <vector>

using namespace std;

// A node represents a point in the map
class Node {
    public:
        Node(int id);
        int GetId();
        void AddEdge(Node node, double distance);
        vector<pair<Node, double>> GetEdges();
        void SetNumOrders(int orders);
        int GetNumOrders() const;

    private:
        vector<pair<Node, double>> _edges;
        int _num_orders=0;
        int _id;
};


// A graph represents the entire map
class Graph {
    public:
        Graph(vector<vector<double>> dist_matrix);
        double ShortestPath(int nodeIndex1, int nodeIndex2, bool verbose);
        void UpdateOrders(int seed);
        int NumNodes();
        vector<int> GetOrderList();

    private:
        vector<Node> _nodes;
};