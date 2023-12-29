#include "tPosicao.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

tPosicao* CriaPosicao(int linha, int coluna) {
    tPosicao* posicao;

    posicao = (tPosicao*) malloc(sizeof(tPosicao));

    if(posicao == NULL) {
        printf("Problema na alocacao - CriaPosicao()\n");
        exit(0);
    }

    posicao->linha = linha;
    posicao->coluna = coluna;

    return posicao;
}

tPosicao* ClonaPosicao(tPosicao* posicao) {
    tPosicao* posicao_clone;

    posicao_clone = CriaPosicao(posicao->linha, posicao->coluna);

    return posicao_clone;
}

int ObtemLinhaPosicao(tPosicao* posicao) {
    return posicao->linha;
}

int ObtemColunaPosicao(tPosicao* posicao) {
    return posicao->coluna;
}

void AtualizaPosicao(tPosicao* posicaoAtual, tPosicao* posicaoNova) {
    posicaoAtual->linha = posicaoNova->linha;
    posicaoAtual->coluna = posicaoNova->coluna;
}

bool SaoIguaisPosicao(tPosicao* posicao1, tPosicao* posicao2) {
    if(posicao1->linha == posicao2->linha && posicao1->coluna == posicao2->coluna) return true;
    return false;
}

void DesalocaPosicao(tPosicao* posicao) {
    free(posicao);
}