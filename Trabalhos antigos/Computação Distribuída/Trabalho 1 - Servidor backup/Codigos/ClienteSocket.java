package Codigos;

//IMPORT DAS BIBLIOTES NECESSÁRIAS
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Date;

import javax.swing.JFileChooser;

//CLASSE CLIENTE SOCKET PRINCIPAL
public class ClienteSocket extends javax.swing.JFrame {
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

    private Arquivo arquivo;   

    private static final long serialVersionUID = 1L;

    public ClienteSocket() {    //CLIENTE SOCKET QUE CHAMARÁ A FUNÇÃO PARA INICIAR OS COMPONENTES
        initComponents();       //EXECUTA A FUNÇÃO PARA INICIAR OS COMPONENTES
    }

    //FUNÇÃO SEM RETORNO QUE INSTANCIA OS COMPONENTES GRAFICOS
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

        jTextField1.setText("Informe aqui um diretorio");
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
        // TODO add your handling code here:
    }  
    
    private void jButtonEnviarActionPerformed(java.awt.event.ActionEvent evt) {     //FUNÇÃO PARA O EVENTO DE ENVIAR O ARQUIVO PARA O SERVIDOR
        enviarArquivoServidor();
    }
    
    private void jButtonArquivoActionPerformed(java.awt.event.ActionEvent evt) {    //FUNÇÃO PARA ESCOLHER O ARQUIVO
        FileInputStream fis;    //INSTANCIA UMA VARIÁVEL PARA RECEBER UM ARQUIVO DE ENTRADA
        try {
        
        //CRIA UMA VARIAVEL PARA ESCOLHER O ARQUIVO
        JFileChooser escolha = new JFileChooser();
        escolha.setFileSelectionMode(JFileChooser.FILES_ONLY);
        escolha.setDialogTitle("Escolha o arquivo");
            
        if (escolha.showOpenDialog(this) == JFileChooser.OPEN_DIALOG) { //SE SELECIONAR O ARQUIVO
            File arquivoSelecionado = escolha.getSelectedFile();  //VARIAVEL RECEBE O ARQUIVO SELECIONADO
    
            byte[] bFile = new byte[(int) arquivoSelecionado.length()];   //EXTRAI OS BYTES DA VARIÁVEL
            fis = new FileInputStream(arquivoSelecionado);                //CRIA UM ARQUIVO DE ENTRADA
            fis.read(bFile);                                        //FAZ A LEITURA DOS BYTES
            fis.close();                                            //FECHA A ESCOLHA DE ARQUIVOS
    
            //DEFINE O TAMANHO DO ARQUIVO TRANSFORMANDO PARA KBs
            long kbTamanho = arquivoSelecionado.length() / 1024;
            jTextFieldNome.setText(arquivoSelecionado.getName());
            jLabelTamanho.setText(kbTamanho + " KB");
            
            //CRIA UM NOVO ARQUIVO QUE É RESPONSÁVEL POR RECEBER OS DADOS
            arquivo = new Arquivo();
            arquivo.setConteudo(bFile);                 //DEFINE O CONTEUDO DO ARQUIVO
            arquivo.setDataHoraUpload(new Date());      //DEFINE A HORA DE UPLOAD
            arquivo.setNome(arquivoSelecionado.getName());    //DEFINE O NOME DO ARQUIVO
            arquivo.setTamanhoKB(kbTamanho);               //DEFINE O TAMANHO EM KBs
            arquivo.setIpDestino(jTextFieldIP.getText()); //DEFINE O IP DE DESTINO (SERVIDOR)
            arquivo.setPortaDestino(jTextFieldPorta.getText()); //DEFINE A PORTA DE DESTINO (SERVIDOR)
            //VARIAVEL COM O DIRETORIO
            //String diretorio = "C:/Users/Deividi/Documents/GitHub/UFFS/Trabalhos antigos/Computação Distribuída/Trabalho 1 - Servidor backup/Servidor backup - Arquivos";
            String diretorio = jTextField1.getText();
            arquivo.setDiretorioDestino(diretorio); //DEFINE O DIRETÓRIO DO ARQUIVO
        }
    
        } catch (Exception e) {
            System.out.println("Erro: não foi possível enviar o arquivo, verifique os dados..");
            jLabelAguardando.setText("Falha no envio");   
        }
    }
    
    //FUNÇÃO PARA ENVIAR O ARQUIVO PARA O SERVIDOR
    private void enviarArquivoServidor(){

        //THREAD PARA ATUALIZAR O COMPONENTE DURANTE EXECUÇÃO
        new Thread (new Runnable() {
                @Override
                public void run() {
                    while(true) {
                        
                        System.out.println("Dados :");
                        try {
                          MulticastSocket mcs = new MulticastSocket(12347);
                          InetAddress grp = InetAddress.getByName("239.0.0.1");
                          mcs.joinGroup(grp);
                          byte rec[] = new byte[256];
                          DatagramPacket pkg = new DatagramPacket(rec, rec.length);
                          mcs.receive(pkg);
                          String data = new String(pkg.getData());
                          System.out.println("Dados recebidos:" + data);
                      }
                      catch(Exception e) {
                        System.out.println("Erro: " + e.getMessage());
                      }
                    }
            }
            }).start(); //CRIANDO A THREAD
        
        try {
            Socket socket = new Socket(jTextFieldIP.getText().trim(),  //CRIA UM SOCKET COM OS DADOS DE DESTINO
            Integer.parseInt(jTextFieldPorta.getText().trim()));       //COM A PORTA DE DESTINO
    
            BufferedOutputStream bf = new BufferedOutputStream(socket.getOutputStream());   //DEFINE UM BUFFER DE SAIDA PARA O ENVIO
    
            byte[] bytea = serializarArquivo();     //EXTRAI OS DADOS O OBJETO EM BYTES PARA PODER COMPACTA-LOS NOVAMENTE NO SERVIDOR
            bf.write(bytea);                        //ESCREVE OS BYTES
            bf.flush();                             //FORÇA O SALVAMENTO EM ARQUIVO
            bf.close();
            //socket.close();
            System.out.println("Info: enviado com sucesso.");
            jLabelAguardando.setText("Enviado com sucesso");
            
        } catch (UnknownHostException e) {
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
    }
    
    //FUNÇÃO PARA EXTRAIR OS DADOS/BYTES DO ARQUIVO
    private byte[] serializarArquivo(){
        try {
            ByteArrayOutputStream bao = new ByteArrayOutputStream();    //DEFINE UM ARRAY DE BYTES PARA A SAÍDA
            ObjectOutputStream ous;                                     //DEFINE UM OBJETO DE SAÍDA
            ous = new ObjectOutputStream(bao);                          //OBJETO RECEBE UM NOVO ARRAY
            ous.writeObject(arquivo);                                   //ESCREVE O OBJETO PARA BYTE NA SAIDA DE ARRAYS
            return bao.toByteArray();                                   //RETORNA O ARRAY COM OS DADOS DO ARQUIVO EXTRAIDO
        } catch (IOException e) {
            System.out.println("Erro: não foi possível extrair os dados do arquivo..\n");
            jLabelAguardando.setText("Erro na extração dos dados");   
        }
    
        return null;
    }
    
    //FUNÇÃO PRINCIPAL QUE CRIA O CLIENTE SOCKET COM VISUALIZAÇÃO ATIVA
    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {
        public void run() {
            new ClienteSocket().setVisible(true);
        }
        });
    }
}
