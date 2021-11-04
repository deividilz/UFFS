#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct Header {
  int memFree;
  int next;
  int qtdItems;
} header;

typedef struct Item {
  int offset;
  int totalLen;
  int writed;
} item;

typedef struct Attribute {
  char name[15];
  int size;
  char type;
} attribute;

void getTableName(char *sql, char *name);

void buildHeader(char *sql, char *tableName, int qtdPages);

void getAllAtributes(char *sql, char *attributes);

void leArquivo(char *tableName);

void getOp(char *sql, char *operation){
  char sqlCopy[1000], *token;
  char key[2] = " ";

  memset(sqlCopy, '\0', sizeof(sqlCopy));

  strcpy(sqlCopy, sql);

  token = strtok(sqlCopy, key);

  strcpy(operation, token);
}

/*
     Criação da página, passando como parâmetros, nome da tabela e número de páginas, no cabeçalho é definido seu tamanho 
  de memória livre, o tamanho do próximo arquivo e a quantidade de itens. A estrutura do stat vai retornar as informações de
  um determinado arquivo, se esse arquivo for inexistente, retorna -1, caso contrário retorna 0. 'Snprintf' é utilizado 
  para armazenar o nome da tabela e o numero de páginas no buffer apontado pelo nome da página, sizeof vai ser considerado 
  como tamanho máximo do buffer(600). Arquivo página é criado e habilitado para escrita, escreve o endereço da memoria 
  livre com seu tamanho, após o endereço para inserir o próximo elemento, escreve também a quantidade de elementos, 
  assim que escrever fecha o arquivo. Caso não conseguir criar o arquivo apresenta o erro 'Failed to create page'.
*/

void createPage(char *tableName, int numPage){
  header head;
  head.memFree = 8180;
  head.next = 8191;
  head.qtdItems = 0;

  struct stat stateDir = {0};

  char pageName[600];

  //printf("tableName: %s\n\n", tableName);
  //printf("NumPage: %d\n\n", numPage);

  snprintf(pageName, sizeof(pageName), "%s/page%d.dat", tableName, numPage);

  FILE *page = fopen(pageName, "wb");
  fwrite(&head.memFree, sizeof(int), 1, page);  // free
  fwrite(&head.next, sizeof(int), 1, page);     // onde inserir o proximo elemento
  fwrite(&head.qtdItems, sizeof(int), 1, page);// n elementos
  fclose(page);
  if(page == NULL){
    printf("Failed to create page\n");
    return;
  }
}

void createTable(char *sql){
  header head;
  head.memFree = 8180;
  head.next = 8191;
  head.qtdItems = 0;

  char special = '0';

  struct stat stateDir = {0};

  char tableName[500];
  char pageName[600];
  memset(tableName, '\0', sizeof(tableName));
  getTableName(sql, tableName);

/*
    Verifica se o arquivo é existente, caso ele for inexistente, cria um diretório com a permissão de escrita e leitura, 
  imprime o nome da tabela criada, salva o seu nome no buffer. Após cria o arquivo com permissão de escrita e fecha em 
  seguida. Salva o nome dessa tabela com uma página no buffer, cria o arquivo página com permissão para escrita. Com o 
  fseek ele vai deslocar para o final da página, onde o deslocamento é adicionado, escreve o caractere especial '0' na 
  página, fecha em seguida. Caso a página não foi possível ser criada, igual a NULL, informa o usuário que houve uma falha.
*/

  if(stat(tableName, &stateDir) == -1){
    if(mkdir(tableName, 0777) == 0)
      printf("Created table %s\n", tableName);
      snprintf(pageName, sizeof(pageName), "%s/header.dat", tableName);
      FILE *headerPage = fopen(pageName, "wb");
      fclose(headerPage);
      snprintf(pageName, sizeof(pageName), "%s/page1.dat", tableName);
      FILE *page = fopen(pageName, "wb");
      fwrite(&head.memFree, sizeof(int), 1, page);  // free
      fwrite(&head.next, sizeof(int), 1, page);     // onde inserir o proximo elemento
      fwrite(&head.qtdItems, sizeof(int), 1, page);// n elementos

      fseek(page, 8191, SEEK_SET);
      fwrite(&special, 1, 1, page);
      fclose(page);
      if(page == NULL){
        printf("Failed to create page\n");
        return;
      }
      buildHeader(sql, tableName, 1);
  }else{
    printf("Table %s already created\n", tableName);
  }
}


/*
    No buildheader é a criação do cabeçalho, onde os parâmetros passados é o SQL, nome da tabela e a quantidade de páginas, 
  após as declarações das variáveis, o memset é chamado para preencher com zeros a quantidade obtida no tamanho do filedName,
  é feito uma cópia do SQL para uma outra variável auxiliar, salva o nome dessa tabela com uma página no buffer. É feito a 
  abertura do arquivo para fazer uma nova gravação, escrito o endereço de i, seu tamanho, e onde é para ser escrito.
*/

void buildHeader(char *sql, char *tableName, int qtdPages){
  char fieldName[15], fieldType;
  int fieldSize, i = 0;

  char sqlCopy[1000], attribute[50], pageName[600];;
  char *token, *tokenAttribute;

  memset(fieldName, '\0', sizeof(fieldName));

  strcpy(sqlCopy, sql);

  snprintf(pageName, sizeof(pageName), "%s/header.dat", tableName);
  FILE *headerPage = fopen(pageName, "rb+");

  fwrite(&i, sizeof(int), 1, headerPage);

  token = strtok(sqlCopy, "(");
  token = strtok(NULL, "()[], ");

  while(token != NULL){
    if(strcmp(token, "char") == 0){
      fieldType = 'C';
      token = strtok(NULL, "()[], ");
      fieldSize = atoi(token);
      token = strtok(NULL, "()[], ");
      strcpy(fieldName, token);
      fwrite(fieldName, 15, 1, headerPage);
      fwrite(&fieldSize, sizeof(int), 1, headerPage);
      fwrite(&fieldType, 1, 1, headerPage);
      i++;
    } else if(strcmp(token, "int") == 0){
      fieldType = 'I';
      token = strtok(NULL, "()[], ");
      fieldSize = sizeof(int);
      strcpy(fieldName, token);
      fwrite(fieldName, 15, 1, headerPage);
      fwrite(&fieldSize, sizeof(int), 1, headerPage);
      fwrite(&fieldType, 1, 1, headerPage);
      i++;
    } else if(strcmp(token, "varchar") == 0){
      fieldType = 'V';
      token = strtok(NULL, "()[], ");
      fieldSize = atoi(token);
      token = strtok(NULL, "()[], ");
      strcpy(fieldName, token);
      fwrite(fieldName, 15, 1, headerPage);
      fwrite(&fieldSize, sizeof(int), 1, headerPage);
      fwrite(&fieldType, 1, 1, headerPage);
      i++;
    }
    token = strtok(NULL, "()[], ");
  }

  fwrite(&qtdPages, sizeof(int), 1, headerPage);
  fseek(headerPage, 0, SEEK_SET);

  fwrite(&i, sizeof(int), 1, headerPage);

  fclose(headerPage);

  leArquivo(tableName);
}


/*
    Com o 'menset' vai definir os primeiros números de bytes do bloco de memória apontado por 'sqlCopy' para '\0'. Vai ser 
  procurado o (), dentro da variável sqlCopy, onde dentro do while vai fazer a segunda pesquisa, que é para o parâmetro da 
  tabela nome. Vai ser escrito no arquivo os atributos da estrutura. Depois que escrever no cabeçalho vai ser fechado esse 
  arquivo, repetindo a primeira parte do processo, até chegar no for onde ele vai repetir até que chegue no tamanho da 
  quantidade de campos, comparando os atributos de determinado tipo, nessa comparação ele vai receber o somar com o tamanho 
  do insert. Verifica se a memória livre é maior que o tamanho que quer inserir, se for maior, vai adicionar um novo item, 
  indo para o inicio de um ponto especificado para fazer a escrita, vai executar o for da quantidade de tamanho de campos 
  novamente e por fim fecha o arquivo página. Caso o seu if for falso ele vai até a parte especifica da página, definida 
  como 8191, assim criando uma nova página, passando como parâmetros nome da tabela e a próxima página. Caso for necessário 
  vai executar o insertInto novamente para a próxima página.
*/

void insertInto(char *sql, int numPage){
  char sqlCopy[1000], *token, tableName[500], pageName[600], attrSql[1000], attrSqlCopy[1000];
  char endVarchar = '$', special = ' ', endChar = '\0';
  int insertSize = 0, qtdFields, nextItem, intVar, nextPage, qtdEndChar = 0;
  int i = 0, countVarchar = 0;

  attribute attributes[64];

  header head;

  memset(sqlCopy, '\0', sizeof(sqlCopy));

  strcpy(sqlCopy, sql);

  token = strtok(sqlCopy, " () ,");

  while(token != NULL){
    // Second parameter table name
    if(i == 2)
      strcpy(tableName, token);
    token = strtok(NULL, " () ,");
    i++;
  }

  snprintf(pageName, sizeof(pageName), "%s/header.dat", tableName);
  FILE *headerPage = fopen(pageName, "rb+");

  fread(&qtdFields, sizeof(int), 1, headerPage);

  for(int i = 0; i < qtdFields; i++){
    fread(attributes[i].name, 15, 1, headerPage);
    fread(&attributes[i].size, sizeof(int), 1, headerPage);
    fread(&attributes[i].type, 1, 1, headerPage);
  }

  fread(&nextPage, sizeof(int), 1, headerPage);

  fclose(headerPage);

  strcpy(sqlCopy, sql);
  token = strtok(sqlCopy, "()");
  token = strtok(NULL, "()");

  memset(attrSql, '\0', sizeof(attrSql));
  memset(attrSqlCopy, '\0', sizeof(attrSqlCopy));

  strcpy(attrSql, token);

  strcpy(attrSqlCopy, attrSql);

  token = strtok(attrSqlCopy, ",");

  for(int i = 0; i < qtdFields; i++){
    if(attributes[i].type == 'C'){
      insertSize += attributes[i].size;
    } else if(attributes[i].type == 'I'){
      insertSize += attributes[i].size;
    } else if(attributes[i].type == 'V'){
      insertSize += strlen(token) + 1; // + 1 for special char of varchar '$'
      //countVarchar
    }
    token = strtok(NULL, ",");
  }

  strcpy(attrSqlCopy, attrSql);

  snprintf(pageName, sizeof(pageName), "%s/page%d.dat", tableName, numPage);
  FILE * page = fopen(pageName, "rb+");

  fread(&head.memFree, sizeof(int), 1, page);
  fread(&head.next, sizeof(int), 1, page);
  fread(&head.qtdItems, sizeof(int), 1, page);

  if(head.memFree > insertSize){
    token = strtok(attrSqlCopy, ",");
    item newItem;
    newItem.offset = head.next - insertSize;
    newItem.totalLen = insertSize;
    newItem.writed = 1;

    nextItem = 12 + 12 * head.qtdItems;

    //printf("Inicio da gravacao: %d\n", newItem.offset);

    fseek(page, nextItem, SEEK_SET);

    fwrite(&newItem.offset, sizeof(int), 1, page);
    fwrite(&newItem.totalLen, sizeof(int), 1, page);
    fwrite(&newItem.writed, sizeof(int), 1, page);

    fseek(page, newItem.offset, SEEK_SET);

    //printf("OFFSET NEW ITEM: %d\n", newItem.offset);

    for(int i = 0; i < qtdFields; i++){
      if(attributes[i].type == 'C'){
        fwrite(token, strlen(token), 1, page);
        qtdEndChar = attributes[i].size - strlen(token);
        fwrite(&endChar, 1, qtdEndChar, page);
      } else if(attributes[i].type == 'I'){
        intVar = atoi(token);
        fwrite(&intVar, attributes[i].size, 1, page);
      } else if(attributes[i].type == 'V'){
        fwrite(token, strlen(token), 1, page);
        fwrite(&endVarchar, 1, 1, page);
      }
      token = strtok(NULL, ",");
    }

    head.memFree = head.memFree - insertSize;
    head.next = newItem.offset;
    head.qtdItems += 1;

    fseek(page, 0, SEEK_SET);

    fwrite(&head.memFree, sizeof(int), 1, page);
    fwrite(&head.next, sizeof(int), 1, page);
    fwrite(&head.qtdItems, sizeof(int), 1, page);

    fclose(page);

    printf("New item inserted\n");
  }else {
    fseek(page, 8191, SEEK_SET);
    fread(&special, 1, 1, page);
    //printf("Special: %c\n", special);
    if(special == '0'){
      //printf("Entrou special\n");
      nextPage++;
      //printf("Nome Pagina: %s\n", tableName);
      //printf("Numero pagina: %d\n", nextPage);
      //printf("SQL - %s\n", sql);
      createPage(tableName, nextPage);
      special = '1';
      fseek(page, 8191, SEEK_SET);
      fwrite(&special, 1, 1, page);
      fclose(page);
      insertInto(sql, nextPage);
    }else{
      fclose(page);
      insertInto(sql, nextPage);
    }
  }
}

void getTableName(char *sql, char *name){
  char sqlCopy[1000];
  char *token;
  char keyParen[2] = "(";
  char keySpace[2] = " ";

  memset(sqlCopy, '\0', sizeof(sqlCopy));

  strcpy(sqlCopy, sql);

  token = strtok(sqlCopy, keyParen);

  strcpy(sqlCopy, token);

  token = strtok(sqlCopy, keySpace);

  while(token != NULL){
    strcpy(name, token);
    token = strtok(NULL, keySpace);
  }
}

void leArquivo(char *tableName){
  char pageName[600];
  char pageCoisa[600];

  snprintf(pageCoisa, sizeof(pageCoisa), "%s/page1.dat", tableName);

  FILE *page = fopen(pageCoisa, "rb+");

  header head;

  fread(&head.memFree, sizeof(int), 1, page);
  fread(&head.next, sizeof(int), 1, page);
  fread(&head.qtdItems, sizeof(int), 1, page);
  //printf("MemFree - %d; Next - %d - QtdItems - %d\n", head.memFree, head.next, head.qtdItems);

  fclose(page);
}

/*
    Passando primeiramente como parâmetro, a string sql e o int de numero de páginas, vai ser criado um novo item para 
  leitura atributos. Vai ser aberto o arquivo com determinado nome, quando encontrado, para ser lido. Com o for vai 
  ser percorrido a quantidade de campos para ser lido os dados dos atributos do cabeçalho da página, após a página é 
  fechada. Vai ser aberto a página para a escrita, no outro for vai ser percorrido até percorra toda a cabeça, fazendo 
  o movimento do item, quando necessário, continua escrevendo na página aberta. No próximo for vai percorrer a quantidade 
  de campos novamente, porém toda vez que encontrar um determinado tipo de atributos, vai executar algo diferente. Após a 
  execuções de determinado atributo, vai ser deslocado a página até determinada parte especifica. Com o ultimo if comparando 
  com '1', caso for verdadeiro vai chamar a função de selectFrom novamente.
*/

void selectFrom(char *sql, int numPage){
  char tableName[50], sqlCopy[1000], pageName[600], *token, charInFile, special = ' ';
  int i = 0, qtdFields = 0, moveItem = 0, moveTupla = 0, intInFile, stopChar;

  item readItem;
  attribute attributes[64];

  memset(sqlCopy, '\0', sizeof(sqlCopy));

  strcpy(sqlCopy, sql);

  token = strtok(sqlCopy, " ");

  while(token != NULL){
    // Second parameter table name
    if(i == 3){
      token = strtok(token, "\n");
      strcpy(tableName, token);
    } else
      token = strtok(NULL, " ");
    i++;
  }

  //printf("\nTable selected - %s\n", tableName);

  snprintf(pageName, sizeof(pageName), "%s/header.dat", tableName);

  FILE *headerPage = fopen(pageName, "rb+");
  if(!headerPage){
    printf("Can not read %s/header.dat\n", tableName);
  }

  fread(&qtdFields, sizeof(int), 1, headerPage);

  //printf("Quantidade de campos: %d\n", qtdFields);

  for(int i = 0; i < qtdFields; i++){
    fread(attributes[i].name, 15, 1, headerPage);
    printf("%s\t", attributes[i].name);
    fread(&attributes[i].size, sizeof(int), 1, headerPage);
    fread(&attributes[i].type, 1, 1, headerPage);
  }
  printf("\n");

  fclose(headerPage);

  snprintf(pageName, sizeof(pageName), "%s/page%d.dat", tableName, numPage);
  FILE *page = fopen(pageName, "rb+");

  header head;

  item items;
  fread(&head.memFree, sizeof(int), 1, page);
  fread(&head.next, sizeof(int), 1, page);
  fread(&head.qtdItems, sizeof(int), 1, page);

  //printf("memFree: %d - next: %d - qtdItems: %d\n", head.memFree, head.next, head.qtdItems);

  for(int i = 0; i < head.qtdItems; i++){
    fseek(page, 0, SEEK_SET);
    moveItem = sizeof(item) * (i + 1);
    fseek(page, moveItem, SEEK_SET);

    fread(&readItem.offset, sizeof(int), 1, page);
    fread(&readItem.totalLen, sizeof(int), 1, page);
    fread(&readItem.writed, sizeof(int), 1, page);

    //printf("offset: %d - totalLen: %d - writed: %d\n", readItem.offset, readItem.totalLen, readItem.writed);

    if(readItem.writed == 0)
      continue;

    fseek(page, readItem.offset, SEEK_SET);

    for(int j = 0; j < qtdFields; j++){
      if(attributes[j].type == 'C'){
        charInFile = ' ';
        stopChar = 0;
        for(int k = 0; k < attributes[j].size; k++){
          fread(&charInFile, 1, 1, page);
          if(charInFile == '\0')
            stopChar = 1;
          if(!stopChar)
            printf("%c", charInFile);
        }
        printf("\t");
      } else if(attributes[j].type == 'I'){
        fread(&intInFile, sizeof(int), 1, page);
        printf("%d\t", intInFile);
      } else if(attributes[j].type == 'V'){
        charInFile = ' ';
        do{
            fread(&charInFile, 1, 1, page);
            if(charInFile != '$')
              printf("%c", charInFile);
        }while(charInFile != '$');
        printf("\t");
      }
    }
    printf("\n");
  }
  fseek(page, 8191, SEEK_SET);
  fread(&special, 1, 1, page);
  if(special == '1'){
    numPage++;
    selectFrom(sql, numPage);
  }else
    fclose(page);
}

/*
    Primeiramente passado como parâmetro, a string sql e o numero de páginas. Parte superior da função semelhantes com as 
  demais já explicadas. No decorrer do código, o arquivo da página de cabeçalho é aberto para a leitura, percorrendo a 
  quantidade de campos e lendo seus atributos, fecha a página de cabeçalho. A página é aberta e é lido sua "cabeça", após 
  dentro do for, onde vai percorrendo até a quantidade de itens da cabeça, com o fseek vai mover até o inicio da página para 
  começar novamente, vai mover o item quando necessário, continuando a leitura. No próximo for vai percorrer a quantidade de 
  campos onde tem as condições dos tipos de atributos, delete = 1 para saber que esta dentro do for que vai ser usado para 
  deletar. O if para deletar vai comparar se há diferença entre o atributo nome e o atributo da estrutura, se haver diferença 
  com o fseek novamente vai mover até onde o moveItem for necessário, vai ser escrito no arquivo alguns dados. Quando o for 
  terminar de executar, vai ser movido a página até onde for determinado e caso special = '1' vai ser fechada a página e a 
  função deleteFrom vai ser chamada novamente, caso contrário apenas fecha a página.
*/

void deleteFrom(char *sql, int numPage){
  char tableName[50], sqlCopy[1000], pageName[600], *token, charInFile, attributeName[20], intInput[20], special = ' ';
  int i = 0, k = 0, qtdFields = 0, moveItem = 0, moveTupla = 0, intInFile, stopChar, delete = 0, inputDelete, count = 0;

  item readItem;
  attribute attributes[64];

  memset(sqlCopy, '\0', sizeof(sqlCopy));

  strcpy(sqlCopy, sql);

  token = strtok(sqlCopy, " ");

  while(token != NULL){
    // Second parameter table name delete from table where name = anderson
    if(i == 2){
      strcpy(tableName, token);
    } else if(i == 5){
      strcpy(attributeName, token);
    } else if(i == 8){
      break;
    } else
      token = strtok(NULL, " ");
    i++;
  }
  token = strtok(token, "\n");

  //printf("\nTable selected - %s\n", tableName);
  //printf("\nAttribute selected - %s\n", attributeName);
  //printf("\nAttribute value - %s\n", token);


  snprintf(pageName, sizeof(pageName), "%s/header.dat", tableName);

  FILE *headerPage = fopen(pageName, "rb+");
  if(!headerPage){
    printf("Can not read %s/header.dat\n", tableName);
  }

  fread(&qtdFields, sizeof(int), 1, headerPage);

  for(int i = 0; i < qtdFields; i++){
    fread(attributes[i].name, 15, 1, headerPage);
    fread(&attributes[i].size, sizeof(int), 1, headerPage);
    fread(&attributes[i].type, 1, 1, headerPage);
  }
  printf("\n");

  fclose(headerPage);

  snprintf(pageName, sizeof(pageName), "%s/page%d.dat", tableName, numPage);
  FILE *page = fopen(pageName, "rb+");

  header head;

  item items;
  fread(&head.memFree, sizeof(int), 1, page);
  fread(&head.next, sizeof(int), 1, page);
  fread(&head.qtdItems, sizeof(int), 1, page);

  for(int i = 0; i < head.qtdItems; i++){
    fseek(page, 0, SEEK_SET);
    moveItem = sizeof(item) * (i + 1);
    fseek(page, moveItem, SEEK_SET);

    fread(&readItem.offset, sizeof(int), 1, page);
    fread(&readItem.totalLen, sizeof(int), 1, page);
    fread(&readItem.writed, sizeof(int), 1, page);

    if(readItem.writed == 0)
      continue;

    fseek(page, readItem.offset, SEEK_SET);

    for(int j = 0; j < qtdFields; j++){
      delete = 1;
      if(attributes[j].type == 'C') {
        charInFile = ' ';
        stopChar = 0;
        if(j > 0)
          fread(&charInFile, 1, 1, page); // Read '\0' varchar
        for(int l = 0; l < attributes[j].size; l++){
          fread(&charInFile, 1, 1, page);
          if(delete == 1 && charInFile == '\0')
            stopChar = 1;
            //printf("%c = %c\n", charInFile, token[l]);
          if(charInFile != token[l] && !stopChar)
            delete = 0;
        }
        if(delete && !(strcmp(attributeName, attributes[j].name))){
          fseek(page, moveItem, SEEK_SET);
          readItem.writed = 0;
          fwrite(&readItem.offset, sizeof(int), 1, page);
          fwrite(&readItem.totalLen, sizeof(int), 1, page);
          fwrite(&readItem.writed, sizeof(int), 1, page);
          printf("Attribute deleted\n");
          count++;
          break;
        }else if(j > 0)
          fseek(page, -1, SEEK_CUR); // Read '\0' varchar
      } else if(attributes[j].type == 'I'){
        if(j > 0)
          fread(&charInFile, 1, 1, page); // Read '\0' varchar
        fread(&intInFile, sizeof(int), 1, page);
        //printf("From file: %d\n", intInFile);
        inputDelete = atoi(token);
        if(intInFile != inputDelete)
          delete = 0;
        if(delete && !(strcmp(attributeName, attributes[j].name))){
          fseek(page, moveItem, SEEK_SET);
          readItem.writed = 0;
          fwrite(&readItem.offset, sizeof(int), 1, page);
          fwrite(&readItem.totalLen, sizeof(int), 1, page);
          fwrite(&readItem.writed, sizeof(int), 1, page);
          printf("Attribute deleted\n");
          count++;
          break;
        }else if(j > 0)
          fseek(page, -1, SEEK_CUR); // Read '\0' varchar
      } else if(attributes[j].type == 'V'){
        k = 0;
        if(j > 0)
          fread(&charInFile, 1, 1, page); // Read '\0' varchar
        while(charInFile != '$'){
            fread(&charInFile, 1, 1, page);
            //printf("%c = %c\n", charInFile, token[k]);
            if(delete == 1 && charInFile == '$')
              break;
            if(charInFile != token[k])
              delete = 0;
            k++;
        }
        if(delete && !(strcmp(attributeName, attributes[j].name))){
          fseek(page, moveItem, SEEK_SET);
          readItem.writed = 0;
          fwrite(&readItem.offset, sizeof(int), 1, page);
          fwrite(&readItem.totalLen, sizeof(int), 1, page);
          fwrite(&readItem.writed, sizeof(int), 1, page);
          printf("Attribute deleted\n");
          count++;
          break;
        }else if(j > 0)
          fseek(page, -1, SEEK_CUR); // Read '\0' varchar
      }
    }
    fseek(page, 0, SEEK_SET);
  }
  printf("Attributes deleted = %d\n", count);
  fseek(page, 8191, SEEK_SET);
  fread(&special, 1, 1, page);
  if(special == '1'){
    numPage++;
    fclose(page);
    deleteFrom(sql, numPage);
  }else
    fclose(page);
  //fclose(page);
}

int main(){
  char sql[1000], operation[10], attributes[500];

  do{
    printf(">> ");
    fgets(sql, 1000, stdin);
    getOp(sql, operation);

    if(strcmp(operation, "create") == 0){
      createTable(sql);
    }else if(strcmp(operation, "delete") == 0){
      deleteFrom(sql, 1);
    }else if(strcmp(operation, "insert") == 0){
      insertInto(sql, 1);
    }else if(strcmp(operation, "select") == 0){
      selectFrom(sql, 1);
    }else{
      printf("Something went wrong");
    }
  }while(strcmp(operation, "quit") != 0);

  return 0;
}
