#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "root"
#define DB_NAME "irctc3"

MYSQL* get_connection() {
    MYSQL *conn = mysql_init(NULL);
    if (!conn) {
        fprintf(stderr, "MySQL initialization failed\n");
        return NULL;
    }
    if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0)) {
        fprintf(stderr, "MySQL Connection Error: %s\n", mysql_error(conn));
        return NULL;
    }
    return conn;
}

void close_database(MYSQL* conn) {
    if (conn) {
        mysql_close(conn);
    }
}

int execute_query(MYSQL* conn, const char* query, MYSQL_RES** result) {
    if (!conn) return 0;
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query Error: %s\n", mysql_error(conn));
        return 0;
    }
    *result = mysql_store_result(conn);
    return 1;
}

int initialize_database() {
    MYSQL* conn = get_connection();
    if (!conn) return 0;

    const char* create_tables =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INT AUTO_INCREMENT PRIMARY KEY, "
        "username VARCHAR(50) UNIQUE, "
        "password VARCHAR(50));"
        "CREATE TABLE IF NOT EXISTS bookings ("
        "pnr INT AUTO_INCREMENT PRIMARY KEY, "
        "user_id INT, "
        "train_id INT, "
        "booking_date DATE, "
        "coach_type VARCHAR(10), "
        "status VARCHAR(10), "
        "FOREIGN KEY(user_id) REFERENCES users(id));";

    int success = execute_query(conn, create_tables, NULL);
    close_database(conn);
    return success;
}
