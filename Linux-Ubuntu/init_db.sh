#!/bin/bash

DB_NAME="irctc3"
MYSQL_USER="root"
MYSQL_PASS="root"

# Create the database if it doesn't exist
mysql -u $MYSQL_USER -p$MYSQL_PASS -e "CREATE DATABASE IF NOT EXISTS $DB_NAME;"

# Create the tables
mysql -u $MYSQL_USER -p$MYSQL_PASS $DB_NAME <<EOF

-- Create stations table
CREATE TABLE IF NOT EXISTS stations (
    station_id INT AUTO_INCREMENT PRIMARY KEY,
    station_name VARCHAR(100),
    station_code VARCHAR(10) UNIQUE
);

-- Create users table
CREATE TABLE IF NOT EXISTS users (
    user_id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE,
    password VARCHAR(50),
    name VARCHAR(50),
    dob DATE,
    phone VARCHAR(15),
    govt_id VARCHAR(20)
);

-- Create trains table
CREATE TABLE IF NOT EXISTS trains (
    train_id INT AUTO_INCREMENT PRIMARY KEY,
    train_name VARCHAR(50),
    train_number VARCHAR(10) UNIQUE,
    source VARCHAR(50),
    destination VARCHAR(50),
    departure_time TIME,
    arrival_time TIME,
    fare_ac1 INT, fare_ac2 INT, fare_sl INT,
    available_ac1 INT, available_ac2 INT, available_sl INT
);

-- Create bookings table
CREATE TABLE IF NOT EXISTS bookings (
    booking_id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT,
    train_id INT,
    booking_date DATE,
    coach_type VARCHAR(10),
    pnr VARCHAR(12) UNIQUE,
    status VARCHAR(20),
    FOREIGN KEY (user_id) REFERENCES users(user_id),
    FOREIGN KEY (train_id) REFERENCES trains(train_id)
);

EOF

echo "✅ Database and tables created successfully!"
