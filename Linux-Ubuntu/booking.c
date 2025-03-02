#include "booking.h"
#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void search_trains() {
    char source[50], destination[50];
    printf("Enter Source Station: ");
    scanf("%s", source);
    printf("Enter Destination Station: ");
    scanf("%s", destination);

    char query[256];
    sprintf(query, "SELECT train_number, train_name FROM trains WHERE source='%s' AND destination='%s'", source, destination);

    MYSQL* conn = get_connection();
    MYSQL_RES* result;
    if (!execute_query(conn, query, &result)) {
        printf("No trains found!\n");
        close_database(conn);
        return;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        printf("Train Number: %s | Train Name: %s\n", row[0], row[1]);
    }

    mysql_free_result(result);
    close_database(conn);
}

void book_train(int user_id) {
    int train_id;
    char booking_date[20], coach_type[5];
    printf("Enter Train ID: ");
    scanf("%d", &train_id);
    printf("Enter Coach Type (AC1/AC2/SL): ");
    scanf("%s", coach_type);
    printf("Enter Booking Date (YYYY-MM-DD): ");
    scanf("%s", booking_date);

    char query[256];
    sprintf(query, "INSERT INTO bookings (user_id, train_id, booking_date, coach_type, status) VALUES (%d, %d, '%s', '%s', 'CONFIRMED')", user_id, train_id, booking_date, coach_type);

    MYSQL* conn = get_connection();
    if (!execute_query(conn, query, NULL)) {
        printf("Booking failed!\n");
        close_database(conn);
        return;
    }

    printf("Booking successful!\n");
    close_database(conn);
}
