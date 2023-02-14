/**
 * @file task_queue.h
 * @brief Defines classes and functions for a task queue.
 */

#include <iostream>
#include <queue>
#include <vector>
#include "topological_map.h"

using namespace std;

/// A task performed by the delivery robot.
class Task {
public:
    /// A constructor.
    Task(int robot_id, vector<pair<int,int>> delivery_orders);

    /// Returns the ID of the robot.
    int GetRobotId() const;

    /// Returns the list of delivery orders.
    vector<pair<int,int>> GetDeliveryOrders() const;

    /// Display the path taken by the robot in completing tasks.
    void DisplayPath(Graph graph);

private:
    /// The ID of the robot.
    int _robot_id;

    /// The list of orders for each node.
    vector<pair<int,int>> _delivery_orders;
};

/// A robot with fixed id and carrying capacity.
class Robot {
public:
    /// A constructor. 
    Robot(int id, int carrying_capacity);

    /// Returns the id of the robot.
    int GetId() const;

    /// Returns the carrying capacity of the robot.
    int GetCarryingCapacity() const;

private:
    /// The ID of the robot.
    const int _id;
    
    /// The carrying capacity of the robot.
    const int _carrying_capacity;
};

/// A queue that represents the list of tasks to be performed by the robot.
class TaskQueue {
public:
    /// A constructor.
    TaskQueue(vector<pair<int, int>> orders, Robot robot);

    /// Perform the listed tasks and output to console. 
    void PerformTasks(Graph graph);

private:
    /// List of tasks to carry out.
    vector<Task> _queue;
};
