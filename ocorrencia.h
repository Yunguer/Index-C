#ifndef _OCORRENCIA_H_
#define _OCORRENCIA_H_


typedef struct Ocorrencia {
    int arquivo; //posição do arquivo na lista de arquivos processados.
    int qtdOcorrencias; //qtd de vezes em que a palavra foi encontrada
    int *linhas; //linhas onde a palavra foi encontrada
    struct Ocorrencia *prox; //ponteiro para a próxima ocorrência da palavra em outro arquivo
}OCORRENCIA;

int MesmoArquivo(OCORRENCIA *lista, int arquivo) {
    OCORRENCIA *aux = lista->prox;
    while(aux->prox != NULL) {
        aux = aux->prox;
    }

    if(aux->arquivo == arquivo) 
        return 1;

    return 0;
} 

OCORRENCIA* CriaOcorrencia(int arquivo, int linha) {
    OCORRENCIA *novo = (OCORRENCIA *) malloc(sizeof(OCORRENCIA));
    novo->arquivo = arquivo;
    novo->qtdOcorrencias = 1;
    novo->linhas = (int*)malloc(sizeof(int));
    novo->linhas[0] = linha;
    novo->prox = NULL;

    return novo;
}

OCORRENCIA* CriaOcorrenciaDoArqDat(int numArq, int qtdOcorrencia, int *linhas) {
    OCORRENCIA *novo = (OCORRENCIA *) malloc(sizeof(OCORRENCIA));
    novo->arquivo = numArq;
    novo->qtdOcorrencias = qtdOcorrencia;
    novo->linhas = (int*)malloc(sizeof(qtdOcorrencia));
    for(int i = 0; i < qtdOcorrencia; i++) {
        novo->linhas[i] = linhas[i];
    }
    novo->prox = NULL;

    return novo;
}

OCORRENCIA* CriaListaOcorrencia() {
    OCORRENCIA *sentinela = (OCORRENCIA *) malloc(sizeof(OCORRENCIA));
    sentinela->prox = NULL;
    return sentinela;
}

void AdicionaLinha(OCORRENCIA *lista, int linha) {
    OCORRENCIA *aux = lista->prox;
    
    while(aux->prox != NULL) {
        aux = aux->prox;
    }
    
    aux->qtdOcorrencias++;
    aux->linhas = realloc(aux->linhas, sizeof(aux->qtdOcorrencias));
    aux->linhas[aux->qtdOcorrencias - 1] = linha;
}

void ImprimeListaOcorrencia(OCORRENCIA *lista) {
    OCORRENCIA *aux = lista->prox;
    while(aux != NULL) {
            printf("  Arquivo: %d\n", aux->arquivo);
            printf("  Quantidade de ocorrencia: %d\n", aux->qtdOcorrencias);
            printf("  Linhas em que aparece: ");
            for(int i = 0; i < aux->qtdOcorrencias ; i++) {
                printf(" %d -", aux->linhas[i]);
            }
            printf("\n");
            aux = aux->prox;
        }
}

void InsereOcorrenciaNoFim(OCORRENCIA* lista, int arquivo, int linha) {
    OCORRENCIA *aux = lista->prox;
    OCORRENCIA *novo = CriaOcorrencia(arquivo, linha);
    while(aux->prox != NULL) {
        aux = aux->prox;
    }
    
    aux->prox = novo;
}

void InsereOcorrenciaExistente(OCORRENCIA* lista, OCORRENCIA *novo) {
    if(lista->prox != NULL) {
        OCORRENCIA *aux = lista->prox;
        while(aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = novo;

    }else {
        lista->prox = novo;
    }
    
    
}

OCORRENCIA* DestruirListaOcorrencia(OCORRENCIA* lista) {
    OCORRENCIA *aux = lista;
    while(aux != NULL) {
        lista = lista->prox;
        free(aux->linhas);
        free(aux);
        aux = lista;
    }
    return NULL;
}

#endif