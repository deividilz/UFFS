import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.Socket;

public class cliente {
    public static void main(String[] args) throws Exception {
        Socket socket = new Socket(InetAddress.getByName("127.0.0.1"),5000);

        ObjectOutputStream output = new ObjectOutputStream(socket.getOutputStream());

        output.flush();

        ObjectInputStream input = new ObjectInputStream(socket.getInputStream());

        String mensagem = "Olá servidor";

        output.writeObject(mensagem);
        output.flush();

        String resposta = (String) input.readObject();
        System.out.println("Recebido" + resposta);
    }
}
    