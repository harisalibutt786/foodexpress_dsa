// ================================================================
//  main.cpp  —  FoodExpress Dispatch Optimization Engine
//  DSA Open Ended Lab
//
//  TEAM DIVISION:
//    Person 1  →  person1.h / person1.cpp
//                 FoodMenu (menu display + cart builder)
//                 Module 1: Dynamic Order Scheduling (Max-Heap)
//                 Module 2: Kitchen Load Balancer    (Linked Queues)
//
//    Person 2  →  person2.h / person2.cpp
//                 Module 3: Rider Dispatch           (Dynamic Array)
//                 Module 4: Route Optimization       (Graph + Dijkstra)
//
//    Person 3  →  person3.h / person3.cpp
//                 Module 5: Search & Retrieval       (BST)
//                 Module 6: Order History & Undo     (Linked Stack)
//
//    Person 4  →  person4.h / person4.cpp
//                 Module 7: Performance Analysis     (Benchmarks)
//                 Module 8: Scalability Simulation
//                 main.cpp  (this file)
// ================================================================

#include "person1.h"
#include "person2.h"
#include "person3.h"
#include "person4.h"


int main() {
    srand((unsigned)time(nullptr));

    // ---- Instantiate all subsystems ----
    FoodMenu            menu;             // Person 1
    OrderScheduler      scheduler;        // Person 1
    KitchenManager      kitchens;         // Person 1
    RiderManager        riders;           // Person 2
    CityMap             city;             // Person 2
    SearchEngine        searchDB;         // Person 3
    HistoryTracker      history;          // Person 3
    PerformanceAnalyzer analyzer;         // Person 4

    // ---- City Infrastructure ----
    city.addLocation("Model Town");
    city.addLocation("Gulberg");
    city.addLocation("DHA");
    city.addLocation("Raya");
    city.addLocation("Ksk");

    city.addRoute("Model Town", "Gulberg", 10);
    city.addRoute("Gulberg", "DHA", 25);
    city.addRoute("DHA", "Raya", 12);
    city.addRoute("Raya", "Ksk", 18);
    city.addRoute("DHA", "Ksk", 30);
    city.addRoute("Ksk", "Model Town", 20);

    // ---- Rider Fleet ----
    riders.addRider("Haris", city.getIndex("Model Town"), 2);
    riders.addRider("Aleem", city.getIndex("Gulberg"), 2);
    riders.addRider("Nouman", city.getIndex("DHA"), 1);
    riders.addRider("Ahmed", city.getIndex("Raya"), 2);

    // System starts with empty queue - add orders via menu option 1

    // ================================================================
    //  MAIN MENU LOOP
    // ================================================================
    int choice;
    do {
        cout << "\n  *======== FoodExpress Dispatch Engine ========*\n";
        cout << "  | 1.  Dynamic Order Scheduling               |\n";
        cout << "  | 2.  Kitchen Load Analysis                  |\n";
        cout << "  | 3.  Rider Dispatch Optimization            |\n";
        cout << "  | 4.  Route Optimization                     |\n";
        cout << "  | 5.  Search and Retrieval Engine            |\n";
        cout << "  | 6.  Order History Tracking                 |\n";
        cout << "  | 7.  Performance Analysis                   |\n";
        cout << "  | 8.  Scalability Simulation                 |\n";
        cout << "  | 9.  System Reports                         |\n";
        cout << "  | 10. Exit                                   |\n";
        cout << "  *============================================*\n";
        cout << "  Choice: "; cin >> choice; cout << "\n";

        // ============================================================
        //  1. ORDER SCHEDULING  (Person 1)
        // ============================================================
        if (choice == 1) {
            cout << "  [1] Add Order  [2] Cancel Order  [3] Update Priority"
                "  [4] Check Deadlines  [5] View Queue\n";
            int sub; cin >> sub;

            if (sub == 1) {
                // --- Collect basic info first ---
                int id, dl; string name, cat;
                cout << "  Order ID        : "; cin >> id;
                cout << "  Customer Name   : "; cin.ignore(); getline(cin, name);
                cout << "  Category (Regular/VIP/Bulk): "; cin >> cat;
                cout << "  Deadline mins (0=none)     : "; cin >> dl;

                // --- Let FoodMenu build the order with items & price ---
                Order ord = menu.buildOrderFromCart(id, name, cat, dl);

                if (ord.itemCount == 0) {
                    cout << "  [WARN] No items selected. Order not placed.\n";
                }
                else {
                    scheduler.addOrder(ord);
                    searchDB.indexOrder(id, name, cat, "Placed");

                    // Build a log string listing items
                    string itemLog = "";
                    for (int i = 0; i < ord.itemCount; i++) {
                        itemLog += ord.items[i].itemName + " x"
                            + to_string(ord.items[i].quantity);
                        if (i < ord.itemCount - 1) itemLog += ", ";
                    }
                    history.logAction("Order #" + to_string(id) + " Placed | "
                        + itemLog + " | Rs." + to_string(ord.totalPrice),
                        name, to_string(id));
                }
            }
            else if (sub == 2) {
                int id; cout << "  Order ID to cancel: "; cin >> id;
                scheduler.cancelOrder(id);
                searchDB.setOrderStatus(id, "Cancelled");
                history.logAction("Order #" + to_string(id) + " Cancelled", "", to_string(id));
            }
            else if (sub == 3) {
                int id, pri;
                cout << "  Order ID: ";     cin >> id;
                cout << "  New Priority: "; cin >> pri;
                scheduler.updatePriority(id, pri);
                history.logAction("Order #" + to_string(id) + " Priority Updated to " + to_string(pri));
            }
            else if (sub == 4) {
                int elapsed; cout << "  Elapsed time (mins): "; cin >> elapsed;
                scheduler.checkDeadlines(elapsed);
            }
            else if (sub == 5) {
                scheduler.displayQueue();
            }
        }

        // ============================================================
        //  2. KITCHEN LOAD ANALYSIS  (Person 1)
        // ============================================================
        else if (choice == 2) {
            cout << "  [1] Process Next Order  [2] Rebalance Kitchens  [3] Kitchen Status\n";
            int sub; cin >> sub;

            if (sub == 1) {
                Order next = scheduler.extractHighestPriority();
                if (next.orderId != -1) {
                    kitchens.assignToBestKitchen(next);
                    searchDB.setOrderStatus(next.orderId, "Queued");
                    history.logAction("Order #" + to_string(next.orderId) + " sent to Kitchen",
                        "", to_string(next.orderId));
                }
                else {
                    cout << "  [INFO] No pending orders.\n";
                }
            }
            else if (sub == 2) kitchens.rebalanceKitchens();
            else if (sub == 3) kitchens.displayKitchenStatus();
        }

        // ============================================================
        //  3. RIDER DISPATCH  (Person 2)
        // ============================================================
        else if (choice == 3) {
            cout << "  [1] Assign Rider  [2] Release Rider  [3] View Riders\n";
            int sub; cin >> sub;

            if (sub == 1) {
                int id; cout << "  Order ID to dispatch: "; cin >> id;
                riders.assignRider(id, city.getIndex("Gulberg"), history, city);
                searchDB.setOrderStatus(id, "Assigned");
            }
            else if (sub == 2) {
                string name; cout << "  Rider Name: "; cin >> name;
                riders.releaseRider(name);
            }
            else if (sub == 3) riders.displayRiders();
        }

        // ============================================================
        //  4. ROUTE OPTIMIZATION  (Person 2)
        // ============================================================
        else if (choice == 4) {
            cout << "  [1] Shortest Path  [2] Block Road  [3] Unblock Road"
                "  [4] Compare Routes  [5] Display Map\n";
            int sub; cin >> sub;
            string src, dest;

            if (sub == 5) { city.displayMap(); }
            else if (sub == 1) {
                cout << "  From: "; cin >> src; cout << "  To: "; cin >> dest;
                city.optimizeRoute(src, dest);
            }
            else if (sub == 2) {
                cout << "  From: "; cin >> src; cout << "  To: "; cin >> dest;
                city.blockRoad(src, dest);
            }
            else if (sub == 3) {
                cout << "  From: "; cin >> src; cout << "  To: "; cin >> dest;
                city.unblockRoad(src, dest);
            }
            else if (sub == 4) {
                string a1, a2;
                cout << "  Start Option 1: "; cin >> a1;
                cout << "  Start Option 2: "; cin >> a2;
                cout << "  Destination: ";    cin >> dest;
                city.compareRoutes(a1, a2, dest);
            }
        }

        // ============================================================
        //  5. SEARCH & RETRIEVAL  (Person 3)
        // ============================================================
        else if (choice == 5) {
            cout << "  [1] Search by Order ID  [2] Search by Customer"
                "  [3] Search by Category  [4] Update Status\n";
            int sub; cin >> sub;

            if (sub == 1) {
                int id; cout << "  Order ID: "; cin >> id;
                searchDB.findById(id);
            }
            else if (sub == 2) {
                string name; cout << "  Customer Name: "; cin.ignore(); getline(cin, name);
                searchDB.findByCustomer(name);
            }
            else if (sub == 3) {
                string cat; cout << "  Category (Regular/VIP/Bulk): "; cin >> cat;
                searchDB.findByCategory(cat);
            }
            else if (sub == 4) {
                int id; string status;
                cout << "  Order ID: ";    cin >> id;
                cout << "  New Status: "; cin >> status;
                searchDB.setOrderStatus(id, status);
                history.logAction("Order #" + to_string(id) + " status -> " + status);
            }
        }

        // ============================================================
        //  6. ORDER HISTORY  (Person 3)
        // ============================================================
        else if (choice == 6) {
            cout << "  [1] View History  [2] Replay Timeline  [3] Undo Last Action\n";
            int sub; cin >> sub;
            if (sub == 1) history.displayHistory();
            else if (sub == 2) history.replayTimeline();
            else if (sub == 3) history.undo();
        }

        // ============================================================
        //  7. PERFORMANCE ANALYSIS  (Person 4)
        // ============================================================
        else if (choice == 7) {
            analyzer.runBenchmark();
        }

        // ============================================================
        //  8. SCALABILITY SIMULATION  (Person 4)
        // ============================================================
        else if (choice == 8) {
            int n; cout << "  How many orders to inject? "; cin >> n;
            cout << "  [SIMULATION] Injecting " << n << " orders...\n";
            auto t1 = high_resolution_clock::now();
            for (int i = 1000; i < 1000 + n; i++) {
                Order sim;
                sim.orderId = i;
                sim.customerName = "BotUser";
                sim.priority = rand() % 10 + 1;
                sim.status = "Placed";
                sim.prepTime = 5;
                sim.deadline = 0;
                sim.category = "Regular";
                sim.totalPrice = 250;
                sim.itemCount = 1;
                sim.items[0] = { 1, "Burger", 1, 250, 250 };
                scheduler.addOrder(sim);
            }
            auto t2 = high_resolution_clock::now();
            cout << "  [SIMULATION] Done in "
                << duration_cast<milliseconds>(t2 - t1).count()
                << " ms. Heap resized dynamically. No overflow.\n";
        }

        // ============================================================
        //  9. SYSTEM REPORTS  (Person 4)
        // ============================================================
        else if (choice == 9) {
            cout << "  --- Live System Report ---\n";
            cout << "    City Locations Indexed  : " << city.getCount() << "\n";
            cout << "    Pending Orders          : " << scheduler.getSize() << "\n";
            cout << "    History Log Entries     : " << history.getCount() << "\n";
            cout << "    Memory Policy           : Dynamic resize (no fixed cap)\n";
            cout << "    Data Privacy            : Customer names masked in all logs\n";
            menu.displayMenu();
            riders.displayRiders();
            kitchens.displayKitchenStatus();
        }

    } while (choice != 10);

    cout << "  [SYSTEM] FoodExpress shutting down. Thank you.\n";
    return 0;
}
