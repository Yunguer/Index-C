#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>

#include "palavra.h"
#include "indice.h"
#include "ocorrencia.h"

/* GRUPO: DOUGLAS MARELI, LUIZ EDUARDO CUNHA E YUNGUER DVORAK */

INDICE *indice;

void Menu () {
    printf("Escolha uma das opcoes abaixo:\n");
    printf("    1. Processar um (novo) arquivo texto\n");
    printf("    2. Salvar um índice atual\n");
    printf("    3. Ler um arquivo de índice\n");
    printf("    4. Realizar Buscas usando o índice atual\n");
    printf("    5. Encerrar o programa\n");
    printf(">> ");
}

void MenuBuscas () {
    printf("    1. Busca simples\n");
    printf("    2. Busca composta\n");
    printf(">> ");
}

void ProcessarArquivo() {
    printf("Digite o nome do arquivo: \n");
    char nome[20];
    scanf("%s", nome);

    int existe = verificarExistenciaArq(indice->arquivos, nome);
    if(existe) {
        printf("\n ----- Esse arquivo já foi processado -----\n");
        return;
    }

    InsereFimArq(indice->arquivos, nome);
    indice->qtdArquivos++;
    
    FILE *arq = AbreArquivoTXT(nome);
    char linha[250];
    int linha_atual = 0;

    while(fscanf(arq, " %[^\n]s", linha) > 0){
        linha_atual++;
        
        char *palavra = strtok(linha, " ");
        while( palavra != NULL) {
            char letra_inicial = pegaLetraInicial(palavra[0]); 
            if(ExisteNaLista(indice->palavras[letra_inicial], palavra, indice->qtdArquivos, linha_atual) == 0) {
                indice->qtdPalavras++;
                InsereEmOrdemAlfabetica(indice->palavras[letra_inicial], palavra, indice->qtdArquivos, linha_atual);
            }

            palavra = strtok(NULL, " ");
        }
        
    }
    
    fclose(arq);
}

void LerArquivoDat() {

    FILE *arq = AbreArquivoDat();
    int ctArquivos;
    int ctPalavras, qtdCaracter, qtdOcorrencias;

    indice = CriaIndice();

    fread(&ctArquivos, sizeof(int), 1, arq);
    indice->qtdArquivos = ctArquivos;
    printf("Quantidade de arquivos: %d\n", ctArquivos);

    char nomeArq[50];
    for(int i = 0; i < ctArquivos; i++) {
        fread(&qtdCaracter, sizeof(int), 1, arq);
        fread(nomeArq, sizeof(char), qtdCaracter, arq);
        printf("Quantidade de caracter: %d\n", qtdCaracter);
        printf("Nome: %s\n", nomeArq);
        InsereFimArq(indice->arquivos, nomeArq);
    }

    fread(&ctPalavras, sizeof(int), 1, arq);
    printf("Quantidade de palavras: %d\n", ctPalavras);
   for(int i = 0; i < ctPalavras; i++) {
        char palavra[50];
        fread(&qtdCaracter, sizeof(int), 1, arq);
        printf(" --------------------------------- \n");
        fread(palavra, sizeof(char), qtdCaracter, arq);
        palavra[qtdCaracter] = '\0';
        
        int letra_inicial = pegaLetraInicial(palavra[0]);

        fread(&qtdOcorrencias, sizeof(int), 1, arq);
        printf("Quantidade de caracteres: %d\n", qtdCaracter);
        printf("Palavra: %s\n", palavra);
        printf("Quantidade de ocorrencia em arquivos: %d\n", qtdOcorrencias);

        PALAVRA *plvr = CriaPalavraDoArqDat(palavra, qtdOcorrencias);

        for(int i = 0; i < qtdOcorrencias; i++) {
            int num, qtdOcorrenciasNoArq;
            fread(&num, sizeof(int), 1, arq);
            fread(&qtdOcorrenciasNoArq, sizeof(int), 1, arq);
            int *linhas = (int *) malloc (qtdOcorrenciasNoArq*sizeof(int));

            printf("  Numero arquivo: %d\n", num);
            printf("  Quantidade de ocorrencia no arquivo: %d\n", qtdOcorrenciasNoArq);
            printf("  Linhas: ");
            for(int i = 0; i < qtdOcorrenciasNoArq; i++) {
                fread(&linhas[i], sizeof(int), 1, arq);
                printf(" - %d", linhas[i]);
            }
            OCORRENCIA *ocr = CriaOcorrenciaDoArqDat(num, qtdOcorrenciasNoArq, linhas);
            InsereOcorrenciaExistente(plvr->ocorrencias, ocr);
            printf("\n");
             free(linhas); 
        }
        printf("\n");
        InserePalavraNoFim(indice->palavras[letra_inicial], plvr);
    }
    printf(" --------------------------------- \n");

    fclose(arq);
}


void BuscaSimples() {
    char palavra[50];
    printf(" Digite a palavra: ");
    fflush(stdin);
    scanf("%s", palavra);
    fflush(stdin);

    int letra_inicial = pegaLetraInicial(palavra[0]);
    PALAVRA *plvr = BuscarPalavra(indice->palavras[letra_inicial], palavra);
    if(plvr != NULL) {
        ImprimePalavra(plvr);
    }else {
        printf("----- Palavra não encontrada na lista -----\n");
    }
}

void Busca_E(PALAVRA* plvr1, PALAVRA* plvr2, OCORRENCIA* ocr1, OCORRENCIA* ocr2) {
    
    for(int i = 0; i < plvr1->qtdOcorrencias; i++) {
        for(int j = 0; j < plvr2->qtdOcorrencias; j++) {
            if(ocr1->arquivo == ocr2->arquivo) {
                ARQUIVO *arqAux = indice->arquivos->prox;
                for(int k = 1; k < ocr1->arquivo; k++) {
                    arqAux = arqAux->prox;
                }
                
                printf("As duas palavras estão no arquivo: %s\n", arqAux->nomeArquivo);
                return;
            }
            ocr2 = ocr2->prox;
        }
        ocr1 = ocr1->prox;
    }
}

void Busca_OU(PALAVRA* plvr1, PALAVRA* plvr2, OCORRENCIA* ocr1, OCORRENCIA* ocr2) {
    
    int *arquivos = (int *)malloc(sizeof(plvr1->qtdOcorrencias));

    printf("Nomes dos arquivos: \n");
    ARQUIVO *arqAux;
    for(int i = 0; i < plvr1->qtdOcorrencias; i++) {
        arqAux = indice->arquivos->prox;
        for(int j = 1; j < ocr1->arquivo; j++) {
            arqAux = arqAux->prox;
        }
        printf("- %s \n", arqAux->nomeArquivo);
        arquivos[i] = ocr1->arquivo;
        ocr1 = ocr1->prox;
    }

    for(int i = 0; i < plvr2->qtdOcorrencias; i++) {
        arqAux = indice->arquivos->prox;
        for(int j = 1; j < ocr2->arquivo; j++) {
            arqAux = arqAux->prox;
        }
        int igual = 0;
        for(int k = 0; k < plvr1->qtdOcorrencias; k++) {
            if(arquivos[k] == ocr2->arquivo) {
                igual = 1;
                break;
            }
        }
        if(igual == 0) {
            printf("- %s \n", arqAux->nomeArquivo);
        }
        ocr2 = ocr2->prox;
    }

    free(arquivos);
}

void BuscaComposta() {
    printf("    1. Buscar com operador E\n");
    printf("    2. Buscar com operador OU\n");
    printf(">> ");
    int op;
    scanf(" %d",&op);

    char palavra1[50];
    char palavra2[50];
    printf("Digite a primeira palavra: ");
    scanf("%s",palavra1);
    fflush(stdin);
    printf("Digite a segunda palavra: ");
    scanf("%s",palavra2);
    fflush(stdin);

    int letra_inicial_1 = pegaLetraInicial(palavra1[0]);
    int letra_inicial_2 = pegaLetraInicial(palavra2[0]);
    PALAVRA* plvr1 = BuscarPalavra(indice->palavras[letra_inicial_1], palavra1);
    PALAVRA* plvr2 = BuscarPalavra(indice->palavras[letra_inicial_2], palavra2);

    if(plvr1 == NULL || plvr2 == NULL) {
        printf(" ----- Uma ou mais palavras não está na lista -----\n");
        return;
    }

    OCORRENCIA* ocr1 = plvr1->ocorrencias->prox;
    OCORRENCIA* ocr2 = plvr2->ocorrencias->prox;

    if(op == 1) {
        Busca_E(plvr1, plvr2, ocr1, ocr2);
    }else if(op == 2) {
        Busca_OU(plvr1, plvr2, ocr1, ocr2);
    }
}


int main() {
    
    setlocale(LC_ALL, "Portuguese");
    indice = CriaIndice();
    int opcao = 0;
    while(opcao != 5) {
    
        Menu();
        scanf("%d", &opcao);

        if(opcao == 1) {
            ProcessarArquivo();
            printf("\n ---------- INDICE CRIADO ----------\n");
        }else if(opcao == 2) {

            SalvarIndiceAtual(indice);
            printf("\n ---------- INDICE SALVO ----------\n");

        }else if(opcao == 3) {

            if(indice != NULL) {
                indice = DestruirListaIndice(indice);
            }
            
            LerArquivoDat();

        }else if(opcao == 4) {
            MenuBuscas();
            int opcaoBusca;
            scanf("%d", &opcaoBusca);

            if(opcaoBusca == 1)
                BuscaSimples();
            else if(opcaoBusca == 2)
                BuscaComposta();

        }else if (opcao == 5) {
            if(indice != NULL) {
                indice = DestruirListaIndice(indice);
            }else {
                free(indice);
            }
            printf(" ----- Índice Destruida -----\n");
        }

        printf("\n");
        system("pause");
        system("cls");
    }
    
    printf(" ---------- SAIU DO PROGRAMA ----------\n\n");
    return 0;

}
