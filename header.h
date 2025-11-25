// headers.h

#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // For getting the current date/time

// --- Constants ---
#define MAX_ITEMS 50
#define MAX_ORDER_ITEMS 10 // Max items a customer can order in one bill

// --- Data Structures ---
typedef struct {
    int id;
    char name[50];
    float price;
} MenuItem;

// Structure for a single item within a bill
typedef struct {
    int itemId;
    char name[50];
    int quantity;
    float subTotal;
} OrderItem;

// --- Global Data (Declared here, defined in functions.c) ---
extern MenuItem menu[MAX_ITEMS];
extern int menuCount;
extern int lastBillNumber;

// --- Function Prototypes (Declarations) ---
void getCurrentDateTime(char *dateStr, char *timeStr);
void loadMenu();
void displayMenu();
void generateBill();
void viewHistory();
void mainMenu();

#endif