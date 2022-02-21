package Codigos2;
import java.io.*;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.UnknownHostException;
import java.util.ArrayList;

import javax.swing.JFileChooser;


public class Cliente extends javax.swing.JFrame{
    //INSTANCIANDO AS VARIÁVEIS PARA O MENU GRÁFICO
    private javax.swing.JButton jButtonArquivo;
    private javax.swing.JButton jButtonEnviar;
    private javax.swing.JLabel jLabelArquivo;
    private javax.swing.JLabel jLabelIP;
    private javax.swing.JLabel jLabelPorta;
    private javax.swing.JLabel jLabelTamanhoL;
    private javax.swing.JLabel jLabelAguardando;
    private javax.swing.JLabel jLabelTamanho;
    private javax.swing.JTextField jTextFieldIP;
    private javax.swing.JTextField jTextFieldNome;
    private javax.swing.JTextField jTextFieldPorta;
    private javax.swing.JTextField jTextField1;
    
    private File arquivoSelecionado;
    private String informacoes;
    private long kbTamanho;
    private static ArrayList<Integer> servidorOnline = new ArrayList<>();
    
    public Cliente() {    //CLIENTE SOCKET QUE CHAMARÁ A FUNÇÃO PARA INICIAR OS COMPONENTES
        initComponents();       //EXECUTA A FUNÇÃO PARA INICIAR OS COMPONENTES
    }

    private void initComponents() {
        jLabelArquivo = new javax.swing.JLabel();
        jTextFieldNome = new javax.swing.JTextField();
        jButtonArquivo = new javax.swing.JButton();
        jLabelTamanho = new javax.swing.JLabel();
        jButtonEnviar = new javax.swing.JButton();
        jLabelIP = new javax.swing.JLabel();
        jTextFieldIP = new javax.swing.JTextField();
        jLabelPorta = new javax.swing.JLabel();
        jLabelTamanhoL = new javax.swing.JLabel();
        jTextFieldPorta = new javax.swing.JTextField();
        jLabelAguardando = new javax.swing.JLabel();
        jTextField1 = new javax.swing.JTextField();
        

        this.setLocationRelativeTo(null);
        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        jLabelArquivo.setText("Selecione o arquivo");

        jTextFieldNome.setEnabled(false);

        jButtonArquivo.setText("Selecionar Arquivo");
        jButtonArquivo.addActionListener(new java.awt.event.ActionListener() {  //FUNÇÃO PARA SELECIONAR O ARQUIVO
            public void actionPerformed(java.awt.event.ActionEvent evt) {       //PASSANDO O EVENTO POR PARAMETRO
                jButtonArquivoActionPerformed(evt);                             //CHAMA A FUNÇÃO PASSANDO O EVENTO OCORRIDO
                jLabelArquivo.setText("Arquivo carregado");
                
            }
        });

        jLabelTamanho.setFont(new java.awt.Font("Dialog", 0, 12));
        jLabelTamanho.setText("KB");

        jButtonEnviar.setText("Enviar");
        jButtonEnviar.addActionListener(new java.awt.event.ActionListener() {   //FUNÇÃO PARA ENVIAR O ARQUIVO SELECIONADO
            public void actionPerformed(java.awt.event.ActionEvent evt) {       //PASSANDO O EVENTO POR PARAMETRO
                jButtonEnviarActionPerformed(evt);                              //CHAMA A FUNÇÃO PASSANDO O EVENTO OCORRIDO
            }
        });

        //ALTERANDO O TEXTO DOS COMPONENTES GRÁFICOS
        jLabelIP.setText("IP");
        jLabelPorta.setText("Porta");
        jLabelTamanhoL.setText("Tamanho");
        jLabelAguardando.setText("Aguardando envio");

        jTextFieldIP.setText("230.1.2.3");

        jTextField1.setText("C:/Users/Deividi/Downloads/folder/");
        jTextField1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jTextField1ActionPerformed(evt);
            }
        });

        //CRIAÇÃO DO LAYOUT GRÁFICO
        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(getContentPane());
            getContentPane().setLayout(jPanel1Layout);
            jPanel1Layout.setHorizontalGroup(
                jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(jPanel1Layout.createSequentialGroup()
                    .addGap(16, 16, 16)
                    .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                        .addComponent(jButtonEnviar, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jLabelArquivo)
                        .addComponent(jButtonArquivo, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jTextFieldNome)
                        .addGroup(jPanel1Layout.createSequentialGroup()
                            .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                                .addComponent(jLabelTamanhoL, javax.swing.GroupLayout.DEFAULT_SIZE, 66, Short.MAX_VALUE)
                                .addComponent(jLabelPorta, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(jLabelIP, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                            .addGap(18, 18, 18)
                            .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel1Layout.createSequentialGroup()
                                .addComponent(jLabelTamanho)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(jLabelAguardando))
                                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jTextFieldIP, javax.swing.GroupLayout.DEFAULT_SIZE, 306, Short.MAX_VALUE)
                            .addComponent(jTextFieldPorta))))
                        .addComponent(jTextField1))
                    .addContainerGap(17, Short.MAX_VALUE))
            );
            jPanel1Layout.setVerticalGroup(
                jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(jPanel1Layout.createSequentialGroup()
                    .addGap(14, 14, 14)
                    .addComponent(jLabelArquivo)
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                    .addComponent(jTextFieldNome, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGap(18, 18, 18)
                    .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(jLabelIP, javax.swing.GroupLayout.PREFERRED_SIZE, 26, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(jTextFieldIP, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGap(8, 8, 8)
                    .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(jLabelPorta)
                        .addComponent(jTextFieldPorta, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGap(18,18,18)
                    .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(jLabelTamanhoL)
                        .addComponent(jLabelTamanho)
                        .addComponent(jLabelAguardando))
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 24, Short.MAX_VALUE)
                    .addComponent(jTextField1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)   
                    .addGap(18, 18, 18)
                    .addComponent(jButtonArquivo)
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                    .addComponent(jButtonEnviar)
                    .addGap(21, 21, 21))
            );
        pack();
    }

    private void jTextField1ActionPerformed(java.awt.event.ActionEvent evt) {                                            

    }  
    
    private void jButtonEnviarActionPerformed(java.awt.event.ActionEvent evt) {     //FUNÇÃO PARA O EVENTO DE ENVIAR O ARQUIVO PARA O SERVIDOR
        if (arquivoSelecionado != null){
            String diretorio_local = jTextField1.getText();
            File diretorio_verifica = new File(diretorio_local);

            if(!diretorio_verifica.exists()){
                System.out.println("Erro: diretório informado não existe");
                jLabelAguardando.setText("Diretório incorreto");
            }else{
                if(kbTamanho < 1024*3){
                    enviarArquivoServidor();
                }else{
                    System.out.println("Erro: arquivo muito pesado");
                    jLabelAguardando.setText("Arquivo muito pesado");
                }
            }
            
        }else{
            System.out.println("Erro: arquivo não selecionado.");
            jLabelAguardando.setText("Arquivo não selecionado");
        }        
    }
    
    private void jButtonArquivoActionPerformed(java.awt.event.ActionEvent evt) {    //FUNÇÃO PARA ESCOLHER O ARQUIVO
        FileInputStream fis;    //INSTANCIA UMA VARIÁVEL PARA RECEBER UM ARQUIVO DE ENTRADA
        try {
        
            //CRIA UMA VARIAVEL PARA ESCOLHER O ARQUIVO
            JFileChooser escolha = new JFileChooser();
            escolha.setFileSelectionMode(JFileChooser.FILES_ONLY);
            escolha.setDialogTitle("Escolha o arquivo");
                
            if (escolha.showOpenDialog(this) == JFileChooser.OPEN_DIALOG) { //SE SELECIONAR O ARQUIVO
                arquivoSelecionado = escolha.getSelectedFile();  //VARIAVEL RECEBE O ARQUIVO SELECIONADO
                
                byte[] bFile = new byte[(int) arquivoSelecionado.length()];   //EXTRAI OS BYTES DA VARIÁVEL
                fis = new FileInputStream(arquivoSelecionado);                //CRIA UM ARQUIVO DE ENTRADA
                fis.read(bFile);                                        //FAZ A LEITURA DOS BYTES
                fis.close();                                            //FECHA A ESCOLHA DE ARQUIVOS
                
                //DEFINE O TAMANHO DO ARQUIVO TRANSFORMANDO PARA KBs
                kbTamanho = arquivoSelecionado.length() / 1024;
                jTextFieldNome.setText(arquivoSelecionado.getName());
                jLabelTamanho.setText(kbTamanho + " KB");    

                informacoes = "";
                informacoes = arquivoSelecionado.getName() + "," + kbTamanho + "," + jTextField1.getText() + "," + jTextFieldPorta.getText();
            }    
        } catch (Exception e) {
            System.out.println("Erro: não foi possível enviar o arquivo, verifique os dados..");
            jLabelAguardando.setText("Falha no envio");   
        }
    }

    //FUNÇÃO PARA ENVIAR O ARQUIVO PARA O SERVIDOR
    private void enviarArquivoServidor(){    
        int porta = Integer.parseInt(jTextFieldPorta.getText());

        if(servidorOnline.contains(porta)){
            try {
                // BufferedInputStream bis = new BufferedInputStream(new FileInputStream("C://Users/Deividi/Downloads/folder/arquivo.txt"));
                 DatagramSocket socket = new DatagramSocket();
     
                 BufferedInputStream bis = new BufferedInputStream(new FileInputStream(arquivoSelecionado));
                 int length=bis.available();
     
                 byte[] dados = new byte[length];
                 byte[] dados2 = informacoes.getBytes();
     
                 bis.read(dados);
     
                 String ip = jTextFieldIP.getText();
                 
                 InetAddress grupo = InetAddress.getByName(ip);
     
                 jTextFieldIP.setText(ip);
     
                 System.out.println("Ip: " + ip + " Porta: " +porta);
     
                 DatagramPacket packet = new DatagramPacket(dados,dados.length, grupo, porta);
                 DatagramPacket packet2 = new DatagramPacket(dados2,dados2.length, grupo, porta);
                 
                 socket.send(packet2);
                 socket.send(packet);
                 socket.close();
                 bis.close();
     
                 System.out.println("Info: enviado com sucesso.");
                 jLabelAguardando.setText("Enviado com sucesso");
             }catch (UnknownHostException e) {
                 System.out.println("Erro: não foi possível encontrar o servidor informado..\n");
                 jLabelAguardando.setText("Servidor não encontrado");
             } catch (IOException e) {
                 System.out.println("Erro: não foi possível encontrar o servidor informado..\n");
                 jLabelAguardando.setText("Servidor não encontrado");
             }catch(NullPointerException e){
                 System.out.println("Erro: dados não informados corretamente..\n");  
                 jLabelAguardando.setText("Dados preenchidos incorretos");  
             }catch(NumberFormatException e){
                 System.out.println("Erro: dados não informados corretamente..\n"); 
                 jLabelAguardando.setText("Dados preenchidos incorretos");   
             }
        }else{
            System.out.println("Erro: servidor offline.");
            jLabelAguardando.setText("Servidor offline");
        }     
    }

    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {
        public void run() {
            //THREAD PARA ATUALIZAR O COMPONENTE DURANTE EXECUÇÃO
            new Thread (new Runnable() {
                @Override
                public void run() {
                    while(true) {
                        
                        try {
                            MulticastSocket mcs = new MulticastSocket(7777);
                            InetAddress grupo = InetAddress.getByName("230.1.2.3");
                            mcs.joinGroup(grupo);

                            byte rec[] = new byte[256];
                            DatagramPacket pkg = new DatagramPacket(rec, rec.length);

                            mcs.receive(pkg);
                            mcs.close();
                            String data = new String(pkg.getData());
                            
                            String array[] = new String[7];
                            array = data.split(": ");
                            int porta = Integer.parseInt(array[1].trim());

                            if(!servidorOnline.contains(porta)) {    
                                servidorOnline.add(porta);
                            }
                            
                            System.out.println(data);
                        }catch(Exception e) {
                            System.out.println("Erro: " + e.getMessage());
                        }
                    }
                }
            }).start(); //CRIANDO A THREAD
            
            new Cliente().setVisible(true);
        }
        });
    }
}
