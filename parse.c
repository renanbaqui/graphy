#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*int main()
{
  int v[1000];
  int a[100000][2];
parse_json(v,a);
}*/
int parse_json(int *ve,int **ar,int *vertices, int *arestas,char *fileName, int debug)
{
  //int ar[][2]
   

    FILE* Arquivo = NULL; 
    //int vertices_json[1000]; //não é possivel saber com antecendencia o numero de vertices. colocamos o maximo n=1000
    //int arestas_json[100000][2]; //max arestas = n*n. Meu limite de memoria: 250k. para n=1000 deveria ser 1M

    char chars_read[100]; //string auxiliar para ler segmentos do json
    char sepa_read[100]; //string auxiliar para ler segmentos do json
    char sep[] = ","; //string para comparação
    char bra[] = "[";//string para comparação
    char br2[] = "]";//string para comparação
    int n = 0; //num vertices -1
    int m = 0; //num arestas  -1

    //fscanf utilizada intensivamente para ler um json com formato estabelecido
    //o json contem três chaves: nome, vertices e arestas
    //a chave vertices contem um array de strings com os nome do vertice
    //a chave arestas contem um array 2d de string com o nome do vertice origem e o vertice destino que define a aresta
    //dado que o json tem uma estrutura estabelecida, se utiliza letura caracter a caracter utilizando fscanf.

    //char file[]=name;
    Arquivo = fopen (fileName,"r");
    if(debug==1) printf("Lendo arquivo %s\n",fileName);
    fscanf(Arquivo, "%[{\" ]", chars_read);
    if(debug==2) printf("Primeiros char %s\n",chars_read);
    fscanf(Arquivo, "%[^\"]", chars_read);
    if(debug==2) printf("Nome: %s\n",chars_read);
    fscanf(Arquivo, "%[\": ]", chars_read);
    if(debug==2) printf("segue: %s\n",chars_read);
    fscanf(Arquivo, "%[^\"]", chars_read);
    if(debug==2) printf("O nome: %s\n",chars_read);
    fscanf(Arquivo, "%[\", ]", chars_read);
    if(debug==2) printf("segue: %s\n",chars_read);
    fscanf(Arquivo, "%[^\"]", chars_read);
    if(debug==2) printf("Vertices: %s\n",chars_read);
    fscanf(Arquivo, "%[^[]", chars_read);
    if(debug==2) printf("segue: %s\n",chars_read);
    fscanf(Arquivo, "%[[]", chars_read);
    if(debug==2) printf("segue: %s\n",chars_read);
    int last = 0;
    while(last==0){
      fscanf(Arquivo, "%[\" ]", chars_read);
      if(debug==2) printf("abre quote: %s\n",chars_read);
      fscanf(Arquivo, "%[^\"]", chars_read);
      if(debug==2) printf("numero: %s<-\n",chars_read);
      fscanf(Arquivo, "%[\"]", sepa_read);
      if(debug==2) printf("fecha quote: %s\n",sepa_read);

      fscanf(Arquivo, "%c", sepa_read);
      
      if(debug==2) printf("Achado elemento:%d\n",atoi(chars_read));
      //Guarda o vertice
      //vertices_json[n]=atoi(chars_read);
      ve[n]=atoi(chars_read);
  

      if(strcmp(sep, sepa_read)){
        last = 1;
        if(debug==2) printf("Ultimo elemento!>%s<\n",sepa_read);
      }else{
        n++;   
      }
    }

    fscanf(Arquivo, "%[\", ]", chars_read);
    if(debug==2) printf("segue: %s\n",chars_read);
    fscanf(Arquivo, "%[^\"]", chars_read);
    if(debug==2) printf("segue I: %s\n",chars_read);
    fscanf(Arquivo, "%[^[]", chars_read);
    if(debug==2) printf("segue II: %s\n",chars_read);
    //Read the opening bracket
    fscanf(Arquivo, "%c", chars_read);
    last = 0;
    while(last==0){
      //Read the second bracket, is a braket?
      fscanf(Arquivo, "%c", sepa_read);
        if(strcmp(bra, sepa_read)){
          last = 1;
          if(debug==2) printf("Fim de itens!>%s<\n",sepa_read);
        }else{
          //Read "1", "2"],
          fscanf(Arquivo, "%[\" ]", chars_read);
          if(debug==2) printf("abre quote: %s\n",chars_read);
          fscanf(Arquivo, "%[^\"]", chars_read);
          if(debug==2) printf("numero: %s<-\n",chars_read);
          fscanf(Arquivo, "%[\",]", sepa_read);
          if(debug==2) printf("fecha quote: %s\n",sepa_read);
          if(debug==2) printf("Achado item 1:%d\n",atoi(chars_read)); 
          //arestas_json[m][0]= atoi(chars_read);
          ar[m][0]= atoi(chars_read);
          fscanf(Arquivo, "%[\", ]", chars_read);
          fscanf(Arquivo, "%[^\"]", chars_read);
          if(debug==2) printf("numero: %s<-\n",chars_read);
          fscanf(Arquivo, "%[\",]", sepa_read);
          if(debug==2) printf("fecha quote: %s\n",sepa_read);
          if(debug==2) printf("Achado item 2:%d\n",atoi(chars_read)); 
          //arestas_json[m][1]= atoi(chars_read);
          ar[m][1]= atoi(chars_read);
          //close ]
          fscanf(Arquivo, "%c", sepa_read);
          if(debug==2) printf("fecha bracket: %s\n",sepa_read);
          //read ,
          fscanf(Arquivo, "%c", sepa_read);
          if(strcmp(sep, sepa_read)){
            last = 1;
            if(debug==2) printf("Fim de vetor arestas!>%s<\n",sepa_read);
          }else{
            m++;
          }
          //read ' '
          fscanf(Arquivo, "%c", chars_read);
        }
    }
    if (debug==1) printf("Grafo com %d vertices e %d arestas\n",n+1,m+1); 
    *vertices = n+1;
    *arestas = m+1;
    //printVertices(vertices_json,n+1);
    //printArestas(arestas_json,m+1);
   
    return 0;
}  

int parse_json_capacity(int *ve,int **ar, int *cap, int *vertices, int *arestas,char *fileName, int debug)
{
  //int ar[][2]
   

    FILE* Arquivo = NULL; 
    //int vertices_json[1000]; //não é possivel saber com antecendencia o numero de vertices. colocamos o maximo n=1000
    //int arestas_json[100000][2]; //max arestas = n*n. Meu limite de memoria: 250k. para n=1000 deveria ser 1M

    char chars_read[100]; //string auxiliar para ler segmentos do json
    char sepa_read[100]; //string auxiliar para ler segmentos do json
    char sep[] = ","; //string para comparação
    char bra[] = "[";//string para comparação
    char br2[] = "]";//string para comparação
    int n = 0; //num vertices -1
    int m = 0; //num arestas  -1

    //fscanf utilizada intensivamente para ler um json com formato estabelecido
    //o json contem três chaves: nome, vertices e arestas
    //a chave vertices contem um array de strings com os nome do vertice
    //a chave arestas contem um array 2d de string com o nome do vertice origem e o vertice destino que define a aresta
    //dado que o json tem uma estrutura estabelecida, se utiliza letura caracter a caracter utilizando fscanf.

    //char file[]=name;
    Arquivo = fopen (fileName,"r");
    if(debug==1) printf("Lendo arquivo %s\n",fileName);
    fscanf(Arquivo, "%[{\" ]", chars_read);
    if(debug==2) printf("Primeiros char %s\n",chars_read);
    fscanf(Arquivo, "%[^\"]", chars_read);
    if(debug==2) printf("Nome: %s\n",chars_read);
    fscanf(Arquivo, "%[\": ]", chars_read);
    if(debug==2) printf("segue: %s\n",chars_read);
    fscanf(Arquivo, "%[^\"]", chars_read);
    if(debug==2) printf("O nome: %s\n",chars_read);
    fscanf(Arquivo, "%[\", ]", chars_read);
    if(debug==2) printf("segue: %s\n",chars_read);
    fscanf(Arquivo, "%[^\"]", chars_read);
    if(debug==2) printf("Vertices: %s\n",chars_read);
    fscanf(Arquivo, "%[^[]", chars_read);
    if(debug==2) printf("segue: %s\n",chars_read);
    fscanf(Arquivo, "%[[]", chars_read);
    if(debug==2) printf("segue: %s\n",chars_read);
    int last = 0;
    while(last==0){
      fscanf(Arquivo, "%[\" ]", chars_read);
      if(debug==2) printf("abre quote: %s\n",chars_read);
      fscanf(Arquivo, "%[^\"]", chars_read);
      if(debug==2) printf("numero: %s<-\n",chars_read);
      fscanf(Arquivo, "%[\"]", sepa_read);
      if(debug==2) printf("fecha quote: %s\n",sepa_read);

      fscanf(Arquivo, "%c", sepa_read);
      
      if(debug==2) printf("Achado elemento:%d\n",atoi(chars_read));
      //Guarda o vertice
      //vertices_json[n]=atoi(chars_read);
      ve[n]=atoi(chars_read);
  

      if(strcmp(sep, sepa_read)){
        last = 1;
        if(debug==2) printf("Ultimo elemento!>%s<\n",sepa_read);
      }else{
        n++;   
      }
    }

    fscanf(Arquivo, "%[\", ]", chars_read);
    if(debug==2) printf("segue: %s\n",chars_read);
    fscanf(Arquivo, "%[^\"]", chars_read);
    if(debug==2) printf("segue I: %s\n",chars_read);
    fscanf(Arquivo, "%[^[]", chars_read);
    if(debug==2) printf("segue II: %s\n",chars_read);
    //Read the opening bracket
    fscanf(Arquivo, "%c", chars_read);
    last = 0;
    while(last==0){
      //Read the second bracket, is a braket?
      fscanf(Arquivo, "%c", sepa_read);
        if(strcmp(bra, sepa_read)){
          last = 1;
          if(debug==2) printf("Fim de itens!>%s<\n",sepa_read);
        }else{
          //Read "1", "2"],
          fscanf(Arquivo, "%[\" ]", chars_read);
          if(debug==2) printf("abre quote: %s\n",chars_read);
          fscanf(Arquivo, "%[^\"]", chars_read);
          if(debug==2) printf("numero: %s<-\n",chars_read);
          fscanf(Arquivo, "%[\",]", sepa_read);
          if(debug==2) printf("fecha quote: %s\n",sepa_read);
          if(debug==2) printf("Achado item 1:%d\n",atoi(chars_read)); 
          //arestas_json[m][0]= atoi(chars_read);
          ar[m][0]= atoi(chars_read);
          fscanf(Arquivo, "%[\", ]", chars_read);
          fscanf(Arquivo, "%[^\"]", chars_read);
          if(debug==2) printf("numero: %s<-\n",chars_read);
          fscanf(Arquivo, "%[\",]", sepa_read);
          if(debug==2) printf("fecha quote: %s\n",sepa_read);
          if(debug==2) printf("Achado item 2:%d\n",atoi(chars_read)); 
          //arestas_json[m][1]= atoi(chars_read);
          ar[m][1]= atoi(chars_read);
          
          //Capacidade no terceiro elemento do array
          fscanf(Arquivo, "%[\", ]", chars_read);
          fscanf(Arquivo, "%[^\"]", chars_read);
          if(debug==2) printf("numero: %s<-\n",chars_read);
          fscanf(Arquivo, "%[\", ]", sepa_read);
          if(debug==2) printf("fecha quote: >%s<\n",sepa_read);
          if(debug==2) printf("Achado item 3:%d\n",atoi(chars_read)); 
        
          cap[m]= atoi(chars_read);

          //close ]
          fscanf(Arquivo, "%c", sepa_read);
          if(debug==2) printf("fecha bracket: %s\n",sepa_read);
          //read ,
          fscanf(Arquivo, "%c", sepa_read);
          if(strcmp(sep, sepa_read)){
            last = 1;
            if(debug==2) printf("Fim de vetor arestas!>%s<\n",sepa_read);
          }else{
            m++;
          }
          //read ' '
          fscanf(Arquivo, "%c", chars_read);
        }
    }
    if (debug==1) printf("Grafo com %d vertices e %d arestas\n",n+1,m+1); 
    *vertices = n+1;
    *arestas = m+1;
    //printVertices(vertices_json,n+1);
    //printArestas(arestas_json,m+1);
   
    return 0;
}  

int dump_json(int *ve,int **ar,int *vertices, int *arestas,char *fileName, int debug)
{
  FILE* Arquivo = NULL; 
  Arquivo = fopen (fileName,"w");
  fprintf(Arquivo,"{\"nome\": \"GRAFO_TESTE_8\", \"vertices\": [");
  int i;
  for(i=0;i<*vertices;i++){
    fprintf(Arquivo, "\"%d\"",i);  
    if(i<(*vertices)-1) fprintf(Arquivo,", ");
  }
  fprintf(Arquivo,"], \"arestas\": [");
  for(i=0;i<*arestas;i++){
    fprintf(Arquivo, "[\"%d\", \"%d\"]",ar[i][0],ar[i][1]);  
    if(i<(*arestas)-1) fprintf(Arquivo,", ");
  }
  fprintf(Arquivo,"]}");
  fclose(Arquivo); 
}

void printVertices(int vert[], int n){
  int i=0;
  printf("vertices: ");
  for(i=0;i<n;i++){
    printf("%d ",vert[i]);
  }
  printf("\n");
}


void printArestas(int arestas[][2], int m){
  int i=0;
  printf("arestas:\n");
  for(i=0;i<m;i++){
    printf("%d\t%d\n",arestas[i][0],arestas[i][1]);
  }
}