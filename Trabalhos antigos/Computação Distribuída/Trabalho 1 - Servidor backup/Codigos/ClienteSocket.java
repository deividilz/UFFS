package Codigos;


import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

//import br.com.transientfield.bean.Arquivo;

public class ClienteSocket extends javax.swing.JFrame {

    private long tamanhoPermitidoKB = 5120; //Igual a 5MB
    private Arquivo arquivo;


    private static final long serialVersionUID = 1L;

    public ClienteSocket() {
            initComponents();
    }

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

        this.setLocationRelativeTo(null);
        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        jLabel1.setText("Arquivo carregado");

        jTextFieldNome.setEnabled(false);

        jButtonArquivo.setText("Selecionar Arquivo");
        jButtonArquivo.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonArquivoActionPerformed(evt);
            }
        });

        jLabelTamanho.setFont(new java.awt.Font("Dialog", 0, 12));
        jLabelTamanho.setText("KB");

        jButtonEnviar.setText("Enviar");
        jButtonEnviar.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButtonEnviarActionPerformed(evt);
            }
        });

        jLabel2.setText("IP");

        jLabel3.setText("Porta");

        jLabel4.setText("Tamanho");

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
                                .addComponent(jLabelTamanho)
                                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(jTextFieldIP, javax.swing.GroupLayout.DEFAULT_SIZE, 306, Short.MAX_VALUE)
                                    .addComponent(jTextFieldPorta)))))
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
                        .addComponent(jLabelTamanho))
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 43, Short.MAX_VALUE)
                    .addComponent(jButtonArquivo)
                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                    .addComponent(jButtonEnviar)
                    .addGap(21, 21, 21))
            );
        pack();
    }

    private void jButtonEnviarActionPerformed(java.awt.event.ActionEvent evt) {
        enviarArquivoServidor();
    }
    
    private void jButtonArquivoActionPerformed(java.awt.event.ActionEvent evt) {
        FileInputStream fis;
        try {
    
        JFileChooser chooser = new JFileChooser();
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        chooser.setDialogTitle("Escolha o arquivo");
    
        if (chooser.showOpenDialog(this) == JFileChooser.OPEN_DIALOG) {
            File fileSelected = chooser.getSelectedFile();
    
            byte[] bFile = new byte[(int) fileSelected.length()];
            fis = new FileInputStream(fileSelected);
            fis.read(bFile);
            fis.close();
    
            long kbSize = fileSelected.length() / 1024;
            jTextFieldNome.setText(fileSelected.getName());
            jLabelTamanho.setText(kbSize + " KB");
    
            arquivo = new Arquivo();
            arquivo.setConteudo(bFile);
            arquivo.setDataHoraUpload(new Date());
            arquivo.setNome(fileSelected.getName());
            arquivo.setTamanhoKB(kbSize);
            arquivo.setIpDestino(jTextFieldIP.getText());
            arquivo.setPortaDestino(jTextFieldPorta.getText());
            String diretorio = "C:/Users/Deividi/Documents/GitHub/UFFS/Trabalhos antigos/Computação Distribuída/Trabalho 1 - Servidor backup/Servidor backup - Arquivos";
            arquivo.setDiretorioDestino(diretorio);
        }
    
        } catch (Exception e) {
            System.out.println("Erro: não foi possível enviar o arquivo, verifique os dados..");
        }
    }

    private void enviarArquivoServidor(){
        if (validaArquivo() == true){
            try {
                Socket socket = new Socket(jTextFieldIP.getText().trim(),
                Integer.parseInt(jTextFieldPorta.getText().trim()));
        
                BufferedOutputStream bf = new BufferedOutputStream(socket.getOutputStream());
        
                byte[] bytea = serializarArquivo();
                bf.write(bytea);
                bf.flush();
                bf.close();
                socket.close();
                System.out.println("Info: enviado com sucesso.");
            } catch (UnknownHostException e) {
                System.out.println("Erro: não foi possível encontrar o servidor informado..\n");
            } catch (IOException e) {
                System.out.println("Erro: não foi possível encontrar o servidor informado..\n");
            }catch(NullPointerException e){
                System.out.println("Erro: dados não informados corretamente..\n");    
            }catch(NumberFormatException e){
                System.out.println("Erro: dados não informados corretamente..\n");    
            }
        }
    }
    
    private byte[] serializarArquivo(){
        try {
        ByteArrayOutputStream bao = new ByteArrayOutputStream();
        ObjectOutputStream ous;
        ous = new ObjectOutputStream(bao);
        ous.writeObject(arquivo);
        return bao.toByteArray();
        } catch (IOException e) {
            System.out.println("Erro: não foi possível extrair os dados doa rquivo..\n");
        }
    
        return null;
    }
    
    private boolean validaArquivo(){
        try{
            if (arquivo.getTamanhoKB() > tamanhoPermitidoKB){
                JOptionPane.showMessageDialog(this,
                    "Tamanho máximo permitido atingido ("+(tamanhoPermitidoKB/1024)+")");
                return false;
            }else{
                return true;
            }
        }catch (NullPointerException e) {
            System.out.println("Erro: verifique os dados informados..\n");
        }
        return false;
    }
    
    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {
        public void run() {
            new ClienteSocket().setVisible(true);
        }
        });
        
        Timer timer = new Timer(); 
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override public void run() { 
            
            
            
            System.out.println("Executando a primeira vez em " +
            "1 segundo e as demais a cada 5 segundos!");
            
        }

            }, 5000, 5000);
    }
    
    
    private javax.swing.JButton jButtonArquivo;
    private javax.swing.JButton jButtonEnviar;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabelTamanho;
    private javax.swing.JTextField jTextFieldIP;
    private javax.swing.JTextField jTextFieldNome;
    private javax.swing.JTextField jTextFieldPorta;
}
