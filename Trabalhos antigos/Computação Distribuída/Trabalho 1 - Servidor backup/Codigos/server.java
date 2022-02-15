package Codigos;

import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Server extends javax.swing.JFrame {

    private static javax.swing.JLabel jLabelTamanho;
    private static javax.swing.JLabel jLabelTamanhoImprimir;
    private static javax.swing.JLabel jLabelInfo;
    private static javax.swing.JLabel jLabelPorta;
    private static javax.swing.JLabel jLabelNomeArquivo;
    private static javax.swing.JLabel jLabelNomeImprimir;
    private static javax.swing.JTextField jTextField1;
    private static javax.swing.JButton jButtonAbrir;
    private static javax.swing.JButton jButton2;
    
    private static Arquivo arquivo;

    public Server(){
        initComponents();
    }

    private void initComponents() {

        jLabelInfo = new javax.swing.JLabel();
        jLabelPorta = new javax.swing.JLabel();
        jTextField1 = new javax.swing.JTextField();
        jLabelNomeArquivo = new javax.swing.JLabel();
        jLabelNomeImprimir = new javax.swing.JLabel();
        jLabelTamanho = new javax.swing.JLabel();
        jLabelTamanhoImprimir = new javax.swing.JLabel();
        jButtonAbrir = new javax.swing.JButton();
        jButton2 = new javax.swing.JButton();

        this.setLocationRelativeTo(null);
        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        jLabelInfo.setText("Servidor offline");

        jLabelPorta.setText("Porta");

        jButtonAbrir.setText("Abrir porta");
        jButtonAbrir.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                
                new Thread (new Runnable() {
                    @Override
                    public void run() {

                        try{
                            Integer.parseInt(jTextField1.getText().trim());
                            jLabelInfo.setText("Servidor online");
                            System.out.println("Online: ");
                            System.out.println(jLabelInfo);

                            jButtonAbrirActionPerformed(evt);
                        }catch(Exception e){
                            jLabelInfo.setText("Servidor com porta inválida");
                        }
                        
                   }
                }).start();
            }
        });

        jButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                System.exit(0);          
            }
        });

        jLabelNomeArquivo.setText("Nome arquivo");

        jLabelNomeImprimir.setText(" ");

        jLabelTamanho.setText("Tamanho");

        jLabelTamanhoImprimir.setText(" ");

        jButton2.setText("Encerrar servidor");

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGap(25, 25, 25)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addComponent(jLabelPorta)
                        .addGap(29, 29, 29)
                        .addComponent(jTextField1, javax.swing.GroupLayout.PREFERRED_SIZE, 92, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(59, 59, 59)
                        .addComponent(jButtonAbrir))
                    .addComponent(jLabelInfo)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jLabelNomeArquivo)
                            .addComponent(jLabelTamanho))
                        .addGap(18, 18, 18)
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jLabelTamanhoImprimir)
                            .addComponent(jLabelNomeImprimir, javax.swing.GroupLayout.PREFERRED_SIZE, 280, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addComponent(jButton2, javax.swing.GroupLayout.PREFERRED_SIZE, 388, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(22, Short.MAX_VALUE))
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGap(14, 14, 14)
                .addComponent(jLabelInfo)
                .addGap(18, 18, 18)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabelPorta)
                    .addComponent(jTextField1, javax.swing.GroupLayout.PREFERRED_SIZE, 24, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jButtonAbrir))
                .addGap(44, 44, 44)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabelNomeArquivo)
                    .addComponent(jLabelNomeImprimir))
                .addGap(18, 18, 18)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabelTamanho)
                    .addComponent(jLabelTamanhoImprimir))
                .addGap(41, 41, 41)
                .addComponent(jButton2)
                .addContainerGap(69, Short.MAX_VALUE))
        );
        pack();
    }
    
    public class InicializaServidor extends Thread{

        private final Socket socket;
        //private Arquivo arquivo;
        private final byte[] objectAsByte;

        public InicializaServidor(final Socket cliente) throws IOException {
            this.socket = cliente;
            this.objectAsByte = new byte[socket.getReceiveBufferSize()];            
        }    
    
        public void run(){
            try {
            
                System.out.println("Info: aguardando o envio do arquivo pelo cliente.\n");
                
               // byte[] objectAsByte = new byte[socket.getReceiveBufferSize()];

                System.out.println("objectAsByte: " + objectAsByte);

                BufferedInputStream buffer = new BufferedInputStream(this.socket.getInputStream());

                buffer.read(this.objectAsByte);
                
                System.out.println("Buffer: " + buffer);

                arquivo = getObjectFromByte(this.objectAsByte);

                System.out.println(arquivo);

                jLabelNomeImprimir.setText(arquivo.getNome());
                
                long kbSize = arquivo.getTamanhoKB();
                jLabelTamanhoImprimir.setText(kbSize + " KB");
                System.out.println("Tamanho: "+kbSize);

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
        System.out.println("Iniciou - porta: " + port);

        try (ServerSocket server = new ServerSocket(port)) {
            for(;;){
                //jLabelInfo = new javax.swing.JLabel();
                new InicializaServidor(server.accept()).start();
                
            }
        }catch(Exception e){
            System.out.println("Erro: " + e.getMessage());
        }
    }

    protected void jButtonAbrirActionPerformed(java.awt.event.ActionEvent evt) {
        try{
            new Server(Integer.parseInt(jTextField1.getText().trim()));        
        }catch(Exception e){
            System.out.println("Erro1: " + e.getMessage());
        }
    }

    public static void main(String[] args) throws IOException, ClassNotFoundException{
        java.awt.EventQueue.invokeLater(new Runnable() {
        public void run() {
            new Server().setVisible(true);
        }
        });
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