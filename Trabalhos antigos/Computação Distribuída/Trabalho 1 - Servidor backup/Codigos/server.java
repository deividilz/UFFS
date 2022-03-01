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

        //FUNÇÃO PARA O BOTÃO DE ABRIR PORTA
        jButtonAbrir.setText("Abrir porta");
        jButtonAbrir.addActionListener(new java.awt.event.ActionListener() {    //FUNÇÃO PARA O EVENTO DE ABRIR PORTA DO SERVIDOR
        public void actionPerformed(java.awt.event.ActionEvent evt) {       //PASSANDO O EVENTO POR PARAMETRO
            new Thread (new Runnable() { //CRIA UMA NOVA THREAD
                @Override
                public void run() {
                    try{    //VAI TENTAR ALTERAR O TEXTO DO COMPONENTE
                        Integer.parseInt(jTextField1.getText().trim());
                        jLabelInfo.setText("Servidor online");
                        jButtonAbrirActionPerformed(evt); //CHAMA O EVENTO CASO PRESSIONADO O BOTÃO
                    }catch(Exception e){ //CASO HOUVER ERRO
                        jLabelInfo.setText("Servidor com porta inválida");
                    }            
                }
            }).start(); //INICIALIZA A THREAD
        }
        });

        //FUNÇÃO PARA O BOTÃO DE FECHAR PORTA
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

         //CRIAÇÃO DO LAYOUT GRÁFICO - GERADO ATRAVÉS DO NETBEANS        
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
            MulticastSocket msocket = new MulticastSocket(port);                //DEFINE UM SOCKET MULTICAST
            InetAddress grupo = InetAddress.getByName("230.1.2.3");             //ADICIONA O GRUPO NO ENDEREÇO
            msocket.joinGroup(grupo);                                           //VINCULA O SOCKET NO GRUPO

            byte[] dados = new byte[msocket.getReceiveBufferSize()];            //RECEBE O BYTE COM OS DADOS DO SOCKET
            DatagramPacket packet = new DatagramPacket(dados, dados.length);    //CRIA UM DATAGRAM COM OS DADOS

            msocket.receive(packet);                                            //SOCKET RECEBE O SEGUNDO PACK COM INFORMAÇÕES DO ARQUIVO
            
            String info = new String(packet.getData(), 0, packet.getLength());  //INFO RECEBE A MENSAGEM DO CLIENTE
            String array[] = new String[4];                                     //INSTANCIA UM ARRAY QUE RECEBE STRING
            array = info.split(",");                                            //SPLIT PARA SEPARAR O ARRAY E PEGAR A PORTA CORRETAMENTE
            
            String diretorio = array[2] + array[0];                             //DIRETORIO RECEBE A INFO DO DIRETORIO + O NOME DO ARQUIVO PARA SALVAR
            int tamanho = Integer.parseInt(array[1])*1025;                      //TAMANHO RECEBE A INFO DO TAMANHO CONVERTIDO PARA INTEIRO

            if(tamanho == 0){                                                   //SE O TAMANHO FOR 0kb (ele pode estar entre 0,1 e 0,9)
                tamanho = 1000;                                                 //ENTÃO DEFINE O TAMANHO DELE PARA 1kb
            }

            byte[] dados2 = new byte[tamanho];                                  //SEGUNDO DADO
            DatagramPacket packet2 = new DatagramPacket(dados2, dados2.length); //CRIA UM DATAGRAMA COM OS DADOS
            msocket.receive(packet2);                                           //SOCKET RECEBE O SEGUNDO PACK COM INFORMAÇÕES DO ARQUIVO

            BufferedOutputStream fos = new BufferedOutputStream(new FileOutputStream(diretorio));   //CRIA UM OUTPUT STREAM RECEBENDO DO BUFFER O ARQUIVO VIA SOCKET
            fos.write(packet2.getData());                                       //ESCREVE O ARQUIVO RECEBIDO VIA DATAGRAM
            fos.flush();                                                        //FORÇA A ESCRITA NO DIRETÓRIO
            fos.close();                                                        //FECHA O OUTPUT STREAM
            msocket.close();                                                    //FECHA O SOCKET

            jLabelNomeImprimir.setText(array[0]);                               //IMPRIME O NOME DO ARQUIVO NA TELA
            jLabelTamanhoImprimir.setText(array[1] + "KB");                     //IMPRIME O TAMANHO DO ARQUIVO NA TELA

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
        
        //CRIA UM TIMER COM TEMPORIZADOR - EXECUTA NO INSTANTE ZERO E DEPOIS DE 30 EM 30 SEGUNDOS
        timer.scheduleAtFixedRate(new TimerTask() {
            public void run() {
                try (MulticastSocket msocket = new MulticastSocket()) {                             //TENTA CRIAR UM NOVO MULTICAST
                    InetAddress grupo = InetAddress.getByName("230.1.2.3");                         //ADICIONA O GRUPO NO ENDEREÇO
                    msocket.joinGroup(grupo);                                                       //VINCULA O SOCKET NO GRUPO

                    String mensagem = "Servidor online > porta: " + porta;
                    byte[] envia = mensagem.getBytes();

                    DatagramPacket pac = new DatagramPacket(envia, envia.length, grupo, 7777);      //CRIA UM DATAGRAM PARA ENVIAR OS DADOS

                    msocket.send(pac);  //ENVIA O DATAGRAM
                    msocket.close();    //FECHA O SOCKET

                } catch (IOException e) {
                    System.out.println("Tentando novamente.\n");
                }
            }
        }, 0, 30000);   //EXECUTA NO INSTANTE ZERO E DEPOIS DE 30 EM 30 SEGUNDOS

        while(true){    //ENQUANTO VERDADEIRO
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