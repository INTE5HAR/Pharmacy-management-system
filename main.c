//made by Inteshar Alam
//date > 11-Dec-2023
//GitHub: github.com/INTE5HAR

#include "pharmacy.h"

void clear_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            clear_buffer();
        }
    }
}

int main(void) {
    char userinput;

    while (1) {
        system("cls");
        system("Color 0A");
        printf("============================================================\n");
        printf("                         WELCOME!\n");
        printf("                     ...MINI PHARMACY...\n");
        printf("              All your medicines in one place!!!\n");
        printf("============================================================\n");
        printf("\nAre you the pharmacist or a customer? (p/c, or 'q' to quit): ");

        if (scanf(" %c", &userinput) != 1) {
            clear_buffer();
            continue;
        }
        clear_buffer();

        switch (userinput) {
            case 'p':
            case 'P':
                if (login()) {
                    pharmacist_menu();
                }
                break;
            case 'c':
            case 'C':
                customer_menu();
                break;
            case 'q':
            case 'Q':
                printf("\nThank you for using Mini Pharmacy! Goodbye.\n");
                return 0;
            default:
                printf("\033[31m");
                printf("\nInvalid input! Returning to main menu...\n");
                Sleep(1500);
                break;
        }
    }
    return 0;
}