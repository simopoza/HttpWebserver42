<?php
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    header("Content-Type: text/plain");
    $name = $_GET['name'] ?? '';
    $email = $_GET['email'] ?? '';
    echo "Name: $name\n";
    echo "Email: $email\n";
}
?>