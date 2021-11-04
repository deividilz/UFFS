function calcularTotal(){
	var valor = (parseInt(quantPedidoP)+parseInt(quantPedidoM)+parseInt(quantPedidoG)+parseInt(quantPedidoGG))*20;
	document.getElementById("total").attribute=valor;
}