#include "user.h"
#include "booking.h"
#include <stdio.h>

int main() {
    int choice, user_id = -1;
    char username[50];

    while (1) {
        printf("\n1. Create Account\n2. Login\n3. Search Trains\n4. Book Train Ticket\n5. Exit\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: create_account(); break;
            case 2: login(username, &user_id); break;
            case 3: search_trains(); break;
            case 4: if (user_id != -1) book_train(user_id); else printf("Login first!\n"); break;
            case 5: return 0;
            default: printf("Invalid choice!\n");
        }
    }
}
