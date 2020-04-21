<?php
$url=$_SERVER['REQUEST_URI'];
header("Refresh: 60; URL=$url"); // Refresh the webpage every 5 seconds
?>
<html>
<head>
<title>Field weather and monitoring Data Table</title>
<style type="text/css">
.table_titles {
padding-right: 20px;
padding-left: 20px;
color: #000;
}
.table_titles {
color: #FFF;
background-color: #0000FF;
}
table {
border: 2px solid #333;
}
body { font-family: "Trebuchet MS", Courier; }
</style>
</head>
<body>
<h1>Field weather and monitoring </h1>
<table border="0" cellspacing="0" cellpadding="4">
<tr>
<td class="table_titles">ID</td>
<td class="table_titles">Date and Time</td>
<td class="table_titles">Temperature</td>
<td class="table_titles">Humidity</td>
<td class="table_titles">SoilM</td>
<td class="table_titles">CND</td>
<td class="table_titles">Ip</td>
<td class="table_titles">Heat_index</td>
</tr>
<?php
include('connection.php');
$result = mysqli_query($db,'SELECT * FROM data ORDER BY id DESC');
// Process every record
$oddrow = true;
while($row = mysqli_fetch_array($result))
{
if ($oddrow)
{
$css_class=' class="table_cells_odd"';
}
else
{
$css_class=' class="table_cells_even"';
}
$oddrow = !$oddrow; 
echo "<tr>";
echo "<td '.$css_class.'>" . $row['id'] . "</td>";
echo "<td '.$css_class.'>" . $row['event'] . "</td>";
echo "<td '.$css_class.'>" . $row['temperature'] . "</td>";
echo "<td '.$css_class.'>" . $row['humidity'] . "</td>";
echo "<td '.$css_class.'>" . $row['soilm'] . "</td>";
echo "<td '.$css_class.'>" . $row['cnd'] . "</td>";
echo "<td '.$css_class.'>" . $row['ip'] . "</td>";
echo "<td '.$css_class.'>" . $row['heat_index'] . "</td>";
echo "</tr>"; 
}
 
// Close the connection
mysqli_close($db);
?>
</table>
</body>
</html>