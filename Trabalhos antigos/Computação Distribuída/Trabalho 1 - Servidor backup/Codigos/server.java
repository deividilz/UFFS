package Codigos;

//IMPORT DAS BIBLIOTECAS NECESSÁRIAS
import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.ServerSocket;
import java.net.Socket;

//CLASSE SERVER PRINCIPAL
public class Server extends javax.swing.JFrame {

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
    
    private static Arquivo arquivo; 

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
                
                //THREAD PARA ATUALIZAR O COMPONENTE DURANTE EXECUÇÃO
                new Thread (new Runnable() {
                    @Override
                    public void run() {
                        try{    //VAI TENTAR ALTERAR O TEXTO DO COMPONENTE
                            Integer.parseInt(jTextField1.getText().trim());
                            jLabelInfo.setText("Servidor online");
                            System.out.println("Online: ");
                            System.out.println(jLabelInfo);

                            jButtonAbrirActionPerformed(evt); //CHAMA O EVENTO CASO PRESSIONADO O BOTÃO
                        }catch(Exception e){ //CASO HOUVER ERRO
                            jLabelInfo.setText("Servidor com porta inválida");
                        }
                   }
                }).start(); //CRIANDO A THREAD
            }
        });

        jButton2.addActionListener(new java.awt.event.ActionListener() {    //FUNÇÃO PARA O BOTÃO DE EXIT
            public void actionPerformed(java.awt.event.ActionEvent evt) {   //PASSANDO O EVENTO POR PARAMETRO
                System.exit(0);                                             //FECHA A INTERFACE E FINALIZA O SERVIDOR
            }
        });

        //ALTERANDO O TEXTO DOS COMPONENTES GRÁFICOS
        jLabelNomeArquivo.setText("Nome arquivo");
        jLabelNomeImprimir.setText(" ");
        jLabelTamanho.setText("Tamanho");
        jLabelTamanhoImprimir.setText(" ");
        jButton2.setText("Encerrar servidor");

        //CRIAÇÃO DE TODO O LAYOUT GRÁFICO
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
    
    //CLASSE PARA INICIALIZAR
    public class InicializaServidor extends Thread{

        //DECLARAÇÃO DAS VARIÁVEIS
        private final Socket socket;
        private final byte[] objectAsByte;

        //FUNÇÃO PARA INICIALIZAR O SERVIDOR PASSANDO COMO PARÂMETRO O SOCKET DO CLIENTE PARA CRIAR A CONEXÃO
        public InicializaServidor(final Socket cliente) throws IOException {
            this.socket = cliente;
            this.objectAsByte = new byte[socket.getReceiveBufferSize()];            
        }    
        
        //FUNÇÃO QUE IRÁ RECEBER O ARQUIVO ENVIADO PELO CLIENTE/USUÁRIO
        public void run(){
            try {            
                System.out.println("Info: aguardando o envio do arquivo pelo cliente.\n");

                System.out.println("objectAsByte: " + objectAsByte);

                BufferedInputStream buffer = new BufferedInputStream(this.socket.getInputStream()); //CRIA UM BUFFER PARA RECEBER A ENTRADA DO SOCKET

                buffer.read(this.objectAsByte); //FICA AGUARDANDO A LEITURA DO OBJETO DE ENTRADA
                
                System.out.println("Buffer: " + buffer);

                arquivo = getObjectFromByte(this.objectAsByte); //ARQUIVO RECEBE O OBJETO

                System.out.println(arquivo);

                jLabelNomeImprimir.setText(arquivo.getNome()); //ALTERA O NOME NA INTERFACE GRÁFICA
                
                //PEGA O TAMANHO DO ARQUIVO RECEBIDO
                long kbSize = arquivo.getTamanhoKB();
                jLabelTamanhoImprimir.setText(kbSize + " KB");
                System.out.println("Tamanho: "+kbSize);

                //STRING QUE IRÁ CONTER TODO O DIRETÓRIO PARA SALVAR O ARQUIVO
                String diretorio = arquivo.getDiretorioDestino().endsWith("/") ? arquivo.getDiretorioDestino() + arquivo.getNome() : arquivo.getDiretorioDestino() + "/" + arquivo.getNome();
                
                System.out.println("Escrevendo arquivo " + diretorio);

                //ARQUIVO QUE FOI RECEBIDO É SALVO/ESCRITO
                FileOutputStream fos = new FileOutputStream(diretorio);
                fos.write(arquivo.getConteudo());
                fos.close();
            }catch(Exception e){
                System.out.println("Erro: " + e.toString());
            }
        }
    }

    //FUNÇÃO SERVIDOR QUE RECEBE A PORTA QUE SERÁ ABERTA COMO PARAMETRO
    public Server (final int port) throws IOException {
        System.out.println("Iniciou - porta: " + port);
        try (ServerSocket server = new ServerSocket(port)) { //TENTA ABRIR O SOCKET COM A PORTA INFORMADA
            for(;;){
                new InicializaServidor(server.accept()).start(); //INSTANCIA UM NOVO SERVIDOR, ACEITANDO O CLIENTE CONECTADO
            }
        }catch(Exception e){
            System.out.println("Erro: " + e.getMessage());
        }
    }

    //FUNÇÃO PARA O BOTÃO QUE IRÁ ABRIR A PORTA DO SERVIDOR
    protected void jButtonAbrirActionPerformed(java.awt.event.ActionEvent evt) {
        try{
            new Server(Integer.parseInt(jTextField1.getText().trim())); //TENTA CRIR UM SERVIDOR PASSANDO A PORTA INFORMADA COMO PARAMETRO
        }catch(Exception e){
            System.out.println("Erro1: " + e.getMessage());
        }
    }

    //FUNÇÃO PRINCIPAL QUE IRÁ INICIALIZAR O PROGRAMA
    public static void main(String[] args) throws IOException, ClassNotFoundException{
        java.awt.EventQueue.invokeLater(new Runnable() { //CRIA UMA FILA DE EVENTOS
        public void run() {
            new Server().setVisible(true); //CRIA UM NOVO SERVIDOR DEFININDO A VISIBILIDADE COMO ATIVA
        }
        });
    }

    //FUNÇÃO DO ARQUIVO PARA PEGAR OS DADOS NECESSÁRIOS
    private static Arquivo getObjectFromByte(byte[] objectAsByte) {
        Object obj = null;                  //INSTANCIA UM OBJETO
        ByteArrayInputStream bis = null;    //INSTANCIA UM ARRAY DE BYTES
        ObjectInputStream ois = null;       //INSTANCIA UM OBJETO DE ENTRADA
        try {
            bis = new ByteArrayInputStream(objectAsByte);   //OBJETO VAI RECEBER O ARRAY DE BYTES QUE CONTEM NO DADO
            ois = new ObjectInputStream(bis);               //RECEBE UM OBJETO DE ENTRADA
            obj = ois.readObject();                         //LE O OBJETO RECEBIDO

            bis.close();    //FECHA O OBJETO INSTANCIADO
            ois.close();    //FECHA A ENTRADA DO OBJETO

        } catch (IOException e) {
            System.out.println("Erro: " + e.toString());
        } catch (ClassNotFoundException e) {
            System.out.println("Erro: " + e.toString());
        }
        return (Arquivo) obj;
    }
}
