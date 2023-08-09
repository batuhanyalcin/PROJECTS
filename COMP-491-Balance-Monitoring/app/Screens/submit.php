<?php

$db_host = 'db-balancemonitoring.cwjo9swcga53.eu-north-1.rds.amazonaws.com';
$db_user = 'admin';
$db_pass = 'eebgm491';
$db_name = 'balancemonitoringdb';

$conn = mysqli_connect($db_host, $db_user, $db_pass, $db_name);

if (!$conn) {
  die('Database connection failed!');
}

$query = "INSERT into PATIENT (PatientId, Pname, Email, PhoneNumber, Gender, Age, Weight, Height) VALUES ('1', 'EraySozer', 'eraysozer20@gmail.com', '05310845533', 'T', '23', '85', '190');";

if (mysqli_query($conn, $query)) {
echo 'Form submitted successfully!';
} else {
echo 'Error submitting form.';
}


mysqli_close($conn);

?>
