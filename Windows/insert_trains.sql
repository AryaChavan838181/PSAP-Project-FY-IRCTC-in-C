-- First check if tables exist
SET @table_check = (SELECT COUNT(*)
    FROM information_schema.tables 
    WHERE table_schema = 'u265056410_IRCTC_Psap'
    AND table_name IN ('stations', 'trains'));

SET @continue = CASE WHEN @table_check = 2 THEN 1 ELSE 0 END;

-- Only proceed if both tables exist
DELIMITER //
BEGIN
    IF @continue = 1 THEN

        -- Clear existing data (optional)
        -- DELETE FROM trains;
        -- DELETE FROM stations;

        -- Insert or update stations
        INSERT INTO stations (station_code, station_name) VALUES
        ('NDLS', 'New Delhi'),
        ('MAS', 'Chennai Central'),
        ('BCT', 'Mumbai Central'),
        ('CSTM', 'Mumbai CST'),
        ('HWH', 'Howrah Junction'),
        ('SBC', 'Bangalore City'),
        ('PUNE', 'Pune Junction'),
        ('ADI', 'Ahmedabad Junction'),
        ('BPL', 'Bhopal Junction'),
        ('ST', 'Surat'),
        ('CNB', 'Kanpur Central'),
        ('PRYJ', 'Prayagraj Junction')
        ON DUPLICATE KEY UPDATE 
        station_name = VALUES(station_name);

        -- Insert trains with complete information
        INSERT INTO trains (
            train_number, 
            train_name, 
            source, 
            destination, 
            departure_time, 
            arrival_time,
            total_seats_ac1,
            total_seats_ac2,
            total_seats_sl,
            available_ac1,
            available_ac2,
            available_sl,
            fare_ac1,
            fare_ac2,
            fare_sl
        ) VALUES
        -- Premium Rajdhani Routes
        ('12951', 'New Delhi Rajdhani', 'Mumbai Central', 'New Delhi', 
         '16:30', '08:35', 72, 168, 600, 72, 168, 600, 4500.00, 3200.00, 1500.00),
        
        ('12953', 'August Kranti Rajdhani', 'Mumbai Central', 'New Delhi',
         '17:40', '10:45', 72, 168, 600, 72, 168, 600, 4500.00, 3200.00, 1500.00),
        
        ('12301', 'Howrah Rajdhani', 'Howrah Junction', 'New Delhi',
         '16:50', '09:55', 72, 168, 600, 72, 168, 600, 4200.00, 3000.00, 1400.00),

        -- Shatabdi Express Routes
        ('12002', 'New Delhi Shatabdi', 'New Delhi', 'Bhopal Junction',
         '06:40', '14:30', 100, 200, 400, 100, 200, 400, 3000.00, 2200.00, 1000.00),
        
        ('12009', 'Mumbai Shatabdi', 'Mumbai Central', 'Ahmedabad Junction',
         '06:25', '13:10', 100, 200, 400, 100, 200, 400, 2800.00, 2000.00, 900.00),

        -- Duronto Express Routes
        ('12223', 'Mumbai Duronto', 'Mumbai CST', 'New Delhi',
         '23:05', '16:30', 80, 240, 600, 80, 240, 600, 4000.00, 2800.00, 1200.00),
        
        ('12273', 'Howrah Duronto', 'Howrah Junction', 'New Delhi',
         '20:00', '07:55', 80, 240, 600, 80, 240, 600, 3800.00, 2600.00, 1100.00),

        -- Superfast Express Routes
        ('12137', 'Punjab Mail', 'Mumbai CST', 'New Delhi',
         '19:40', '21:45', 60, 240, 900, 60, 240, 900, 3500.00, 2400.00, 1000.00),
        
        ('12621', 'Tamil Nadu Express', 'New Delhi', 'Chennai Central',
         '22:30', '19:15', 60, 240, 900, 60, 240, 900, 3600.00, 2500.00, 1100.00),

        -- Regional Express Routes
        ('12123', 'Deccan Queen', 'Mumbai CST', 'Pune Junction',
         '07:15', '10:25', 50, 150, 800, 50, 150, 800, 1800.00, 1200.00, 500.00),
        
        ('12921', 'Flying Ranee', 'Mumbai Central', 'Surat',
         '05:40', '11:25', 50, 150, 800, 50, 150, 800, 1600.00, 1000.00, 400.00)
        
        ON DUPLICATE KEY UPDATE
            train_name = VALUES(train_name),
            source = VALUES(source),
            destination = VALUES(destination),
            departure_time = VALUES(departure_time),
            arrival_time = VALUES(arrival_time),
            total_seats_ac1 = VALUES(total_seats_ac1),
            total_seats_ac2 = VALUES(total_seats_ac2),
            total_seats_sl = VALUES(total_seats_sl),
            available_ac1 = VALUES(available_ac1),
            available_ac2 = VALUES(available_ac2),
            available_sl = VALUES(available_sl),
            fare_ac1 = VALUES(fare_ac1),
            fare_ac2 = VALUES(fare_ac2),
            fare_sl = VALUES(fare_sl);

        -- Verify insertion
        SELECT CONCAT('Inserted/Updated ', ROW_COUNT(), ' trains') AS Result;
        
    ELSE
        SELECT 'Error: Required tables do not exist' AS Result;
    END IF;
END //
DELIMITER ;
