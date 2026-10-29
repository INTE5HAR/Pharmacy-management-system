#include "pharmacy.h"

void get_password(char *password, int max_len) {
    int j = 0;
    char c;
    while ((c = getch()) != '\r' && c != '\n') {
        if (c == '\b') { // Handle backspace properly
            if (j > 0) {
                j--;
                printf("\b \b");
            }
        } else if (j < max_len - 1 && c >= 32 && c <= 126) {
            password[j++] = c;
            printf("*");
        }
    }
    password[j] = '\0';
    printf("\n");
}

void setup_initial_users(void) {
    struct account users[3];
    FILE *fp = fopen("users.bin", "wb");
    if (!fp) {
        printf("\nError creating users.bin!\n");
        return;
    }

    printf("\n--- INITIAL PHARMACIST ACCOUNT SETUP ---\n");
    printf("No pharmacist accounts found. Please register up to 3 accounts.\n\n");
    for (int i = 0; i < 3; i++) {
        printf("Enter username %d: ", i + 1);
        scanf("%19s", users[i].username);
        clear_buffer();

        printf("Enter password %d: ", i + 1);
        get_password(users[i].password, 20);

        fwrite(&users[i], sizeof(struct account), 1, fp);
    }
    fclose(fp);
    printf("\nAccounts saved successfully! Please log in.\n");
    Sleep(1500);
}

int login(void) {
    struct account usr;
    FILE *fp = fopen("users.bin", "rb");

    //check if users.bin is missing or empty
    if (fp == NULL) {
        setup_initial_users();
        fp = fopen("users.bin", "rb");
    } else {
        fseek(fp, 0, SEEK_END);
        if (ftell(fp) == 0) {
            fclose(fp);
            setup_initial_users();
            fp = fopen("users.bin", "rb");
        } else {
            rewind(fp);
        }
    }

    char username[20], password[20];
    printf("\n--- PHARMACIST LOGIN ---\n");
    printf("Enter username: ");
    scanf("%19s", username);
    clear_buffer();

    printf("Enter password: ");
    get_password(password, 20);

    int flag = 0;
    while (fread(&usr, sizeof(struct account), 1, fp) == 1) {
        if (strcmp(usr.username, username) == 0 && strcmp(usr.password, password) == 0) {
            flag = 1;
            break;
        }
    }
    fclose(fp);

    if (flag) {
        printf("\nLogin successful!\n");
        Sleep(1000);
        return 1;
    } else {
        printf("\nInvalid username or password!\n");
        Sleep(1500);
        return 0;
    }
}

void change_login(void) {
    int choice;
    printf("\n1. Set a single new admin login\n");
    printf("2. Setup 3 pharmacist accounts (Reset all)\n");
    printf(">> ");
    scanf("%d", &choice);
    clear_buffer();

    if (choice == 2) {
        setup_initial_users();
        return;
    }

    struct account usr;
    FILE *fp = fopen("users.bin", "wb");
    if (!fp) {
        printf("Error opening users.bin!\n");
        return;
    }

    printf("Enter new username: ");
    scanf("%19s", usr.username);
    clear_buffer();

    printf("Enter new password: ");
    get_password(usr.password, 20);

    fwrite(&usr, sizeof(struct account), 1, fp);
    fclose(fp);
    printf("\nLogin credentials updated successfully!\n");
}
