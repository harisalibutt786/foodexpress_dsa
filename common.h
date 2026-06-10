#pragma once
// ================================================================
//  common.h
//  Shared constants and base data structures for FoodExpress
//  Used by ALL modules — include this in every .h file
// ================================================================

#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace std::chrono;

// ================================================================
//  GLOBAL CONSTANTS
// ================================================================
const int KITCHEN_COUNT        = 3;
const int KITCHEN_OVERLOAD_CAP = 60;   // minutes
const int INF                  = 999999;

// ================================================================
//  FOOD MENU — default items with prices
// ================================================================
const int MENU_SIZE = 10;

struct MenuItem {
    int    itemId;
    string name;
    int    price;        // Rs.
    int    prepTime;     // minutes
    string category;     // "Fast Food", "Drinks", "Dessert"
};

// Global menu table — all modules can read this
const MenuItem MENU[MENU_SIZE] = {
    { 1, "Burger",          250, 10, "Fast Food" },
    { 2, "Zinger Burger",   350, 12, "Fast Food" },
    { 3, "French Fries",    120,  5, "Fast Food" },
    { 4, "Pizza (Small)",   500, 20, "Fast Food" },
    { 5, "Pizza (Large)",   900, 25, "Fast Food" },
    { 6, "Broast Piece",    300, 15, "Fast Food" },
    { 7, "Cold Drink",       80,  2, "Drinks"    },
    { 8, "Juice",           150,  3, "Drinks"    },
    { 9, "Ice Cream",       200,  5, "Dessert"   },
    {10, "Brownie",         180,  7, "Dessert"   }
};

// ================================================================
//  ORDER ITEM — one row in a customer's cart
// ================================================================
struct OrderItem {
    int    itemId;
    string itemName;
    int    quantity;
    int    unitPrice;
    int    subtotal;
};

// ================================================================
//  BASE DATA STRUCTURES
// ================================================================
struct Order {
    int       orderId;
    string    customerName;
    int       priority;
    string    status;
    int       prepTime;
    int       deadline;
    string    category;
    // ---- NEW fields ----
    int       totalPrice;                  // Rs. total
    OrderItem items[MENU_SIZE];            // up to MENU_SIZE distinct items
    int       itemCount;                   // how many distinct items chosen
};

struct Rider {
    string name;
    bool   available;
    int    currentZone;
    int    currentLoad;
    int    capacity;
};
