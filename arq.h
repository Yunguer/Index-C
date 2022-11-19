#ifndef _ARQ_H_
#define _ARQ_H_

#include <stdio.h>
#include <stdlib.h>
#include "indice.h"

typedef struct arquivo{
    char nomeArquivo[50]; //nome de um arquivo texto já processado
    struct arquivo *prox; //ponteiro para o nome do próximo
}ARQUIVO;

ARQUIVO* CriaArq(char *nome) {
    ARQUIVO *arq = (ARQUIVO*)malloc(sizeof(ARQUIVO));
    strcpy(arq->nomeArquivo, nome);
    arq->prox = NULL;

    return arq;
}

ARQUIVO* CriaListaArq() {
    ARQUIVO *sentinela = (ARQUIVO *) malloc(sizeof(ARQUIVO));
    sentinela->prox = sentinela;
    return sentinela;
}

void InsereFimArq(ARQUIVO *lista, char *nome_arq) {
    ARQUIVO *aux = lista->prox;
    
    while(aux->prox != lista) {
        aux = aux->prox;
    }

    ARQUIVO* novo = CriaArq(nome_arq);
    aux->prox = novo;
    novo->prox = lista;
}

int verificarExistenciaArq(ARQUIVO *lista, char *nome) {
    ARQUIVO *aux = lista->prox;
    
    while(aux != lista) {
        
        if(strcmp(aux->nomeArquivo, nome) == 0){
            return 1;
        }
        aux = aux->prox;
    }
    return 0;
}

FILE* AbreArquivoTXT(char *nome) {
    FILE *arq;
    arq = fopen(nome, "r");

    if(arq == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(0);
    }

    return arq;
}

void ImprimeListaArq(ARQUIVO *lista) {
    ARQUIVO* aux = lista->prox;
    
    while(aux != lista) {
        printf("Nome: %s \n", aux->nomeArquivo);
        aux = aux->prox;
    }
}

ARQUIVO* DestruirListaArquivos(ARQUIVO* lista) {
    ARQUIVO *aux = lista->prox;
    while(aux != lista) {
        free(aux);
        aux = aux->prox;
    }
    free(lista);
    return NULL;
}

#endif