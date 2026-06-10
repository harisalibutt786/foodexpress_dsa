// ================================================================
//  person1.cpp  —  PERSON 1
//  Module 1: Dynamic Order Scheduling   (Max-Heap)
//  Module 2: Kitchen Load Balancer      (3 Linked Queues)
//  Module NEW: FoodMenu helper
// ================================================================

#include "person1.h"

// ================================================================
//  FoodMenu Implementation
// ================================================================

void FoodMenu::displayMenu() {
    cout << "\n  +--------------------------------------------------+\n";
    cout << "  |           ** FoodExpress Menu **                |\n";
    cout << "  +----+----------------------+---------+-----------+\n";
    cout << "  | ID | Item                 | Price   | Prep Time |\n";
    cout << "  +----+----------------------+---------+-----------+\n";
    for (int i = 0; i < MENU_SIZE; i++) {
        cout << "  | " << MENU[i].itemId
            << "  | ";
        // pad item name to 20 chars
        string name = MENU[i].name;
        while ((int)name.length() < 20) name += " ";
        cout << name
            << " | Rs. ";
        string price = to_string(MENU[i].price);
        while ((int)price.length() < 3) price = " " + price;
        cout << price
            << "  | " << MENU[i].prepTime << " mins    |\n";
    }
    cout << "  +----+----------------------+---------+-----------+\n";
}

Order FoodMenu::buildOrderFromCart(int orderId, string customerName,
    string category, int deadline) {
    Order ord;
    ord.orderId = orderId;
    ord.customerName = customerName;
    ord.category = category;
    ord.deadline = deadline;
    ord.status = "Placed";
    ord.totalPrice = 0;
    ord.prepTime = 0;
    ord.itemCount = 0;
    ord.priority = 5;   // default; overridden below based on category

    // Auto-priority: VIP=10, Bulk=3, Regular=5
    if (category == "VIP")  ord.priority = 10;
    else if (category == "Bulk") ord.priority = 3;
    else                         ord.priority = 5;

    displayMenu();

    cout << "\n  Enter items (item ID then quantity). Type 0 to finish.\n";

    while (true) {
        int itemId;
        cout << "  Item ID (0 to finish): ";
        cin >> itemId;
        if (itemId == 0) break;

        // Validate ID
        bool found = false;
        for (int i = 0; i < MENU_SIZE; i++) {
            if (MENU[i].itemId == itemId) {
                found = true;

                int qty;
                cout << "  Quantity for " << MENU[i].name << ": ";
                cin >> qty;
                if (qty <= 0) { cout << "  [WARN] Quantity must be >= 1. Skipped.\n"; break; }

                // Check if item already in cart
                bool alreadyIn = false;
                for (int j = 0; j < ord.itemCount; j++) {
                    if (ord.items[j].itemId == itemId) {
                        ord.items[j].quantity += qty;
                        ord.items[j].subtotal = ord.items[j].quantity * MENU[i].price;
                        alreadyIn = true;
                        break;
                    }
                }

                if (!alreadyIn && ord.itemCount < MENU_SIZE) {
                    ord.items[ord.itemCount].itemId = itemId;
                    ord.items[ord.itemCount].itemName = MENU[i].name;
                    ord.items[ord.itemCount].quantity = qty;
                    ord.items[ord.itemCount].unitPrice = MENU[i].price;
                    ord.items[ord.itemCount].subtotal = qty * MENU[i].price;
                    ord.itemCount++;
                }

                // Add max prepTime of all items (parallel kitchen assumption)
                if (MENU[i].prepTime > ord.prepTime)
                    ord.prepTime = MENU[i].prepTime;

                break;
            }
        }
        if (!found) cout << "  [ERROR] Invalid item ID. Try again.\n";
    }

    // Sum total price
    for (int i = 0; i < ord.itemCount; i++)
        ord.totalPrice += ord.items[i].subtotal;

    // Print order summary
    cout << "\n  ============ ORDER SUMMARY ============\n";
    cout << "  Order #" << orderId << "  |  Customer: " << customerName << "\n";
    cout << "  Category: " << category << "  |  Priority: " << ord.priority << "\n";
    cout << "  ---------------------------------------\n";
    for (int i = 0; i < ord.itemCount; i++) {
        cout << "  " << ord.items[i].itemName
            << " x" << ord.items[i].quantity
            << "  =>  Rs. " << ord.items[i].subtotal << "\n";
    }
    cout << "  ---------------------------------------\n";
    cout << "  TOTAL PRICE  : Rs. " << ord.totalPrice << "\n";
    cout << "  EST. PREP    : " << ord.prepTime << " mins\n";
    if (deadline > 0)
        cout << "  DEADLINE     : " << deadline << " mins\n";
    cout << "  =======================================\n";

    return ord;
}

// ================================================================
//  OrderScheduler — Max-Heap
// ================================================================

OrderScheduler::OrderScheduler() { capacity = 10; size = 0; heap = new Order[capacity]; }
OrderScheduler::~OrderScheduler() { delete[] heap; }

void OrderScheduler::resize() {
    capacity *= 2;
    Order* temp = new Order[capacity];
    for (int i = 0; i < size; i++) temp[i] = heap[i];
    delete[] heap; heap = temp;
}

void OrderScheduler::heapifyUp(int idx) {
    while (idx > 0 && heap[(idx - 1) / 2].priority < heap[idx].priority) {
        swap(heap[idx], heap[(idx - 1) / 2]);
        idx = (idx - 1) / 2;
    }
}

void OrderScheduler::heapifyDown(int idx) {
    int largest = idx;
    int l = 2 * idx + 1, r = 2 * idx + 2;
    if (l < size && heap[l].priority > heap[largest].priority) largest = l;
    if (r < size && heap[r].priority > heap[largest].priority) largest = r;
    if (largest != idx) { swap(heap[idx], heap[largest]); heapifyDown(largest); }
}

int OrderScheduler::findIndex(int id) {
    for (int i = 0; i < size; i++) if (heap[i].orderId == id) return i;
    return -1;
}

int OrderScheduler::getSize() { return size; }

// Accepts a fully built Order object
void OrderScheduler::addOrder(Order ord) {
    if (size == capacity) resize();
    heap[size] = ord;
    heapifyUp(size++);
    cout << "  [SCHEDULER] Order #" << ord.orderId
        << " | " << ord.category
        << " | Priority " << ord.priority
        << " | Rs. " << ord.totalPrice
        << " | Dynamically inserted.\n";
}

void OrderScheduler::updatePriority(int id, int newPriority) {
    int i = findIndex(id);
    if (i == -1) { cout << "  [ERROR] Order #" << id << " not found.\n"; return; }
    int old = heap[i].priority;
    heap[i].priority = newPriority;
    if (newPriority > old) heapifyUp(i); else heapifyDown(i);
    cout << "  [SCHEDULER] Order #" << id << " priority: " << old << " -> " << newPriority << "\n";
}

void OrderScheduler::checkDeadlines(int elapsed) {
    cout << "  [DEADLINE CHECK] Elapsed: " << elapsed << " mins\n";
    for (int i = 0; i < size; i++) {
        if (heap[i].deadline > 0 && elapsed > heap[i].deadline) {
            heap[i].status = "delayed";
            cout << "  [ALERT] Order #" << heap[i].orderId
                << " (" << heap[i].customerName << ") is DELAYED!\n";
        }
    }
}

Order OrderScheduler::extractHighestPriority() {
    if (size == 0) {
        Order empty = {}; empty.orderId = -1; return empty;
    }
    Order top = heap[0];
    heap[0] = heap[--size];
    heapifyDown(0);
    return top;
}

void OrderScheduler::cancelOrder(int id) {
    int i = findIndex(id);
    if (i == -1) { cout << "  [ERROR] Order #" << id << " not found.\n"; return; }
    cout << "  [SCHEDULER] Order #" << id << " (" << heap[i].customerName << ") cancelled.\n";
    heap[i] = heap[--size];
    heapifyDown(i);
}

void OrderScheduler::displayQueue() {
    if (size == 0) { cout << "  [INFO] No pending orders.\n"; return; }
    cout << "  --- Pending Orders ---\n";
    for (int i = 0; i < size; i++) {
        cout << "    #" << heap[i].orderId
            << " | " << heap[i].customerName
            << " | Pri: " << heap[i].priority
            << " | Rs. " << heap[i].totalPrice
            << " | " << heap[i].category
            << " | " << heap[i].status << "\n";
    }
}

// ================================================================
//  KitchenQueue — Linked Queue
// ================================================================

KitchenQueue::KitchenQueue() : front(nullptr), rear(nullptr), totalLoad(0) {}
KitchenQueue::~KitchenQueue() { while (front) dequeue(); }

void KitchenQueue::enqueue(Order ord) {
    QueueNode* n = new QueueNode(ord);
    if (!rear) front = rear = n;
    else { rear->next = n; rear = n; }
    totalLoad += ord.prepTime;
}

Order KitchenQueue::dequeue() {
    if (!front) { Order e = {}; e.orderId = -1; return e; }
    QueueNode* t = front;
    Order ord = t->data;
    totalLoad -= ord.prepTime;
    front = front->next;
    if (!front) rear = nullptr;
    delete t;
    return ord;
}

bool KitchenQueue::isOverloaded() { return totalLoad > KITCHEN_OVERLOAD_CAP; }
int  KitchenQueue::getLoad() { return totalLoad; }
bool KitchenQueue::isEmpty() { return front == nullptr; }

// ================================================================
//  KitchenManager — Load Balancing
// ================================================================

void KitchenManager::assignToBestKitchen(Order ord) {
    int best = 0;
    for (int i = 1; i < KITCHEN_COUNT; i++)
        if (kitchens[i].getLoad() < kitchens[best].getLoad()) best = i;
    if (kitchens[best].isOverloaded())
        cout << "  [WARNING] All kitchens overloaded!\n";
    kitchens[best].enqueue(ord);
    cout << "  [BALANCER] Order #" << ord.orderId
        << " -> Kitchen " << (best + 1)
        << " | Wait: " << kitchens[best].getLoad() << " mins"
        << (kitchens[best].isOverloaded() ? " [OVERLOADED]" : "") << "\n";
}

void KitchenManager::rebalanceKitchens() {
    int busiest = 0, lightest = 0;
    for (int i = 1; i < KITCHEN_COUNT; i++) {
        if (kitchens[i].getLoad() > kitchens[busiest].getLoad())  busiest = i;
        if (kitchens[i].getLoad() < kitchens[lightest].getLoad()) lightest = i;
    }
    int diff = kitchens[busiest].getLoad() - kitchens[lightest].getLoad();
    if (diff > 20 && !kitchens[busiest].isEmpty()) {
        Order moved = kitchens[busiest].dequeue();
        kitchens[lightest].enqueue(moved);
        cout << "  [REBALANCE] Order #" << moved.orderId
            << " moved K" << (busiest + 1) << " -> K" << (lightest + 1)
            << " (diff was " << diff << " mins)\n";
    }
    else {
        cout << "  [REBALANCE] Balanced (max diff: " << diff << " mins)\n";
    }
}

void KitchenManager::displayKitchenStatus() {
    cout << "  --- Kitchen Status ---\n";
    for (int i = 0; i < KITCHEN_COUNT; i++) {
        cout << "    Kitchen " << (i + 1)
            << " | Load: " << kitchens[i].getLoad() << " mins"
            << (kitchens[i].isOverloaded() ? " [OVERLOADED]" : " [OK]") << "\n";
    }
}

