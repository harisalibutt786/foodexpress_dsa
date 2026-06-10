#pragma once
// ================================================================
//  person1.h  —  PERSON 1
//  Author: harisalibutt786 <2025sse52@uet.edu.pk>
//  Module 1: Dynamic Order Scheduling   (Max-Heap)
//  Module 2: Kitchen Load Balancer      (3 Linked Queues)
//  Module NEW: FoodMenu helper
// ================================================================

#include "common.h"

// ================================================================
//  FoodMenu — Display menu and build orders interactively
// ================================================================
class FoodMenu {
public:
    void displayMenu();
    Order buildOrderFromCart(int orderId, std::string customerName,
                             std::string category, int deadline);
};

// ================================================================
//  MODULE 1: DYNAMIC ORDER SCHEDULING  (Max-Heap)
// ================================================================
class OrderScheduler {
private:
    Order* heap;
    int    size;
    int    capacity;
    void   resize();
    void   heapifyUp(int idx);
    void   heapifyDown(int idx);
    int    findIndex(int id);
public:
    OrderScheduler();
    ~OrderScheduler();
    void  addOrder(Order ord);
    void  updatePriority(int id, int newPriority);
    void  checkDeadlines(int elapsed);
    Order extractHighestPriority();
    void  cancelOrder(int id);
    void  displayQueue();
    int   getSize();
};

// ================================================================
//  MODULE 2: KITCHEN LOAD BALANCER  (Linked Queues)
// ================================================================
struct QueueNode {
    Order      data;
    QueueNode* next;
    QueueNode(Order o) : data(o), next(nullptr) {}
};

class KitchenQueue {
private:
    QueueNode* front;
    QueueNode* rear;
    int        totalLoad;
public:
    KitchenQueue();
    ~KitchenQueue();
    void  enqueue(Order ord);
    Order dequeue();
    bool  isOverloaded();
    int   getLoad();
    bool  isEmpty();
};

class KitchenManager {
private:
    KitchenQueue kitchens[KITCHEN_COUNT];
public:
    void assignToBestKitchen(Order ord);
    void rebalanceKitchens();
    void displayKitchenStatus();
};
