<!DOCTYPE html>
<html lang="pt-br">
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta charset="utf-8">
        <link href="css/base.css" rel="stylesheet">
        <title>Home</title>
    </head>
    <body>    
        <?php
            require_once("menu.php");
        ?>     

        <div class="container">
            <div class="categorias">
                <nav class="col-1">
                  <h3>Categorias</h3>
                  <ul>
                    <li><a href="aros.php" title="Aros">Aros</a>
                    <li><a href="bikes.php" title="Bikes">Bikes</a></li>
                    <li><a href="acessorios.php" title="Acessórios">Acessórios</a></li>
                  </ul>
                </nav>

                <section class="col-2">
                    <h2>Entenda um pouco sobre BMX...</h2>  
                    <p>BMX, também chamada de bicicross é uma corrida com bicicletas especiais, que surgiu na década de 50, na Europa. BMX é um esporte igual ao motocross, só que utilizando bicicletas, já adaptadas para fazer muitas manobras e corridas na areia.</p>
                    <p>O BMX se divide em duas modalidades, o BMX Racing e o BMX Freestyle. O BMX Racing são competições do esporte mais focado na parte de corrida, onde o competidor tem que fazer o percurso no menor tempo. O BMX Freestyle é a competição e a modalidade onde o indivíduo faz diversos tipos de manobras, e o tempo que ele utiliza não é tão importante mas sim a dificuldade das manobras. O BMX Freestyle, também chamado de estilo livre, possui suas próprias modalidades, que são diferenciadas pelo local e a forma de como são executadas. 
                    </p>

                    <p>O BMX pode ser um esporte profissional, para quem participa de competições, e muitos até vivem das rendas que obtêm nos campeonatos. Para outras pessoas é mais um estilo de vida, onde o indivíduo pode relaxar fazendo várias manobras, das mais simples, às mais radicais e arriscadas. O BMX já está entre os esportes que mais cresce no mundo, em número de participantes.</p>
                </section>

                <aside class="col-3">
                    <h2>Eventos</h2>
                    <p>Encontro de bmx's</p>
                </aside>

            </div>
        </div>
        
        <footer>
            <h3>Multmidia:</h3>
            <a href="http://www.facebook.com"><img src="Imagens/fb.png" alt="Facebook" class id="imgrod"></a>
            <a href="https://twitter.com/breathingbmxofc"><img src="Imagens/tt.png" alt="Twitter" class id="imgrod2"></a>
            <br><br><br>        
        </footer>
    </body>
</html>