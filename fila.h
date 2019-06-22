#ifndef FILA_H_   /* Include guard */
#define FILA_H_

struct par{
    int value1;
    int value2;
};

int filaVazia();
void resetFila();
void entraFila(int n);
int sacaFila();
void printFila();
int filaElem();
void entraFila2(int n,int n2);
struct par sacaFila2();

#endif // FILA_H_