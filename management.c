#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store contact information

struct Contact {
    char name[50];
    char phone[20];
    char email[60];
    char address[100];
};

// Function declarations

void addContact();
void viewContacts();
void searchContact();
void editContact();
void deleteContact();
int phoneExists(char phone[]);
void clearInputBuffer();


// Main Function

int main() {

    int choice;

    while (1) {

        printf("\n====================================\n");
        printf("       CONTACT MANAGEMENT SYSTEM\n");
        printf("====================================\n");

        printf("1. Add Contact\n");
        printf("2. View All Contacts\n");
        printf("3. Search Contact\n");
        printf("4. Edit Contact\n");
        printf("5. Delete Contact\n");
        printf("6. Exit\n");

        printf("------------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();

        switch (choice) {

            case 1:
                addContact();
                break;

            case 2:
                viewContacts();
                break;

            case 3:
                searchContact();
                break;

            case 4:
                editContact();
                break;

            case 5:
                deleteContact();
                break;

            case 6:
                printf("\nThank you for using Contact Management System!\n");
                return 0;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    }

    return 0;
}


// Clear input buffer

void clearInputBuffer() {

    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF);

}


// Check duplicate phone number

int phoneExists(char phone[]) {

    FILE *fp;

    struct Contact c;

    fp = fopen("contacts.dat", "rb");

    if (fp == NULL) {
        return 0;
    }

    while (fread(&c, sizeof(struct Contact), 1, fp) == 1) {

        if (strcmp(c.phone, phone) == 0) {

            fclose(fp);
            return 1;

        }

    }

    fclose(fp);

    return 0;
}


// 1. ADD CONTACT

void addContact() {

    FILE *fp;

    struct Contact c;

    printf("\n========== ADD CONTACT ==========\n");

    printf("Enter Name: ");
    if (scanf(" %49[^\n]", c.name) != 1) return;
    clearInputBuffer();

    printf("Enter Phone Number: ");
    if (scanf(" %19[^\n]", c.phone) != 1) return;
    clearInputBuffer();

    if (strlen(c.phone) == 0) {
        printf("Phone number cannot be empty!\n");
        return;
    }

    if (phoneExists(c.phone)) {

        printf("\nContact with this phone number already exists!\n");
        return;

    }

    printf("Enter Email: ");
    if (scanf(" %59[^\n]", c.email) != 1) return;
    clearInputBuffer();

    printf("Enter Address: ");
    if (scanf(" %99[^\n]", c.address) != 1) return;
    clearInputBuffer();

    fp = fopen("contacts.dat", "ab");

    if (fp == NULL) {

        printf("\nError opening file!\n");
        return;

    }

    fwrite(&c, sizeof(struct Contact), 1, fp);

    fclose(fp);

    printf("\nContact added successfully!\n");

}


// 2. VIEW ALL CONTACTS

void viewContacts() {

    FILE *fp;

    struct Contact c;

    int count = 0;

    fp = fopen("contacts.dat", "rb");

    if (fp == NULL) {

        printf("\nNo contacts found!\n");
        return;

    }

    printf("\n========== ALL CONTACTS ==========\n");

    while (fread(&c, sizeof(struct Contact), 1, fp) == 1) {

        count++;

        printf("\nContact %d\n", count);

        printf("Name    : %s\n", c.name);
        printf("Phone   : %s\n", c.phone);
        printf("Email   : %s\n", c.email);
        printf("Address : %s\n", c.address);

        printf("----------------------------------\n");

    }

    if (count == 0) {
        printf("No contacts available!\n");
    }

    printf("\nTotal Contacts: %d\n", count);

    fclose(fp);

}


// 3. SEARCH CONTACT

void searchContact() {

    FILE *fp;

    struct Contact c;

    char search[50];

    int found = 0;

    printf("\n========== SEARCH CONTACT ==========\n");

    printf("Enter Name or Phone Number: ");

    if (scanf(" %49[^\n]", search) != 1) return;
    clearInputBuffer();

    fp = fopen("contacts.dat", "rb");

    if (fp == NULL) {

        printf("\nNo contacts found!\n");
        return;

    }

    while (fread(&c, sizeof(struct Contact), 1, fp) == 1) {

        if (strcmp(c.name, search) == 0 ||
            strcmp(c.phone, search) == 0) {

            printf("\nContact Found!\n");

            printf("Name    : %s\n", c.name);
            printf("Phone   : %s\n", c.phone);
            printf("Email   : %s\n", c.email);
            printf("Address : %s\n", c.address);

            found = 1;

        }

    }

    if (!found) {
        printf("\nContact not found!\n");
    }

    fclose(fp);

}


// 4. EDIT CONTACT

void editContact() {

    FILE *fp;
    struct Contact c;
    char phone[20];
    char originalPhone[20];
    int found = 0;
    int updated = 0;

    printf("\n========== EDIT CONTACT ==========\n");
    printf("Enter Phone Number of Contact to Edit: ");

    if (scanf(" %19[^\n]", phone) != 1) return;
    clearInputBuffer();

    fp = fopen("contacts.dat", "rb+");
    if (fp == NULL) {
        printf("\nNo contacts found!\n");
        return;
    }

    while (fread(&c, sizeof(struct Contact), 1, fp) == 1) {
        if (strcmp(c.phone, phone) == 0) {
            found = 1;
            strcpy(originalPhone, c.phone);

            printf("\nContact Found! Enter New Details.\n");

            printf("Enter New Name: ");
            if (scanf(" %49[^\n]", c.name) != 1) break;
            clearInputBuffer();

            printf("Enter New Phone Number: ");
            if (scanf(" %19[^\n]", c.phone) != 1) break;
            clearInputBuffer();

            if (strcmp(c.phone, originalPhone) != 0 && phoneExists(c.phone)) {
                printf("\nAnother contact already uses this phone number!\n");
                break;
            }

            printf("Enter New Email: ");
            if (scanf(" %59[^\n]", c.email) != 1) break;
            clearInputBuffer();

            printf("Enter New Address: ");
            if (scanf(" %99[^\n]", c.address) != 1) break;
            clearInputBuffer();

            if (fseek(fp, -(long)sizeof(struct Contact), SEEK_CUR) == 0 &&
                fwrite(&c, sizeof(struct Contact), 1, fp) == 1) {
                updated = 1;
            }
            break;
        }
    }

    fclose(fp);

    if (updated) {
        printf("\nContact updated successfully!\n");
    } else if (!found) {
        printf("\nContact not found!\n");
    }
}


// 5. DELETE CONTACT

void deleteContact() {

    FILE *fp, *temp;

    struct Contact c;

    char phone[20];

    int found = 0;

    printf("\n========== DELETE CONTACT ==========\n");

    printf("Enter Phone Number to Delete: ");

    if (scanf(" %19[^\n]", phone) != 1) return;
    clearInputBuffer();

    fp = fopen("contacts.dat", "rb");

    if (fp == NULL) {

        printf("\nNo contacts found!\n");
        return;

    }

    temp = fopen("temp.dat", "wb");

    if (temp == NULL) {

        printf("\nError creating temporary file!\n");
        fclose(fp);
        return;

    }

    while (fread(&c, sizeof(struct Contact), 1, fp) == 1) {

        if (strcmp(c.phone, phone) == 0) {

            found = 1;

        } else {

            fwrite(&c, sizeof(struct Contact), 1, temp);

        }

    }

    fclose(fp);
    fclose(temp);

    if (found) {

        if (remove("contacts.dat") != 0 ||
            rename("temp.dat", "contacts.dat") != 0) {

            printf("\nError updating contact file!\n");

        } else {

            printf("\nContact deleted successfully!\n");

        }

    } else {

        remove("temp.dat");

        printf("\nContact not found!\n");

    }

}