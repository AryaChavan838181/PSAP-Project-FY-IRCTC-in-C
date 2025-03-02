#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curl/curl.h"
#include <cJSON.h>  // Changed include path

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    
    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

const char* get_string(cJSON *item) {
    return item->valuestring ? item->valuestring : "";
}

double get_number(cJSON *item) {
    return item->valuedouble;
}

void print_train_info(cJSON *train) {  // Removed const
    cJSON *name = cJSON_GetObjectItem(train, "train_name");
    cJSON *number = cJSON_GetObjectItem(train, "train_number");
    cJSON *departure = cJSON_GetObjectItem(train, "departure_time");
    cJSON *arrival = cJSON_GetObjectItem(train, "arrival_time");
    cJSON *fare_ac1 = cJSON_GetObjectItem(train, "fare_ac1");
    cJSON *fare_ac2 = cJSON_GetObjectItem(train, "fare_ac2");
    cJSON *fare_sl = cJSON_GetObjectItem(train, "fare_sl");
    cJSON *avail_ac1 = cJSON_GetObjectItem(train, "available_ac1");
    cJSON *avail_ac2 = cJSON_GetObjectItem(train, "available_ac2");
    cJSON *avail_sl = cJSON_GetObjectItem(train, "available_sl");
    cJSON *source_code = cJSON_GetObjectItem(train, "source_code");
    cJSON *dest_code = cJSON_GetObjectItem(train, "dest_code");
    cJSON *source_name = cJSON_GetObjectItem(train, "source_name");
    cJSON *dest_name = cJSON_GetObjectItem(train, "dest_name");

    if (name && number && departure && arrival && 
        fare_ac1 && fare_ac2 && fare_sl &&
        avail_ac1 && avail_ac2 && avail_sl &&
        source_code && dest_code && source_name && dest_name) {
            
        printf("\n%s (%s)\n", 
               get_string(name),
               get_string(number));
        printf("   Route: %s (%s) -> %s (%s)\n",
               get_string(source_name),
               get_string(source_code),
               get_string(dest_name),
               get_string(dest_code));
        printf("   Departure: %s\n   Arrival: %s\n", 
               get_string(departure),
               get_string(arrival));
        printf("   Fares: AC1: ₹%.2f, AC2: ₹%.2f, SL: ₹%.2f\n",
               get_number(fare_ac1),
               get_number(fare_ac2),
               get_number(fare_sl));
        printf("   Available Seats: AC1: %d, AC2: %d, SL: %d\n",
               (int)get_number(avail_ac1),
               (int)get_number(avail_ac2),
               (int)get_number(avail_sl));
    }
}

void print_formatted_train_info(cJSON *train) {
    printf("\n----------------------------------------\n");
    printf("Train: %s (%s)\n", 
           get_string(cJSON_GetObjectItem(train, "train_name")),
           get_string(cJSON_GetObjectItem(train, "train_number")));
    
    printf("From: %s (%s) -> To: %s (%s)\n",
           get_string(cJSON_GetObjectItem(train, "source_name")),
           get_string(cJSON_GetObjectItem(train, "source_code")),
           get_string(cJSON_GetObjectItem(train, "dest_name")),
           get_string(cJSON_GetObjectItem(train, "dest_code")));
    
    printf("Departure: %s  Arrival: %s\n",
           get_string(cJSON_GetObjectItem(train, "departure_time")),
           get_string(cJSON_GetObjectItem(train, "arrival_time")));
    
    printf("Total Seats: %s\n",
           get_string(cJSON_GetObjectItem(train, "total_seats")));
    printf("----------------------------------------\n");
}

void book_ticket(cJSON *train) {
    printf("\n=== Booking Ticket ===\n");
    
    // Get passenger details
    char name[100], mobile[15], email[100];
    printf("\nEnter passenger details:\n");
    printf("Name (required): ");
    getchar(); // Clear buffer
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; // Remove newline
    
    printf("Mobile (required): ");
    fgets(mobile, sizeof(mobile), stdin);
    mobile[strcspn(mobile, "\n")] = 0;
    
    printf("Email (required): ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = 0;
    
    // Validate mandatory fields
    if (strlen(name) == 0 || strlen(mobile) == 0 || strlen(email) == 0) {
        printf("\nError: All fields are mandatory!\n");
        return;
    }

    // Display available classes and seats
    printf("\nAvailable Classes and Seats:\n");
    printf("1. AC First Class  - %d seats (₹%.2f)\n", 
           (int)get_number(cJSON_GetObjectItem(train, "available_ac1")),
           get_number(cJSON_GetObjectItem(train, "fare_ac1")));
    printf("2. AC Second Class - %d seats (₹%.2f)\n",
           (int)get_number(cJSON_GetObjectItem(train, "available_ac2")),
           get_number(cJSON_GetObjectItem(train, "fare_ac2")));
    printf("3. Sleeper Class  - %d seats (₹%.2f)\n",
           (int)get_number(cJSON_GetObjectItem(train, "available_sl")),
           get_number(cJSON_GetObjectItem(train, "fare_sl")));

    int class_choice;
    printf("\nSelect class (1-3): ");
    scanf("%d", &class_choice);

    int num_tickets;
    printf("Enter number of tickets: ");
    scanf("%d", &num_tickets);

    double fare = 0;
    int available = 0;
    switch(class_choice) {
        case 1:
            fare = get_number(cJSON_GetObjectItem(train, "fare_ac1"));
            available = (int)get_number(cJSON_GetObjectItem(train, "available_ac1"));
            break;
        case 2:
            fare = get_number(cJSON_GetObjectItem(train, "fare_ac2"));
            available = (int)get_number(cJSON_GetObjectItem(train, "available_ac2"));
            break;
        case 3:
            fare = get_number(cJSON_GetObjectItem(train, "fare_sl"));
            available = (int)get_number(cJSON_GetObjectItem(train, "available_sl"));
            break;
    }

    if (num_tickets > available) {
        printf("\nSorry, only %d seats available in this class\n", available);
        return;
    }

    double total_fare = fare * num_tickets;
    printf("\nBooking Summary:");
    printf("\nTrain: %s (%s)", 
           get_string(cJSON_GetObjectItem(train, "train_name")),
           get_string(cJSON_GetObjectItem(train, "train_number")));
    printf("\nClass: %s", 
           class_choice == 1 ? "AC First Class" : 
           class_choice == 2 ? "AC Second Class" : "Sleeper Class");
    printf("\nNumber of tickets: %d", num_tickets);
    printf("\nTotal fare: ₹%.2f", total_fare);

    printf("\n\nConfirm booking? (1 for Yes, 0 for No): ");
    int confirm;
    scanf("%d", &confirm);

    if (confirm) {
        // Make booking API call
        CURL *booking_curl = curl_easy_init();
        if (booking_curl) {
            char booking_url[1000];
            sprintf(booking_url, 
                "https://www.thegroup11.com/api/irctc?action=create_booking"
                "&train_id=%s&name=%s&mobile=%s&email=%s"
                "&class_type=%s&num_tickets=%d",
                get_string(cJSON_GetObjectItem(train, "train_id")),
                name, mobile, email,
                class_choice == 1 ? "AC1" : (class_choice == 2 ? "AC2" : "SL"),
                num_tickets);

            struct MemoryStruct booking_response;
            booking_response.memory = malloc(1);
            booking_response.size = 0;

            curl_easy_setopt(booking_curl, CURLOPT_URL, booking_url);
            curl_easy_setopt(booking_curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
            curl_easy_setopt(booking_curl, CURLOPT_WRITEDATA, (void *)&booking_response);
            curl_easy_setopt(booking_curl, CURLOPT_SSL_VERIFYPEER, 0L);

            CURLcode booking_res = curl_easy_perform(booking_curl);
            if (booking_res == CURLE_OK) {
                cJSON *booking_json = cJSON_Parse(booking_response.memory);
                if (booking_json && cJSON_IsTrue(cJSON_GetObjectItem(booking_json, "success"))) {
                    cJSON *booking = cJSON_GetObjectItem(booking_json, "booking");
                    
                    printf("\n=== Booking Confirmed! ===\n");
                    printf("PNR: %s\n", get_string(cJSON_GetObjectItem(booking, "pnr")));
                    printf("Train: %s (%s)\n", 
                        get_string(cJSON_GetObjectItem(booking, "train_name")),
                        get_string(cJSON_GetObjectItem(booking, "train_number")));
                    printf("From: %s\nTo: %s\n",
                        get_string(cJSON_GetObjectItem(booking, "source")),
                        get_string(cJSON_GetObjectItem(booking, "destination")));
                    printf("Departure: %s\n", get_string(cJSON_GetObjectItem(booking, "departure_time")));
                    printf("Class: %s\n", get_string(cJSON_GetObjectItem(booking, "coach_type")));
                    printf("Seats: %d\n", num_tickets);
                    printf("\nPlease save your PNR for future reference!\n");
                }
                if (booking_json) cJSON_Delete(booking_json);
            }
            free(booking_response.memory);
            curl_easy_cleanup(booking_curl);
        }
    }
}

void send_booking_email(CURL *curl, const char *email, const char *booking_details) {
    struct MemoryStruct email_response;
    email_response.memory = malloc(1);
    email_response.size = 0;

    // URL encode the booking details
    char *encoded_details = curl_easy_escape(curl, booking_details, 0);
    
    char email_url[2000];
    sprintf(email_url, 
            "https://thegroup11.com/api/sendmail"
            "?api_key=dGhlZ3JvdXAxMQ=="
            "&to=%s"
            "&subject=Your+IRCTC+Booking+Details"
            "&message=%s",
            email,
            encoded_details);

    curl_easy_setopt(curl, CURLOPT_URL, email_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&email_response);
    
    printf("\nSending email...\n");
    CURLcode email_res = curl_easy_perform(curl);
    
    if(email_res == CURLE_OK) {
        printf("Booking details sent to %s successfully!\n", email);
    } else {
        printf("Failed to send email. Please try again later.\n");
    }

    curl_free(encoded_details);
    free(email_response.memory);
}

void check_pnr(CURL *curl, struct MemoryStruct *chunk, struct curl_slist *headers) {
    printf("\nEnter PNR number: ");
    char pnr[20];
    scanf("%s", pnr);

    // Reset chunk memory
    free(chunk->memory);
    chunk->memory = malloc(1);
    chunk->size = 0;

    // Reset and set headers
    if (headers) {
        curl_slist_free_all(headers);
        headers = NULL;
    }
    headers = curl_slist_append(headers, "Accept: application/json");

    char url[500];
    sprintf(url, "https://www.thegroup11.com/api/irctc?action=check_booking&pnr=%s", pnr);

    // Set CURL options
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    printf("\nChecking PNR status...\n");
    CURLcode res = curl_easy_perform(curl);
    
    if(res == CURLE_OK) {
        chunk->memory[chunk->size] = 0;  // Ensure null termination
        printf("\nAPI Response: %s\n", chunk->memory); // Debug line - remove in production

        cJSON *json = cJSON_Parse(chunk->memory);
        if (json) {
            if (cJSON_IsTrue(cJSON_GetObjectItem(json, "success"))) {
                cJSON *booking = cJSON_GetObjectItem(json, "booking");
                if (booking) {
                    // Store booking details for email
                    char booking_details[1000];
                    snprintf(booking_details, sizeof(booking_details),
                        "PNR: %s\n"
                        "Train: %s (%s)\n"
                        "Route: %s -> %s\n"
                        "Departure: %s\n"
                        "Coach: %s, Seat: %s\n"
                        "Status: %s\n",
                        get_string(cJSON_GetObjectItem(booking, "pnr")),
                        get_string(cJSON_GetObjectItem(booking, "train_name")),
                        get_string(cJSON_GetObjectItem(booking, "train_number")),
                        get_string(cJSON_GetObjectItem(booking, "source")),
                        get_string(cJSON_GetObjectItem(booking, "destination")),
                        get_string(cJSON_GetObjectItem(booking, "departure_time")),
                        get_string(cJSON_GetObjectItem(booking, "coach_type")),
                        get_string(cJSON_GetObjectItem(booking, "seat_number")),
                        get_string(cJSON_GetObjectItem(booking, "status")));

                    // Display booking details
                    printf("\n=== Booking Details ===\n%s", booking_details);

                    // Ask if user wants email
                    printf("\nWould you like to receive these details via email? (1 for Yes, 0 for No): ");
                    int want_email;
                    scanf("%d", &want_email);

                    if (want_email) {
                        printf("Enter your email address: ");
                        char email[100];
                        scanf("%s", email);
                        send_booking_email(curl, email, booking_details);
                    }
                } else {
                    printf("\nError: Invalid booking data in response\n");
                }
            } else {
                printf("\nBooking not found! Please check your PNR number.\n");
            }
            cJSON_Delete(json);
        } else {
            printf("\nError: Invalid JSON response\n");
        }
    } else {
        printf("\nError: Failed to connect to server (%s)\n", curl_easy_strerror(res));
    }

    printf("\nPress Enter to continue...");
    getchar(); getchar();
}

void cancel_ticket(CURL *curl, struct MemoryStruct *chunk, struct curl_slist *headers) {
    printf("\nEnter PNR number to cancel: ");
    char pnr[20];
    scanf("%s", pnr);

    char url[500];
    sprintf(url, "https://www.thegroup11.com/api/irctc?action=cancel_booking&pnr=%s", pnr);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    CURLcode res = curl_easy_perform(curl);
    
    if(res == CURLE_OK) {
        cJSON *json = cJSON_Parse(chunk->memory);
        if (json && cJSON_IsTrue(cJSON_GetObjectItem(json, "success"))) {
            printf("Booking cancelled successfully!\n");
        } else {
            printf("Failed to cancel booking!\n");
        }
        if (json) cJSON_Delete(json);
    }
}

int main() {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;  // Moved declaration here

    curl_global_init(CURL_GLOBAL_ALL);
    
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();
    if(curl) {
        while(1) {
            printf("\n=== IRCTC Train Search & Booking System ===\n");
            printf("1. Search Trains (View Only)\n");
            printf("2. Search & Book Tickets\n");
            printf("3. Check PNR Status\n");
            printf("4. Cancel Ticket\n");
            printf("5. Exit\n");
            printf("Enter choice: ");
            
            int menu_choice;
            scanf("%d", &menu_choice);
            
            switch(menu_choice) {
                case 1: {
                    printf("Enter source station code (e.g., NDLS for New Delhi): ");
                    char source_code[10];
                    scanf("%s", source_code);
                    
                    printf("Enter destination station code (e.g., MAS for Chennai): ");
                    char dest_code[10];
                    scanf("%s", dest_code);

                    char url[500];
                    sprintf(url, "https://www.thegroup11.com/api/irctc?action=search_trains&source=%s&destination=%s",
                            source_code, dest_code);

                    curl_easy_setopt(curl, CURLOPT_URL, url);
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
                    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                    
                    headers = curl_slist_append(headers, "Accept: application/json");
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

                    printf("\nSearching trains from %s to %s...\n\n", source_code, dest_code);
                    
                    printf("\nMaking request to URL: %s\n", url);
                    
                    printf("\nAttempting to connect to: %s\n", url);
                    
                    res = curl_easy_perform(curl);
                    if(res != CURLE_OK) {
                        fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(res));
                    } else {
                        // Don't print raw response
                        cJSON *json = cJSON_Parse(chunk.memory);
                        if (json && cJSON_IsTrue(cJSON_GetObjectItem(json, "success"))) {
                            cJSON *data = cJSON_GetObjectItem(json, "data");
                            if (data) {
                                printf("\n=== Available Trains ===\n");
                                cJSON *train;
                                int train_count = 0;
                                cJSON_ArrayForEach(train, data) {
                                    printf("\n%d. ", train_count + 1);
                                    print_formatted_train_info(train);
                                    train_count++;
                                }
                                printf("\nPress Enter to continue...");
                                getchar(); getchar();
                            }
                            cJSON_Delete(json);
                        }
                    }
                    break;
                }
                
                case 2: {
                    printf("Enter source station code (e.g., NDLS for New Delhi): ");
                    char source_code[10];
                    scanf("%s", source_code);
                    
                    printf("Enter destination station code (e.g., MAS for Chennai): ");
                    char dest_code[10];
                    scanf("%s", dest_code);

                    // Reset chunk memory
                    free(chunk.memory);
                    chunk.memory = malloc(1);
                    chunk.size = 0;

                    char url[500];
                    sprintf(url, "https://www.thegroup11.com/api/irctc?action=search_trains&source=%s&destination=%s",
                            source_code, dest_code);

                    curl_easy_setopt(curl, CURLOPT_URL, url);
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
                    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                    
                    if (headers) {
                        curl_slist_free_all(headers);
                        headers = NULL;
                    }
                    headers = curl_slist_append(headers, "Accept: application/json");
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

                    printf("\nSearching trains from %s to %s...\n\n", source_code, dest_code);
                    
                    res = curl_easy_perform(curl);
                    if(res != CURLE_OK) {
                        fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(res));
                    } else {
                        chunk.memory[chunk.size] = 0;  // Ensure null termination
                        cJSON *json = cJSON_Parse(chunk.memory);
                        if (json) {
                            cJSON *data = cJSON_GetObjectItem(json, "data");
                            if (data) {
                                printf("\n=== Available Trains ===\n");
                                int train_count = 0;
                                cJSON *trains[100];
                                
                                cJSON *train;
                                cJSON_ArrayForEach(train, data) {
                                    trains[train_count] = train;
                                    printf("\n%d. ", train_count + 1);
                                    print_formatted_train_info(train);
                                    train_count++;
                                }

                                if (train_count > 0) {
                                    printf("\nDo you want to book a ticket? (1 for Yes, 0 for No): ");
                                    int want_to_book;
                                    scanf("%d", &want_to_book);

                                    if (want_to_book == 1) {
                                        int train_choice;
                                        printf("\nEnter train number (1-%d): ", train_count);
                                        scanf("%d", &train_choice);
                                        
                                        if (train_choice > 0 && train_choice <= train_count) {
                                            book_ticket(trains[train_choice - 1]);
                                        } else {
                                            printf("Invalid train number!\n");
                                        }
                                    }
                                } else {
                                    printf("\nNo trains found!\n");
                                }

                                printf("\nPress Enter to return to main menu...");
                                getchar(); getchar();
                            }
                            cJSON_Delete(json);
                        }
                    }
                    break;
                }
                
                case 3:
                    check_pnr(curl, &chunk, headers);
                    break;
                case 4:
                    cancel_ticket(curl, &chunk, headers);
                    break;
                case 5:
                    goto cleanup;
                default:
                    printf("Invalid choice!\n");
            }
        }

cleanup:
        if (headers) {  // Add check before cleanup
            curl_slist_free_all(headers);
        }
        curl_easy_cleanup(curl);
    }

    free(chunk.memory);
    curl_global_cleanup();
    return 0;
}
