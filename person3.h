#pragma once
// ================================================================
// Author: Nouman <learning9331@gmail.com>
//  person3.h  —  PERSON 3
//  Module 5: Search & Retrieval Engine   (BST)
//  Module 6: Order History & Tracking   (Linked Stack + Undo)
// ================================================================

#include "common.h"

// ================================================================
//  MODULE 6: ORDER HISTORY & TRACKING  (Linked Stack + Undo)
// ================================================================
struct HistoryNode {
    std::string log;
    std::string rawOrderId;    // for undo support
    HistoryNode* next;
    HistoryNode(std::string l, std::string id = "") : log(l), rawOrderId(id), next(nullptr) {}
};

class HistoryTracker {
private:
    HistoryNode* top;
    int count;
    std::string maskData(std::string name);
public:
    HistoryTracker();
    ~HistoryTracker();
    void logAction(std::string action, std::string rawName = "", std::string orderId = "");
    std::string undo();
    void replayTimeline();
    void displayHistory();
    int getCount();
};

// ================================================================
//  MODULE 5: SEARCH & RETRIEVAL ENGINE  (BST)
// ================================================================
class BSTNode {
public:
    int orderId;
    std::string customer;
    std::string category;
    std::string status;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int id, std::string c, std::string cat, std::string s)
        : orderId(id), customer(c), category(cat), status(s), left(nullptr), right(nullptr) {}
};

class SearchEngine {
private:
    BSTNode* root;
    BSTNode* insert(BSTNode* node, int id, std::string c, std::string cat, std::string s);
    BSTNode* searchById(BSTNode* node, int id);
    void searchByName(BSTNode* node, std::string name, bool& found);
    void searchByCategory(BSTNode* node, std::string cat, bool& found);
    void updateStatus(BSTNode* node, int id, std::string newStatus);
    void destroyTree(BSTNode* node);
public:
    SearchEngine();
    ~SearchEngine();
    void indexOrder(int id, std::string cust, std::string cat = "Regular", std::string status = "Placed");
    void findById(int id);
    void findByCustomer(std::string name);
    void findByCategory(std::string cat);
    void setOrderStatus(int id, std::string newStatus);
};
