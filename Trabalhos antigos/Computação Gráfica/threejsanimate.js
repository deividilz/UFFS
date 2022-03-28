<html>
	<head>
		<title>Hello world</title>
		<style>
			body { margin: 0; }
			canvas { width: 100%; height: 100% }
		</style>
	</head>
	<body>
		<script src="../../libs/three.js-r110/build/three.min.js"></script>
		<script>
			var scene = new THREE.Scene();
			var camera = new THREE.PerspectiveCamera(75, window.innerWidth/window.innerHeight, 0.1, 1000);

			var renderer = new THREE.WebGLRenderer();
			renderer.setSize(window.innerWidth, window.innerHeight);
			document.body.appendChild(renderer.domElement);

			var geometry = new THREE.BoxGeometry(1, 1, 1);
			var material = new THREE.MeshBasicMaterial({color: 0x00ff00});
			var material2 = new THREE.MeshBasicMaterial({color: 0xff0000});
			
			var personagem = new THREE.Mesh(geometry, material2);
			scene.add(personagem);

			personagem.position.y = 1.5

			var cubes = [];
			var coracoes = [];

			for(var i =0; i<10; i++){
				var cube = new THREE.Mesh(geometry, material);
				scene.add(cube);

				if (i%2 == 0)
					cube.position.x = i * 1.5;
				else
					cube.position.x = -(i * 1.5);
				
				cubes.push(cube);
			}
			

			camera.position.z = 20;

			var animate = function() {
				requestAnimationFrame(animate);

				for(var i = 0; i<10; i++){

					if(personagem.position.distanceTo(cubes[i].position)<= 2.0){
						continue;
					}

					cubes[i].rotation.x += 0.01;
					cubes[i].rotation.y += 0.01;
				}	
				

				for(var i=0; i<coracoes.length; i++){
					coracoes[i].rotation.z += 0.01;
					coracoes[i].rotation.x += 0.01;
				}

				personagem.rotation.x += 0.01;
				personagem.rotation.y += 0.01;

				renderer.render(scene, camera);
			};

			animate();

			function onKeyDown(event) {
				var keyCode = event.which;
				var speed = 0.1;

				console.log('keyCode', keyCode);

				if (keyCode == 87 || keyCode == 38) {
					personagem.position.y += speed;
				} else if (keyCode == 83 || keyCode == 40) {
					personagem.position.y -= speed;
				} else if (keyCode == 65 || keyCode == 37) {
					personagem.position.x -= speed;
				} else if (keyCode == 68 || keyCode == 39) {
					personagem.position.x += speed;
				} else if (keyCode == 32) {
					for (var i=0; i<10; i++){
						var coracao = new THREE.Mesh(geometry, material2);
						scene.add(coracao);

						coracao.position.x = personagem.position.x;
						coracao.position.y = personagem.position.y;
						
						coracoes.push(coracao);
					}
				}
			};

			document.addEventListener("keydown", onKeyDown, false);
		</script>
	</body>
</html>

