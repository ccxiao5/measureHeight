<?php
	$host = "localhost";
	$user = "root";
	$pass = "";
	$db = "wx_user";
	$db = new mysqli($host, $user, $pass,$db);
	$db->set_charset("utf8");
    if(!$db){
     die("Unable to connect!");
  	}
  	$query = "select * from heightinfo ";
 	$result = mysqli_query($db, $query);
 	class Info{
 		public $Date;
 		public $Name;
 		public $height;
 	}
 	$data=array();
	if (mysqli_num_rows($result) > 0){
		while($row = mysqli_fetch_assoc($result)) {
			$Info=new Info();
			$Info->Date=$row["measureDate"];
			$Info->Name=$row["measureName"];
			$Info->height=$row["height"];
			$data[]=$Info;

		//echo json_encode($data,JSON_UNESCAPED_UNICODE|JSON_PRETTY_PRINT);
			//将请求结果转换为json格式，微信只能对Json格式进行操作
		}
		echo json_encode($data,JSON_UNESCAPED_UNICODE|JSON_PRETTY_PRINT);
			//将请求结果转换为json格式，微信只能对Json格式进行操作
	}		
 	$db->close();
?>
