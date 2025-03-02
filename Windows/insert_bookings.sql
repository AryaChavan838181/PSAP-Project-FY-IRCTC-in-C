
-- First create a sample user if not exists
INSERT IGNORE INTO users (username, password, name, phone) VALUES
('demo_user', 'demo123', 'Demo User', '9876543210');

-- Insert sample bookings with random PNRs
INSERT INTO bookings 
(user_id, train_id, booking_date, passenger_name, mobile, email, 
coach_type, num_tickets, status, pnr) 
VALUES 
-- Rajdhani Express bookings
(1, (SELECT train_id FROM trains WHERE train_number = '12951'), 
CURDATE(), 'John Doe', '9876543210', 'john@example.com',
'AC1', 2, 'CONFIRMED', CONCAT('PNR', FLOOR(RAND() * 9000000) + 1000000)),

(1, (SELECT train_id FROM trains WHERE train_number = '12951'),
DATE_SUB(CURDATE(), INTERVAL 1 DAY), 'Jane Smith', '9876543211', 'jane@example.com',
'AC2', 1, 'CONFIRMED', CONCAT('PNR', FLOOR(RAND() * 9000000) + 1000000)),

-- Tamil Nadu Express bookings
(1, (SELECT train_id FROM trains WHERE train_number = '12621'),
DATE_SUB(CURDATE(), INTERVAL 2 DAY), 'Robert Brown', '9876543212', 'robert@example.com',
'SL', 3, 'CONFIRMED', CONCAT('PNR', FLOOR(RAND() * 9000000) + 1000000)),

(1, (SELECT train_id FROM trains WHERE train_number = '12621'),
DATE_SUB(CURDATE(), INTERVAL 3 DAY), 'Mary Wilson', '9876543213', 'mary@example.com',
'AC1', 1, 'CANCELLED', CONCAT('PNR', FLOOR(RAND() * 9000000) + 1000000)),

-- Shatabdi Express bookings
(1, (SELECT train_id FROM trains WHERE train_number = '12002'),
DATE_ADD(CURDATE(), INTERVAL 1 DAY), 'David Miller', '9876543214', 'david@example.com',
'AC2', 2, 'CONFIRMED', CONCAT('PNR', FLOOR(RAND() * 9000000) + 1000000)),

-- Duronto Express bookings
(1, (SELECT train_id FROM trains WHERE train_number = '12223'),
DATE_ADD(CURDATE(), INTERVAL 2 DAY), 'Sarah Davis', '9876543215', 'sarah@example.com',
'SL', 4, 'WAITLIST', CONCAT('PNR', FLOOR(RAND() * 9000000) + 1000000));

-- Add payment records for confirmed bookings
INSERT INTO payments (booking_id, amount, payment_status, payment_date, payment_method)
SELECT 
    b.booking_id,
    CASE 
        WHEN b.coach_type = 'AC1' THEN t.fare_ac1 * b.num_tickets
        WHEN b.coach_type = 'AC2' THEN t.fare_ac2 * b.num_tickets
        ELSE t.fare_sl * b.num_tickets
    END as amount,
    'COMPLETED',
    b.booking_date,
    'Credit Card'
FROM bookings b
JOIN trains t ON b.train_id = t.train_id
WHERE b.status = 'CONFIRMED';

-- Show the inserted bookings with their details
SELECT 
    b.pnr,
    t.train_name,
    t.train_number,
    b.passenger_name,
    b.coach_type,
    b.num_tickets,
    b.status,
    p.amount as paid_amount
FROM bookings b
LEFT JOIN trains t ON b.train_id = t.train_id
LEFT JOIN payments p ON b.booking_id = p.booking_id
ORDER BY b.booking_date DESC;
