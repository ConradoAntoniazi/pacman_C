#ifndef TFANTASMA_H_
#define TFANTASMA_H_

#include "tPosicao.h"
#include "tMovimento.h"
#include "tMapa.h"
#include "tPacman.h"
#include "tFantasma.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_FANTASMAS 6

typedef enum {MOV_ESQUERDA_FANT = 1, 
              MOV_CIMA_FANT = 2, 
              MOV_BAIXO_FANT = -2, 
              MOV_DIREITA_FANT = -1} DIRECAO;

typedef struct tFantasma {
    // Posições do fantasma (linha, coluna)
    tPosicao* posicaoAtual;
    tPosicao* posicaoAnterior;
    
    // Direção do fantasma (1: Esquerda / -1: Direita / 2: Cima / -2: Baixo)
    DIRECAO direcao;

    // Sobrepos comida ou não: 1 para caso o fantasma esteja sobre uma '*' e 0 para o contrário
    int sobreposComida;

    // Caracter correspondente ao fantasma
    char aparencia;

    // Se o fantasma eh especial ou nao (atravassa parede)
    int atravessaParede;

    int estaSobreAlgo;
    char algo;

    int estaVivo;
    
} tFantasma;

int EstaVivoFantasma(tFantasma* fantasma);

void MataFantasma (tFantasma* fantasma);

tFantasma* CriaFantasma(tPosicao* posicao, char aparencia);

int AtravessaParedeFantasma (tFantasma* fantasma);

int ObtemDirecaoFantasma (tFantasma* fantasma);

tPosicao* ObtemPosicaoAtualFantasma (tFantasma* fantasma);

tPosicao* ObtemPosicaoAnteriorFantasma (tFantasma* fantasma);

char ObtemAparenciaFantasma (tFantasma* fantasma);

int EstaSobreComidaFantasma (tFantasma* fantasma);

tPosicao* ProximaPosFantasma (tFantasma* fantasma);

void InverteDirecaoFantasma (tFantasma* fantasma);

void MoveFantasmaEspecial (tFantasma* fantasma, tMapa* mapa, tPacman* pacman);

void MoveFantasma (tFantasma* fantasma, tMapa* mapa, tPacman* pacman);

void DesalocaFantasma (tFantasma* fantasma);

#endif