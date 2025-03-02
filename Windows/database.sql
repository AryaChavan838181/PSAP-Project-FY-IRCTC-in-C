CREATE DATABASE u265056410_IRCTC_Psap;
USE u265056410_IRCTC_Psap;

CREATE TABLE users (
    user_id INT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    name VARCHAR(100) NOT NULL,
    dob DATE,
    phone VARCHAR(15),
    gov_id VARCHAR(20),
    payment_info VARCHAR(100)
);

CREATE TABLE IF NOT EXISTS stations (
    station_id INT PRIMARY KEY AUTO_INCREMENT,
    station_code VARCHAR(10) UNIQUE,
    station_name VARCHAR(100)
);

CREATE TABLE IF NOT EXISTS trains (
    train_id INT PRIMARY KEY AUTO_INCREMENT,
    train_number VARCHAR(10) UNIQUE,
    train_name VARCHAR(100),
    source VARCHAR(100),
    destination VARCHAR(100),
    departure_time TIME,
    arrival_time TIME,
    total_seats_ac1 INT DEFAULT 50,
    total_seats_ac2 INT DEFAULT 100,
    total_seats_sl INT DEFAULT 200,
    available_ac1 INT DEFAULT 50,
    available_ac2 INT DEFAULT 100,
    available_sl INT DEFAULT 200,
    fare_ac1 DECIMAL(10,2),
    fare_ac2 DECIMAL(10,2),
    fare_sl DECIMAL(10,2)
);

-- Sample data
INSERT INTO stations (station_code, station_name) VALUES
('NDLS', 'New Delhi'),
('MAS', 'Chennai Central'),
('BOM', 'Mumbai Central'),
('HWH', 'Howrah Junction'),
('BLR', 'Bangalore');

INSERT INTO trains (train_number, train_name, source, destination, departure_time, arrival_time, 
                   fare_ac1, fare_ac2, fare_sl) VALUES
('12345', 'Rajdhani Express', 'New Delhi', 'Chennai Central', '16:00', '20:00', 3000.00, 2000.00, 800.00),
('12346', 'Shatabdi Express', 'Mumbai Central', 'New Delhi', '08:00', '16:00', 2500.00, 1800.00, 700.00),
('12347', 'Duronto Express', 'Howrah Junction', 'Mumbai Central', '10:00', '18:00', 3500.00, 2200.00, 900.00);

CREATE TABLE bookings (
    booking_id INT PRIMARY KEY AUTO_INCREMENT,
    user_id INT,
    train_id INT,
    booking_date DATE,
    seat_number INT,
    coach_type VARCHAR(20),
    status VARCHAR(20),
    food_preference VARCHAR(20),
    pnr VARCHAR(10) UNIQUE,
    FOREIGN KEY (user_id) REFERENCES users(user_id),
    FOREIGN KEY (train_id) REFERENCES trains(train_id)
);

CREATE TABLE payments (
    payment_id INT PRIMARY KEY AUTO_INCREMENT,
    booking_id INT,
    amount DECIMAL(10,2),
    payment_status VARCHAR(20),
    payment_date DATETIME,
    payment_method VARCHAR(50),
    FOREIGN KEY (booking_id) REFERENCES bookings(booking_id)
);

