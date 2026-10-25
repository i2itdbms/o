/* Program 1: Address Book (file handling with structures)
 * Options: Create, View, Insert, Delete, Modify, Exit
 * Records are stored as fixed-size binary structs in "addressbook.dat".
 * Compile: gcc 01_address_book.c -o address_book
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "addressbook.dat"
#define TEMP_NAME "addressbook.tmp"
#define NAME_LEN  50
#define PHONE_LEN 15
#define EMAIL_LEN 50

typedef struct {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
} Record;

/* Reads one line safely (no scanf problems). Returns 0 on EOF. */
static int read_line(const char *prompt, char *buf, size_t size)
{
    printf("%s", prompt);
    if (fgets(buf, (int)size, stdin) == NULL) {
        buf[0] = '\0';
        return 0;
    }
    buf[strcspn(buf, "\n")] = '\0';
    return 1;
}

/* Returns index of record with this name, or -1 if not found / no file. */
static int find_index(const char *name)
{
    FILE *fp = fopen(FILE_NAME, "rb");
    Record r;
    int idx = 0;

    if (fp == NULL)
        return -1;
    while (fread(&r, sizeof(Record), 1, fp) == 1) {
        if (strcmp(r.name, name) == 0) {
            fclose(fp);
            return idx;
        }
        idx++;
    }
    fclose(fp);
    return -1;
}

static void create_book(void)
{
    FILE *fp = fopen(FILE_NAME, "rb");
    char ans[8];

    if (fp != NULL) {
        fclose(fp);
        read_line("Address book already exists. Overwrite (all data lost)? (y/n): ", ans, sizeof ans);
        if (ans[0] != 'y' && ans[0] != 'Y') {
            printf("Cancelled.\n");
            return;
        }
    }
    fp = fopen(FILE_NAME, "wb");
    if (fp == NULL) {
        perror("fopen");
        return;
    }
    fclose(fp);
    printf("Empty address book created.\n");
}

static void view_book(void)
{
    FILE *fp = fopen(FILE_NAME, "rb");
    Record r;
    int count = 0;

    if (fp == NULL) {
        printf("Address book not found. Create it first (option 1).\n");
        return;
    }
    printf("\n%-4s %-22s %-15s %-30s\n", "No.", "Name", "Phone", "Email");
    printf("------------------------------------------------------------------------\n");
    while (fread(&r, sizeof(Record), 1, fp) == 1) {
        count++;
        printf("%-4d %-22s %-15s %-30s\n", count, r.name, r.phone, r.email);
    }
    if (count == 0)
        printf("(no records)\n");
    fclose(fp);
}

static void insert_record(void)
{
    Record r;
    FILE *fp;

    read_line("Name  : ", r.name, sizeof r.name);
    if (r.name[0] == '\0') {
        printf("Name cannot be empty.\n");
        return;
    }
    if (find_index(r.name) >= 0) {
        printf("A record with this name already exists.\n");
        return;
    }
    read_line("Phone : ", r.phone, sizeof r.phone);
    read_line("Email : ", r.email, sizeof r.email);

    fp = fopen(FILE_NAME, "ab");   /* append; creates file if missing */
    if (fp == NULL) {
        perror("fopen");
        return;
    }
    fwrite(&r, sizeof(Record), 1, fp);
    fclose(fp);
    printf("Record inserted.\n");
}

static void delete_record(void)
{
    char name[NAME_LEN];
    Record r;
    FILE *fp, *tmp;
    int found = 0;

    fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        printf("Address book not found.\n");
        return;
    }
    read_line("Name to delete: ", name, sizeof name);
    tmp = fopen(TEMP_NAME, "wb");
    if (tmp == NULL) {
        perror("fopen");
        fclose(fp);
        return;
    }
    /* Copy every record except the one to delete into a temp file. */
    while (fread(&r, sizeof(Record), 1, fp) == 1) {
        if (strcmp(r.name, name) == 0)
            found = 1;
        else
            fwrite(&r, sizeof(Record), 1, tmp);
    }
    fclose(fp);
    fclose(tmp);

    if (found) {
        remove(FILE_NAME);
        rename(TEMP_NAME, FILE_NAME);
        printf("Record deleted.\n");
    } else {
        remove(TEMP_NAME);
        printf("Record not found.\n");
    }
}

static void modify_record(void)
{
    char name[NAME_LEN], buf[EMAIL_LEN];
    Record r;
    FILE *fp = fopen(FILE_NAME, "r+b");

    if (fp == NULL) {
        printf("Address book not found.\n");
        return;
    }
    read_line("Name to modify: ", name, sizeof name);
    while (fread(&r, sizeof(Record), 1, fp) == 1) {
        if (strcmp(r.name, name) == 0) {
            printf("Current -> Phone: %s | Email: %s\n", r.phone, r.email);
            printf("(press Enter to keep the old value)\n");
            read_line("New phone : ", buf, sizeof r.phone);
            if (buf[0] != '\0')
                strcpy(r.phone, buf);
            read_line("New email : ", buf, sizeof r.email);
            if (buf[0] != '\0')
                strcpy(r.email, buf);

            /* Go back one record, then overwrite it. */
            fseek(fp, -(long)sizeof(Record), SEEK_CUR);
            fwrite(&r, sizeof(Record), 1, fp);
            fclose(fp);
            printf("Record modified.\n");
            return;
        }
    }
    fclose(fp);
    printf("Record not found.\n");
}

int main(void)
{
    char buf[16];
    int choice;

    for (;;) {
        printf("\n===== ADDRESS BOOK =====\n");
        printf("1. Create address book\n2. View address book\n3. Insert a record\n");
        printf("4. Delete a record\n5. Modify a record\n6. Exit\n");
        if (!read_line("Enter choice: ", buf, sizeof buf))
            break;                      /* EOF on stdin -> exit */
        choice = atoi(buf);

        switch (choice) {
        case 1: create_book();   break;
        case 2: view_book();     break;
        case 3: insert_record(); break;
        case 4: delete_record(); break;
        case 5: modify_record(); break;
        case 6: printf("Bye.\n"); return 0;
        default: printf("Invalid choice.\n");
        }
    }
    return 0;
}
