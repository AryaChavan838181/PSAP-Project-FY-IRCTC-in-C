#ifndef DATABASE_H
#define DATABASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mysql/mysql.h>

MYSQL* get_connection();
void close_database(MYSQL* conn);
int execute_query(MYSQL* conn, const char* query, MYSQL_RES** result);
int initialize_database();

#ifdef __cplusplus
}
#endif

#endif
