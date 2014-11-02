<?php
  // define an include file and set your mysql credentials
  require_once('../include.php');

  $mysqli = new mysqli($host, $username, $password, $database);
  $return = $mysqli->query("SELECT * FROM temperature ORDER BY id DESC");

  $row = $return->fetch_assoc();

  header("Content-Type:text/xml");
?>
<Response>
  <Say voice="woman" loop="10">The current temperature in your room is <?php echo $row['temperature']; ?> degrees.  The current date and time is <?php echo date("F j, Y, g:i a", strtotime('+2 hours', time())); ?>. </Say>
</Response>
