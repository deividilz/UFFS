package Codigos;
import java.io.BufferedOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.util.Timer;
import java.util.TimerTask;

public class Server extends javax.swing.JFrame{
    //INSTANCIANDO AS VARIÁVEIS PARA O MENU GRÁFICO
    private static javax.swing.JLabel jLabelTamanho;
    private static javax.swing.JLabel jLabelTamanhoImprimir;
    private static javax.swing.JLabel jLabelInfo;
    private static javax.swing.JLabel jLabelPorta;
    private static javax.swing.JLabel jLabelNomeArquivo;
    private static javax.swing.JLabel jLabelNomeImprimir;
    private static javax.swing.JTextField jTextField1;
    private static javax.swing.JButton jButtonAbrir;
    private static javax.swing.JButton jButton2;

    public Server(){        //SERVER QUE CHAMARÁ A FUNÇÃO PARA INICIAR OS COMPONENTES
        initComponents();   //EXECUTA A FUNÇÃO PARA INICIAR OS COMPONENTES
    }

     //FUNÇÃO SEM RETORNO QUE INSTANCIA OS COMPONENTES GRÁFICOS
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
        jButtonAbrir.addActionListener(new java.awt.event.ActionListener() {    //FUNÇÃO PARA O EVENTO DE ABRIR PORTA DO SERVIDOR
        public void actionPerformed(java.awt.event.ActionEvent evt) {       //PASSANDO O EVENTO POR PARAMETRO
            new Thread (new Runnable() {
                @Override
                public void run() {
                    try{    //VAI TENTAR ALTERAR O TEXTO DO COMPONENTE
                        Integer.parseInt(jTextField1.getText().trim());
                        jLabelInfo.setText("Servidor online");
                        //System.out.println(jLabelInfo);

                        jButtonAbrirActionPerformed(evt); //CHAMA O EVENTO CASO PRESSIONADO O BOTÃO
                    }catch(Exception e){ //CASO HOUVER ERRO
                        jLabelInfo.setText("Servidor com porta inválida");
                    }            
                }
            }).start(); //CRIANDO A THREAD*/
        }
        });

        jButton2.addActionListener(new java.awt.event.ActionListener() {    //FUNÇÃO PARA O BOTÃO DE EXIT
            public void actionPerformed(java.awt.event.ActionEvent evt) {   //PASSANDO O EVENTO POR PARAMETRO
                System.exit(0);                                             //FECHA A INTERFACE E FINALIZA O SERVIDOR
            }
        });

        //ALTERANDO O TEXTO DOS COMPONENTES GRÁFICOS
        jLabelNomeArquivo.setText("Nome arquivo: ");
        jLabelNomeImprimir.setText(" ");
        jLabelTamanho.setText("Tamanho: ");
        jLabelTamanhoImprimir.setText(" ");
        jButton2.setText("Encerrar servidor");

        //CRIAÇÃO DO LAYOUT GRÁFICO
        
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

    //FUNÇÃO SERVIDOR QUE RECEBE A PORTA QUE SERÁ ABERTA COMO PARAMETRO
    public Server (final int port) throws IOException, ClassNotFoundException {    
        try {
            MulticastSocket msocket = new MulticastSocket(port);
            InetAddress grupo = InetAddress.getByName("230.1.2.3");
            msocket.joinGroup(grupo);

            byte[] dados = new byte[msocket.getReceiveBufferSize()];
            DatagramPacket packet = new DatagramPacket(dados, dados.length);

            msocket.receive(packet);
            
            String info = new String(packet.getData(), 0, packet.getLength());
            String array[] = new String[4];
            array = info.split(",");           
            String diretorio = "C:/Users/Deividi/Downloads/folder/" + array[0];
            
            int tamanho = Integer.parseInt(array[1])*1025;

            if(tamanho == 0){
                tamanho = 1000;
            }

            byte[] dados2 = new byte[tamanho];
            DatagramPacket packet2 = new DatagramPacket(dados2, dados2.length);
            msocket.receive(packet2);

            BufferedOutputStream fos = new BufferedOutputStream(new FileOutputStream(diretorio));
            fos.write(packet2.getData());
            fos.flush();
            fos.close();
            msocket.close();

            jLabelNomeImprimir.setText(array[0]);
            jLabelTamanhoImprimir.setText(array[1] + "KB");

            System.out.println("Info: arquivo recebido.\n");

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    //FUNÇÃO PARA O BOTÃO QUE IRÁ ABRIR A PORTA DO SERVIDOR
    protected void jButtonAbrirActionPerformed(java.awt.event.ActionEvent evt) throws IOException {
        int porta = Integer.parseInt(jTextField1.getText().trim());
        System.out.println("Servidor online > porta: " + porta );
        Timer timer = new Timer();
        
        timer.scheduleAtFixedRate(new TimerTask() {
            public void run() {
                try (MulticastSocket msocket = new MulticastSocket()) {
                    InetAddress grupo = InetAddress.getByName("230.1.2.3");
                    msocket.joinGroup(grupo);

                    String mensagem = "Servidor online > porta: " + porta;
                    byte[] envia = mensagem.getBytes();

                    DatagramPacket pac = new DatagramPacket(envia, envia.length, grupo, 7777);

                    msocket.send(pac);
                    msocket.close();

                } catch (IOException e) {
                    System.out.println("Tentando novamente.\n");
                }
            }
        }, 0, 30000);

        while(true){
            try{
                new Server(porta); //TENTA CRIR UM SERVIDOR PASSANDO A PORTA INFORMADA COMO PARAMETRO
            }catch(Exception e){
                System.out.println("Erro1: " + e.getMessage());
            }
        }
    }

    //FUNÇÃO PRINCIPAL QUE IRÁ INICIALIZAR O PROGRAMA
    public static void main(String[] args) throws IOException, ClassNotFoundException{
        //THREAD PARA ATUALIZAR O COMPONENTE DURANTE EXECUÇÃO
        java.awt.EventQueue.invokeLater(new Runnable() { //CRIA UMA FILA DE EVENTOS
        public void run() {
            new Server().setVisible(true); //CRIA UM NOVO SERVIDOR DEFININDO A VISIBILIDADE COMO ATIVA
        }
        });
    }
}
