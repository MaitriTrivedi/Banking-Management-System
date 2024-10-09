#ifndef STRUCTURES_H
#define STRUCTURES_H

#include<string.h>
#include <stdbool.h>
#include<openssl/sha.h>

struct User{
    char username[30];
    unsigned char password[SHA256_DIGEST_LENGTH];
    int userid;
};

struct Customer{
    struct User u;
    int account_no;
    float account_balance;
    bool loan_taken; // 0 bydefault
    int loan_id; // -1 by default
    int employee_id; // user_id of that employee
};

struct Employee{
    struct User u;
    float salary;
    // int manager_id;
};

struct Manager{
    struct User u;
    float salary;
};

struct Admin{
    struct User u;
};

#endif