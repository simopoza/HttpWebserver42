<!DOCTYPE html>

<html>

<head>

    <title>PHP Form Example</title>

</head>

<body>
<form method="post" action="test.php">

<label for="name">Name:</label>

<input type="text" name="name" id="name"><br><br>

<label for="email">Email:</label>

<input type="email" name="email" id="email"><br><br>

<input type="submit" name="submit" value="Submit">

</form>
</body>

</html>
<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {

    $name = $_POST["name"];

    $email = $_POST["email"];

    echo "<h2>Your Input:</h2>";

    echo "Name: " . $name . "<br>";

    echo "Email: " . $email;

}

?>