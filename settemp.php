<?php
  // define your own mysql credentials in include.php
  require_once('../include.php');

  // set this to the IP address of your network
  if ("YOURIPADDRESS" != $_SERVER['REMOTE_ADDR']) {
    die();
  }

  $mysqli = new mysqli($host, $username, $password, $database);
  $statement = $mysqli->PREPARE("INSERT INTO temperature (temperature) VALUES (?)");
  $statement->bind_param("d", $_GET['temp']);
  $statement->execute();
?>
OK
