#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

#define MAX_USERS 100
#define MAX_BOOKINGS 100

typedef struct {
    char name[50];
    char dob[15];
    char number[15];
    char payment_info[50];
    char password[20];
    char gov_id[20];
} User;

typedef struct {
    int pnr;
    char train_name[50];
    int train_no;
    char seat_pref[10];
    char timing[10];
    int platform_no;
    int seat_no;
    char food[20];
} Booking;

User users[MAX_USERS];
Booking bookings[MAX_BOOKINGS];
int user_count = 0;
int booking_count = 0;

MYSQL *conn;
int logged_in_user_id = -1;

// Function Prototypes
void home_menu();
void create_account();
void login();
void new_booking(int train_choice);
void check_booking();
void cancel_booking();
void check_availability();
void initialize_database();
void search_trains();
void check_bookings();

int main() {
    initialize_database();
    home_menu();
    return 0;
}

void initialize_database() {
    conn = mysql_init(NULL);
    
    // Disable SSL by not configuring any SSL parameters
    mysql_ssl_set(conn, NULL, NULL, NULL, NULL, NULL);
    
    if (!mysql_real_connect(conn, 
        "thegroup11.com",  // Use actual hosting domain
        "u265056410_IRCTC_Psap", 
        "IRCTC_Psap@1234", 
        "u265056410_IRCTC_Psap", 
        3306,
        NULL,
        0)) {
        fprintf(stderr, "Failed to connect to database: %s\n", mysql_error(conn));
        exit(1);
    }
    printf("Successfully connected to database!\n");
}

void home_menu() {
    int choice;
    while (1) {
        printf("\n===== IRCTC Booking System =====\n");
        printf("1. Create Account / Login\n");
        printf("2. Search & Book Tickets\n");
        printf("3. Check Bookings\n");
        printf("4. Cancel Booking\n");
        printf("5. Check Availability\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: login(); break;
            case 2: search_trains(); break;
            case 3: check_bookings(); break;
            case 4: cancel_booking(); break;
            case 5: check_availability(); break;
            case 6: 
                mysql_close(conn);
                exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
}

void create_account() {
    if (user_count >= MAX_USERS) {
        printf("User limit reached!\n");
        return;
    }

    printf("\n===== Create Account =====\n");
    printf("Enter Name: ");
    fgets(users[user_count].name, 50, stdin);
    printf("Enter DOB (dd/mm/yyyy): ");
    fgets(users[user_count].dob, 15, stdin);
    printf("Enter Phone Number: ");
    fgets(users[user_count].number, 15, stdin);
    printf("Enter Payment Info: ");
    fgets(users[user_count].payment_info, 50, stdin);
    printf("Enter Password: ");
    fgets(users[user_count].password, 20, stdin);
    printf("Enter Govt. ID: ");
    fgets(users[user_count].gov_id, 20, stdin);

    printf("Account created successfully!\n");
    user_count++;
}

void login() {
    char username[50], password[50];
    printf("\nEnter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    char query[200];
    sprintf(query, "SELECT user_id FROM users WHERE username='%s' AND password='%s'", 
            username, password);
    
    if (mysql_query(conn, query)) {
        printf("Login failed!\n");
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result && mysql_num_rows(result) > 0) {
        MYSQL_ROW row = mysql_fetch_row(result);
        logged_in_user_id = atoi(row[0]);
        printf("Login successful!\n");
    } else {
        printf("Invalid credentials!\n");
    }
    mysql_free_result(result);
}

void search_trains() {
    if (logged_in_user_id == -1) {
        printf("Please login first!\n");
        login();
        if (logged_in_user_id == -1) return;
    }

    char source[50], destination[50];
    printf("\nEnter source station: ");
    scanf("%s", source);
    printf("Enter destination station: ");
    scanf("%s", destination);

    char query[300];
    sprintf(query, "SELECT * FROM trains WHERE source='%s' AND destination='%s'",
            source, destination);
    
    if (mysql_query(conn, query)) {
        printf("Error searching trains!\n");
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result) {
        int num_trains = mysql_num_rows(result);
        if (num_trains == 0) {
            printf("No trains found!\n");
            return;
        }

        printf("\nAvailable Trains:\n");
        int i = 1;
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            printf("%d. %s (%s) - Departure: %s\n", 
                   i++, row[1], row[2], row[5]);
        }

        printf("\nSelect train number (1-%d): ", num_trains);
        int choice;
        scanf("%d", &choice);

        if (choice > 0 && choice <= num_trains) {
            // Proceed to booking
            new_booking(choice);
        }
    }
    mysql_free_result(result);
}

void new_booking(int train_choice) {
    printf("\nSelect coach type:\n");
    printf("1. AC First Class\n");
    printf("2. AC Second Class\n");
    printf("3. Sleeper\n");
    printf("Choice: ");
    
    int coach_choice;
    scanf("%d", &coach_choice);

    printf("\nFood preference (Y/N): ");
    char food_choice;
    scanf(" %c", &food_choice);

    // Generate PNR and create booking
    char pnr[11];
    sprintf(pnr, "PNR%d%d", rand() % 1000000, logged_in_user_id);

    char query[500];
    sprintf(query, "INSERT INTO bookings (user_id, train_id, booking_date, "
            "coach_type, food_preference, pnr, status) VALUES "
            "(%d, %d, CURDATE(), '%s', '%s', '%s', 'CONFIRMED')",
            logged_in_user_id, train_choice,
            coach_choice == 1 ? "AC1" : (coach_choice == 2 ? "AC2" : "SL"),
            food_choice == 'Y' ? "VEG" : "NONE",
            pnr);

    if (mysql_query(conn, query) == 0) {
        printf("\nBooking successful! PNR: %s\n", pnr);
    } else {
        printf("Booking failed!\n");
    }
}

void check_bookings() {
    if (logged_in_user_id == -1) {
        printf("Please login first!\n");
        login();
        if (logged_in_user_id == -1) return;
    }

    char query[500];
    sprintf(query, "SELECT b.pnr, t.train_name, t.source, t.destination, "
            "b.booking_date, b.status FROM bookings b "
            "JOIN trains t ON b.train_id = t.train_id "
            "WHERE b.user_id = %d", logged_in_user_id);

    if (mysql_query(conn, query) == 0) {
        MYSQL_RES *result = mysql_store_result(conn);
        if (result) {
            printf("\nYour Bookings:\n");
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(result))) {
                printf("PNR: %s\nTrain: %s\nRoute: %s to %s\nDate: %s\nStatus: %s\n\n",
                       row[0], row[1], row[2], row[3], row[4], row[5]);
            }
            mysql_free_result(result);
        }
    }
}

void check_booking() {
    int pnr, found = 0;
    printf("\n===== Check Booking =====\n");
    printf("Enter PNR: ");
    scanf("%d", &pnr);
    
    for (int i = 0; i < booking_count; i++) {
        if (bookings[i].pnr == pnr) {
            printf("\nBooking Found!\n");
            printf("Train: %s", bookings[i].train_name);
            printf("Train No: %d\n", bookings[i].train_no);
            printf("Seat: %s", bookings[i].seat_pref);
            printf("Timing: %s", bookings[i].timing);
            printf("Platform No: %d\n", bookings[i].platform_no);
            printf("Seat No: %d\n", bookings[i].seat_no);
            printf("Food: %s", bookings[i].food);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("PNR not found!\n");
    }
}

void cancel_booking() {
    int pnr, found = 0;
    printf("\n===== Cancel Booking =====\n");
    printf("Enter PNR: ");
    scanf("%d", &pnr);
    
    for (int i = 0; i < booking_count; i++) {
        if (bookings[i].pnr == pnr) {
            printf("Booking for PNR %d cancelled successfully.\n", pnr);
            for (int j = i; j < booking_count - 1; j++) {
                bookings[j] = bookings[j + 1];
            }
            booking_count--;
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("PNR not found!\n");
    }
}

void check_availability() {
    int type_choice;
    printf("\n===== Check Availability =====\n");
    printf("Enter Date (dd/mm/yyyy): ");
    char date[15];
    getchar();
    fgets(date, 15, stdin);
    
    printf("Enter Location: ");
    char location[50];
    fgets(location, 50, stdin);
    
    printf("Choose Ticket Type:\n");
    printf("1. General\n2. Tatkal\n3. Premium Tatkal\n");
    scanf("%d", &type_choice);
    
    switch (type_choice) {
        case 1: printf("General Tickets Available. Price: ₹500\n"); break;
        case 2: printf("Tatkal Tickets Available. Price: ₹1000\n"); break;
        case 3: printf("Premium Tatkal Tickets Available. Price: ₹1500\n"); break;
        default: printf("Invalid choice!\n");
    }
}