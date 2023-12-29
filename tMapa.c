#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tPosicao.h"
#include "tTunel.h"
#include "tMapa.h"

bool EhPosicaoValidaMapa(tPosicao* posicao, tMapa* mapa) {
    if(mapa == NULL 
    || mapa->grid == NULL
    || posicao == NULL 
    || (ObtemLinhaPosicao(posicao) > mapa->nLinhas) 
    || (ObtemColunaPosicao(posicao) > mapa->nColunas)) return false;

    return true;
}

tMapa* CriaMapa(const char* caminhoConfig) {
    FILE* mapa_txt;
    char caminho_mapa[1020];
    tMapa* mapa = (tMapa*) malloc(sizeof(tMapa));
    mapa->nFrutasAtual = 0;
    mapa->tunel = NULL;

    // Gerando o caminho para o arquivo mapa.txt e fazendo
    sprintf(caminho_mapa, "%s/mapa.txt", caminhoConfig);
    mapa_txt = fopen(caminho_mapa, "r");
    if(mapa_txt == NULL) {
        printf("ERRO: nao foi possível ler o arquivo /%s/mapa.txt\n", caminhoConfig);
        free(mapa);
        fclose(mapa_txt);
        exit(0);
    }

    //lendo informacoes de mapa.txt 
    fscanf(mapa_txt, "%d\n", &mapa->nMaximoMovimentos);

    char caracter_mapa;
    int nLinhas = 1;
    int nColunas = 0;

    //alocando a primeira linha do grid
    mapa->grid = (char**) malloc(1 * sizeof(char*));
    //erro na alocacao:
    if(mapa->grid == NULL) {
        printf("Erro na alocacao da primeira linha do grid - mapa->grid\n");
    }

    while (fscanf(mapa_txt, "%c", &caracter_mapa) == 1) {
        if(caracter_mapa == '\n') break;

        nColunas++;
        if(nColunas == 1) {
            mapa->grid[0] = (char*) malloc(1 * sizeof(char));
        }
        else {
            mapa->grid[0] = (char*) realloc(mapa->grid[0], nColunas * sizeof(char));
        }

        //erro na alocacao:
        if(mapa->grid[0] == NULL) {
            printf("Erro na alocacao da primeira linha do grid - mapa->grid[0]\n");
        }

        mapa->grid[0][nColunas - 1] = caracter_mapa;
    } 

    //alocando as outras linhas do grid e pegando as informacoes inicias de comida:
    while ((caracter_mapa = fgetc(mapa_txt)) != EOF) {

        nLinhas++;
        mapa->grid = (char**) realloc(mapa->grid, nLinhas * sizeof(char*));
        mapa->grid[nLinhas - 1] = (char*) malloc(nColunas * sizeof(char));
        //erro na alocacao:
        if(mapa->grid == NULL || mapa->grid[nLinhas - 1] == NULL) {
            printf("Erro na realocacao do vetor para char* do grid ou erro nas alocacos das linhas individuais\n");
        }

        //atribuindo os valores das linhas:
        mapa->grid[nLinhas - 1][0] = caracter_mapa;
        for(int i = 1; i < nColunas; i++) {
            fscanf(mapa_txt, "%c", &mapa->grid[nLinhas - 1][i]);

            //contando o numero de comidas distribuidas no mapa no momento inicial
            if(mapa->grid[nLinhas - 1][i] == '*') {
                (mapa->nFrutasAtual)++;
            }
        }

        //limpado '\n'
        fscanf(mapa_txt, "%*c");
    }

    mapa->nLinhas = nLinhas;
    mapa->nColunas =  nColunas;

    int linhaAcesso1 = 0, colunaAcesso1 = 0, linhaAcesso2 = 0, colunaAcesso2 = 0, numDeAcessosEncontrados = 0;

    //varrendo o grid para achar os tuneis
    for(int l = 0; l < mapa->nLinhas; l++) {
        for(int c = 0; c < mapa->nColunas; c++) {
            if(mapa->grid[l][c] == '@' && numDeAcessosEncontrados == 0) {
                linhaAcesso1 = l;
                colunaAcesso1 = c;
                numDeAcessosEncontrados++;
            } else if(mapa->grid[l][c] == '@' && numDeAcessosEncontrados == 1) {
                linhaAcesso2 = l;
                colunaAcesso2 = c;
                mapa->tunel = CriaTunel(linhaAcesso1, colunaAcesso1, linhaAcesso2, colunaAcesso2);
            }
        }
    }

    fclose(mapa_txt);

    return mapa;
}

tPosicao* ObtemPosicaoItemMapa(tMapa* mapa, char item) {
    tPosicao* posicao = NULL;

    for(int l = 0; l < mapa->nLinhas; l++) {
        for(int c = 0; c < mapa->nColunas; c++) {
            if(mapa->grid[l][c] == item) {

                posicao = CriaPosicao(l, c);
                return posicao;
            }
        }
    }
    return posicao;
}

tTunel* ObtemTunelMapa(tMapa* mapa) {
    return mapa->tunel;
}

char ObtemItemMapa(tMapa* mapa, tPosicao* posicao) {
    if(EhPosicaoValidaMapa(posicao, mapa)) return (mapa->grid[ObtemLinhaPosicao(posicao)][ObtemColunaPosicao(posicao)]);

    return '\0';
}

int ObtemNumeroLinhasMapa(tMapa* mapa) {
    return mapa->nLinhas;
}

int ObtemNumeroColunasMapa(tMapa* mapa) {
    return mapa->nColunas;
}

int ObtemQuantidadeFrutasIniciaisMapa(tMapa* mapa) {
    return mapa->nFrutasAtual;
}

int ObtemNumeroMaximoMovimentosMapa(tMapa* mapa) {
    return mapa->nMaximoMovimentos;
}

bool EncontrouComidaMapa(tMapa* mapa, tPosicao* posicao) {
    if(EhPosicaoValidaMapa(posicao, mapa) && mapa->grid[ObtemLinhaPosicao(posicao)][ObtemColunaPosicao(posicao)] == '*') return true;

    return false;
}

bool EncontrouParedeMapa(tMapa* mapa, tPosicao* posicao) {
    if(EhPosicaoValidaMapa(posicao, mapa) && mapa->grid[ObtemLinhaPosicao(posicao)][ObtemColunaPosicao(posicao)] == '#') return true;

    return false;
}

bool AtualizaItemMapa(tMapa* mapa, tPosicao* posicao, char item) {
    if(!EhPosicaoValidaMapa(posicao, mapa)) return false;

    mapa->grid[ObtemLinhaPosicao(posicao)][ObtemColunaPosicao(posicao)] = item;
    return true;
}

bool PossuiTunelMapa(tMapa* mapa) {
    if(mapa->tunel == NULL) return false;
    return true;
}

bool AcessouTunelMapa(tMapa* mapa, tPosicao* posicao) {
    return EntrouTunel(mapa->tunel, posicao);
}

void EntraTunelMapa(tMapa* mapa, tPosicao* posicao) {
    return LevaFinalTunel(mapa->tunel, posicao);
}

void DesalocaMapa(tMapa* mapa) {
    if(mapa == NULL) return;

    DesalocaTunel(mapa->tunel);

    for(int i = 0; i < mapa->nLinhas; i++) {
        free(mapa->grid[i]);
    }
    free(mapa->grid);
    free(mapa);

    mapa = NULL;
}
