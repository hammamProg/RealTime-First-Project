#include "ui_global.h"
#include "glopal.h"

// sem_t semaphore;

bool isChosen(ChosenItem chosenItems[], int chosenCount, char itemName[]);
void chooseRandomItems(int msqids[], int cnum, int chossen_cashier);
void DisplayCustomerData();

#define SEM_NAME "/quantity"

int main(int argc, char *argv[])
{
    readInputDataFromFile();

    /*==================================== Shared Memory for customers ====================================*/
    sem_t *semaphore = sem_open(SEM_KEY_ITEMS, O_CREAT, 0666, 1);
    int shmid = createSharedMemory(sizeof(Point)*anumberOfCustomers, SHM_KEY_CUSTOMERS_UI); // Create shared memory segment
    if (shmid == -1)
    {
        printf("Error creating Customers Points shared memory\n");
        return 1;
    }

    sharedMemory = (Point *)attachSharedMemory(shmid); // Attach shared memory
    if (sharedMemory == NULL)
    {
        printf("Error attaching Customers Points shared memory\n");
        return 2;
    }



    int customer_number = atoi(argv[1]);
    int msqids[anumberOfCashiers];
    int chossen_cashier = 0;
    char msg[200];
    int wait_time_factor = 4;
    int behavior_factor = 1;    

    sprintf(msg, "c%s", argv[1]);

    int shmids[anumberOfCashiers];
    CashierData *cashierData[anumberOfCashiers];

    // get all message queues IDs
    char * token = strtok(argv[2], " ");
    int i=0;
    while( token != NULL ) {
        msqids[i] = atoi(token);
        token = strtok(NULL, " ");
        i++;
    }

    for (int i = 0; i < anumberOfCashiers; i++)
        shmids[i] = createSharedMemory(sizeof(CashierData), SHM_KEY_BASE + i);

    for (int i = 0; i < anumberOfCashiers; i++) {     
        // Attach to the shared memory segment
        cashierData[i] = (CashierData *)attachSharedMemory(shmids[i]);
    }

    clock_t start, end;
    double cpu_time_used;

    double total_waiting=0 ;
    while (1) {        
        
        move_index_x_y_delay(customer_number, 0, -60, 0);//! go to the enter
        // while(is_sharedMemory_equal_current_Points()!=true);
        sleep(10); //! Needed time from Enter to reach center
        move_index_x_y_delay(customer_number, 0, 0, 0); //! go to shopping area
        sleep(2);

        int shopping_time = random_range(ashopping_time_min, ashopping_time_max);
        sleep(shopping_time); //! shopping time

        chossen_cashier = random_range_with_randomness(0, anumberOfCashiers-1, i++);//! choose a cashier

        int cashier_queue_size = cashierData[chossen_cashier]->number_of_served_customers;
        move_index_x_y_delay(customer_number, 125, -20*(chossen_cashier+1), 0); //! go to the cashier

        printf("customer%d to cashier%d\n------------------------------------------------------\n", customer_number, chossen_cashier); // testing
        
        
        sleep(8); //! time to go to the cashier 
        
        

        fflush(stdout);

        chooseRandomItems(msqids, customer_number, chossen_cashier); // choose random items and send the order to a cashier

        /*TODO 
            [] if customer wait more than patient_threshold in queue -> leave the queue + leave simulation + increment the customers_leave_impatiently
            [x]: if Cashier drop behaivoral under behaivoral_threshold -> (continue)
        */
        start = clock();
        bool check_if_cashier_behaivor_drop = false;
        while (cashierData[chossen_cashier]->client_being_served != customer_number){
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            if(cpu_time_used > acustomer_impatience_threshold || total_waiting > acustomer_impatience_threshold){
                printf("!!!customer%d left the queue(impatient!!!)\n", customer_number);
                // strcpy(sharedMemory[customer_number].color, "notOrange");
                //! Leave the simulation frame
                move_index_x_y_delay(customer_number, 100, -140, 0); 
                sleep(7);
                move_index_x_y_delay(customer_number, 200, -140, 0); 
                sleep(7);

                // signal to main.c that you(customer) left the simulation
                if (kill(getppid(), SIGUSR1) == -1) {
                    perror("Signal SIGUSR1 sending failed");
                    exit(EXIT_FAILURE);
                }

                // give signal that you customer
                return 0;
            }

            if(cashierData[chossen_cashier]->cashier_behavior <= 0){
                check_if_cashier_behaivor_drop = true;
                total_waiting += cpu_time_used;
                continue;
            }
            if (check_if_cashier_behaivor_drop){
                break;
            }
        } // wait my turn
        if (check_if_cashier_behaivor_drop){
            continue;
        }
                
 
        usleep(TIME_UNIT_us/2);
        while (cashierData[chossen_cashier]->done_customer != customer_number); // wait cashier to scan youre items //! also represent the time spend on waiting line
        
        // Success Signal to main.c 
        if (kill(getppid(), SIGALRM) == -1) {
            perror("Signal sending failed");
            exit(EXIT_FAILURE);
        }

        //! Leave the simulation frame
        move_index_x_y_delay(customer_number, 100, -140, 0);       
        detachSharedMemory(sharedMemory);
    }
    
    // printf("Customer%d Leave ->\n",customer_number);
    return 0;
}

// 4- Choose random items and quantities
bool isChosen(ChosenItem chosenItems[], int chosenCount, char itemName[])
{
    for (int i = 0; i < chosenCount; ++i)
    {
        if (strcmp(chosenItems[i].itemName, itemName) == 0)
        {
            return true;
        }
    }
    return false;
}
void chooseRandomItems(int msqids[], int cnum, int chossen_cashier)
{
    /*==================================== Shared Memory & Semaphore ====================================*/
    int shmid = createSharedMemory(sizeof(Item)*MAX_NUM_ITEMS, SHM_KEY_ITEMS); // Create shared memory segment

    if (shmid == -1)
    {
        printf("Error creating items shared memory\n");
        return;
    }

    Item *shm_items = (Item *)attachSharedMemory(shmid); // Attach shared memory

    if (shm_items == NULL)
    {
        printf("Error attaching shared memory\n");
        return;
    }

    

    srand(getpid() * time(NULL));
    int totalQuantityChosen = 0;

    int Max_num_quantity = (rand() % MAX_TOTAL_QUANTITY) + 1;
    int x = MAX_NUM_ITEMS;

    int totalItemsPrice;
    char msg[100];

    ChosenItem *chosenItems = malloc(x * sizeof(ChosenItem));
    if (chosenItems == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }
    
    int chosenCount = 0;
    int attempts = 0; // Track attempts to avoid infinite loops
    //! ================================ Semaphore ============================================
    sem_t *semaphore = sem_open(SEM_NAME, O_CREAT, 0666, 1); // Open or create a named semaphore
    while (totalQuantityChosen < Max_num_quantity && chosenCount < x && attempts < x)
    {

        int availableCount = 0;
        int availableItems[x];

        for (int i = 0; i < x; ++i)
        {
            if (shm_items[i].quantity > 0 && !isChosen(chosenItems, chosenCount, shm_items[i].itemName))
            {
                availableItems[availableCount++] = i;
            }
        }

        if (availableCount == 0)
        {
            break; // No more available items, exit the loop
        }

        int randomIndex = availableItems[rand() % availableCount];

        int quantityAvailable = shm_items[randomIndex].quantity;
        int remainingQuantity = Max_num_quantity - totalQuantityChosen;

        int chosenQuantity = rand() % (quantityAvailable + 1 < remainingQuantity ? quantityAvailable + 1 : remainingQuantity);

        if (chosenQuantity == 0)
        {
            attempts++;
            continue;
        }

        if (chosenQuantity > quantityAvailable)
        {
            chosenQuantity = quantityAvailable;
        }

        sem_wait(semaphore); // Lock semaphore before modifying shared resource
        // printf("Item %s - Quantity before reduction: %d\n", shm_items[randomIndex].itemName, shm_items[randomIndex].quantity);

        if (shm_items[randomIndex].quantity >= chosenQuantity)
        {
            shm_items[randomIndex].quantity -= chosenQuantity; // Reduce item quantity
            // printf("Item %s - Quantity after reduction: %d\n", shm_items[randomIndex].itemName, shm_items[randomIndex].quantity);
        }
        else
        {
            printf("Chosen quantity exceeds available quantity.\n");
            sem_post(semaphore); // Release semaphore before exiting
            continue;            // Skip this iteration
        }

        sem_post(semaphore); // Release semaphore after modifying shared resource

        int totalprice = shm_items[randomIndex].price * chosenQuantity;

        chosenItems[chosenCount].itemName = malloc(strlen(shm_items[randomIndex].itemName) + 1);

        chosenItems[chosenCount].chosenQuantity = chosenQuantity;
        chosenItems[chosenCount].Price = shm_items[randomIndex].price;
        chosenItems[chosenCount].totalPrice = totalprice;

        // printf ("name:%s quantity:%d price:%d totsIprice:%d newquantity:%d\n",chosenItems[chosenCount].itemName,chosenItems[chosenCount].chosenQuantity,chosenItems[chosenCount].Price,chosenItems[chosenCount].totalPrice,shm_items[randomIndex].quantity);
        chosenCount++;
        totalQuantityChosen += chosenQuantity;
        attempts++;
    }

    totalItemsPrice = 0;
    for (int i = 0; i < chosenCount; ++i)
        totalItemsPrice += chosenItems[i].totalPrice;
    
    sprintf(msg, "%d %d %d", totalItemsPrice, chosenCount, cnum);

    // printf("%d\n", chosenCount);
    // fflush(stdout);

    // printf("msqids[%d] = %d, msg = %s\n", chossen_cashier, msqids[chossen_cashier], msg);
    // fflush(stdout);

    detachSharedMemory(shm_items);
    
    sendMessage(msqids[chossen_cashier], msg);

    for (int i = 0; i < chosenCount; ++i)
    {
        free(chosenItems[i].itemName);
    }

    free(chosenItems);
}