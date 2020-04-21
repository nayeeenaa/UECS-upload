<?php
  define("DB_HOST", "localhost");
  define("DB_USER", "root");
  define("DB_PASSWORD", "");
  define("DB_DATABASE", "test_uardecs");

  $db = new mysqli(DB_HOST, DB_USER, DB_PASSWORD, DB_DATABASE);
  // you could test connection eventually using a if and else conditional statement, 
  // feel free to take out the code below once you see Connected!
/*
if ($db->connect_error) {
   die("Connection failed: " . $db->connect_error);
}
  echo "Connected successfully";
  */
?>
