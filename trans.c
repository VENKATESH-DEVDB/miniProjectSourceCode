
// ==============================
// BANK ACCOUNT MANAGEMENT SYSTEM
// ==============================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUPER_PASSWORD "admin123"
#define MINIMUM_BALANCE 500.0

// ==============================
// STRUCTURE DEFINITION
// ==============================

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    char passKey[20];
    double balance;
};

// ==============================
// FUNCTION PROTOTYPES
// ==============================

unsigned int enterChoice(void);

void textFile(FILE *readPtr);

void updateRecord(FILE *fPtr);

void newRecord(FILE *fPtr);

void deleteRecord(FILE *fPtr);

// ==============================
// MAIN FUNCTION
// ==============================

int main(int argc, char *argv[])
{
    FILE *cfPtr;

    unsigned int choice;

    // open binary file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("\nERROR : File could not be opened.\n");

        exit(-1);
    }

    // menu loop
    while ((choice = enterChoice()) != 5)
    {
        switch (choice)
        {
        // administrator section
        case 1:
            textFile(cfPtr);
            break;

        // update account
        case 2:
            updateRecord(cfPtr);
            break;

        // create new account
        case 3:
            newRecord(cfPtr);
            break;

        // delete account
        case 4:
            deleteRecord(cfPtr);
            break;

        default:
            printf("\nInvalid choice.\n");
            break;
        }
    }

    fclose(cfPtr);

    printf("\nProgram terminated successfully.\n");

    return 0;
}

// ==============================
// ADMINISTRATOR : EXPORT ACCOUNTS
// ==============================

void textFile(FILE *readPtr)
{
    FILE *writePtr;

    int result;

    char superPass[20];

    struct clientData client = {0, "", "", "", 0.0};

    printf("\n========================================\n");
    printf("        ADMINISTRATOR ACCESS\n");
    printf("========================================\n");

    printf("Enter administrator password : ");

    scanf("%19s", superPass);

    // password validation
    if (strcmp(superPass, SUPER_PASSWORD) != 0)
    {
        printf("\nACCESS DENIED.\n");

        return;
    }

    // create text file
    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("\nUnable to create accounts.txt\n");
    }
    else
    {
        rewind(readPtr);

        fprintf(writePtr,
                "========================================================================\n");

        fprintf(writePtr,
                "%-8s%-16s%-16s%-16s%-10s\n",
                "Acct",
                "Last Name",
                "First Name",
                "PassKey",
                "Balance");

        fprintf(writePtr,
                "========================================================================\n");

        while ((result = fread(&client,
                               sizeof(struct clientData),
                               1,
                               readPtr)) == 1)
        {
            if (client.acctNum != 0)
            {
                fprintf(writePtr,
                        "%-8d%-16s%-16s%-16s%-10.2f\n",
                        client.acctNum,
                        client.lastName,
                        client.firstName,
                        client.passKey,
                        client.balance);
            }
        }

        fprintf(writePtr,
                "========================================================================\n");

        fclose(writePtr);

        printf("\naccounts.txt created successfully.\n");
    }
}

// ==============================
// UPDATE ACCOUNT
// ==============================

void updateRecord(FILE *fPtr)
{
    unsigned int account;

    double transaction;

    char enteredPass[20];

    struct clientData client = {0, "", "", "", 0.0};

    printf("\n========================================\n");
    printf("           UPDATE ACCOUNT\n");
    printf("========================================\n");

    printf("Enter account number (1 - 100) : ");

    scanf("%d", &account);

    // validation
    if (account < 1 || account > 100)
    {
        printf("\nInvalid account number.\n");

        return;
    }

    // move pointer
    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    // read record
    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    // check existence
    if (client.acctNum == 0)
    {
        printf("\nAccount does not exist.\n");
    }
    else
    {
        printf("\nEnter passkey : ");

        scanf("%19s", enteredPass);

        // authentication
        if (strcmp(client.passKey, enteredPass) != 0)
        {
            printf("\nAuthentication failed.\n");

            return;
        }

        // display current details
        printf("\n----------------------------------------\n");

        printf("Account Number : %d\n",
               client.acctNum);

        printf("Last Name      : %s\n",
               client.lastName);

        printf("First Name     : %s\n",
               client.firstName);

        printf("Current Balance: %.2f\n",
               client.balance);

        printf("----------------------------------------\n");

        // transaction
        printf("\nEnter charge (+) or payment (-) : ");

        scanf("%lf", &transaction);

        // minimum balance validation
        if ((client.balance + transaction) < MINIMUM_BALANCE)
        {
            printf("\nTransaction denied.\n");

            printf("Minimum balance %.2f must be maintained.\n",
                   MINIMUM_BALANCE);

            return;
        }

        client.balance += transaction;

        // updated details
        printf("\nUpdated Balance : %.2f\n",
               client.balance);

        // move back
        fseek(fPtr,
              -sizeof(struct clientData),
              SEEK_CUR);

        // write updated data
        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("\nAccount updated successfully.\n");
    }
}

// ==============================
// DELETE ACCOUNT
// ==============================

void deleteRecord(FILE *fPtr)
{
    struct clientData client;

    struct clientData blankClient = {0, "", "", "", 0.0};

    unsigned int accountNum;

    char enteredPass[20];

    printf("\n========================================\n");
    printf("           DELETE ACCOUNT\n");
    printf("========================================\n");

    printf("Enter account number (1 - 100) : ");

    scanf("%d", &accountNum);

    // validation
    if (accountNum < 1 || accountNum > 100)
    {
        printf("\nInvalid account number.\n");

        return;
    }

    // move pointer
    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    // read record
    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    // existence check
    if (client.acctNum == 0)
    {
        printf("\nAccount does not exist.\n");
    }
    else
    {
        printf("\nEnter passkey : ");

        scanf("%19s", enteredPass);

        // authentication
        if (strcmp(client.passKey, enteredPass) != 0)
        {
            printf("\nAuthentication failed.\n");

            return;
        }

        // move pointer
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        // overwrite blank record
        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("\nAccount deleted successfully.\n");
    }
}

// ==============================
// CREATE NEW ACCOUNT
// ==============================

void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", "", 0.0};

    unsigned int accountNum;

    printf("\n========================================\n");
    printf("          CREATE NEW ACCOUNT\n");
    printf("========================================\n");

    printf("Enter new account number (1 - 100) : ");

    scanf("%d", &accountNum);

    // validation
    if (accountNum < 1 || accountNum > 100)
    {
        printf("\nInvalid account number.\n");

        return;
    }

    // move pointer
    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    // read record
    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    // account existence validation
    if (client.acctNum != 0)
    {
        printf("\nAccount already exists.\n");
    }
    else
    {
        printf("\nEnter details\n");

        printf("----------------------------------------\n");

        printf("Last Name       : ");
        scanf("%14s", client.lastName);

        printf("First Name      : ");
        scanf("%9s", client.firstName);

        printf("Opening Balance : ");
        scanf("%lf", &client.balance);

        // minimum opening balance validation
        if (client.balance < MINIMUM_BALANCE)
        {
            printf("\nMinimum balance required is %.2f\n",
                   MINIMUM_BALANCE);

            return;
        }

        printf("Create Passkey  : ");
        scanf("%19s", client.passKey);

        printf("----------------------------------------\n");

        client.acctNum = accountNum;

        // move pointer
        fseek(fPtr,
              (client.acctNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        // write record
        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("\nAccount created successfully.\n");
    }
}

// ==============================
// MENU SECTION
// ==============================

unsigned int enterChoice(void)
{
    unsigned int menuChoice;

    printf("\n");
    printf("========================================\n");
    printf("     BANK ACCOUNT MANAGEMENT SYSTEM\n");
    printf("========================================\n");

    printf("            ADMINISTRATOR\n");
    printf("----------------------------------------\n");
    printf("1. Export all account details\n\n");

    printf("               USER\n");
    printf("----------------------------------------\n");
    printf("2. Update an account\n");
    printf("3. Add a new account\n");
    printf("4. Delete an account\n");
    printf("5. Exit program\n");

    printf("========================================\n");

    printf("Enter your choice : ");

    scanf("%u", &menuChoice);

    return menuChoice;
}

