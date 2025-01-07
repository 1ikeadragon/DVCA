#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_NAME_LEN 32
#define MAX_SECRET_LEN 64
#define STOCKS_COUNT 5
#define BUFFER_SIZE 64

typedef struct {
    char name[MAX_NAME_LEN];
    int quantity;
    double price;
} Stock;

Stock stocks[STOCKS_COUNT] = {
    {"WE45", 10, 100.00},
    {"AAPL", 100, 145.67},
    {"GOOG", 50, 2745.16},
    {"TSLA", 30, 850.12},
    {"AMZN", 20, 3344.55},
    {"MSFT", 150, 299.13}
};

char secret[MAX_SECRET_LEN];
char *dangling_ptr = NULL; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void load_config() {
    FILE *file = fopen("secret.txt", "r");
    if (!file) {
        perror("Failed to open secret file. Is secret.txt present?");
        exit(1);
    }
    fread(secret, 1, MAX_SECRET_LEN, file);
    fclose(file);
}

void display_user_info() {
    char buffer[BUFFER_SIZE];
    printf("Enter your display name: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    printf("Welcome, ");
    printf(buffer); 
    printf("!\n");
}

void search_stock() {
    char ticker[16];
    int found = 0;

    printf("Enter stock ticker (max 15 characters): ");
    fgets(ticker, sizeof(ticker), stdin);
    ticker[strcspn(ticker, "\n")] = '\0'; 

    for (int i = 0; i < STOCKS_COUNT; i++) {
        if (strcasecmp(stocks[i].name, ticker) == 0) { 
            printf("Ticker found: %s\n", stocks[i].name);
            printf("Price: $%.2f\n", stocks[i].price);
            printf("Available Quantity: %d\n", stocks[i].quantity);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Stock ticker '%s' not found.\n", ticker);
    }
}


void add_trade_note() {
    char *note = malloc(16);
    if (!note) {
        perror("malloc failed");
        exit(1);
    }
    printf("Enter a note for this trade (max 15 characters): ");
    gets(note); 
    printf("Trade note: %s\n", note);
    free(note);
}

void manage_trade_data() {
    char *data = malloc(32);
    if (!data) {
        perror("malloc failed");
        exit(1);
    }
    strcpy(data, "Temporary Trade Data");
    printf("Trade data before free: %s\n", data);

    free(data); 
    printf("Trade data after free: %s\n", data);
}

void view_system_diagnostics() {
    char *system_data = NULL; 

    printf("System uptime: %s\n", system_data); 
}

void track_portfolio_history() {
    char *portfolio_data = NULL;

    portfolio_data = malloc(64);
    if (!portfolio_data) {
        perror("malloc failed");
        return;
    }

    strcpy(portfolio_data, "Historical portfolio data.");
    printf("Data stored: %s\n", portfolio_data);

    if (rand() % 2) {
        free(portfolio_data);
        printf("Data released.\n");
    }

    printf("Accessing historical portfolio data: %s\n", portfolio_data);
}



void *update_stock_prices(void *arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);
        stocks[0].quantity++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void simulate_market() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, update_stock_prices, NULL);
    pthread_create(&t2, NULL, update_stock_prices, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Updated stock quantity of %s: %d\n", stocks[0].name, stocks[0].quantity);
}

void execute_trade() {
    char buffer[32];
    int stock_index;
    int quantity;
    float total_price;
    
    printf("Available stocks:\n");
    for (int i = 0; i < STOCKS_COUNT; i++) {
        printf("%d. %s - $%.2f (%d available)\n", 
               i + 1, 
               stocks[i].name, 
               stocks[i].price, 
               stocks[i].quantity);
    }

    printf("Enter stock index to trade (1-%d): ", STOCKS_COUNT);
    scanf("%d", &stock_index);
    getchar(); 
    
    if (stock_index < 1 || stock_index > STOCKS_COUNT) {
        printf("Invalid stock selection\n");
        return;
    }
    
    printf("Enter quantity to trade: ");
    scanf("%d", &quantity);
    getchar(); 
    
    total_price = stocks[stock_index - 1].price * quantity;
    
    printf("Enter confirmation message: ");
    gets(buffer); 
    
    stocks[stock_index - 1].quantity -= quantity;
    
    printf("Trade executed for %s:\n", stocks[stock_index - 1].name);
    printf("Quantity: %d\n", quantity);
    printf("Price per share: $%.2f\n", stocks[stock_index - 1].price);
    printf("Total cost: $%.2f\n", total_price);
    printf("Confirmation: %s\n", buffer);
}

void display_stock_info() {
    int index;

    printf("Enter stock index (1 to %d): ", STOCKS_COUNT);
    scanf("%d", &index);

    if (index < 1 || index > STOCKS_COUNT + 1) { 
        printf("Invalid stock index.\n");
        return;
    }

    printf("Stock Name: %s\n", stocks[index - 1].name);
    printf("Stock Price: $%.2f\n", stocks[index - 1].price);
}

void trade_stocks() {
    int choice, quantity;
    printf("Available stocks:\n");
    for (int i = 0; i < STOCKS_COUNT; i++) {
        printf("%d. %s - $%.2f (%d available)\n", i + 1, stocks[i].name, stocks[i].price, stocks[i].quantity);
    }

    printf("Enter stock number to trade: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > STOCKS_COUNT) {
        printf("Invalid stock choice.\n");
        return;
    }

    printf("Enter quantity to buy: ");
    scanf("%d", &quantity);

    if (quantity > stocks[choice - 1].quantity) {
        printf("Not enough stocks available.\n");
        return;
    }

    stocks[choice - 1].quantity -= quantity;
    printf("Bought %d of %s for $%.2f each.\n", quantity, stocks[choice - 1].name, stocks[choice - 1].price);
}

void menu() {
    int choice;

    while (1) {
        printf("\n--- Stock Trading CLI ---\n");
        printf("1. Trade Stocks\n");
        printf("2. Enter User Info\n");
        printf("3. Search Stock\n");
        printf("4. Add Trade Note\n");
        printf("5. Manage Trade Data\n");
        printf("6. Simulate Market\n");
        printf("7. Execute Trade\n");
        printf("8. Display Stock Info\n");
        printf("9. View System Diagnostics\n");
        printf("10. Historical portfolio\n");
        printf("11. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1:
                trade_stocks();
                break;
            case 2:
                display_user_info(); 
                break;
            case 3:
                search_stock();
                break;
            case 4:
                add_trade_note(); 
                break;
            case 5:
                manage_trade_data();
                break;
            case 6:
                simulate_market(); 
                break;
            case 7:
                execute_trade(); 
                break;
            case 8:
                display_stock_info(); 
                break;
            case 9:
                view_system_diagnostics(); 
                break;
            case 10:
                track_portfolio_history(); 
                break;
            case 11:
                printf("Exiting...\n");
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

int main() {
    load_config();
    menu();
    return 0;
}
