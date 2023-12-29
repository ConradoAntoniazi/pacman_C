#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "tPosicao.h"
#include "tTunel.h"

tTunel* CriaTunel(int linhaAcesso1, int colunaAcesso1, int linhaAcesso2, int colunaAcesso2) {
    tTunel* tunel;

    tunel = (tTunel*) malloc (sizeof(tTunel));

    if(tunel == NULL) {
        printf("Problema na alocacao - CriaTunel()\n");
        exit(0);
    }

    tunel->acesso1 = CriaPosicao(linhaAcesso1, colunaAcesso1);
    tunel->acesso2 = CriaPosicao(linhaAcesso2, colunaAcesso2);

    return tunel;
}

bool EntrouTunel(tTunel* tunel, tPosicao* posicao) {
    if(SaoIguaisPosicao(posicao, tunel->acesso1) || SaoIguaisPosicao(posicao, tunel->acesso2)) return true;
    return false;
}

void LevaFinalTunel(tTunel* tunel, tPosicao* posicao) {
    if(EntrouTunel(tunel, posicao) && SaoIguaisPosicao(tunel->acesso1, posicao)) { //caso esteja sobre o acesso 1 do tunel
        AtualizaPosicao(posicao, tunel->acesso2);
    }
    else if(EntrouTunel(tunel, posicao) && SaoIguaisPosicao(tunel->acesso2, posicao)) { //caso esteja sobre o acesso 2 do tunel
        AtualizaPosicao(posicao, tunel->acesso1);
    }
}

void DesalocaTunel(tTunel* tunel) {
    if(tunel != NULL){
        DesalocaPosicao(tunel->acesso1);
        DesalocaPosicao(tunel->acesso2);
        free(tunel);
    }
}