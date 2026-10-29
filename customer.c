#include "pharmacy.h"

void display_cart(void) {
    struct product item;
    FILE *fpc = fopen("cart.txt", "rb");

    printf("\n=============================== YOUR CART ===============================\n");
    printf("%-8s | %-20s | %-10s | %-8s | %-10s\n", "ID", "Medicine Name", "Unit Price", "Qty", "Subtotal");
    printf("-------------------------------------------------------------------------\n");

    if (!fpc) {
        printf("Your cart is currently empty.\n");
        printf("=========================================================================\n");
        return;
    }

    float grand_total = 0.0f;
    int count = 0;
    while (fread(&item, sizeof(struct product), 1, fpc) == 1) {
        float subtotal = item.price * item.stk;
        printf("%-8d | %-20s | %-10.2f | %-8d | %-10.2f\n",
               item.id, item.nam, item.price, item.stk, subtotal);
        grand_total += subtotal;
        count++;
    }
    fclose(fpc);

    if (count == 0) {
        printf("Your cart is currently empty.\n");
    } else {
        printf("-------------------------------------------------------------------------\n");
        printf("GRAND TOTAL: %.2f\n", grand_total);
    }
    printf("=========================================================================\n");
}

void add_to_cart(void) {
    struct product item;
    int n, id, qty;

    display_inventory();

    FILE *check = fopen("product.txt", "rb");
    if (!check) {
        return;
    }
    fclose(check);

    printf("\nHow many different medicines do you want to buy?\n>> ");
    scanf("%d", &n);
    clear_buffer();

    for (int i = 0; i < n; i++) {
        printf("\nEnter the medicine ID you want to buy\n>> ");
        scanf("%d", &id);
        clear_buffer();

        FILE *fpi = fopen("product.txt", "rb");
        int found = 0;

        while (fread(&item, sizeof(struct product), 1, fpi) == 1) {
            if (id == item.id) {
                found = 1;
                printf("Selected: %s (Available Stock: %d, Price: %.2f)\n", item.nam, item.stk, item.price);
                printf("Enter quantity to buy >> ");
                scanf("%d", &qty);
                clear_buffer();

                if (qty <= 0) {
                    printf("Invalid quantity! Must be greater than 0.\n");
                } else if (qty > item.stk) {
                    printf("Sorry, only %d unit(s) available in stock!\n", item.stk);
                } else {
                    item.stk = qty; // Store customer's desired quantity in cart item
                    FILE *fpc = fopen("cart.txt", "ab");
                    if (fpc) {
                        fwrite(&item, sizeof(struct product), 1, fpc);
                        fclose(fpc);
                        printf("Added %d x %s to cart!\n", qty, item.nam);
                    }
                }
                break;
            }
        }
        fclose(fpi);

        if (!found) {
            printf("Medicine ID %d not found in inventory!\n", id);
        }
    }
}

void delete_from_cart(int id) {
    struct product item;
    int found = 0;

    FILE *fpc = fopen("cart.txt", "rb");
    if (!fpc) {
        printf("\nCart is empty!\n");
        return;
    }

    FILE *fpct = fopen("temp_cart.txt", "wb");
    if (!fpct) {
        fclose(fpc);
        return;
    }

    while (fread(&item, sizeof(struct product), 1, fpc) == 1) {
        if (id == item.id && !found) {
            found = 1; // Skip writing this item to delete it
        } else {
            fwrite(&item, sizeof(struct product), 1, fpct);
        }
    }
    fclose(fpc);
    fclose(fpct);

    remove("cart.txt");
    rename("temp_cart.txt", "cart.txt");

    if (found) {
        printf("\nMedicine removed from cart successfully!\n");
    } else {
        printf("\nMedicine ID %d not found in your cart!\n", id);
    }
}

void edit_cart(void) {
    int x, id, new_qty;
    while (1) {
        system("cls");
        display_cart();
        printf("\n--- EDIT CART ---\n");
        printf("1. Add more products to cart\n");
        printf("2. Remove a product from cart\n");
        printf("3. Change quantity of a product in cart\n");
        printf("0. Back to Customer Menu\n");
        printf("\n>> ");
        scanf("%d", &x);
        clear_buffer();

        if (x == 0) {
            break;
        } else if (x == 1) {
            add_to_cart();
            Sleep(1200);
        } else if (x == 2) {
            printf("Enter Medicine ID to remove from cart >> ");
            scanf("%d", &id);
            clear_buffer();
            delete_from_cart(id);
            Sleep(1200);
        } else if (x == 3) {
            printf("Enter Medicine ID to update quantity >> ");
            scanf("%d", &id);
            clear_buffer();

            // Check maximum stock available in inventory first
            int max_stock = -1;
            struct product inv_item;
            FILE *fpi = fopen("product.txt", "rb");
            if (fpi) {
                while (fread(&inv_item, sizeof(struct product), 1, fpi) == 1) {
                    if (inv_item.id == id) {
                        max_stock = inv_item.stk;
                        break;
                    }
                }
                fclose(fpi);
            }

            FILE *fpc = fopen("cart.txt", "rb+");
            if (!fpc) {
                printf("Cart is empty!\n");
                Sleep(1200);
                continue;
            }

            struct product cart_item;
            int found = 0;
            while (fread(&cart_item, sizeof(struct product), 1, fpc) == 1) {
                if (cart_item.id == id) {
                    found = 1;
                    printf("Enter new quantity (Available in stock: %d) >> ", max_stock);
                    scanf("%d", &new_qty);
                    clear_buffer();

                    if (new_qty <= 0) {
                        fclose(fpc);
                        delete_from_cart(id);
                    } else if (max_stock != -1 && new_qty > max_stock) {
                        printf("Cannot exceed available stock (%d)!\n", max_stock);
                        fclose(fpc);
                    } else {
                        cart_item.stk = new_qty;
                        fseek(fpc, -(long)sizeof(struct product), SEEK_CUR);
                        fwrite(&cart_item, sizeof(struct product), 1, fpc);
                        fclose(fpc);
                        printf("Quantity updated successfully!\n");
                    }
                    break;
                }
            }
            if (!found) {
                fclose(fpc);
                printf("Medicine ID %d not found in cart!\n", id);
            }
            Sleep(1200);
        } else {
            printf("Invalid choice!\n");
            Sleep(1000);
        }
    }
}

void checkout_order(void) {
    FILE *fpc = fopen("cart.txt", "rb");
    if (!fpc) {
        printf("\nYour cart is empty! Add items before placing an order.\n");
        return;
    }

    fseek(fpc, 0, SEEK_END);
    if (ftell(fpc) == 0) {
        fclose(fpc);
        printf("\nYour cart is empty! Add items before placing an order.\n");
        return;
    }
    rewind(fpc);

    display_cart();

    struct customer_info cust;
    printf("\n--- CHECKOUT DETAILS ---\n");
    printf("Your name >> ");
    read_line(cust.name, sizeof(cust.name));

    printf("Enter your phone number >> ");
    read_line(cust.num, sizeof(cust.num));

    printf("Enter your delivery address >> ");
    read_line(cust.add, sizeof(cust.add));

    FILE *fpo = fopen("orders.txt", "ab");
    FILE *fpi = fopen("product.txt", "rb+");

    struct product cart_item, inv_item;
    struct order_record ord;

    while (fread(&cart_item, sizeof(struct product), 1, fpc) == 1) {
        //save to orders.txt
        ord.cust = cust;
        ord.item = cart_item;
        if (fpo) {
            fwrite(&ord, sizeof(struct order_record), 1, fpo);
        }

        //reduce stock from product.txt
        if (fpi) {
            rewind(fpi);
            while (fread(&inv_item, sizeof(struct product), 1, fpi) == 1) {
                if (inv_item.id == cart_item.id) {
                    inv_item.stk -= cart_item.stk;
                    if (inv_item.stk < 0) inv_item.stk = 0;
                    fseek(fpi, -(long)sizeof(struct product), SEEK_CUR);
                    fwrite(&inv_item, sizeof(struct product), 1, fpi);
                    break;
                }
            }
        }
    }

    if (fpi) fclose(fpi);
    if (fpo) fclose(fpo);
    fclose(fpc);

    //clear cart after checkout
    remove("cart.txt");

    printf("\nOrder placed successfully! Thank you for shopping with us, %s!\n", cust.name);
    Sleep(2000);
}

void customer_menu(void) {
    int choice;
    system("cls");
    printf("\n\t  Welcome, Customer!\n");

    while (1) {
        printf("\n--- CUSTOMER MENU ---\n");
        printf("1. View Inventory & Buy Medicines\n");
        printf("2. View Cart\n");
        printf("3. Edit Cart\n");
        printf("4. Checkout & Place Order\n");
        printf("5. Back to Main Menu\n");
        printf("0. Exit Program\n");
        printf("\n>> ");

        if (scanf("%d", &choice) != 1) {
            clear_buffer();
            printf("Invalid input!\n");
            continue;
        }
        clear_buffer();

        switch (choice) {
            case 0:
                exit(0);
            case 1:
                system("cls");
                add_to_cart();
                break;
            case 2:
                system("cls");
                display_cart();
                break;
            case 3:
                edit_cart();
                break;
            case 4:
                system("cls");
                checkout_order();
                break;
            case 5:
                return;
            default:
                printf("\nInvalid Input!\n");
                break;
        }
    }
}
