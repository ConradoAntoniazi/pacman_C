#include "tFantasma.h"

tFantasma* CriaFantasma(tPosicao* posicao, char aparencia) {
    //posicao invalida:
    if(posicao == NULL) {
        printf("Posicao invalida - CriaFantasma(tPosicao* posicao)\n");
        return NULL;
    }
    //alocacao dinamica do fantasma e mensagem de erro:
    tFantasma* fantasma = (tFantasma*) malloc(sizeof(tFantasma));
    if(fantasma == NULL) {
        printf("Erro na alocacao do fantasma - CriaFantasma(tPosicao* posicao)\n");
        exit(1);
    }
    //definindo atributos
    fantasma->aparencia = aparencia;
    fantasma->posicaoAtual = posicao;
    fantasma->posicaoAnterior = ClonaPosicao(posicao);
    fantasma->sobreposComida = 0;
    fantasma->atravessaParede = 0;
    fantasma->estaSobreAlgo = 0;
    fantasma->algo = '\0';

    fantasma->estaVivo = 1;

    //definindo a direcao de cada fantasma de acondo sua representacao
    if(aparencia == 'B') {
        fantasma->direcao = MOV_ESQUERDA_FANT;
    } else if(aparencia == 'P') {
        fantasma->direcao = MOV_CIMA_FANT;
    } else if(aparencia == 'I') {
        fantasma->direcao = MOV_BAIXO_FANT;
    } else if(aparencia == 'C') {
        fantasma->direcao = MOV_DIREITA_FANT;
    } else if(aparencia == 'H') {
        fantasma->direcao = MOV_DIREITA_FANT;
        fantasma->atravessaParede = 1;
    } else if(aparencia == 'V') {
        fantasma->direcao = MOV_CIMA_FANT;
        fantasma->atravessaParede = 1;
    }

    return fantasma;
}

int EstaVivoFantasma(tFantasma* fantasma) {
    return fantasma->estaVivo;
}

void MataFantasma (tFantasma* fantasma) {
    fantasma->estaVivo = 0;
}

int ObtemDirecaoFantasma (tFantasma* fantasma) {
    return fantasma->direcao;
}

int AtravessaParedeFantasma(tFantasma* fantasma) {
    return fantasma->atravessaParede;
}

tPosicao* ObtemPosicaoAtualFantasma (tFantasma* fantasma) {
    return fantasma->posicaoAtual;
}

tPosicao* ObtemPosicaoAnteriorFantasma (tFantasma* fantasma) {
    return fantasma->posicaoAnterior;
}

char ObtemAparenciaFantasma (tFantasma* fantasma) {
    return fantasma->aparencia;
}

int EstaSobreComidaFantasma (tFantasma* fantasma) {
    return fantasma->sobreposComida;
}

tPosicao* ProximaPosFantasma (tFantasma* fantasma) {
    tPosicao* proxima_pos_fantasma = NULL;

    if(fantasma->direcao == MOV_ESQUERDA_FANT) {
        proxima_pos_fantasma = CriaPosicao(ObtemLinhaPosicao(fantasma->posicaoAtual), ObtemColunaPosicao(fantasma->posicaoAtual) - 1);

    } else if(fantasma->direcao == MOV_DIREITA_FANT) {
        proxima_pos_fantasma = CriaPosicao(ObtemLinhaPosicao(fantasma->posicaoAtual), ObtemColunaPosicao(fantasma->posicaoAtual) + 1);

    } else if(fantasma->direcao == MOV_CIMA_FANT) {
        proxima_pos_fantasma = CriaPosicao(ObtemLinhaPosicao(fantasma->posicaoAtual) - 1, ObtemColunaPosicao(fantasma->posicaoAtual));

    } else if(fantasma->direcao == MOV_BAIXO_FANT) {
        proxima_pos_fantasma = CriaPosicao(ObtemLinhaPosicao(fantasma->posicaoAtual) + 1, ObtemColunaPosicao(fantasma->posicaoAtual));

    }

    return proxima_pos_fantasma;
}

void InverteDirecaoFantasma (tFantasma* fantasma) {
    if(fantasma->direcao == MOV_ESQUERDA_FANT) {
        fantasma->direcao = MOV_DIREITA_FANT;
    } else if(fantasma->direcao == MOV_CIMA_FANT) {
        fantasma->direcao = MOV_BAIXO_FANT;
    } else if(fantasma->direcao == MOV_BAIXO_FANT) {
        fantasma->direcao = MOV_CIMA_FANT;
    } else if(fantasma->direcao == MOV_DIREITA_FANT) {
        fantasma->direcao = MOV_ESQUERDA_FANT;
    }
}

//funcao auxiliar
int EhFantasmaProximaPosicao (tMapa* mapa, tPosicao* proxima_pos_fantasma) {
    if(ObtemItemMapa(mapa, proxima_pos_fantasma) == 'B') {
        return 1;
    } else if(ObtemItemMapa(mapa, proxima_pos_fantasma) == 'C') {
        return 1;
    } else if(ObtemItemMapa(mapa, proxima_pos_fantasma) == 'I') {
        return 1;
    } else if(ObtemItemMapa(mapa, proxima_pos_fantasma) == 'P') {
        return 1;
    }

    return 0;
}

int EhExtremidadeMapa(tMapa* mapa, tPosicao* proxima_pos_fantasma) {
    if(EncontrouParedeMapa(mapa, proxima_pos_fantasma) && (ObtemLinhaPosicao(proxima_pos_fantasma) == 0)) {
        return 1;
    } else if(EncontrouParedeMapa(mapa, proxima_pos_fantasma) && (ObtemLinhaPosicao(proxima_pos_fantasma) + 1 == ObtemNumeroLinhasMapa(mapa))) {
        return 1;
    } else if(EncontrouParedeMapa(mapa, proxima_pos_fantasma) && (ObtemColunaPosicao(proxima_pos_fantasma) + 1 == ObtemNumeroColunasMapa(mapa))) {
        return 1;
    } else if(EncontrouParedeMapa(mapa, proxima_pos_fantasma) && (ObtemColunaPosicao(proxima_pos_fantasma) == 0)) {
        return 1;
    }

    return 0;
}

char ObtemAlgoFantasma(tFantasma* tFantasma) {
    return tFantasma->algo;
}

void MoveFantasmaEspecial (tFantasma* fantasma, tMapa* mapa, tPacman* pacman) {
    //ponteiro para a posicao auxiliar para as condicoes de movimentacao do pacman
    tPosicao* proxima_pos_fantasma = ProximaPosFantasma(fantasma);

    if (EncontrouParedeMapa(mapa, proxima_pos_fantasma) && EhExtremidadeMapa(mapa, proxima_pos_fantasma) && AtravessaParedeFantasma(fantasma)) {
        //atualizando a proxima posicao do fantasma
        tPosicao* aux = NULL;
        if(ObtemDirecaoFantasma(fantasma) == MOV_ESQUERDA_FANT) {
            aux = CriaPosicao(ObtemLinhaPosicao(ObtemPosicaoAtualFantasma(fantasma)), ObtemNumeroColunasMapa(mapa) - 2);
            AtualizaPosicao(proxima_pos_fantasma, aux);

        } else if(ObtemDirecaoFantasma(fantasma) == MOV_DIREITA_FANT) {
            aux = CriaPosicao(ObtemLinhaPosicao(ObtemPosicaoAtualFantasma(fantasma)), 1);
            AtualizaPosicao(proxima_pos_fantasma, aux);

        } else if(ObtemDirecaoFantasma(fantasma) == MOV_CIMA_FANT) {
            aux = CriaPosicao(ObtemNumeroLinhasMapa(mapa) - 2, ObtemColunaPosicao(ObtemPosicaoAtualFantasma(fantasma)));
            AtualizaPosicao(proxima_pos_fantasma, aux);

        } else if(ObtemDirecaoFantasma(fantasma) == MOV_BAIXO_FANT) {
            aux = CriaPosicao(1 , ObtemColunaPosicao(ObtemPosicaoAtualFantasma(fantasma)));
            AtualizaPosicao(proxima_pos_fantasma, aux);
        } 

        //atualizando a posicao em que o fantasma deixará
        if(EstaSobreComidaFantasma(fantasma)) {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, '*');
            fantasma->sobreposComida = 0;
        } else if(fantasma->estaSobreAlgo){
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, fantasma->algo);
            fantasma->estaSobreAlgo = 0;
        } else {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, ' ');
        }
        //atualizando a proxima posicao dele se vai sobrepor algo
        fantasma->estaSobreAlgo = 1;
        fantasma->algo = ObtemItemMapa(mapa, proxima_pos_fantasma);

        AtualizaPosicao(fantasma->posicaoAtual, proxima_pos_fantasma);

    } else if(EncontrouComidaMapa(mapa, proxima_pos_fantasma)) {
        //atualizando a posicao em que o fantasma deixará
        if(EstaSobreComidaFantasma(fantasma)) {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, '*');
            fantasma->sobreposComida = 0;
        } else if(fantasma->estaSobreAlgo){
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, fantasma->algo);
            fantasma->estaSobreAlgo = 0;
        } else {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, ' ');
        }

        AtualizaPosicao(fantasma->posicaoAtual, proxima_pos_fantasma);
        //atribuindo que o fantasma pegou comida
        fantasma->sobreposComida = 1;

    } else if (ObtemItemMapa(mapa, proxima_pos_fantasma) == '>') {
        //fantasma morreu
        if(pacman->mataFantasma) {
            fantasma->estaVivo = 0;
        }
        if(EstaSobreComidaFantasma(fantasma)) {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, '*');
            fantasma->sobreposComida = 0;
        } else if(fantasma->estaSobreAlgo){
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, fantasma->algo);
            fantasma->estaSobreAlgo = 0;
        } else {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, ' ');
        }
        AtualizaPosicao(fantasma->posicaoAtual, proxima_pos_fantasma);

    } else {
        //atualizando a posicao em que o fantasma deixará
        if(EstaSobreComidaFantasma(fantasma)) {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, '*');
            fantasma->sobreposComida = 0;
        } else if(fantasma->estaSobreAlgo){
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, fantasma->algo);
            fantasma->estaSobreAlgo = 0;
        } else {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, ' ');
        }

        AtualizaPosicao(fantasma->posicaoAtual, proxima_pos_fantasma);
        //atribuindo que o fantasma pegou comida
        fantasma->estaSobreAlgo = 1;
        char prox_item = ObtemItemMapa(mapa, proxima_pos_fantasma);
        if(prox_item == 'V' || prox_item == 'H') {
            fantasma->algo = '#';
        } else {
            fantasma->algo = ObtemItemMapa(mapa, proxima_pos_fantasma);
        }
    }

    DesalocaPosicao(proxima_pos_fantasma);
}
    
void MoveFantasma (tFantasma* fantasma, tMapa* mapa, tPacman* pacman) {
    //ponteiro para a posicao auxiliar para as condicoes de movimentacao do pacman
    tPosicao* proxima_pos_fantasma = ProximaPosFantasma(fantasma);

    //atualizando a posicao anterior do fantasma
    AtualizaPosicao(fantasma->posicaoAnterior, fantasma->posicaoAtual);

    if(EncontrouComidaMapa(mapa, proxima_pos_fantasma)) {
        //atualizando a posicao em que o fantasma deixará
        if(EstaSobreComidaFantasma(fantasma)) {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, '*');
            fantasma->sobreposComida = 0;
        } else if (PossuiTunelMapa(mapa) && AcessouTunelMapa(mapa, fantasma->posicaoAtual)) {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, '@');
        } else {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, ' ');
        }
        AtualizaPosicao(fantasma->posicaoAtual, proxima_pos_fantasma);
        //atribuindo que o fantasma pegou comida
        fantasma->sobreposComida = 1;

    } else if (ObtemItemMapa(mapa, proxima_pos_fantasma) == '>') {
        //fantasma morreu
        if(pacman->mataFantasma) {
            fantasma->estaVivo = 0;
        }
        if(EstaSobreComidaFantasma(fantasma)) {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, '*');
            fantasma->sobreposComida = 0;
        } else if (PossuiTunelMapa(mapa) && AcessouTunelMapa(mapa, fantasma->posicaoAtual)) {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, '@');
        } else {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, ' ');
        }
        AtualizaPosicao(fantasma->posicaoAtual, proxima_pos_fantasma);

    } else if (EncontrouParedeMapa(mapa, proxima_pos_fantasma)) {
        InverteDirecaoFantasma(fantasma);
        //atualizando a proxima posicao do fantasma
        tPosicao* aux = ProximaPosFantasma(fantasma);
        AtualizaPosicao(proxima_pos_fantasma, aux);
        DesalocaPosicao(aux);

        //atualizando a posicao em que o fantasma deixará
        if(EstaSobreComidaFantasma(fantasma)) {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, '*');
            fantasma->sobreposComida = 0;
        } else if (PossuiTunelMapa(mapa) && AcessouTunelMapa(mapa, fantasma->posicaoAtual)) {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, '@');
        } else {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, ' ');
        }

        if(EncontrouComidaMapa(mapa, proxima_pos_fantasma)) {
            //atribuindo que o fantasma pegou comida
            fantasma->sobreposComida = 1;
        }

        AtualizaPosicao(fantasma->posicaoAtual, proxima_pos_fantasma);

    } else {
        if(EstaSobreComidaFantasma(fantasma)) {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, '*');
            fantasma->sobreposComida = 0;
        } else if (PossuiTunelMapa(mapa) && AcessouTunelMapa(mapa, fantasma->posicaoAtual)) {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, '@');
        } else {
            AtualizaItemMapa(mapa, fantasma->posicaoAtual, ' ');
        }
        AtualizaPosicao(fantasma->posicaoAtual, proxima_pos_fantasma);
    }

    DesalocaPosicao(proxima_pos_fantasma);
}

void DesalocaFantasma (tFantasma* fantasma) {
    if(fantasma != NULL) {
        DesalocaPosicao(fantasma->posicaoAtual);
        DesalocaPosicao(fantasma->posicaoAnterior);
        free(fantasma);
    }
}