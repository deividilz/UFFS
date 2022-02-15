package Codigos;

//IMPORT DAS BIBLIOTES NECESSÁRIAS
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Date;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

//CLASSE CLIENTE SOCKET PRINCIPAL
public class ClienteSocket extends javax.swing.JFrame {
    //INSTANCIANDO AS VARIÁVEIS PARA O MENU GRÁFICO
    private javax.swing.JButton jButtonArquivo;
    private javax.swing.JButton jButtonEnviar;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JLabel jLabelTamanho;
    private javax.swing.JTextField jTextFieldIP;
    private javax.swing.JTextField jTextFieldNome;
    private javax.swing.JTextField jTextFieldPorta;

    private long tamanhoPermitidoKB = 5120; //DEFINE O TAMANHO MÁXIMO DO ARQUIVO
    private Arquivo arquivo;   

    private static final long serialVersionUID = 1L;

    public ClienteSocket() {    //CLIENTE SOCKET QUE CHAMARÁ A FUNÇÃO PARA INICIAR OS COMPONENTES
        initComponents();       //EXECUTA A FUNÇÃO PARA INICIAR OS COMPONENTES
    }

    //FUNÇÃO SEM RETORNO QUE INSTANCIA OS COMPONENTES GRAFICOS
    private void initComponents() {
        jLabel1 = new javax.swing.JLabel();
        jTextFieldNome = new javax.swing.JTextField();
        jButtonArquivo = new javax.swing.JButton();
        jLabelTamanho = new javax.swing.JLabel();
        jButtonEnviar = new javax.swing.JButton();
        jLabel2 = new javax.swing.JLabel();
        jTextFieldIP = new javax.swing.JTextField();
        jLabel3 = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        jTextFieldPorta = new javax.swing.JTextField();
        jLabel5 = new javax.swing.JLabel();
        
        this.setLocationRelativeTo(null);
        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        jLabel1.setText("Arquivo carregado");

        jTextFieldNome.setEnabled(false);

        jButtonArquivo.setText("Selecionar Arquivo");
        jButtonArquivo.addActionListener(new java.awt.event.ActionListener() {  //FUNÇÃO PARA SELECIONAR O ARQUIVO
            public void actionPerformed(java.awt.event.ActionEvent evt) {       //PASSANDO O EVENTO POR PARAMETRO
                jButtonArquivoActionPerformed(evt);                             //CHAMA A FUNÇÃO PASSANDO O EVENTO OCORRIDO
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
        jLabel2.setText("IP");
        jLabel3.setText("Porta");
        jLabel4.setText("Tamanho");
        jLabel5.setText("Aguardando envio");

        //CRIAÇÃO DE TODO O LAYOUT GRÁFICO
        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(getContentPane());
            getContentPane().setLayout(jPanel1Layout);
            jPanel1Layout.setHorizontalGroup(
                jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(jPanel1Layout.createSequentialGroup()
                    .addGap(16, 16, 16)
                    .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                        .addComponent(jButtonEnviar, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jLabel1)
                        .addComponent(jButtonArquivo, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jTextFieldNome)
                        .addGroup(jPanel1Layout.createSequentialGroup()
                            .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                                .addComponent(jLabel4, javax.swing.GroupLayout.DEFAULT_SIZE, 66, Short.MAX_VALUE)
                                .addComponent(jLabel3, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(jLabel2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                            .addGap(18, 18, 18)
                            .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel1Layout.createSequentialGroup()
                                .addComponent(jLabelTamanho)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(jLabel5))
                            .addComponent(jTextFieldIP, javax.swing.GroupLayout.DEFAULT_SIZE, 306, Short.MAX_VALUE)
                            .addComponent(jTextFieldPorta))))
                .addContainerGap(17, Short.MAX_VALUE))
            );
            jPanel1Layout.setVerticalGroup(
                jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(jPanel1Layout.createSequentialGroup()
                    .addGap(14, 14, 14)
                    .addComponent(jLabel1)
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                    .addComponent(jTextFieldNome, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGap(18, 18, 18)
                    .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(jLabel2, javax.swing.GroupLayout.PREFERRED_SIZE, 26, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addComponent(jTextFieldIP, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGap(8, 8, 8)
                    .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(jLabel3)
                        .addComponent(jTextFieldPorta, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGap(18,18,18)
                    .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                        .addComponent(jLabel4)
                        .addComponent(jLabelTamanho)
                        .addComponent(jLabel5))
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 43, Short.MAX_VALUE)
                    .addComponent(jButtonArquivo)
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                    .addComponent(jButtonEnviar)
                    .addGap(21, 21, 21))
            );
        pack();
    }

    private void jButtonEnviarActionPerformed(java.awt.event.ActionEvent evt) {     //FUNÇÃO PARA O EVENTO DE ENVIAR O ARQUIVO PARA O SERVIDOR
        enviarArquivoServidor();
    }
    
    private void jButtonArquivoActionPerformed(java.awt.event.ActionEvent evt) {    //FUNÇÃO PARA ESCOLHER O ARQUIVO
        FileInputStream fis;    //INSTANCIA UMA VARIÁVEL PARA RECEBER UM ARQUIVO DE ENTRADA
        try {
        
        //CRIA UMA VARIAVEL PARA ESCOLHER O ARQUIVO
        JFileChooser chooser = new JFileChooser();
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        chooser.setDialogTitle("Escolha o arquivo");
            
        if (chooser.showOpenDialog(this) == JFileChooser.OPEN_DIALOG) { //SE SELECIONAR O ARQUIVO
            File fileSelected = chooser.getSelectedFile();  //VARIAVEL RECEBE O ARQUIVO SELECIONADO
    
            byte[] bFile = new byte[(int) fileSelected.length()];   //EXTRAI OS BYTES DA VARIÁVEL
            fis = new FileInputStream(fileSelected);                //CRIA UM ARQUIVO DE ENTRADA
            fis.read(bFile);                                        //FAZ A LEITURA DOS BYTES
            fis.close();                                            //FECHA A ESCOLHA DE ARQUIVOS
    
            //DEFINE O TAMANHO DO ARQUIVO TRANSFORMANDO PARA KBs
            long kbSize = fileSelected.length() / 1024;
            jTextFieldNome.setText(fileSelected.getName());
            jLabelTamanho.setText(kbSize + " KB");
            
            //CRIA UM NOVO ARQUIVO QUE É RESPONSÁVEL POR RECEBER OS DADOS
            arquivo = new Arquivo();
            arquivo.setConteudo(bFile);                 //DEFINE O CONTEUDO DO ARQUIVO
            arquivo.setDataHoraUpload(new Date());      //DEFINE A HORA DE UPLOAD
            arquivo.setNome(fileSelected.getName());    //DEFINE O NOME DO ARQUIVO
            arquivo.setTamanhoKB(kbSize);               //DEFINE O TAMANHO EM KBs
            arquivo.setIpDestino(jTextFieldIP.getText()); //DEFINE O IP DE DESTINO (SERVIDOR)
            arquivo.setPortaDestino(jTextFieldPorta.getText()); //DEFINE A PORTA DE DESTINO (SERVIDOR)
            //VARIAVEL COM O DIRETORIO
            String diretorio = "C:/Users/User/Documents/GitHub/UFFS/Trabalhos antigos/Computação Distribuída/Trabalho 1 - Servidor backup/Servidor backup - Arquivos";
            arquivo.setDiretorioDestino(diretorio); //DEFINE O DIRETÓRIO DO ARQUIVO
        }
    
        } catch (Exception e) {
            System.out.println("Erro: não foi possível enviar o arquivo, verifique os dados..");
            jLabel5.setText("Falha no envio");   
        }
    }
    
    //FUNÇÃO PARA ENVIAR O ARQUIVO PARA O SERVIDOR
    private void enviarArquivoServidor(){
        if (validaArquivo() == true){   //EFETUA A VALIDAÇÃO DO ARQUIVO SELECIONADO
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
                jLabel5.setText("Enviado com sucesso");
                
            } catch (UnknownHostException e) {
                System.out.println("Erro: não foi possível encontrar o servidor informado..\n");
                jLabel5.setText("Servidor não encontrado");
            } catch (IOException e) {
                System.out.println("Erro: não foi possível encontrar o servidor informado..\n");
                jLabel5.setText("Servidor não encontrado");
            }catch(NullPointerException e){
                System.out.println("Erro: dados não informados corretamente..\n");  
                jLabel5.setText("Dados preenchidos incorretos");  
            }catch(NumberFormatException e){
                System.out.println("Erro: dados não informados corretamente..\n"); 
                jLabel5.setText("Dados preenchidos incorretos");   
            }
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
            jLabel5.setText("Erro na extração dos dados");   
        }
    
        return null;
    }

    //FUNÇÃO PARA VALIDAR O ARQUIVO
    private boolean validaArquivo(){
        try{
            if (arquivo.getTamanhoKB() > tamanhoPermitidoKB){  //CASO O TAMANHO FOR MAIOR QUE O PERMITIDO, NAO CARREGA, RETORNANDO FALSE
                JOptionPane.showMessageDialog(this,
                    "Tamanho máximo permitido atingido ("+(tamanhoPermitidoKB/1024)+")");
                return false;
            }else{
                return true;                                   //SE FOR ACEITO, RETORNA TRUE
            }
        }catch (NullPointerException e) {
            System.out.println("Erro: verifique os dados informados..\n");
            jLabel5.setText("Dados incorretos");   
        }
        return false;
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
