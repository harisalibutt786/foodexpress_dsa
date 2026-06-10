#pragma once
// ================================================================
//  person1.h  —  PERSON 1
//  Module 1: Dynamic Order Scheduling   (Max-Heap)
//  Module 2: Kitchen Load Balancer      (3 Linked Queues)
//  Module NEW: FoodMenu helper          (display + build cart)
// ================================================================

#include "common.h"

// ================================================================
//  FOOD MENU HELPER  (Person 1 owns this)
//  Displays the menu, lets user pick items & quantities,
//  returns a fully filled Order with price + prepTime set.
// ================================================================
class FoodMenu {
public:
    // Print the full menu to console
    void displayMenu();

    // Interactive: user picks items → returns filled Order shell.
    // Caller still provides orderId, customerName, category, deadline.
    Order buildOrderFromCart(int orderId, string customerName,
        string category, int deadline);
};

// ================================================================
//  MODULE 1: DYNAMIC ORDER SCHEDULING  (Max-Heap)
// ================================================================
class OrderScheduler {
private:
    Order* heap;
    int    capacity, size;

    void resize();
    void heapifyUp(int idx);
    void heapifyDown(int idx);
    int  findIndex(int id);

public:
    OrderScheduler();
    ~OrderScheduler();

    int   getSize();
    void  addOrder(Order ord);          // ← now accepts full Order
    void  updatePriority(int id, int newPriority);
    void  checkDeadlines(int elapsed);
    Order extractHighestPriority();
    void  cancelOrder(int id);
    void  displayQueue();
};

// ================================================================
//  MODULE 2: KITCHEN LOAD BALANCER  (3 Linked Queues)
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
