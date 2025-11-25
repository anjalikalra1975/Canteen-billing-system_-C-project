// main.c

#include "header.h" // Includes all necessary headers and function prototypes

/**
 * Displays the main interface and handles user navigation.
 */
void mainMenu() {
    int choice;
    do {
        printf("\n===================================\n");
        printf("===== CANTEEN BILLING SYSTEM ======\n");
        printf("===================================\n");
        printf("1. Display Menu\n");
        printf("2. Generate New Bill\n");
        printf("3. View Transaction History\n");
        printf("4. Exit Program\n");
        printf("-----------------------------------\n");
        printf("Enter your choice: ");
        
        // Basic input handling
        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input. Please enter a number.\n");
            // Clear input buffer to prevent infinite loop
            while (getchar() != '\n');
            choice = 0; 
            continue;
        }

        switch(choice) {
            case 1: 
                displayMenu(); 
                break;
            case 2: 
                generateBill(); 
                break;
            case 3: 
                viewHistory(); 
                break;
            case 4: 
                printf("\nExiting Canteen Billing System. Goodbye!\n"); 
                break;
            default: 
                printf("\nInvalid choice. Please enter 1, 2, 3, or 4.\n");
        }
    } while (choice != 4);
}

// =================================================================
//                            MAIN FUNCTION
// =================================================================
int main() {
    // 1. Load the menu data when the program starts
    loadMenu(); 
    
    // 2. Load the transaction history to determine the last Bill ID.
    // This allows the system to generate continuous bill numbers.
    viewHistory(); 
    
    // 3. Start the main user interface
    mainMenu();

    return 0;
}