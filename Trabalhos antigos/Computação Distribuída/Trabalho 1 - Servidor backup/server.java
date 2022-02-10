import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class server {
    public static void main(String[] args) throws IOException, ClassNotFoundException{
        try (ServerSocket s = new ServerSocket(5000,10)) {
            Socket c = s.accept();
            ObjectOutputStream output =new ObjectOutputStream(c.getOutputStream());
            output.flush();
            ObjectInputStream input = new ObjectInputStream(c.getInputStream());

            String mensagem = (String)input.readObject();
            System.out.println("Recebido" + mensagem);

            String resposta = "Olá cliente";

            output.writeObject(resposta);
            //output.flush;
        }
    }
}