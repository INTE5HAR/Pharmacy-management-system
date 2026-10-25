#include <stdio.h>
#include <string.h>
#include <conio.h>
//set username and password to login.
struct user {
    char username[20];
    char password[20];
};

int main() {
    struct user users[3];
    FILE *fp;
    int i;
    char c;

    // Storing data in binary file
    fp = fopen("users.bin", "wb");
    for (i = 0; i < 3; i++) {
        printf("Enter username %d: ", i + 1);
        scanf("%s", users[i].username);
        printf("Enter password %d: ", i + 1);
        int j = 0;
        while ((c = getch()) != '\r') {
            users[i].password[j++] = c;
            printf("*");
        }
        users[i].password[j] = '\0';
        fwrite(&users[i], sizeof(struct user), 1, fp);
        printf("\n");
    }
    fclose(fp);
    return 0;
}


