<?php
include ('connection.php');
date_default_timezone_set('Asia/Tokyo');
$sql_insert = "INSERT INTO send_data (temperature, humidity, HD, Temperature_Room, Temperature_Region, Temperature_Order, Temperature_Priority, Humidity_Room, Humidity_Region, Humidity_Order, Humidity_Priority, HD_Room, HD_Region, HD_Order, HD_Priority, Controller_thermostat) VALUES ('".$_GET["temperature"]."', '".$_GET["humidity"]."', '".$_GET["HD"]."', '".$_GET["Temperature_Room"]."', '".$_GET["Temperature_Region"]."', '".$_GET["Temperature_Order"]."', '".$_GET["Temperature_Priority"]."', '".$_GET["Humidity_Room"]."', '".$_GET["Humidity_Region"]."', '".$_GET["Humidity_Order"]."', '".$_GET["Humidity_Priority"]."', '".$_GET["HD_Room"]."', '".$_GET["HD_Region"]."', '".$_GET["HD_Order"]."', '".$_GET["HD_Priority"]."', '".$_GET["Controller_thermostat"]."')";
if(mysqli_query($db,$sql_insert))
{
echo "Done";
mysqli_close($db);
}
else
{
echo "error is ".mysqli_error($db);
}
?>
