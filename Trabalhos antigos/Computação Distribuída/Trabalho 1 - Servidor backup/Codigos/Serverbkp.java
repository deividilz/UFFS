package Codigos;

import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Serverbkp {
    public class InicializaServidor extends Thread{

        private final Socket socket;
        private Arquivo arquivo;

        public InicializaServidor(final Socket cliente) throws IOException {
            this.socket = cliente;
            System.out.println("Inicializando o servidor\n");
        }    
    
        public void run(){
            try {
               
                System.out.println("Info: aguardando o envio do arquivo pelo cliente.\n");
                
                byte[] objectAsByte = new byte[socket.getReceiveBufferSize()];

                System.out.println("objectAsByte: " + objectAsByte);

                BufferedInputStream buffer = new BufferedInputStream(socket.getInputStream());

                buffer.read(objectAsByte);
                
                System.out.println("Buffer: " + buffer);

                arquivo = getObjectFromByte(objectAsByte);

                System.out.println("arquivo: " + arquivo);

                String diretorio = arquivo.getDiretorioDestino().endsWith("/") ? arquivo.getDiretorioDestino() + arquivo.getNome() : arquivo.getDiretorioDestino() + "/" + arquivo.getNome();
                
                System.out.println("Escrevendo arquivo " + diretorio);

                FileOutputStream fos = new FileOutputStream(diretorio);
                fos.write(arquivo.getConteudo());
                fos.close();
            }catch(Exception e){
                System.out.println("Erro: " + e.toString());
            }
        }
    }

    public Server (final int port) throws IOException {
        try (ServerSocket server = new ServerSocket(port)) {
            for(;;){
                new InicializaServidor(server.accept()).start();
            }
        }
    }

    public static void main(String[] args) throws IOException, ClassNotFoundException{
        new Server(1234);
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