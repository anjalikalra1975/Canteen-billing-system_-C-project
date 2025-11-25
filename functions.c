// functions.c

#include "header.h" 

MenuItem menu[MAX_ITEMS]; 
int menuCount = 0;
int lastBillNumber = 0; // Tracks the last Bill ID used for continuity

/*
Helper function to get the current date and time.
 */
void getCurrentDateTime(char *dateStr, char *timeStr) {
    time_t timer;
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    // Format date as YYYY-MM-DD
    strftime(dateStr, 11, "%Y-%m-%d", tm_info); 
    // Format time as HH:MM:SS
    strftime(timeStr, 9, "%H:%M:%S", tm_info); 
}

/**
 * Reads menu items from the 'menu.txt' file.
 */
void loadMenu() {
    FILE *fp = fopen("menu.txt", "r");
    
    if (fp == NULL) {
        printf("\n  ERROR: 'menu.txt' file not found. Ensure the file exists.\n");
        exit(1); 
    }
    
    printf("Loading menu items...\n");
    
    while (menuCount < MAX_ITEMS && 
           fscanf(fp, "%d,%49[^,],%f\n", 
                  &menu[menuCount].id, 
                  menu[menuCount].name, 
                  &menu[menuCount].price) == 3) {
        menuCount++;
    }

    fclose(fp);
    printf(" Menu loaded successfully! Total items: %d\n", menuCount);
}

/**
 * Displays the loaded menu items to the user.
 */
void displayMenu() {
    if (menuCount == 0) {
        printf("\nMenu is empty. Load failed or file is blank.\n");
        return;
    }
    
    printf("\n============================================\n");
    printf("|              CANTEEN MENU                |\n");
    printf("============================================\n");
    printf("| %-4s | %-20s | %-8s |\n", "ID", "ITEM NAME", "PRICE");
    printf("============================================\n");
    
    for (int i = 0; i < menuCount; i++) {
        printf("| %-4d | %-20s | %-8.2f |\n", 
               menu[i].id, 
               menu[i].name, 
               menu[i].price);
    }
    printf("============================================\n");
}


/**
 * Core logic for taking orders, calculating the total, printing the receipt, 
 * and saving the transaction to history.txt.
 */
void generateBill() {
    if (menuCount == 0) {
        printf("\nCannot generate bill: Menu is empty. Please load menu first.\n");
        return;
    }

    // Prepare bill variables
    OrderItem currentOrder[MAX_ORDER_ITEMS];
    int currentItemCount = 0;
    float finalTotal = 0.0;
    int itemID, quantity;
    int foundIndex = -1;

    displayMenu();
    printf("\n--- Generating New Bill (Next ID: %d) ---\n", lastBillNumber + 1);

    // Order input loop
    do {
        printf("Enter Item ID (0 to finalize bill): ");
        if (scanf("%d", &itemID) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear buffer
            continue;
        }

        if (itemID == 0) 
        break; // Exit loop

        printf("Enter Quantity: ");
        if (scanf("%d", &quantity) != 1 || quantity <= 0) {
            printf("Invalid quantity. Must be a positive number.\n");
            while (getchar() != '\n');
            continue;
        }

        // Search for the item in the loaded menu
        foundIndex = -1;
        for (int i = 0; i < menuCount; i++) {
            if (menu[i].id == itemID) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex != -1) {
            if (currentItemCount >= MAX_ORDER_ITEMS) {
                printf("Cannot add more items. Bill limit reached.\n");
                break;
            }
            
            // Calculate and record the item
            float price = menu[foundIndex].price;
            float subTotal = price * quantity;
            finalTotal += subTotal;

            // Save details to the current order array
            currentOrder[currentItemCount].itemId = itemID;
            strcpy(currentOrder[currentItemCount].name, menu[foundIndex].name);
            currentOrder[currentItemCount].quantity = quantity;
            currentOrder[currentItemCount].subTotal = subTotal;
            currentItemCount++;

            printf("Added: %s x %d. Current Total: $%.2f\n", 
                   menu[foundIndex].name, quantity, finalTotal);

        } else {
            printf(" Error: Item ID %d not found in menu.\n", itemID);
        }

    } while (1); // Loop until 0 is entered

    // Finalize and print receipt if items were ordered
    if (finalTotal > 0.0) {
        lastBillNumber++;
        char dateStr[11], timeStr[9];
        getCurrentDateTime(dateStr, timeStr);

        printf("\n\n============================================\n");
        printf("              CANTEEN RECEIPT               \n");
        printf("============================================\n");
        printf("Bill No: %-3d | Date: %s | Time: %s\n", lastBillNumber, dateStr, timeStr);
        printf("--------------------------------------------\n");
        printf("%-20s %-4s %8s\n", "ITEM", "QTY", "AMOUNT");
        printf("--------------------------------------------\n");

        for (int i = 0; i < currentItemCount; i++) {
            printf("%-20s %-4d %8.2f\n", 
                   currentOrder[i].name, 
                   currentOrder[i].quantity, 
                   currentOrder[i].subTotal);
        }
        
        printf("--------------------------------------------\n");
        printf("%-20s Total: %15.2f\n", "FINAL AMOUNT", finalTotal);
        printf("============================================\n");
        printf("Thank you for your order!\n");

        // Save bill to history.txt
        FILE *fp_history = fopen("history.txt", "a");
        if (fp_history != NULL) {
            // Format: BillID,Date,Time,TotalAmount
            fprintf(fp_history, "%d,%s,%s,%.2f\n", 
                    lastBillNumber, dateStr, timeStr, finalTotal);
            fclose(fp_history);
            printf("\n Bill saved to history.txt.\n");
        } else {
            printf("\n WARNING: Could not save bill to history.txt.\n");
        }

    } else {
        printf("\nBill cancelled. No items ordered.\n");
    }
}

/**
 * Reads and displays the transaction history from 'history.txt'.
 */
void viewHistory() {
    FILE *fp = fopen("history.txt", "r");
    if (fp == NULL) {
        printf("\nHistory file 'history.txt' not found or empty.\n");
        return;
    }

    int billID;
    char date[11], time[9];
    float total;
    int recordCount = 0;

    printf("\n======================================================\n");
    printf("|              TRANSACTION HISTORY                   |\n");
    printf("======================================================\n");
    printf("| %-7s | %-12s | %-8s | %-10s |\n", 
           "BILL ID", "DATE", "TIME", "TOTAL");
    printf("======================================================\n");

    // Loop to read data until End Of File
    while (fscanf(fp, "%d,%10[^,],%8[^,],%f\n", 
           &billID, date, time, &total) == 4) {
        
        printf("| %-7d | %-12s | %-8s | %-10.2f |\n", 
               billID, date, time, total);
        recordCount++;
        // This updates lastBillNumber based on file contents
        if (billID > lastBillNumber) {
            lastBillNumber = billID;
        }
    }
    
    if (recordCount == 0 && lastBillNumber == 0) {
        printf("| No records found in history.txt.                   |\n");
    }
    
    fclose(fp);
    printf("======================================================\n");
}