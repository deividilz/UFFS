package Arquivos;

import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.Socket;

public class cliente{

    public static void main(String[] args) throws Exception {
        try{
            Socket socket = new Socket(InetAddress.getByName("127.0.0.1"),1234);
            ObjectOutputStream output = new ObjectOutputStream(socket.getOutputStream());
            output.flush();

            ObjectInputStream input = new ObjectInputStream(socket.getInputStream());

            String mensagem = "Olá servidor";

            output.writeObject(mensagem);
            output.flush();

            String resposta = (String) input.readObject();
            System.out.println("Recebido: " + resposta + "\n");
            //input.flush();
        }catch(Exception e){
            System.out.println("Erro: " + e.toString());
        }
    }        
}
    