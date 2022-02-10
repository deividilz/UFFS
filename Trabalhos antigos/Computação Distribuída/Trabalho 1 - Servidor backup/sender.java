import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.SocketAddress;
import java.net.SocketException;

public class sender {
    public static void main(String[] args) throws IOException{
        try (DatagramSocket socket = new DatagramSocket()) {
            InetAddress grupo = InetAddress.getByName("230.1.2.3");
            String mensagem = "Qual é o seu endereço de IP?";
            byte[] dados = mensagem.getBytes();
            int porta = 5000;
            DatagramPacket pacote = new DatagramPacket(dados, dados.length, grupo, porta);
            socket.send(pacote);
        } catch (SocketException e) {
            throw e;
        } catch (IOException e) {
            e.printStackTrace();
        }

        int porta = 5000;
        try (MulticastSocket msocket = new MulticastSocket(porta)) {
            System.out.printf("Teste\n");
            InetAddress grupo = InetAddress.getByName("230.1.2.3");
            msocket.joinGroup(grupo);
            System.out.printf("Teste2\n");
            byte[] dados = new byte[100];
            DatagramPacket pacote = new DatagramPacket(dados, dados.length);
            System.out.printf("teste3");
            msocket.receive(pacote);
            try{
                System.out.println("Mensagem retornada: " + new String(pacote.getData(), 0, pacote.getLength())); 
            }
            catch (Exception e){
                System.out.println("Mensagem retornada");
            }
            
        }
    }
}