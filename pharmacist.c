#include "pharmacy.h"

void display_inventory(void) {
    struct product item;
    FILE *fpi = fopen("product.txt", "rb");

    printf("\n============================== INVENTORY ==============================\n");
    printf("%-8s | %-20s | %-10s | %-8s | %-10s\n", "ID", "Medicine Name", "Price", "Stock", "Exp Date");
    printf("-----------------------------------------------------------------------\n");

    if (fpi == NULL) {
        printf("Inventory is currently empty.\n");
        printf("=======================================================================\n");
        return;
    }

    int count = 0;
    while (fread(&item, sizeof(struct product), 1, fpi) == 1) {
        printf("%-8d | %-20s | %-10.2f | %-8d | %-10s\n",
               item.id, item.nam, item.price, item.stk, item.exp);
        count++;
    }
    if (count == 0) {
        printf("No products in inventory.\n");
    }
    printf("=======================================================================\n");
    fclose(fpi);
}

void add_product(void) {
    struct product item;
    int n;

    FILE *fpi = fopen("product.txt", "ab");
    if (!fpi) {
        printf("\nError opening inventory file!\n");
        return;
    }

    printf("\nHow many products do you want to add?\n>> ");
    scanf("%d", &n);
    clear_buffer();

    for (int i = 1; i <= n; i++) {
        printf("\n--- Product %d ---\n", i);
        printf("Enter medicine ID >> ");
        scanf("%d", &item.id);
        clear_buffer();

        printf("Enter medicine name >> ");
        read_line(item.nam, sizeof(item.nam));

        printf("Enter medicine price >> ");
        scanf("%f", &item.price);
        clear_buffer();

        printf("Enter medicine quantity >> ");
        scanf("%d", &item.stk);
        clear_buffer();

        printf("Enter expiry date (MM/YY) >> ");
        scanf("%11s", item.exp);
        clear_buffer();

        fwrite(&item, sizeof(struct product), 1, fpi);
    }
    fclose(fpi);
    printf("\n%d product(s) added successfully!\n", n);
}

void update_product(void) {
    struct product item;
    int id, f = 0;

    display_inventory();
    printf("\nEnter the medicine ID of the medicine you want to update\n>> ");
    scanf("%d", &id);
    clear_buffer();

    FILE *fpi = fopen("product.txt", "rb+");
    if (!fpi) {
        printf("\nInventory file not found!\n");
        return;
    }

    while (fread(&item, sizeof(struct product), 1, fpi) == 1) {
        if (id == item.id) {
            f = 1;
            printf("Enter new medicine name >> ");
            read_line(item.nam, sizeof(item.nam));

            printf("Enter new medicine price >> ");
            scanf("%f", &item.price);
            clear_buffer();

            printf("Enter new medicine quantity >> ");
            scanf("%d", &item.stk);
            clear_buffer();

            printf("Enter new expiry date (MM/YY) >> ");
            scanf("%11s", item.exp);
            clear_buffer();

            fseek(fpi, -(long)sizeof(struct product), SEEK_CUR);
            fwrite(&item, sizeof(struct product), 1, fpi);
            break;
        }
    }
    fclose(fpi);

    if (f == 1) {
        printf("\nMedicine information updated successfully!\n");
    } else {
        printf("\nMedicine ID %d not found!\n", id);
    }
}

void delete_product(int id) {
    struct product item;
    int found = 0;

    FILE *fpi = fopen("product.txt", "rb");
    if (!fpi) {
        printf("\nInventory file not found!\n");
        return;
    }

    FILE *fpit = fopen("temp.txt", "wb");
    if (!fpit) {
        fclose(fpi);
        printf("\nError creating temporary file!\n");
        return;
    }

    while (fread(&item, sizeof(struct product), 1, fpi) == 1) {
        if (id == item.id) {
            found = 1;
        } else {
            fwrite(&item, sizeof(struct product), 1, fpit);
        }
    }
    fclose(fpi);
    fclose(fpit);

    remove("product.txt");
    rename("temp.txt", "product.txt");

    if (found) {
        printf("\nMedicine deleted successfully!\n");
    } else {
        printf("\nMedicine ID %d not found!\n", id);
    }
}

void view_orders(void) {
    struct order_record ord;
    FILE *fpo = fopen("orders.txt", "rb");

    printf("\n================================ CUSTOMER ORDERS ================================\n");
    if (!fpo) {
        printf("No customer orders placed yet.\n");
        printf("=================================================================================\n");
        return;
    }

    printf("%-15s | %-12s | %-20s | %-15s | %-4s | %-8s\n",
           "Customer", "Phone", "Address", "Medicine", "Qty", "Total");
    printf("---------------------------------------------------------------------------------\n");

    int count = 0;
    while (fread(&ord, sizeof(struct order_record), 1, fpo) == 1) {
        printf("%-15s | %-12s | %-20s | %-15s | %-4d | %.2f\n",
               ord.cust.name, ord.cust.num, ord.cust.add,
               ord.item.nam, ord.item.stk, ord.item.price * ord.item.stk);
        count++;
    }
    if (count == 0) {
        printf("No customer orders placed yet.\n");
    }
    printf("=================================================================================\n");
    fclose(fpo);
}

void pharmacist_menu(void) {
    int x, idd;
    system("cls");
    printf("                     WELCOME!\n");
    printf("               ...MINI PHARMACY...\n");
    printf("         All your medicines in one place!!!\n");
    printf("\n                 Welcome, Pharmacist!\n");

    while (1) {
        printf("\n--- PHARMACIST MENU ---\n");
        printf("1. Display products\n");
        printf("2. Add product\n");
        printf("3. Update product\n");
        printf("4. Delete product\n");
        printf("5. View customer orders\n");
        printf("6. Change login info\n");
        printf("7. Logout (Back to Main Menu)\n");
        printf("0. Exit Program\n");
        printf("\n>> ");

        if (scanf("%d", &x) != 1) {
            clear_buffer();
            printf("Invalid input!\n");
            continue;
        }
        clear_buffer();

        switch (x) {
            case 0:
                exit(0);
            case 1:
                system("cls");
                display_inventory();
                break;
            case 2:
                add_product();
                break;
            case 3:
                system("cls");
                update_product();
                break;
            case 4:
                system("cls");
                display_inventory();
                printf("\nEnter the medicine ID of the medicine you want to delete\n>> ");
                scanf("%d", &idd);
                clear_buffer();
                delete_product(idd);
                break;
            case 5:
                system("cls");
                view_orders();
                break;
            case 6:
                change_login();
                break;
            case 7:
                return;
            default:
                printf("\nInvalid Input!\n");
                break;
        }
    }
}
