#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct noA {
    char info;
    struct noA *esq;
    struct noA *dir;
} TNoA;


/*
 * Funcao deve retornar 1 caso as arvores tenham os mesmos nos (mesmo que em posicoes distintas), e 0 caso contrario
 */

void bubbleSort(char* v, int n) {
  int i, j, aux;
  for(i = n-1; i > 0; i--) {
    for(j = 0; j < i; j++) {
      if(v[j] > v[j+1]) {
        aux = v[j]; v[j] = v[j+1]; v[j+1] = aux;
      }
    }
  }
}

void profundidade(char* nos, TNoA* a, int *i) {
    if (a!= NULL) {
        (*i)++;
        nos = realloc(nos, (*i) * sizeof(char));
        nos[(*i)-1] = a->info;
        profundidade(nos, a->esq, i);
        printf(nos);
        printf("\n");
        printf(i);
        printf("\n");
        profundidade(nos, a->dir, i);
        printf(nos);
        printf("\n");
        printf(i);
    }
}

int mesmos_nos(TNoA *a1, TNoA *a2) {
    char *nos1 = NULL;
    char *nos2 = NULL;
    int aux = 0;
    int *i= &aux;

    nos1 = (char *) malloc(sizeof(char));
    nos2 = (char *) malloc(sizeof(char));

    profundidade(nos1, a1, i);
    aux = 0;
    printf(nos1);
    profundidade(nos2, a2, i);
    printf(nos2);

    /* não chamei a função de ordenar ainda porque queria resolver o vetor primeiro */

    return 1;
}

void imprime(TNoA *nodo, int tab) {
    for (int i = 0; i < tab; i++) {
        printf("-");
    }
    if (nodo != NULL) {
        printf("%c\n", nodo->info);
        imprime(nodo->esq, tab + 2);
        printf("\n");
        imprime(nodo->dir, tab + 2);
    } else printf("vazio");
}

void imprimeProfundidade(TNoA *nodo, int altura) {
    altura = altura - 1;
    if (nodo != NULL) {
        printf("%c", nodo->info);
        if (altura > 0) {
            imprimeProfundidade(nodo->esq, altura);
            imprimeProfundidade(nodo->dir, altura);
        }
    } else {
        printf("*");
        //Trata caso de subarvore NULL em nível menor que altura da árvore
        if (altura > 0) {
            imprimeProfundidade(NULL, altura);
            imprimeProfundidade(NULL, altura);
        }
    }
}

TNoA *criaArvore(char entrada[100], int tamanho) {
    char novaEntrada[100];
    int i,j;
    TNoA *novo;
    novo = NULL;
    if ((tamanho > 0) && (entrada[0] != '*')) {
        novo = (TNoA *) malloc(sizeof(TNoA));
        novo->info = entrada[0];
        tamanho = tamanho / 2;

        //divide a entrada em duas partes e chama a função criaArvore recursivamente
        i = 1; //inicio da subarvore esquerda na string entrada
        j = 0; //cursor da nova string de entrada que conterá apenas a subárvore desejada
        while (i <= tamanho) {
            novaEntrada[j] = entrada[i];
            i++;
            j++;
        }
        novaEntrada[i] = '\0';
        novo->esq = criaArvore(novaEntrada, tamanho);

        i = tamanho+1; //inicio da subarvore direita na string entrada
        j = 0; //cursor da nova string de entrada que conterá apenas a subárvore desejada
        while (i <= tamanho*2) {
            novaEntrada[j] = entrada[i];
            i++;
            j++;
        }
        novaEntrada[i] = '\0';
        novo->dir = criaArvore(novaEntrada, tamanho);
    }
    return novo;
}

int main (void) {
    /* A função main lê os dados de entrada, cria as árvores e chama a função solicitada no exercício
     * depois imprime o resultado
     * Ela NÃO DEVE SER MODIFICADA
     * */
    TNoA* a1 = NULL;
    TNoA* a2 = NULL;
    int tam;
    char entrada1[100];
    char entrada2[100];

    /* lê valores para criar a árvore 1
     * exemplo: FBC */
    scanf("%s", entrada1);
    tam = strlen(entrada1);
    a1 = criaArvore(entrada1 , tam);

    /* le valores para criar a arvore 2 */
    scanf("%s", entrada2);
    tam = strlen(entrada2);
    a2 = criaArvore(entrada2 , tam);

    int aaa = mesmos_nos(a1, a2);

    printf("%d", aaa);
}

