/**
 * @file task_queue.h
 * @brief Defines classes and functions for a task queue.
 */

#include <iostream>
#include <queue>
#include <vector>
#include "topological_map.h"

using namespace std;

class Task {
public:
    Task(int robot_id, vector<pair<int,int>> delivery_orders);
    int GetRobotId() const;
    vector<pair<int,int>> GetDeliveryOrders() const;

private:
    int _robot_id;
    vector<pair<int,int>> _delivery_orders;
};

std::ostream & operator<<(std::ostream &os, Task task);

class Robot {
public:
    Robot(int id, int carrying_capacity);
    int GetId() const;
    int GetCarryingCapacity() const;

private:
    const int _id;
    const int _carrying_capacity;
};

class TaskQueue {
public:
    TaskQueue(vector<pair<int, int>> orders, Robot robot);
    void PerformTasks(Graph graph);

private:
    vector<Task> _queue;
};
