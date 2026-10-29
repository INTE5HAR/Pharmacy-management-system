#ifndef PHARMACY_H
#define PHARMACY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

//pharmacist login credentials
struct account {
    char username[20];
    char password[20];
};

struct customer_info {
    char name[30];
    char num[15];
    char add[50];
};

//med product structure
struct product {
    int id;
    int stk;
    float price;
    char nam[25];
    char exp[12];
};

struct order_record {
    struct customer_info cust;
    struct product item;
};

//utility functions (in main.c)
void clear_buffer(void);
void read_line(char *buffer, int size);

//authentication functions (in auth.c)
void get_password(char *password, int max_len);
void setup_initial_users(void);
int login(void);
void change_login(void);

//pharmacist functions (in pharmacist.c)
void pharmacist_menu(void);
void display_inventory(void);
void add_product(void);
void update_product(void);
void delete_product(int id);
void view_orders(void);

//customer functions (in customer.c)
void customer_menu(void);
void add_to_cart(void);
void display_cart(void);
void edit_cart(void);
void delete_from_cart(int id);
void checkout_order(void);

#endif
