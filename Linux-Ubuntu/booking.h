#ifndef BOOKING_H
#define BOOKING_H

#ifdef __cplusplus
extern "C" {
#endif

void search_trains();
void book_train(int user_id);
void check_booking(int user_id);
void cancel_booking(int user_id);

#ifdef __cplusplus
}
#endif

#endif
