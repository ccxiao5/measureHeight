<?php
	$delArray=array(
		"measureDate"=>$_GET["measureDate"],
        "measureName"=>$_GET["measureName"],
        "height"=>$_GET["height"],
	);//接收参数
	$host = "localhost";
	$user = "root";
	$pass = "";
	$db = "wx_user";
	$db = new mysqli($host, $user, $pass,$db);
	$db->set_charset("utf8");
    if(!$db){
     die("Unable to connect!");
  	}
  	//删除记录注意使用"`"!!!!!
  	$delquery = "DELETE FROM `heightinfo` WHERE `measureName`='"
	  	.$delArray["measureName"]."' AND `height`='"
	  	.$delArray["height"]."'AND `measureDate`='"
	  	.$delArray["measureDate"]."'";
 	$delresult = mysqli_query($db, $delquery);
 	if($delresult)
 	{
	 	//删除后查询
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
	}
	else
	{
		echo "shibai";
	}
 	$db->close();
?>