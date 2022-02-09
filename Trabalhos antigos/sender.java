import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

public class sender {
    public static void main(String[] args) throws SocketException{
        try (DatagramSocket socket = new DatagramSocket()) {
            InetAddress grupo = InetAddress.getByName("230.1.2.3");
            String mensagem = "Hello";
            byte[] dados = mensagem.getBytes();
            int porta = 5000;
            DatagramPacket pacote = new DatagramPacket(dados, dados.length, grupo, porta);
            socket.send(pacote);
        } catch (SocketException e) {
            throw e;
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}