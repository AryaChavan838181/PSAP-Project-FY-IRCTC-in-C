<?php
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

$host = 'localhost';
$db   = 'u265056410_IRCTC_Psap';
$user = 'u265056410_IRCTC_Psap';
$pass = 'IRCTC_Psap@1234';

$conn = new mysqli($host, $user, $pass, $db);

if ($conn->connect_error) {
    die(json_encode(['error' => 'Connection failed: ' . $conn->connect_error]));
}

$method = $_SERVER['REQUEST_METHOD'];
$action = isset($_GET['action']) ? $_GET['action'] : '';

switch ($method) {
    case 'GET':
        switch ($action) {
            case 'search_trains':
                $source = $conn->real_escape_string($_GET['source']);
                $dest = $conn->real_escape_string($_GET['destination']);
                $sql = "SELECT * FROM trains WHERE source='$source' AND destination='$dest'";
                break;
                
            case 'check_booking':
                $pnr = $conn->real_escape_string($_GET['pnr']);
                $sql = "SELECT * FROM bookings WHERE pnr='$pnr'";
                break;

            default:
                echo json_encode(['error' => 'Invalid action']);
                exit;
        }
        break;

    case 'POST':
        $data = json_decode(file_get_contents('php://input'), true);
        
        switch ($action) {
            case 'login':
                $username = $conn->real_escape_string($data['username']);
                $password = $conn->real_escape_string($data['password']);
                $sql = "SELECT user_id FROM users WHERE username='$username' AND password='$password'";
                break;

            case 'book_ticket':
                $user_id = $conn->real_escape_string($data['user_id']);
                $train_id = $conn->real_escape_string($data['train_id']);
                $coach = $conn->real_escape_string($data['coach_type']);
                $food = $conn->real_escape_string($data['food']);
                $pnr = 'PNR' . rand(100000, 999999);
                
                $sql = "INSERT INTO bookings (user_id, train_id, booking_date, coach_type, 
                        food_preference, pnr, status) VALUES 
                        ($user_id, $train_id, CURDATE(), '$coach', '$food', '$pnr', 'CONFIRMED')";
                break;

            default:
                echo json_encode(['error' => 'Invalid action']);
                exit;
        }
        break;

    default:
        echo json_encode(['error' => 'Invalid method']);
        exit;
}

$result = $conn->query($sql);

if ($result === TRUE) {
    // For INSERT queries
    echo json_encode(['success' => true, 'id' => $conn->insert_id]);
} elseif ($result) {
    // For SELECT queries
    $rows = [];
    while ($row = $result->fetch_assoc()) {
        $rows[] = $row;
    }
    echo json_encode(['success' => true, 'data' => $rows]);
} else {
    echo json_encode(['error' => $conn->error]);
}

$conn->close();
?>
