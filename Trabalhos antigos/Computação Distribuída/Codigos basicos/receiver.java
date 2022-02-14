package Arquivos;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

public class receiver {
    InetAddress InetAdress = null;

    public static void main(String[] args) throws IOException {
        int porta = 5000;
        
        try (MulticastSocket msocket = new MulticastSocket(porta)) {
            InetAddress grupo = InetAddress.getByName("230.1.2.3");
            msocket.joinGroup(grupo);
            byte[] dados = new byte[100];
            DatagramPacket pacote = new DatagramPacket(dados, dados.length);
            msocket.receive(pacote);
            System.out.println("Mensagem: " + new String(pacote.getData(), 0, pacote.getLength())); 

            DatagramPacket pacote2 = new DatagramPacket(dados, dados.length, grupo, porta);
            System.out.println("Porta: " + dados + "Int: " + porta );
            msocket.send(pacote2);
            System.out.println("cacetinho");
        }

        /*try(DatagramSocket socket = new DatagramSocket()){
            String mensagem = Integer.toString(porta);
            byte[] dados = mensagem.getBytes();
            InetAddress grupo = InetAddress.getByName("230.1.2.3");
            DatagramPacket pacote = new DatagramPacket(dados, dados.length, grupo, porta);
            System.out.println("Porta: " + dados + "Int: " + porta );
            socket.send(pacote);
        }*/
    }
}