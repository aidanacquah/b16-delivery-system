/**
 * @file delivery_system.cpp
 * @brief Programme designed for a robot delivery system.
 */

#include <cstdio>
#include <limits>
#include <algorithm>
#include "task_queue.h"

using namespace std;

// Global parameters
const double epsilon = 1e-6;

// Implementation of Node class
Node::Node(int id): _id(id) {};

/**
 * @brief Returns the ID of the current node.
 *
 * @return The ID of the current node.
 */
int Node::GetId() const { return _id; }

/**
 * @brief Adds an edge from the current node to another node with a given distance.
 *
 * @param node The node to add an edge to.
 * @param distance The distance of the edge.
 */
void Node::AddEdge(Node node, double distance) {
    _edges.push_back(make_pair(node, distance));
}

/**
 * @brief Returns a vector of pairs, where each pair represents an edge from the current node to another node and its distance.
 *
 * @return A vector of pairs representing the edges of the current node.
 */
vector<pair<Node, double>> Node::GetEdges() {
    return _edges;
}

/**
 * @brief Sets the number of orders for the current node.
 *
 * @param orders The number of orders to set.
 */
void Node::SetNumOrders(int orders) { _num_orders = orders; }

/**
 * @brief Returns the number of orders for the current node.
 *
 * @return The number of orders for the current node.
 */
int Node::GetNumOrders() const { return _num_orders; }

/**
 * @brief Compares two nodes for equality based on their IDs.
 *
 * @param lhs The left-hand side node to compare.
 * @param rhs The right-hand side node to compare.
 * @return True if the nodes are equal, false otherwise.
 */
bool operator==(Node &lhs, Node &rhs) {
	return lhs.GetId() == rhs.GetId();
}


// Implementation of Graph class

/**
 * @brief Constructs a graph from a distance matrix.
 *
 * @param dist_matrix The distance matrix to use for constructing the graph.
 */
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

/**
 * @brief Returns the number of nodes in the graph.
 *
 * @return The number of nodes in the graph.
 */
int Graph::NumNodes() {
    return _nodes.size();
}

/**
 * @brief Updates the number of orders for each node in the graph randomly.
 *
 * @param seed The seed for the random number generator.
 */
void Graph::UpdateOrders(int seed) {
    srand(seed);
    // Exclude the first node that is the store
    for (int i=1; i<NumNodes(); i++) {
        int order = rand() % 3; // one can order up to 2 things
        _nodes[i].SetNumOrders(order);
    }
}

/**
 * @brief Returns a list of pairs of node ids and number of orders for each node in the graph.
 *
 * @return A vector of node id and order count pairs.
 */
vector<pair<int, int>> Graph::GetOrderList() {
    vector<pair<int, int>> order_list;
    for (const auto& node : _nodes) {
        order_list.emplace_back(node.GetId(), node.GetNumOrders());
    }
    return order_list;
}

/**
 * @brief Computes the shortest path between two nodes using Dijkstra algorithm.
 *
 * @param nodeIndex1 Index of the first node.
 * @param nodeIndex2 Index of the second node.
 * @param verbose If 1, prints just the path to console.
 *                If >1, prints the path and distance to the console.
 *                Else, no output to console.
 * @return A double, representing the shortest distance between the nodes.
 */
double Graph::ShortestPath(int nodeIndex1, int nodeIndex2, int verbose=0) {
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

    if (verbose > 0) {
        if (verbose > 1) {
            cout << "The shortest path between nodes " << nodeIndex1 << " and "
                << nodeIndex2 << ":" << endl;
        }

        for (int i = 0; i < path.size(); i++) {
            cout << path[i];

            if (i < path.size() - 1) {
                cout << " -> ";
            }
        }
        if (verbose > 1) {
            cout << "\nPath distance: " << dist[nodeIndex2] << endl;
        }
    }

    // Return the shortest distance between the input nodes
    return dist[nodeIndex2];
}

// Implementation of Robot class

/**
  * @brief Constructor for Robot class.
  * @param id The unique ID of the robot.
  * @param carrying_capacity The maximum weight the robot can carry.
  */
Robot::Robot(int id, int carrying_capacity): _id(id), _carrying_capacity(carrying_capacity) {};

/**
  * @brief Getter for robot ID.
  * @return The unique ID of the robot.
  */
int Robot::GetId() const { return _id; }

/**
  * @brief Getter for robot carrying capacity.
  * @return The maximum weight the robot can carry.
  */
int Robot::GetCarryingCapacity() const { return _carrying_capacity; }

// Implementation of Task class

/**
  * @brief Constructor for Task class.
  * @param robot_id The ID of the robot that will perform the task.
  * @param delivery_orders A vector of delivery orders, represented as pairs of house IDs and package weights.
  */
Task::Task(int robot_id, vector<pair<int, int>> delivery_orders): 
    _robot_id(robot_id), _delivery_orders(delivery_orders) {};

/**
 * @brief Getter for the robot ID associated with the task.
 * @return The ID of the robot that will perform the task.
 */
int Task::GetRobotId() const { return _robot_id; }

/**
  * @brief Getter for the vector of delivery orders associated with the task.
  * @return A vector of delivery orders, represented as pairs of house IDs and package weights.
  */
vector<pair<int,int>> Task::GetDeliveryOrders() const { return _delivery_orders; }

/**
  * @brief Display the shortest path for the robot to complete the delivery orders associated with the task.
  * @param graph The graph representing the delivery area.
  */
void Task::DisplayPath(Graph graph) {
    string pkg_str = "packages";
    int prev_node;
    vector<pair<int,int>> orders = GetDeliveryOrders();
    for (int i=0; i < orders.size(); i++) {
        if (orders[i].second == 1) {
            pkg_str = "package";
        }
        else {
            pkg_str = "packages";
        }

        if (i==0) {
            prev_node = 0;
        }
        else {
            prev_node = orders[i-1].first;
        }

        cout << "Robot " << GetRobotId() << " delivers " << orders[i].second 
            << " " << pkg_str << " to house " << orders[i].first << ", via: ";

        graph.ShortestPath(prev_node, orders[i].first, 1);
        cout << endl;
    }
}

// Implementation of TaskQueue class
/**
  * @brief Constructor for TaskQueue class.
  * @param orders A vector of delivery orders, represented as pairs of house IDs and package weights.
  * @param robot The robot that will perform the delivery tasks in the queue.
  */
TaskQueue::TaskQueue(vector<pair<int, int>> orders, Robot robot) {
    vector<vector<pair<int,int>>> delivery_groups;

    // Split orders into delivery groups that do not exceed robot's carrying capacity
    vector<pair<int,int>> group;
    int total_weight = 0;
    for (pair<int,int> order : orders) {
        if (order.second != 0) {
            if (total_weight + order.second > robot.GetCarryingCapacity()) {
                delivery_groups.push_back(group);
                group.clear();
                total_weight = 0;
            }
            group.push_back(order);
            total_weight += order.second;
        }
    }
    if (!group.empty()) {
        delivery_groups.push_back(group);
    }

    // Create a task for each delivery group
    for (const auto& group : delivery_groups) {
        Task task(robot.GetId(), group);
        _queue.push_back(task);
    }
}

/**
  * @brief Perform all the tasks in the queue.
  * @param graph The graph representing the delivery area.
  */
void TaskQueue::PerformTasks(Graph graph) {
    for (int i=0; i<_queue.size(); i++) {
        cout << "Task " << i+1 << ":" <<endl;
        _queue[i].DisplayPath(graph);
    }
    _queue.clear();
}
 vector<Task> TaskQueue::getQueue(){
    return _queue;
 }

/**
 * @brief Generates a weighted adjacency matrix based on the given parameters.
 *
 * This function generates a weighted adjacency matrix based on the given size, 
 * connectivity and random seed. 
 * Here, the weights represent distance in km between nodes.
 *
 * @param size The number of nodes to include in the distance matrix.
 * @param connectivity The probability of a random edge being created between two nodes.
 * @param seed The seed to use for the random number generator.
 * @return A 2D vector representing the distance matrix.
 */
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
            if ((p < connectivity) && (i != j)) {
                dist = (float)rand()/RAND_MAX;
                matrix[i][j] = dist;
                matrix[j][i] = dist;
            }
        }
    }

    return matrix;
}

/**
 * @brief Overload operator<< for vectors of any type.
 *
 * This function overloads the << operator for vectors of any type.
 * It outputs the vector's elements to the given output stream.
 *
 * @param os The output stream to write to.
 * @param v The vector to output.
 * @return The output stream.
 */
template <typename T>
std::ostream & operator<<(std::ostream &os, vector<T> v) {
    for (int i=0; i<v.size(); i++) {
        os << v[i] << " ";
    }
    os << endl;

    return os;
}

/**
 * @brief Overload operator<< for any 2d vector of doubles.
 *
 * This function overloads the << operator for 2d vectors of doubles.
 * It outputs the vector's elements to the given output stream.
 *
 * @param os The output stream to write to.
 * @param matrix The 2d vector to output.
 * @return The output stream.
 */
std::ostream & operator<<(std::ostream &os, vector<vector<double>> matrix) {
    os << "Neighbourhood Distance Matrix:" << endl;
    for (int i=0; i<matrix.size(); i++) {
        for (int j=0; j<matrix.size(); j++) {
            printf(" %.2f ", matrix[i][j]);
        }
        os << endl;
    }

    return os;
}

/**
 * @brief The main function of the program.
 * 
 * @return 0 on successful execution.
 */
int main ()
{
    const int num_nodes = 11; // 10 houses + 1 store
    const double connectivity = 0.1;
    const int num_days = 4;
    const Robot robot(101, 3);

    vector<vector<double>> dist_mat = generate_dist_matrix(num_nodes, connectivity, 0);
    cout << dist_mat;

    Graph graph(dist_mat);

    // Example path
    double u = graph.ShortestPath(1, 2, 2);

    for (int i=1; i<num_days+1; i++) {
        cout << "Day " << i << ":" << endl;
        graph.UpdateOrders(i);
        TaskQueue taskQueue(graph.GetOrderList(), robot);
        taskQueue.PerformTasks(graph);
    }

    return 0;
}