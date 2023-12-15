#include <stdbool.h>

// #include "glopal.h"

#ifndef READ_INPUT_DATA_H
#define READ_INPUT_DATA_H

#define MAX_LINE_LENGTH_INPUT_FILE 100
#define MAX_CASHIERS 6
#define MAX_TOTAL_QUANTITY 20
#define MAX_NUM_ITEMS 12
#define MAX_LINE_LENGTH_INPUT_FILE 100

typedef struct
{
    char quality[MAX_LINE_LENGTH_INPUT_FILE];
} CashierQuality;

typedef struct
{
    int timeToFinishTheQueue; // for customer, instead of cashier’s queue size, number of items in the cart and scanning time for the cashier
    int cashier_behavior; // for customer
    int cashier_income; // for main 
    int done_customer; // for customer
    int number_of_served_customers; //not necessary  -> //? i will deal with it as current customer number
    int client_being_served;
    bool cashier_availability; // for main and customer 
} CashierData;

typedef struct
{
    char itemName[MAX_LINE_LENGTH_INPUT_FILE];
    int quantity;
    int price;
} Item;

typedef struct
{
    char *itemName;
    int chosenQuantity;
    int Price;
    int totalPrice;
} ChosenItem;

void readInputDataFromFile();
int readItemsFromFile(int maxItems);

#endif
