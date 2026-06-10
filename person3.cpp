// ================================================================
//  person3.cpp  —  PERSON 3
//  Module 5: Search & Retrieval Engine   (BST)
//  Module 6: Order History & Tracking   (Linked Stack + Undo)
// ================================================================

#include "person3.h"

// ================================================================
//  HistoryTracker — Linked Stack Implementation
// ================================================================

HistoryTracker::HistoryTracker() : top(nullptr), count(0) {}

HistoryTracker::~HistoryTracker() {
    while (top) { HistoryNode* t = top; top = top->next; delete t; }
}

std::string HistoryTracker::maskData(std::string name) {
    if (name.length() <= 2) return name;
    std::string masked = "";
    masked += name[0];
    for (size_t i = 1; i < name.length() - 1; i++)
        masked += (name[i] == ' ' ? ' ' : '*');
    masked += name[name.length() - 1];
    return masked;
}

void HistoryTracker::logAction(std::string action, std::string rawName, std::string orderId) {
    if (rawName != "") action += " | Customer: " + maskData(rawName);
    HistoryNode* n = new HistoryNode(action, orderId);
    n->next = top; top = n; count++;
}

std::string HistoryTracker::undo() {
    if (!top) return "";
    HistoryNode* temp = top;
    std::string log = temp->log;
    std::string id = temp->rawOrderId;
    top = top->next; count--;
    delete temp;
    std::cout << "  [UNDO] Reversed: " << log << "\n";
    return id;
}

void HistoryTracker::replayTimeline() {
    if (!top) { std::cout << "  [HISTORY] No records.\n"; return; }
    std::string* logs = new std::string[count];
    HistoryNode* cur = top;
    for (int i = count - 1; i >= 0; i--) { logs[i] = cur->log; cur = cur->next; }
    std::cout << "  --- Order Timeline (Oldest to Newest) ---\n";
    for (int i = 0; i < count; i++)
        std::cout << "    [" << (i + 1) << "] " << logs[i] << "\n";
    delete[] logs;
}

void HistoryTracker::displayHistory() {
    std::cout << "  --- Secure Order History (Most Recent First) ---\n";
    HistoryNode* temp = top;
    int i = 1;
    while (temp) {
        std::cout << "    [" << i++ << "] " << temp->log << "\n";
        temp = temp->next;
    }
}

int HistoryTracker::getCount() { return count; }

// ================================================================
//  SearchEngine — BST Implementation
// ================================================================

SearchEngine::SearchEngine() : root(nullptr) {}
SearchEngine::~SearchEngine() { destroyTree(root); }

BSTNode* SearchEngine::insert(BSTNode* node, int id, std::string c, std::string cat, std::string s) {
    if (!node) return new BSTNode(id, c, cat, s);
    if (id < node->orderId) node->left = insert(node->left, id, c, cat, s);
    else if (id > node->orderId) node->right = insert(node->right, id, c, cat, s);
    return node;
}

BSTNode* SearchEngine::searchById(BSTNode* node, int id) {
    if (!node || node->orderId == id) return node;
    if (id < node->orderId) return searchById(node->left, id);
    return searchById(node->right, id);
}

void SearchEngine::searchByName(BSTNode* node, std::string name, bool& found) {
    if (!node) return;
    searchByName(node->left, name, found);
    if (node->customer == name) {
        std::cout << "    -> Order #" << node->orderId << " | " << node->category << " | Status: " << node->status << "\n";
        found = true;
    }
    searchByName(node->right, name, found);
}

void SearchEngine::searchByCategory(BSTNode* node, std::string cat, bool& found) {
    if (!node) return;
    searchByCategory(node->left, cat, found);
    if (node->category == cat) {
        std::cout << "    -> Order #" << node->orderId << " | " << node->customer << " | Status: " << node->status << "\n";
        found = true;
    }
    searchByCategory(node->right, cat, found);
}

void SearchEngine::updateStatus(BSTNode* node, int id, std::string newStatus) {
    if (!node) return;
    if (node->orderId == id) { node->status = newStatus; return; }
    if (id < node->orderId) updateStatus(node->left, id, newStatus);
    else updateStatus(node->right, id, newStatus);
}

void SearchEngine::destroyTree(BSTNode* node) {
    if (node) { destroyTree(node->left); destroyTree(node->right); delete node; }
}

void SearchEngine::indexOrder(int id, std::string cust, std::string cat, std::string status) {
    root = insert(root, id, cust, cat, status);
}

void SearchEngine::findById(int id) {
    std::cout << "  [DATABASE] Looking up Order #" << id << "...\n";
    BSTNode* n = searchById(root, id);
    if (n) std::cout << "    -> Found: " << n->customer << " | " << n->category << " | " << n->status << "\n";
    else std::cout << "    -> No record found.\n";
}

void SearchEngine::findByCustomer(std::string name) {
    std::cout << "  [DATABASE] Searching by customer: " << name << "\n";
    bool found = false; searchByName(root, name, found);
    if (!found) std::cout << "    -> No records found.\n";
}

void SearchEngine::findByCategory(std::string cat) {
    std::cout << "  [DATABASE] Filtered search — Category: " << cat << "\n";
    bool found = false; searchByCategory(root, cat, found);
    if (!found) std::cout << "    -> No records in this category.\n";
}

void SearchEngine::setOrderStatus(int id, std::string newStatus) {
    updateStatus(root, id, newStatus);
    std::cout << "  [DATABASE] Order #" << id << " status updated to: " << newStatus << "\n";
}
