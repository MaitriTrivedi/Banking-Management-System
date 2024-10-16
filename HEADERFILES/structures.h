#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string.h>
#include <stdbool.h>
#include <openssl/sha.h>
#include <time.h>

struct User{
    char username[30];
    unsigned char password[SHA256_DIGEST_LENGTH];
    int userid;
    bool is_logged_in;
    bool is_active;
};

struct Customer{
    struct User u;
    int account_no;
    float account_balance;
    bool loan_taken;
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

struct Loan{
    struct Customer borrower;
    struct Employee approving_employee;
    int loan_id;
    float loan_amount;
    bool is_approved;
};

struct Transaction{
    char transaction_type[30];
    float amount;
    time_t transaction_time;
};

struct TransactionHistory{
    struct Customer customer;
    struct Transaction *transactions;
    int transactions_counts;
    int current_capacity;
};

struct Feedback{
    struct Customer customer;
    int rating;
    char feedback[200];
};

#endif