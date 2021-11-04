<?php
	$mensagem00 = '';
	$mensagem01 = '';
	$mensagem = '';
	$mensagem2 = '';
	$mensagem3 = '';
	$mensagem4 = '';
	$mensagem5 = '';
	$mensagem6 = '';
	$mensagem7 = '';
	$mensagem8 = '';
	$mensagem9 = '';
	$mensagem10 = '';

	if(isset($_POST['nome'])){
        $valor = $_POST['nome'];
        if(empty($valor)){ //Se esta vazio
			$mensagem00 = "Campo nome esta vazio!";
		}
	}	

	if(isset($_POST['email'])){
        $valor = $_POST['email'];
        if(empty($valor)){ //Se esta vazio
			$mensagem01 = "Campo email esta vazio!";
		}
	}

    if(isset($_POST['q1'])){
        $valor = $_POST['q1'];
        if(empty($valor)){ //Se esta vazio
			$mensagem = "Questão 1 vazio";
		}elseif(is_numeric($valor)){ //Se não é numero
			$mensagem = "O valor deve ser em texto";		
		}else if(strcmp($valor, 'HTML') == 0 || strcmp($valor, 'html') == 0){
            $mensagem = "Questão 1 correta! Reposta: HTML";
        }else{
        	$mensagem = "Questão 1 incorreta! Resposta certa: HTML";
        }
    }

    if(isset($_POST['q2'])){
        $valor = $_POST['q2'];
        if(empty($valor)){ //Se esta vazio
			$mensagem2 = "Questão 2 vazio";
		}elseif(is_numeric($valor)){ //Se não é numero
			$mensagem = "O valor deve ser em texto";		
		}else if(strcmp($valor, 'CSS') == 0 || strcmp($valor, 'css') == 0){
            $mensagem2 = "Questão 2 correta! Reposta: CSS";
        }else{
        	$mensagem2 = "Questão 2 incorreta! Resposta certa: CSS";
        }
    }

    if(isset($_POST['q3'])){
        $valor = $_POST['q3'];
  		foreach($_POST['q3'] as $valor) {
			if($valor == 'c'){
				$mensagem3 = "Questão 3 correta! Reposta: PHP";
			}
		}
    }

    if(isset($_POST['q4'])){
        $valor = $_POST['q4'];
        if($valor == 'a'){
            $mensagem4 = "Questão 4 correta! Reposta: APACHE";
        }else{
        	$mensagem4 = "Questão 4 incorreta! Resposta certa: APACHE";
        }
	}

	if(isset($_POST['q5'])){
        $valor = $_POST['q5'];
    
  		foreach($_POST['q5'] as $valor) {
			if($valor == 'a'){
				$mensagem5 = "Questão 5 correta! Reposta: APACHE";
			}else{
        		$mensagem5 = "Questão 5 incorreta! Resposta certa: APACHE";
       		}
		}
    }

	if(isset($_POST['q6'])){
        $valor = $_POST['q6'];
        if($valor == 'a'){
            $mensagem6 = "Questão 6 correta! Reposta: MySQL";
        }else{
        	$mensagem6 = "Questão 6 incorreta! Resposta certa: MySQL";
        }
	}

	if(isset($_POST['q7'])){
        $valor = $_POST['q7'];
        if($valor == 'Escolha'){ //Se esta vazio
			$mensagem7 = "Questão 7 vazia!";
		}else if($valor == 'Apache'){
        	if(isset($_POST['q8'])){
				$valor = $_POST['q8'];
			    if($valor == 'Escolha'){ //Se esta vazio
					$mensagem8 = "Questão 7 vazia!";
					}if($valor == 'Mosaic'){
			      	  $mensagem7 = "Questão 7! Apache é o software responsável por criar e exibir a interface com o usuário. O primeiro navegador gráfico foi criado em 1993 e chamava-se Mosaic";
			    }else{
			    	$mensagem8 = "Primeira caixa de seleção correta: Apache! Segunda caixa de seleção incorreta! Resposta certa: Mosaic.";
			    }
			}            
        }else{
        	$mensagem7 = "Primeira caixa de seleção incorreta! Resposta certa: Apache";
        }
	}

	if(isset($_POST['comentario'])){
        $valor = $_POST['comentario'];
        if(empty($valor)){ //Se esta vazio
			$mensagem9 = "Campo comentários esta vazio!";
		}else{
			$mensagem9 = nl2br($valor);
		}
	}	

	if(isset($_POST['concordo'])){
        $valor = $_POST['concordo'];
        if($valor != 'on'){
            $mensagem10 = "Concordo marcado!";
        }else{
        	$mensagem10 = "Concordo precisa estar marcado";
        }
	}
?>

<p><?=$mensagem00;?></p>
<p><?=$mensagem01;?></p>
<p><?=$mensagem;?></p>
<p><?=$mensagem2;?></p>
<p><?=$mensagem3;?></p>
<p><?=$mensagem4;?></p>
<p><?=$mensagem5;?></p>
<p><?=$mensagem6;?></p>
<p><?=$mensagem7;?></p>
<p><?=$mensagem8;?></p>
<p><?=$mensagem9;?></p>
<p><?=$mensagem10;?></p>