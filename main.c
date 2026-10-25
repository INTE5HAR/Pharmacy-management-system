#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_LINE_LENGTH 100
//PHARMACY MANAGEMENT SYSTEM in C

struct user{
    char name[12],num[11],add[30];
}user;

struct product{
    int id,stk;
    float price;
    char nam[14],exp[11];
}item;

FILE *fpi,*fpc;

void order() {
    fpc = fopen("cart.txt", "ab");
    printf("Your name >>");
    fgets(user.name, 12, stdin);
    fflush(stdin);
    printf("Enter your number >>");
    scanf("%s", user.num);
    fflush(stdin);
    printf("Enter your address >>");
    fgets(user.add, 30, stdin);
    fflush(stdin);
    fwrite(&user, sizeof(user), 1, fpc);
    printf("\nOrder placed successfully!");
    fclose(fpc);
}



void display_cart(){
    system("cls");
    fpc=fopen("cart.txt","rb");
    while (fread(&item,sizeof(item),1,fpc)==1){
        printf("\n%d\t%s\t%.2f\t%d\t%s\n",item.id,item.nam,item.price,item.stk,item.exp);
    }
    fclose(fpc);

}

void display_inventory(){
    system("cls");
    fpi=fopen("product.txt","rb");
    printf("--------------------------INVENTORY--------------------------");
    printf("\nItem(ID)  |  Item Name  |  Item Price  |  Stock | exp date(mm/yy)\n",item.id,item.nam,item.price,item.stk,item.exp);
    while (fread(&item,sizeof(item),1,fpi)==1){
        printf("\n%d       | %s |   %.2f       |  %d   |  %s\n",item.id,item.nam,item.price,item.stk,item.exp);
    }
    fclose(fpi);
}


void del(int id){
    int f=0;
    FILE *fpit;

    fpi=fopen("product.txt","rb");
    fpit=fopen("temp.txt","wb");
    while (fread(&item,sizeof(item),1,fpi)==1){
        if (id==item.id){
            f=1;
        }
        else{
            fwrite(&item,sizeof(item),1,fpit);
        }
        if (f==1){
                printf("\nMedicine deleted successfully!");
            }
    }
    fclose(fpi);
    fclose(fpit);

    remove("product.txt");
    rename("temp.txt","product.txt");
}

void change_login(){
    struct user {
    char username[20];
    char password[20];
    }user;
    FILE *fp;
    char c;

    // Storing data in binary file
    fp = fopen("users.bin", "wb");
    printf("Enter username: ");
    scanf("%s", user.username);
    printf("Enter password: ");
    int j = 0;
    while ((c = getch()) != '\r') {
        user.password[j++] = c;
        printf("*");
    }
    user.password[j] = '\0';
    fwrite(&user, sizeof(struct user), 1, fp);
    printf("\n");
    fclose(fp);
}

void p1(){
    int i=1;
    int idd,flag=0;
    int x;
    int id,f=0;
    while (1){
        printf("\nWhat do you want to do now?\n");
        printf("1.Display product.\n");
        printf("2.Add product.\n");
        printf("3.Update product.\n");
        printf("4.Delete product.\n");
        printf("5.View order.\n");
        printf("6.Change login info.\n");
        printf("\n>> ");
        scanf("%d",&x);
        switch (x){
        case 0:
            exit (0);
            break;
        case 1:
            display_inventory();
            break;
        case 2:
            fpi=fopen("product.txt","ab");
            int n;
            printf("How many product do you want to add?\n>>");
            scanf("%d",&n);
            for (i; i <= n; i++)
            {
                printf("Enter medicine ID >> ");
                scanf("%d",&item.id);

                printf("Enter medicine name >>");
                fflush(stdin);
                gets(item.nam);

                printf("Enter medicine price >>");
                scanf("%f",&item.price);

                printf("Enter medicine quantity >>");
                scanf("%d",&item.stk);

                printf("Enter expiry date of medicine(MM/YY) >>");
                scanf("%s",item.exp);

                fwrite(&item,sizeof(item),1,fpi);
            }
            fclose(fpi);
            break;
        case 3:


            system("cls");

            printf("Enter the medicine ID of the medicine you want to update\n>>");
            scanf("%d",&id);

            fpi=fopen("product.txt","rb+");
            while (fread(&item,sizeof(item),1,fpi)==1){
                if (id==item.id){
                    f=1;
                    printf("Enter new medicine name >>");
                    fflush(stdin);
                    gets(item.nam);

                    printf("Enter new medicine price >>");
                    scanf("%f",&item.price);

                    printf("Enter new medicine quantity >>");
                    scanf("%d",&item.stk);

                    printf("Enter new expiry date of medicine(MM/YY) >>");
                    scanf("%s",item.exp);

                    fseek(fpi, -(long)sizeof(item), SEEK_CUR);
                    fwrite(&item,sizeof(item),1,fpi);
                    break;
                }
            }
            fclose(fpi);

            if (f==1){
                printf("\nMedicine information updated successfully!");
            }
            else{
                printf("\nMedicine information not found!");
            }
            break;
        case 4:


            system("cls");

            printf("Enter the medicine ID of the medicine you want to delete\n>>");
            scanf("%d",&idd);
            fpi=fopen("product.txt","rb");
            while (fread(&item,sizeof(item),1,fpi)==1){
                if (idd==item.id){
                    flag=1;
                    fclose(fpi);
                    break;
                }
            }
            if (flag==1){
                printf("\nMedicine deleted succesfully!");
                del(idd);
            }
            else{
                printf("\nMedicine not found!");
            }
            break;
        case 5:
            display_cart();
            break;
        case 6:
            change_login();
            break;
        default:
            printf("Invalid Input!");
            break;
        }
    }
}


void pharmacist(){
     printf("                     WELCOME!"
         "\n               ...MINI PHARMACY..."
         "\n          All your medicines in one place!!!\n");
    printf("\n\t         Welcome, Pharmacist!\n");
    p1();
}

void login(){
    struct user {
    char username[20];
    char password[20];
    }users;
    FILE *fp;
    int flag = 0,j = 0;
    char username[20], password[20], c;
    printf("\nEnter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    while ((c = getch()) != '\r') {
        password[j++] = c;
        printf("*");
    }
    password[j] = '\0';
    printf("\n");
    if ((fp = fopen("users.bin", "rb")) == NULL) {
        fp = fopen("users.bin", "wb");
        fclose(fp);
        fp = fopen("users.bin", "rb");
    }
    while (fread(&users, sizeof(struct user), 1, fp)) {
        if (strcmp(users.username, username) == 0 && strcmp(users.password, password) == 0) {
            printf("\nLogin successful!\n");
            flag = 1;
            Sleep(1500);
            system("cls");
            pharmacist();
            break;
        }
    }
    if (flag == 0) {
        printf("Invalid username or password.\n");
    }
    fclose(fp);
}


/*
void edit_cart(){
    int i=1;
    int idd,flag=0;
    int x;
    int id,f=0;
    while (1){
        printf("\nWhat do you want to do now?\n");
        printf("1.Add product.\n");
        printf("2.Delete product.\n");
        printf("\n>> ");
        scanf("%d",&x);
        switch (x){
        case 0:
            exit (0);
            break;
        case 1:

            break;
        case 2:
            system("cls");

            printf("Enter the medicine ID of the medicine you want to delete\n>>");
            scanf("%d",&idd);
            fpi=fopen("cart.txt","rb");
            while (fread(&item,sizeof(item),1,fpi)==1){
                if (idd==item.id){
                    flag=1;
                    fclose(fpi);
                    break;
                }
            }
            if (flag==1){
                printf("\nMedicine deleted succesfully!");
                dele(idd);
            }
            else{
                printf("\nMedicine not found!");
            }
            break;
        case 3:


            system("cls");

            printf("Enter the medicine ID of the medicine you want to update\n>>");
            scanf("%d",&id);

            fpi=fopen("product.txt","rb+");
            while (fread(&item,sizeof(item),1,fpi)==1){
                if (id==item.id){
                    f=1;
                    printf("Enter new medicine name >>");
                    fflush(stdin);
                    gets(item.nam);

                    printf("Enter new medicine price >>");
                    scanf("%f",&item.price);

                    printf("Enter new medicine quantity >>");
                    scanf("%d",&item.stk);

                    printf("Enter new expiry date of medicine(MM/YY) >>");
                    scanf("%s",item.exp);

                    fseek(fpi, -(long)sizeof(item), SEEK_CUR);
                    fwrite(&item,sizeof(item),1,fpi);
                    break;
                }
            }
            fclose(fpi);

            if (f==1){
                printf("\nMedicine information updated successfully!");
            }
            else{
                printf("\nMedicine information not found!");
            }
            break;
        case 4:


            system("cls");

            printf("Enter the medicine ID of the medicine you want to delete\n>>");
            scanf("%d",&idd);
            fpi=fopen("product.txt","rb");
            while (fread(&item,sizeof(item),1,fpi)==1){
                if (idd==item.id){
                    flag=1;
                    fclose(fpi);
                    break;
                }
            }
            if (flag==1){
                printf("\nMedicine deleted succesfully!");
                del(idd);
            }
            else{
                printf("\nMedicine not found!");
            }
            break;
        case 5:
            display_cart();
            break;
        case 6:
            change_login();
            break;
        default:
            printf("Invalid Input!");
            break;
        }
    }
}
*/


void c1(){
    int n;
    printf("\nWhat would you like to buy?");
    fpi=fopen("product.txt","rb");
    printf("--------------------------INVENTORY--------------------------");
    printf("\nItem(ID)  |  Item Name  |  Item Price  |  Stock | exp date(mm/yy)\n",item.id,item.nam,item.price,item.stk,item.exp);
    while (fread(&item,sizeof(item),1,fpi)==1){
        printf("\n%d       | %s |   %.2f       |  %d   |  %s\n",item.id,item.nam,item.price,item.stk,item.exp);
    }
    fclose(fpi);
    int id, f = 0;
    printf("How many medicine do you want to buy?\n>>");
    scanf("%d", &n);
    for (int i=0; i<n; i++){
    printf("Enter the medicine ID of the medicine you want to buy\n>>");
    scanf("%d", &id);

    fpi = fopen("product.txt", "rb+");
    fpc = fopen("cart.txt", "a");
    while (fread(&item, sizeof(item), 1, fpi) == 1) {
        if (id == item.id) {
            f = 1;

            printf("How many medicine quantity >>");
            scanf("%d", &item.stk);

            fwrite(&item, sizeof(item), 1, fpc);
            break;
        }
    }
    }
    fclose(fpi);
    fclose(fpc);
    //c2();
}

void customer(){
    printf("\n\t  Welcome, Customer!\n");
    c1();
}

int main(){
    system("Color 0A");
    char userinput;
    printf("                     WELCOME!"
         "\n               ...MINI PHARMACY..."
         "\n          All your medicines in one place!!!");
    printf("\n\nAre you the pharmacist or a customer?(p/c): ");
    scanf(" %c", &userinput);

    switch (userinput){
        case 'p':
            login();
            break;
        case 'c':
            customer();
            break;
        default:
            printf("\033[31m");
            printf("\n\nInvalid input! Please try again after 5 seconds.\n");
            Sleep(4000);
            system("cls");
            main();
    }
    return 0;
}
