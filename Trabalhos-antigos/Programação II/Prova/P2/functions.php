<?php
function exibeData($data){
	if($data == "00/00/0000"){
		return "-";
	}
	else{
		return $data;
	}
}

function exibeCategoria($gola){
	switch ($gola) {
		case 1:
		  return "Polo";
		  break;
		case 2:
		  return "Ribana";
		  break;
		case 3:
		  return "Sem Gola";
		  break;    
	}
}
?>