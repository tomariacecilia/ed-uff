#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct vizinho {
    char nome[10];
    struct vizinho *prox;
} TVizinho;

typedef struct grafo {
    char nome[10];
    int idade;
    TVizinho *prim_vizinho;
    struct grafo *prox;
} TGrafo;

TGrafo *insere_vertice(TGrafo *g, char *nome, int idade) {
    TGrafo *vertice = (TGrafo *) malloc(sizeof(TGrafo));
    strcpy(vertice->nome, nome);
    vertice->idade = idade;
    vertice->prox = g;
    vertice->prim_vizinho = NULL;
    return vertice;
}

TGrafo *busca_vertice(TGrafo *vertice, char *nome) {
    while ((vertice != NULL) && (strcmp(vertice->nome, nome) != 0)) {
        vertice = vertice->prox;
    }
    return vertice;
}

TVizinho *busca_vizinho(TVizinho *vizinho, char *nome) {
    while ((vizinho != NULL) && (strcmp(vizinho->nome, nome) != 0)) {
        vizinho = vizinho->prox;
    }
    return vizinho;
}

void insere_aresta(TGrafo *g, char *nome_origem, char *nome_destino) {
    TGrafo *vertice = busca_vertice(g, nome_origem);
    TVizinho *vizinho = (TVizinho *) malloc(sizeof(TVizinho));
    strcpy(vizinho->nome, nome_destino);
    vizinho->prox = vertice->prim_vizinho;
    vertice->prim_vizinho = vizinho;
}

void imprime(TGrafo *vertice) {
    while (vertice != NULL) {
        printf("Pessoa: %s - %d anos\n", vertice->nome, vertice->idade);
        printf("Segue: ");
        TVizinho *vizinho = vertice->prim_vizinho;
        while (vizinho != NULL) {
            printf("%s ", vizinho->nome);
            vizinho = vizinho->prox;
        }
        printf("\n\n");
        vertice = vertice->prox;
    }
}

int numero_seguidos(TGrafo *g, char *nome) {
    TGrafo *pv1 = busca_vertice(g,nome);
    TVizinho *resp = NULL;
    int cont = 0;

    if(pv1 != NULL) {
        resp = pv1->prim_vizinho;
        while (resp != NULL) {
            cont++;
            resp = resp->prox;
        }
    }
    return cont;
}

int seguidores(TGrafo *g, char *nome, int imprime) {
    TGrafo *pv1 = g;
    TVizinho *resp = NULL;
    int cont = 0;

    while (pv1 != NULL) {
        if ((strcmp(pv1->nome, nome) != 0)) {
            resp = pv1->prim_vizinho;
            while ((resp != NULL) && (strcmp(resp->nome, nome) != 0)) {
                resp = resp->prox;
            }
            if((resp != NULL) && (strcmp(resp->nome, nome) == 0)){
                cont++;
                if (imprime == 1) {
                    printf("%s ", pv1->nome);
                }
            }
        }
        pv1 = pv1->prox;
    }
    if (imprime == 1) {
        printf("\n");
    }

    return cont;
}

TGrafo *mais_popular(TGrafo *g) {
    TGrafo *p = g;
    TGrafo *maisPop = NULL;
    int maior = 0, aux = 0, cont = 0;
    int *n_seguidores = (int *) malloc(sizeof(int));

    while(p != NULL) {
        aux = seguidores(p,p->nome,0);
        if (aux >= maior) {
            maior = aux;
            maisPop = p;
        }
        cont++;
        n_seguidores = realloc (n_seguidores,cont * sizeof(int));
        n_seguidores[(cont-1)] = aux;
        p = p->prox;
    }
    /* int i=0, j=0;
    while ((i==0) && (j=!cont)) {
        for (j=0; j<cont; j++) {
            if (n_seguidores[j] =! aux) i=1;
        }
    }

    if (i==0) return NULL;
 */
    return maisPop;
}

int segue_mais_velho(TGrafo *g, int imprime) {
    TGrafo *pv1 = g;
    TGrafo *pv2 = NULL;
    TVizinho *resp = NULL;

    while (pv1 != NULL) {
        if((pv1->prim_vizinho) != NULL){
            resp = pv1->prim_vizinho;
            pv2 = busca_vertice(g,resp->nome);
            while ((resp != NULL) && ((pv2->idade) > (pv1->idade))) {
                resp = resp->prox;
            }
            if((resp == NULL) && (imprime == 1)){
                printf("%s ", pv1->nome);
            }
        }
        pv1 = pv1->prox;
    }
    return 0;
}

void libera_vizinho(TVizinho *vizinho) {
    if (vizinho != NULL) {
        libera_vizinho(vizinho->prox);
        free(vizinho);
    }
}

void libera_vertice(TGrafo *vertice) {
    if (vertice != NULL) {
        libera_vizinho(vertice->prim_vizinho);
        libera_vertice(vertice->prox);
        free(vertice);
    }
}

int main() {
    /* A função main lê os dados de entrada, cria o grafo e chama as funções solicitadas no problema
    * depois imprime os resultados solicitados
    * ELA NÃO DEVE SER MODIFICADA
    * */
    int num_vertices, num_arestas;
    char nome[30];
    char origem[30], destino[30];
    char l[100];
    char delimitador[] = "-";
    char *ptr;
    int idade;
    int i;
    TGrafo *g = NULL;

    //le numero de vertices
    scanf("%d", &num_vertices);
    //le e cria os vertices
    for (i = 0; i < num_vertices; i++) {
        scanf("%s", l);
        //quebra a string de entrada
        ptr = strtok(l, delimitador);
        strcpy(nome,ptr);
        //printf("%s", nome);
        ptr = strtok(NULL, delimitador);
        idade = atoi(ptr);
        g = insere_vertice(g, nome, idade);
    }

    //Le numero de arestas e depois le os dados de cada aresta
    //Cria as arestas no grafo
    scanf("%d", &num_arestas);
    for (i = 0; i < num_arestas; i++) {
        scanf("%s", l);
        //quebra a string de entrada
        ptr = strtok(l, delimitador);
        strcpy(origem,ptr);
        //printf("%s", nome);
        ptr = strtok(NULL, delimitador);
        strcpy(destino,ptr);
        insere_aresta(g, origem, destino);
    }

    //Le nome de pessoa
    scanf("%s", nome);

    //Encontra o número de seguidos dessa pessoa
    printf("SEGUIDOS por %s: %d\n", nome, numero_seguidos(g, nome));

    //Encontra os seguidores de uma determinada pessoa
    printf("SEGUIDORES de %s:\n", nome);
    seguidores(g, nome, 1);

    TGrafo *p;

    //Encontra mais popular
    p = mais_popular(g);
    printf("MAIS POPULAR: %s\n", p->nome);

    //Encontra as pessoas que seguem apenas pessoas mais velhas
    printf("SEGUEM APENAS PESSOAS MAIS VELHAS:\n");
    segue_mais_velho(g,1);

    libera_vertice(g);
}
