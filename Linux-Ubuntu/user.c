#include "user.h"
#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int create_account() {
    char username[50], password[50];
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);

    char query[200];
    sprintf(query, "INSERT INTO users (username, password) VALUES ('%s', '%s')", username, password);

    MYSQL* conn = get_connection();
    if (!execute_query(conn, query, NULL)) {
        printf("Account creation failed!\n");
        close_database(conn);
        return 0;
    }

    printf("Account created successfully!\n");
    close_database(conn);
    return 1;
}

int login(char* username, int* user_id) {
    char password[50];
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);

    char query[200];
    sprintf(query, "SELECT id FROM users WHERE username='%s' AND password='%s'", username, password);

    MYSQL* conn = get_connection();
    MYSQL_RES* result;
    if (!execute_query(conn, query, &result)) {
        printf("Login failed!\n");
        close_database(conn);
        return 0;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) {
        *user_id = atoi(row[0]);
        printf("Login successful! Welcome, %s\n", username);
        mysql_free_result(result);
        close_database(conn);
        return 1;
    }

    printf("Invalid credentials!\n");
    mysql_free_result(result);
    close_database(conn);
    return 0;
}
