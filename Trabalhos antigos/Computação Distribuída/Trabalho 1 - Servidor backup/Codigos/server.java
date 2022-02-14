package Codigos;

import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class server {
    public static void main(String[] args) throws IOException, ClassNotFoundException{
        try (ServerSocket server = new ServerSocket(1234,10)) {
            System.out.println("Info: aguardando o envio do arquivo pelo cliente.\n");
            Socket cliente = server.accept();
            
            byte[] objectAsByte = new byte[cliente.getReceiveBufferSize()];
            BufferedInputStream buffer = new BufferedInputStream(cliente.getInputStream());
            buffer.read(objectAsByte);

            Arquivo arquivo = getObjectFromByte(objectAsByte);

            String diretorio = arquivo.getDiretorioDestino().endsWith("/") ? arquivo
           .getDiretorioDestino() + arquivo.getNome() : arquivo
           .getDiretorioDestino() + "/" + arquivo.getNome();
            
           System.out.println("Escrevendo arquivo " + diretorio);

           FileOutputStream fos = new FileOutputStream(diretorio);
           fos.write(arquivo.getConteudo());
           fos.close();
        }catch(Exception e){
            System.out.println("Erro: " + e.toString());
        }
    }

    private static Arquivo getObjectFromByte(byte[] objectAsByte) {
        Object obj = null;
        ByteArrayInputStream bis = null;
        ObjectInputStream ois = null;
        try {
            bis = new ByteArrayInputStream(objectAsByte);
            ois = new ObjectInputStream(bis);
            obj = ois.readObject();

            bis.close();
            ois.close();

        } catch (IOException e) {
            System.out.println("Erro: " + e.toString());
        } catch (ClassNotFoundException e) {
            System.out.println("Erro: " + e.toString());
        }
        return (Arquivo) obj;
    }
}