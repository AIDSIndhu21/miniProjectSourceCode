// Bank-account program reads a random-access file sequentially,
// updates data already written to the file, creates new data to
// be placed in the file, and deletes data previously in the file.
#include <stdio.h>
#include <stdlib.h>
// clientData structure definition
struct clientData
{
    unsigned int acctNum; // account number
    char lastName[15];    // account last name
    char firstName[10];   // account first name
    double balance;       // account balance
}; // end structure clientData

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

int main(int argc, char *argv[])
{
    FILE *cfPtr;         // credit.dat file pointer
    unsigned int choice; // user's choice

    // fopen opens the file; exits if file cannot be opened
    if ((cfPtr = fopen("credit.dat", "wb+")) == NULL)
    {
        printf("%s: File could not be opened.\n", argv[0]);
        exit(-1);
    }

    // enable user to specify action
    while ((choice = enterChoice()) != 5)
    {
        switch (choice)
        {
        // create text file from record file
        case 1:
            textFile(cfPtr);
            break;
        // update record
        case 2:
            updateRecord(cfPtr);
            break;
        // create record
        case 3:
            newRecord(cfPtr);
            break;
        // delete existing record
        case 4:
            deleteRecord(cfPtr);
            break;
        // display if user does not select valid choice
        default:
            puts("Incorrect choice");
            break;
        } // end switch
    } // end while

    fclose(cfPtr); // fclose closes the file
} // end main

// create formatted text file for printing
// create formatted text file for printing
void textFile(FILE *readPtr)
{
    FILE *writePtr; // accounts.txt file pointer
    struct clientData client = {0, "", "", 0.0};
    unsigned int i; // Loop variable for 100 accounts

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
    } 
    else
    {
        rewind(readPtr); // sets pointer to beginning of file
        
        // Headers writing to file and screen
        fprintf(writePtr, "%-10s%-16s%-11s%10s\n", "Acct", "First Name", "Last Name", "Balance");
        printf("\n%-10s%-16s%-11s%10s\n", "Acct", "First Name", "Last Name", "Balance");
        printf("--------------------------------------------------\n");

        // COLLEGE LOGIC: Simply loop from record 1 to 100 sequentially
        for (i = 1; i <= 100; i++)
        {
            // Read each record one by one
            fread(&client, sizeof(struct clientData), 1, readPtr);

            // If account has data, print with names
            if (client.acctNum != 0)
            {
                fprintf(writePtr, "%-10u%-16s%-11s%10.2f\n", client.acctNum, client.firstName, client.lastName, client.balance);
                printf("%-10u%-16s%-11s%10.2f\n", client.acctNum, client.firstName, client.lastName, client.balance);
            }
            // If account is blank, print just the account number and 0.00
            else
            {
                fprintf(writePtr, "%-10u%-16s%-11s%10.2f\n", i, "", "", 0.00);
                printf("%-10u%-16s%-11s%10.2f\n", i, "", "", 0.00);
            }
        } // end for

        fclose(writePtr); 
        printf("--------------------------------------------------\n");
        printf("Your accounts.txt created successfully.\n");
    } // end else
} // end function textFile

// update balance in record
void updateRecord(FILE *fPtr)
{
    unsigned int account; // account number
    double transaction;   // transaction amount
    // create clientData with no information
    struct clientData client = {0, "", "", 0.0};

    // obtain number of account to update
    printf("%s", "Enter account to update ( 1 - 100 ): ");
    scanf("%d", &account);

    // move file pointer to correct record in file
    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    // read record from file
    fread(&client, sizeof(struct clientData), 1, fPtr);
    // display error if account does not exist
    if (client.acctNum == 0)
    {
        printf("Account #%d has no information.\n", account);
    }
    else
    {
        printf("%-6u%-16s%-11s%10.2f\n", client.acctNum, client.firstName, client.lastName, client.balance);

        // request transaction amount from user
        printf("%s", "Enter charge ( + ) or payment ( - ): ");
        scanf("%lf", &transaction);
        client.balance += transaction; // update record balance

        printf("%-6u%-16s%-11s%10.2f\n", client.acctNum, client.firstName, client.lastName, client.balance);

        // move file pointer to correct record in file
        // move back by 1 record length
        fseek(fPtr, (client.acctNum - 1) * sizeof(struct clientData), SEEK_SET);
        // write updated record over old record in file
        fwrite(&client, sizeof(struct clientData), 1, fPtr);
        printf("Account updated successfully\n");
    } // end else
} // end function updateRecord

// delete an existing record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;                       // stores record read from file
    struct clientData blankClient = {0, "", "", 0}; // blank client
    unsigned int accountNum;                        // account number

    // obtain number of account to delete
    printf("%s", "Enter account number to delete ( 1 - 100 ): ");
    scanf("%d", &accountNum);

    // move file pointer to correct record in file
    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    // read record from file
    fread(&client, sizeof(struct clientData), 1, fPtr);
    // display error if record does not exist
    if (client.acctNum == 0)
    {
        printf("Account %d does not exist.\n", accountNum);
    } // end if
    else
    { // delete record
        // move file pointer to correct record in file
        fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
        // replace existing record with blank record
        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
        printf("Accound deleted");
    } // end else
} // end function deleteRecord

// create and insert record
void newRecord(FILE *fPtr)
{
    // create clientData with default information
    struct clientData client = {0, "", "", 0.0};
    unsigned int accountNum; // account number

    // obtain number of account to create
    printf("%s", "Enter new account number ( 1 - 100 ): ");
    scanf("%d", &accountNum);

    // move file pointer to correct record in file
    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    // read record from file
    fread(&client, sizeof(struct clientData), 1, fPtr);
    // display error if account already exists
    if (client.acctNum != 0)
    {
        printf("Account #%d already contains information.\n", client.acctNum);
    } // end if
   else
    { // create record
        // user enters first name, last name and balance
        printf("%s", "Enter firstname, lastname, balance\n? ");
        
        // CHANGED: %9s first for firstName, then %14s for lastName
        scanf("%9s%14s%lf", client.firstName, client.lastName, &client.balance);

        client.acctNum = accountNum;
        // move file pointer to correct record in file
        fseek(fPtr, (client.acctNum - 1) * sizeof(struct clientData), SEEK_SET);
        // insert record in file
        fwrite(&client, sizeof(struct clientData), 1, fPtr);
        printf("\nNew account created successfully\n");
    } // end else
}

// enable user to input menu choice
unsigned int enterChoice(void)
{
    unsigned int menuChoice; // variable to store user's choice
    // display available options
    printf("%s", "\nEnter your choice\n"
                 "1 - Store a formatted text file of accounts called\n"
                 "    \"accounts.txt\" for printing\n"
                 "2 - Update an account\n"
                 "3 - Add a new account\n"
                 "4 - Delete an account\n"
                 "5 - End program\n? ");

    scanf("%u", &menuChoice); // receive choice from user
    return menuChoice;
} // end function enterChoice