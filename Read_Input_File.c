#include <stdio.h>
#include <string.h>
#include "Read_Input_File.h"
#include "glopal.h"

void readInputDataFromFile()
{
    // printf("=> readInputDataFromFile\n");
    FILE *file;
    char data[MAX_LINE_LENGTH_INPUT_FILE];
    char varName[MAX_LINE_LENGTH_INPUT_FILE];
    int value;
    int index = 0;

    file = fopen("InputData.txt", "r");
    if (file == NULL)
    {
        printf("Error When Open File !!!!!\n");
    }

    while (fgets(data, sizeof(data), file))
    {
        if (sscanf(data, "%[^=]=%d", varName, &value) == 2)
        {
            if (index == 0)
            {
                acustomer_arrival_rate_min = value;
            }
            else if (index == 1)
            {
                acustomer_arrival_rate_max = value;
            }
            else if (index == 2)
            {
                ashopping_time_min = value;
            }
            else if (index == 3)
            {
                ashopping_time_max = value;
            }
            else if (index == 4)
            {
                ascanning_time_min = value;
            }
            else if (index == 5)
            {
                ascanning_time_max = value;
            }
            else if (index == 6)
            {
                acustomer_impatience_threshold = value;
            }
            else if (index == 7)
            {
                acashier_behavior_degradation_threshold = value;
            }
            else if (index == 8)
            {
                asupermarket_income_threshold = value;
                // printf("super: %d", asupermarket_income_threshold);
            }
            else if (index == 9)
            {
                anumberOfCashiers = value;
            }
            else if (index == 10)
            {
                anumberOfCustomers = value;
            }
            index++;
            if (index == 11)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    fclose(file);

    if (index < 3)
    {
        printf("Insufficient data in the file.\n");
    }
}

int readItemsFromFile(int maxItems)
{
    // printf("=> readItemsFromFile\n");
    /*==================================== Shared Memory ====================================*/
    int shmid = createSharedMemory(sizeof(Item)*MAX_NUM_ITEMS, SHM_KEY_ITEMS); // Create shared memory segment

    if (shmid == -1)
    {
        printf("Error creating items shared memory\n");
        return EXIT_FAILURE;
    }

    Item *shm_items = (Item *)attachSharedMemory(shmid); // Attach shared memory

    if (shm_items == NULL)
    {
        printf("Error attaching shared memory\n");
        return EXIT_FAILURE;
    }

    
    FILE *file;
    char data[MAX_LINE_LENGTH_INPUT_FILE];
    char itemName[MAX_LINE_LENGTH_INPUT_FILE];
    int quantity, price;
    int index = 0;

    file = fopen("Item_Information.txt", "r");
    if (file == NULL)
    {
        printf("Error When Open File !!!!!\n");
        return 0;
    }

    while (index < maxItems && fgets(data, sizeof(data), file))
    {
        if (sscanf(data, "%[^,], quantity=%d, price=%d", itemName, &quantity, &price) == 3)
        {
            strcpy(shm_items[index].itemName, itemName);
            shm_items[index].quantity = quantity;
            shm_items[index].price = price;
            index++;
        }
        else
        {
            break;
        }
    }

    fclose(file);

    // for(int i=0; i< MAX_NUM_ITEMS; i++){
    //     printf("item: quantity\t[%d] price\t[%d]\n",shm_items[i].quantity, shm_items[i].price);
    // }

    detachSharedMemory(shm_items);

    if (index < maxItems)
    {
        printf("Insufficient data in the file.\n");
        return 0;
    }

    return 1;
}
