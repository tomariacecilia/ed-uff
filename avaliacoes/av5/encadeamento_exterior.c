#include <limits.h>
#include <stdio.h>

#include "cliente.c"
#include "compartimento_hash.c"

#define M 7

void imprime_arquivos(char *nome_arquivo_hash, char *nome_arquivo_dados) {
    //Imprime arquivo de hash
    printf("TABELA HASH:\n");
    imprime_arquivo_hash(nome_arquivo_hash);

    //Imprime arquivo de dados
    printf("\nDADOS:\n");
    imprime_arquivo_dados(nome_arquivo_dados);
}

/* Executa insercao em Arquivos por Encadeamento Exterior (Hash)
 * cod_cli: chave do cliente que esta sendo inserido
 * nome_cli: nome do cliente a ser inserido
 * nome_arquivo_hash: nome do arquivo que contem a tabela hash
 * nome_arquivo_dados: nome do arquivo onde os dados estao armazenados
 * Retorna o endereco (lógico) onde o cliente foi inserido, ou -1 se nao conseguiu inserir
 */

void adiciona_cliente(FILE *in, TCliente *c, int n) {
    fseek(in, tamanho_cliente() * n, SEEK_SET);
    salva_cliente(c, in);
    free(c);
}

int procura_cliente(FILE *dados, TCliente *cli, int endereco_inicial, int n_registros) {
    fseek(dados, tamanho_cliente() * endereco_inicial, SEEK_SET);
    TCliente *cliente = le_cliente(dados);
    int prox = 0, atual = endereco_inicial;

    while (cliente != NULL) {
        if(cliente->cod != cli->cod) {
            if (cliente->ocupado == 0) {
                prox = cliente->prox;
                cli->prox = prox;
                fseek(dados, tamanho_cliente() * -1, SEEK_CUR);
                salva_cliente(cli,dados);
                free(cliente);
                return atual;
            }
            if (cliente->prox == -1) {
                fseek(dados, tamanho_cliente() * -1, SEEK_CUR);
                cliente->prox = n_registros;
                salva_cliente(cliente,dados);
                fseek(dados, 0, SEEK_END);
                salva_cliente(cli,dados);
                free(cliente);
                return n_registros;
            }
            if (cliente->prox != -1) {
                fseek(dados, tamanho_cliente() * cliente->prox, SEEK_SET);
                atual = cliente->prox;
                cliente = le_cliente(dados);
            }
        } else return -1;
    }
}


int insere(int cod_cli, char *nome_cli, char *nome_arquivo_hash, char *nome_arquivo_dados, int m) {

    FILE *hash, *dados;
    int h = cod_cli % m;
    int endereco = 0, final = num_registros(nome_arquivo_dados);

    if ((hash = fopen(nome_arquivo_hash, "r+b")) == NULL || (dados = fopen(nome_arquivo_dados, "r+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    else {
        fseek(hash,tamanho_compartimento() * h, SEEK_SET);
        TCompartimento *compartimento = le_compartimento(hash);

        if (compartimento->prox == -1){
            fseek(hash,tamanho_compartimento() * h, SEEK_SET);
            compartimento->prox = final;
            salva_compartimento(compartimento,hash);
            adiciona_cliente(dados,cliente(cod_cli,nome_cli,-1,1),final);
            endereco = compartimento->prox;
        } else {
            endereco = procura_cliente(dados, cliente(cod_cli,nome_cli,-1,1), compartimento->prox, final);
        }

        fclose(hash);
        fclose(dados);

    }
    
    return endereco;
}

int main() {
    /* Essa função gera a saída que é usada no teste do run.codes. Ela NÃO DEVE SER MODIFICADA */
    int cod;
    char nome[TAM_NOME] = "";
    int pont;

    //le dados a serem inseridos
    scanf("%d", &cod);
    scanf("%s", nome);

    //Imprime arquivos de entrada
    //imprime_arquivos("tabHash.dat", "clientes.dat");

    pont = insere(cod, nome, "tabHash.dat", "clientes.dat", M);
    imprime_arquivos("tabHash.dat", "clientes.dat");
    //Imprime resultado da função
    printf("\nRESULTADO: %d", pont);
}