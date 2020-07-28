#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct par{
    int value1;
    int value2;
};

int fila[1000];
int aux[1000];
int pfila = -1;     //ponteiro de fila.



void printFila(){
    int i;
    for(i=0;i<=pfila;i++){
        printf("%d - ",fila[i]);
    }
    printf("\n");
}
int filaElem(){
    return pfila;
}
int filaVazia(){
    if(pfila<0) return 1;
    else return 0;
}
void resetFila(){
    pfila = -1;
}
void entraFila(int n){
    pfila++;
    fila[pfila]=n;
}
void entraFila2(int n,int n2){
    pfila++;
    fila[pfila]=n;
    aux[pfila]=n2;
}
struct par sacaFila2(){
    if(pfila<0) {
        struct par vazio;
        vazio.value1=-1;
        vazio.value2=-1;
        return vazio;
    }
    else{
        int value1 = fila[pfila];
        int value2 =  aux[pfila];
        pfila--;
        struct par elem;
        elem.value1=value1;
        elem.value2=value2;
        return(elem);
    }
}
int sacaFila(){
    if(pfila<0) {
        return -1;
    }
    else{
        int value = fila[pfila];
        pfila--;
        return(value);
    }
}


int main_(){
  
    printf("vazia: %d\n",filaVazia());
    entraFila(4);
    entraFila(5);
    entraFila(9);
    printf("vazia: %d\n",filaVazia());
    printf("quantos: %d\n",filaElem());
    printFila();
    printf("saca: %d\n",sacaFila());
    printf("saca: %d\n",sacaFila());
    printf("saca: %d\n",sacaFila());
    printf("saca: %d\n",sacaFila());
     return 0;
}
