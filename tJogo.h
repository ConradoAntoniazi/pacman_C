#ifndef TJOGO_H_
#define TJOGO_H_

#include "tFantasma.h"
#include "tPacman.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct tJogo {
    //ponteiro para o pacman
    tPacman* pacman;
    //ponteiro para o mapa
    tMapa* mapa;
    //lista dos fantasmas
    tFantasma** fantasmas;
    int numFantasmas;

}tJogo;

typedef struct tInformacaoJogada {
    //comando correspondente (w, a, s, d)
    char comando;
    //numero vezes que o comando resultou em colisao com a parede
    int numColisoesParede;
    //numero de comidas que foram pegas utilizando ela
    int numComidasPegas;
    //numero de vezes que foi chamada
    int qtdUsada;

} tInformacaoJogada;

tInformacaoJogada AgrupaInformacoesJogada(char comando, int numColisoesParede, int numComidasPegas, int qtdUsadas);

tFantasma** InicializaFantasmas(tJogo* jogo);

void ImprimeMapaJogo(tMapa* mapa);

void MoveFantasmasJogo(tJogo* jogo);

void ImprimeEstadoAtualJogo(tJogo* jogo, char acao);

void AtualizaMapa(tJogo* jogo, tPosicao* posicao_anterior_pacman);

void GeraInicializacao_txt(tMapa* mapa, tPacman* pacman);

tJogo* CriaJogo(char* caminhoConfig);

void DesalocaJogo(tJogo* jogo);

char LeComandoTeclado();

COMANDO ConverteComando(char acao);

char ConverteAcao(COMANDO comando);

int VerificaSePerdeuJogo(tJogo* jogo, char acao, tPosicao* pos_anterior_pacman);

void RealizaJogo(tJogo* jogo);

void GeraRanking_txt(tJogo* jogo);

void GeraEstatistica_txt(tJogo* jogo);

void GeraResumo_txt(tJogo* jogo);

void GeraArquivosGerais(tJogo* jogo);

#endif