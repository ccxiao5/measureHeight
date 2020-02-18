<?php
  $img=$_GET["a"];
  $refColor=$_GET["refColor"];
  $refHeight=$_GET["refHeight"];
 // $success="Success:".$img;
 // $Fail="FAil:".$img;
 // $command="./measure ".$img;
  $command="./measure ".$refColor." ".$refHeight;
  passthru($command,$result); 
?>
