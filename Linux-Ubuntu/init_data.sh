#!/bin/bash

DB_NAME="irctc3"
MYSQL_USER="root"
MYSQL_PASS="root"

mysql -u $MYSQL_USER -p$MYSQL_PASS $DB_NAME <<EOF

-- Insert Stations
INSERT INTO stations (station_name, station_code) VALUES
('Mumbai Central', 'BCT'),
('Mumbai CST', 'CSTM'),
('New Delhi', 'NDLS'),
('Howrah', 'HWH'),
('Chennai', 'MAS'),
('Bangalore', 'SBC'),
('Pune', 'PUNE'),
('Ahmedabad', 'ADI'),
('Bhopal', 'BPL'),
('Surat', 'ST'),
('Firozpur', 'FZR'),
('Kolkata', 'KOAA')
ON DUPLICATE KEY UPDATE station_code=VALUES(station_code);

-- Insert Trains
INSERT INTO trains (
    train_name, train_number, source, destination, 
    departure_time, arrival_time, 
    fare_ac1, fare_ac2, fare_sl, 
    available_ac1, available_ac2, available_sl
) VALUES
('New Delhi Rajdhani', '12951', 'Mumbai Central', 'New Delhi', '16:30', '08:35', 
    4500, 3200, 1500, 72, 168, 600),
('Howrah Rajdhani', '12301', 'New Delhi', 'Howrah', '16:50', '09:55', 
    4200, 3000, 1400, 72, 168, 600),
('Chennai Rajdhani', '12433', 'New Delhi', 'Chennai', '15:55', '20:40', 
    4800, 3500, 1600, 72, 168, 600),
('Bhopal Shatabdi', '12001', 'New Delhi', 'Bhopal', '06:40', '14:30', 
    3000, 2200, 1000, 100, 200, 400),
('Mumbai Shatabdi', '12009', 'Mumbai Central', 'Ahmedabad', '06:25', '13:10', 
    2800, 2000, 900, 100, 200, 400),
('Mumbai Duronto', '12223', 'Mumbai CST', 'New Delhi', '23:05', '16:30', 
    4000, 2800, 1200, 80, 240, 600),
('Howrah Duronto', '12273', 'New Delhi', 'Howrah', '20:00', '07:55', 
    3800, 2600, 1100, 80, 240, 600),
('Punjab Mail', '12137', 'Mumbai CST', 'Firozpur', '19:40', '21:45', 
    3500, 2400, 1000, 60, 240, 900),
('Tamil Nadu Express', '12621', 'New Delhi', 'Chennai', '22:30', '19:15', 
    3600, 2500, 1100, 60, 240, 900),
('Deccan Queen', '12123', 'Mumbai CST', 'Pune', '07:15', '10:25', 
    1800, 1200, 500, 50, 150, 800),
('Flying Ranee', '12921', 'Mumbai Central', 'Surat', '05:40', '11:25', 
    1600, 1000, 400, 50, 150, 800)
ON DUPLICATE KEY UPDATE
train_name=VALUES(train_name),
departure_time=VALUES(departure_time),
arrival_time=VALUES(arrival_time),
fare_ac1=VALUES(fare_ac1),
fare_ac2=VALUES(fare_ac2),
fare_sl=VALUES(fare_sl),
available_ac1=VALUES(available_ac1),
available_ac2=VALUES(available_ac2),
available_sl=VALUES(available_sl);

EOF

echo "✅ Sample station and train data inserted successfully!"
