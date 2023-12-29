#include "tPosicao.h"
#include "tMovimento.h"
#include "tMapa.h"
#include "tPacman.h"
#include <stdio.h>
#include <stdlib.h>

tPacman* CriaPacman(tPosicao* posicao) {
    //posicao invalida:
    if(posicao == NULL) {
        return NULL;
    }
    //alocacao dinamica do pacman e mensagem de erro:
    tPacman* pacman = (tPacman*) malloc(sizeof(tPacman));
    if(pacman == NULL) {
        printf("Erro na alocacao do pacman - CriaPacman(tPosicao* posicao)\n");
        return NULL;
    }
    //=================: atribuindo os valores iniciais do pacman :=================

    //posicao:
    pacman->posicaoAtual = posicao;
    pacman->estaVivo = 1;
    pacman->powerUp = 0;
    pacman->sobreParede = 0;

    pacman->mataFantasma = 0;
    
    //numero de colisoes por cada movimento:
    pacman->nColisoesParedeBaixo = 0;
    pacman->nColisoesParedeCima = 0;
    pacman->nColisoesParedeDireita = 0;
    pacman->nColisoesParedeEsquerda = 0;
    //numero de frutas pegas por cada movimento:
    pacman->nFrutasComidasBaixo = 0;
    pacman->nFrutasComidasCima = 0;
    pacman->nFrutasComidasDireita = 0;
    pacman->nFrutasComidasEsquerda = 0;
    //numero de movimentos:
    pacman->nMovimentosBaixo = 0;
    pacman->nMovimentosCima = 0;
    pacman->nMovimentosDireita = 0;
    pacman->nMovimentosEsquerda = 0;
    //movimentos significativos:
    pacman->nMovimentosSignificativos = 0;
    pacman->historicoDeMovimentosSignificativos = NULL;
    //trilha:
    pacman->nLinhasTrilha = 0;
    pacman->nColunasTrilha = 0;
    pacman->trilha = NULL;

    return pacman;
}

void ZeraMataFantasma (tPacman* pacman) {
    pacman->mataFantasma = 0;
}

int MataFantasmaPacman (tPacman * pacman) {
    return pacman->mataFantasma;
}

tPacman* ClonaPacman(tPacman* pacman) {
    if(pacman == NULL) return NULL;

    //alocacao dinamica do pacman e mensagem de erro:
    tPacman* pacman_clone = NULL;
    pacman_clone = (tPacman*) malloc(sizeof(tPacman));
    if(pacman_clone == NULL) {
        printf("Erro na alocacao dinamica - ClonaPacman(tPacman* pacman)\n");
        exit(0);
    }
    
    //posicao:
    pacman_clone->posicaoAtual = ClonaPosicao(pacman->posicaoAtual);
    pacman_clone->historicoDeMovimentosSignificativos = NULL;
    pacman_clone->trilha = NULL;

    return pacman_clone;
}

tMovimento** ClonaHistoricoDeMovimentosSignificativosPacman(tPacman* pacman) {
    tMovimento** historico_clone = (tMovimento**) malloc (pacman->nMovimentosSignificativos * sizeof(tMovimento*));
    if(historico_clone == NULL) {
        printf("Erro na alocacao dinamica - ClonaHistoricoDeMovimentosSignificativosPacman(tPacman* pacman)\n");
        exit(0);
    }

    for(int i = 0; i < pacman->nMovimentosSignificativos; i++) {
        historico_clone[i] = CriaMovimento(pacman->historicoDeMovimentosSignificativos[i]->numeroDoMovimento, pacman->historicoDeMovimentosSignificativos[i]->comando, pacman->historicoDeMovimentosSignificativos[i]->acao);
    }

    return historico_clone;
}

tPosicao* ObtemPosicaoPacman(tPacman* pacman) {
    return pacman->posicaoAtual;
}

int EstaVivoPacman(tPacman* pacman) {
    return pacman->estaVivo;
}

tPosicao* ProximaPosPacman(tPacman* pacman, COMANDO comando) {
    tPosicao* proxima_pos_pacman = NULL;

    if(comando == MOV_ESQUERDA) {
        proxima_pos_pacman = CriaPosicao(ObtemLinhaPosicao(pacman->posicaoAtual), ObtemColunaPosicao(pacman->posicaoAtual) - 1);
        pacman->nMovimentosEsquerda++;

    } else if(comando == MOV_DIREITA) {
        proxima_pos_pacman = CriaPosicao(ObtemLinhaPosicao(pacman->posicaoAtual), ObtemColunaPosicao(pacman->posicaoAtual) + 1);
        pacman->nMovimentosDireita++;

    } else if(comando == MOV_CIMA) {
        proxima_pos_pacman = CriaPosicao(ObtemLinhaPosicao(pacman->posicaoAtual) - 1, ObtemColunaPosicao(pacman->posicaoAtual));
        pacman->nMovimentosCima++;

    } else if(comando == MOV_BAIXO) {
        proxima_pos_pacman = CriaPosicao(ObtemLinhaPosicao(pacman->posicaoAtual) + 1, ObtemColunaPosicao(pacman->posicaoAtual));
        pacman->nMovimentosBaixo++;
    }

    return proxima_pos_pacman;
}

void AtualizaComidasPegas(tPacman* pacman, COMANDO comando) {
    if(comando == MOV_ESQUERDA) {
        pacman->nFrutasComidasEsquerda++;

    } else if(comando == MOV_DIREITA) {
        pacman->nFrutasComidasDireita++;

    } else if(comando == MOV_CIMA) {
        pacman->nFrutasComidasCima++;

    } else if(comando == MOV_BAIXO) {
        pacman->nFrutasComidasBaixo++;
    }
}

void AtualizaColisoesComPadece(tPacman* pacman, COMANDO comando) {
    if(comando == MOV_ESQUERDA) {
        pacman->nColisoesParedeEsquerda++;

    } else if(comando == MOV_DIREITA) {
        pacman->nColisoesParedeDireita++;

    } else if(comando == MOV_CIMA) {
        pacman->nColisoesParedeCima++;

    } else if(comando == MOV_BAIXO) {
        pacman->nColisoesParedeBaixo++;
    }
}

int EhExtremidadeMapaPac(tMapa* mapa, tPosicao* proxima_pos) {
    if(ObtemLinhaPosicao(proxima_pos) == 0) return 1;
    else if(ObtemLinhaPosicao(proxima_pos) == ObtemNumeroLinhasMapa(mapa) - 1) return 1;
    else if(ObtemColunaPosicao(proxima_pos) == 0) return 1;
    else if(ObtemColunaPosicao(proxima_pos) == ObtemNumeroColunasMapa(mapa) - 1) return 1;

    return 0;
}

void MovePacman(tPacman* pacman, tMapa* mapa, COMANDO comando) {
    //ponteiro para a posicao auxiliar para as condicoes de movimentacao do pacman e ja atualiza o numero de movimentos para cada comando
    tPosicao* proxima_pos = ProximaPosPacman(pacman, comando);

    //inicializando a trilha:
    if(pacman->trilha == NULL) {
        CriaTrilhaPacman(pacman, ObtemNumeroLinhasMapa(mapa), ObtemNumeroColunasMapa(mapa));
    }

    //decrescendo após movimentos os powerUps
    if(pacman->powerUp > 0) {
        pacman->powerUp = pacman->powerUp - 1;
    }
    if(pacman->mataFantasma > 0) {
        pacman->mataFantasma = pacman->mataFantasma - 1;
    }

    if(EncontrouComidaMapa(mapa, proxima_pos)) {
        //atualiza o numero de comidas pegas de acordo com o comando
        AtualizaComidasPegas(pacman, comando);
        //colocando um espaço em branco na posicao antiga do pacman
        AtualizaItemMapa(mapa, pacman->posicaoAtual, ' ');
        //verificando se o pacman estava sobre um tunel:
        if(PossuiTunelMapa(mapa)) {
            if(EntrouTunel(ObtemTunelMapa(mapa), pacman->posicaoAtual)) {
                AtualizaItemMapa(mapa, pacman->posicaoAtual, '@');
            }
        }
        if (pacman->sobreParede) {
            AtualizaItemMapa(mapa, pacman->posicaoAtual, '#');
            pacman->sobreParede = 0;
        }

        AtualizaItemMapa(mapa, proxima_pos, '>');
        AtualizaPosicao(pacman->posicaoAtual, proxima_pos);

        //inserindo movimento significativo
        char acao[50] = "pegou comida";
        InsereNovoMovimentoSignificativoPacman(pacman, comando, acao);
        AtualizaTrilhaPacman(pacman);

    } else if(EncontrouParedeMapa(mapa, proxima_pos) && !EhExtremidadeMapaPac(mapa, proxima_pos) && pacman->powerUp > 0) {
        AtualizaItemMapa(mapa, pacman->posicaoAtual, ' ');
        //verificando se o pacman estava sobre um tunel:
        if(PossuiTunelMapa(mapa)) {
            if(EntrouTunel(ObtemTunelMapa(mapa), pacman->posicaoAtual)) {
                AtualizaItemMapa(mapa, pacman->posicaoAtual, '@');
            }
        } 
        if (pacman->sobreParede) {
            AtualizaItemMapa(mapa, pacman->posicaoAtual, '#');
            pacman->sobreParede = 0;
        }
        pacman->sobreParede = 1;

        AtualizaItemMapa(mapa, proxima_pos, '>');
        AtualizaPosicao(pacman->posicaoAtual, proxima_pos);
        AtualizaTrilhaPacman(pacman);

    } else if(EncontrouParedeMapa(mapa, proxima_pos)) {
        //atualiza o numero de colisoes com a parede de acordo com o comando
        AtualizaColisoesComPadece(pacman, comando);
        //inserindo movimento significativo
        if(pacman->sobreParede == 0) {
            char acao[50] = "colidiu com a parede";
            InsereNovoMovimentoSignificativoPacman(pacman, comando, acao);
        }

        AtualizaTrilhaPacman(pacman);

    } else if(ObtemItemMapa(mapa, proxima_pos) == ' ') {
        AtualizaItemMapa(mapa, pacman->posicaoAtual, ' ');
        //verificando se o pacman estava sobre um tunel:
        if(PossuiTunelMapa(mapa)) {
            if(EntrouTunel(ObtemTunelMapa(mapa), pacman->posicaoAtual)) {
                AtualizaItemMapa(mapa, pacman->posicaoAtual, '@');
            }
        } 
        if (pacman->sobreParede) {
            AtualizaItemMapa(mapa, pacman->posicaoAtual, '#');
            pacman->sobreParede = 0;
        }

        AtualizaPosicao(pacman->posicaoAtual, proxima_pos);
        AtualizaTrilhaPacman(pacman);

    } else if(ObtemItemMapa(mapa, proxima_pos) == '@') {
        AtualizaItemMapa(mapa, pacman->posicaoAtual, ' ');
        //verificando se o pacman estava sobre um tunel:
        if(PossuiTunelMapa(mapa)) {
            if(EntrouTunel(ObtemTunelMapa(mapa), pacman->posicaoAtual)) {
                AtualizaItemMapa(mapa, pacman->posicaoAtual, '@');
            }
        } 
        if (pacman->sobreParede) {
            AtualizaItemMapa(mapa, pacman->posicaoAtual, '#');
            pacman->sobreParede = 0;
        }

        AtualizaPosicao(pacman->posicaoAtual, proxima_pos);
        AtualizaTrilhaPacman(pacman);
        EntraTunelMapa(mapa, pacman->posicaoAtual);
        AtualizaTrilhaPacman(pacman);

    } else if (ObtemItemMapa(mapa, proxima_pos) == '+') {
        //colocando um espaço em branco na posicao antiga do pacman
        AtualizaItemMapa(mapa, pacman->posicaoAtual, ' ');
        //verificando se o pacman estava sobre um tunel:
        if(PossuiTunelMapa(mapa)) {
            if(EntrouTunel(ObtemTunelMapa(mapa), pacman->posicaoAtual)) {
                AtualizaItemMapa(mapa, pacman->posicaoAtual, '@');
            }
        } 
        if (pacman->sobreParede) {
            AtualizaItemMapa(mapa, pacman->posicaoAtual, '#');
            pacman->sobreParede = 0;
        }

        AtualizaItemMapa(mapa, proxima_pos, '>');
        AtualizaPosicao(pacman->posicaoAtual, proxima_pos);
        pacman->powerUp = 15;
        AtualizaTrilhaPacman(pacman);

    } else if (ObtemItemMapa(mapa, proxima_pos) == 'x') {
        //colocando um espaço em branco na posicao antiga do pacman
        AtualizaItemMapa(mapa, pacman->posicaoAtual, ' ');
        //verificando se o pacman estava sobre um tunel:
        if(PossuiTunelMapa(mapa)) {
            if(EntrouTunel(ObtemTunelMapa(mapa), pacman->posicaoAtual)) {
                AtualizaItemMapa(mapa, pacman->posicaoAtual, '@');
            }
        } 
        if (pacman->sobreParede) {
            AtualizaItemMapa(mapa, pacman->posicaoAtual, '#');
            pacman->sobreParede = 0;
        }

        AtualizaItemMapa(mapa, proxima_pos, '>');
        AtualizaPosicao(pacman->posicaoAtual, proxima_pos);
        pacman->mataFantasma = 15;
        AtualizaTrilhaPacman(pacman);

    } else { //caso dê de cara com algum fantasma
        AtualizaItemMapa(mapa, pacman->posicaoAtual, ' ');
        AtualizaPosicao(pacman->posicaoAtual, proxima_pos);

        if(pacman->mataFantasma > 0) {
            AtualizaItemMapa(mapa, proxima_pos, '>');
            AtualizaTrilhaPacman(pacman);
            pacman->mataFantasma = 0;
        }
    }

    if(pacman->sobreParede && pacman->powerUp == 0) {
        MataPacman(pacman);
    }

    DesalocaPosicao(proxima_pos);
}

void CriaTrilhaPacman(tPacman* pacman, int nLinhas, int nColunas) {
    //atribuindo as dimensoes da trilha:
    pacman->nLinhasTrilha = nLinhas;
    pacman->nColunasTrilha = nColunas;  

    //fazendo alocacao dinamia e definindo todas as posicoes como -1:
    if(pacman->trilha == NULL) {
        pacman->trilha = (int**) malloc(nLinhas * sizeof(int*));
        if(pacman->trilha == NULL) {
            printf("Erro na alocacao dinamica - CriaTrilhaPacman(tPacman* pacman, int nLinhas, int nColunas) - malloc int**\n");
            exit(0);
        }
        for(int i = 0; i < nLinhas; i++) {
            pacman->trilha[i] = (int*) malloc(nColunas * sizeof(int));
            if(pacman->trilha[i] == NULL) {
            printf("Erro na alocacao dinamica - CriaTrilhaPacman(tPacman* pacman, int nLinhas, int nColunas) - malloc int*\n");
            exit(0);
            }
            //loop para varrer cada posicao das linhas e atribuir -1
            for(int j = 0; j < nColunas; j++) {
                pacman->trilha[i][j] = -1;
            }
        }
    }
}

void AtualizaTrilhaPacman(tPacman* pacman) {
    int l, c;

    l = ObtemLinhaPosicao(pacman->posicaoAtual);
    c = ObtemColunaPosicao(pacman->posicaoAtual);

    pacman->trilha[l][c] = ObtemNumeroAtualMovimentosPacman(pacman);
}

void SalvaTrilhaPacman(tPacman* pacman) {
    FILE* trilha_txt;

    trilha_txt = fopen("trilha.txt", "w");

    for(int l = 0; l < pacman->nLinhasTrilha; l++) {
        for(int c = 0; c < pacman->nColunasTrilha; c++) {
            if(pacman->trilha[l][c] == -1) {
                fprintf(trilha_txt, "#");
            }
            else {
                fprintf(trilha_txt, "%d", pacman->trilha[l][c]);
            }
            if(c != pacman->nColunasTrilha - 1) {
                fprintf(trilha_txt, " ");
            }
        }
        fprintf(trilha_txt, "\n");
    }

    fclose(trilha_txt);
}

void InsereNovoMovimentoSignificativoPacman(tPacman* pacman, COMANDO comando, const char* acao) {
    pacman->nMovimentosSignificativos++;

    if(pacman->historicoDeMovimentosSignificativos == NULL) {
        pacman->historicoDeMovimentosSignificativos = (tMovimento**) malloc(sizeof(tMovimento*));
        if(pacman->historicoDeMovimentosSignificativos == NULL) printf("Erro na alocacao - InsereNovoMovimentoSignificativoPacman() - malloc\n");
    } else {
        pacman->historicoDeMovimentosSignificativos = (tMovimento**) realloc(pacman->historicoDeMovimentosSignificativos, pacman->nMovimentosSignificativos * sizeof(tMovimento*));
        if(pacman->historicoDeMovimentosSignificativos == NULL) printf("Erro na alocacao - InsereNovoMovimentoSignificativoPacman() - realloc\n");
    }

    pacman->historicoDeMovimentosSignificativos[pacman->nMovimentosSignificativos - 1] = CriaMovimento(ObtemNumeroAtualMovimentosPacman(pacman), comando, acao);
}

void MataPacman(tPacman* pacman) {
    pacman->estaVivo = 0;
}

void DesalocaPacman(tPacman* pacman) {
    if(pacman != NULL) {
        //desalocando posicao:
        DesalocaPosicao(pacman->posicaoAtual);
        //desalocando toda a matriz do historico de movimentos significativos:
        if(pacman->historicoDeMovimentosSignificativos != NULL) {
            for(int i = 0; i < pacman->nMovimentosSignificativos; i++) {
                DesalocaMovimento(pacman->historicoDeMovimentosSignificativos[i]);
            }
            free(pacman->historicoDeMovimentosSignificativos);
        }
        //desalocando a trilha:
        if(pacman->trilha != NULL) {
            for(int j = 0; j < pacman->nLinhasTrilha; j++) {
                free(pacman->trilha[j]);
            }
            free(pacman->trilha);
        }
        //desalocando o proprio pacman:
        free(pacman);
    }
}

int ObtemNumeroAtualMovimentosPacman(tPacman* pacman) {
    return (pacman->nMovimentosBaixo + pacman->nMovimentosCima + pacman->nMovimentosDireita + pacman->nMovimentosEsquerda);
}

int ObtemNumeroMovimentosSemPontuarPacman(tPacman* pacman) {
    return (ObtemNumeroAtualMovimentosPacman(pacman) - ObtemPontuacaoAtualPacman(pacman));
}

int ObtemNumeroColisoesParedePacman(tPacman* pacman) {
    return (pacman->nColisoesParedeBaixo + pacman->nColisoesParedeCima + pacman->nColisoesParedeDireita + pacman->nColisoesParedeEsquerda);
}

int ObtemNumeroMovimentosBaixoPacman(tPacman* pacman) {
    return pacman->nMovimentosBaixo;
}

int ObtemNumeroFrutasComidasBaixoPacman(tPacman* pacman) {
    return pacman->nFrutasComidasBaixo;
}

int ObtemNumeroColisoesParedeBaixoPacman(tPacman* pacman) {
    return pacman->nColisoesParedeBaixo;
}

int ObtemNumeroMovimentosCimaPacman(tPacman* pacman) {
    return pacman->nMovimentosCima;
}

int ObtemNumeroFrutasComidasCimaPacman(tPacman* pacman) {
    return pacman->nFrutasComidasCima;
}

int ObtemNumeroColisoesParedeCimaPacman(tPacman* pacman) {
    return pacman->nColisoesParedeCima;
}

int ObtemNumeroMovimentosEsquerdaPacman(tPacman* pacman) {
    return pacman->nMovimentosEsquerda;
}

int ObtemNumeroFrutasComidasEsquerdaPacman(tPacman* pacman) {
 return pacman->nFrutasComidasEsquerda;
}

int ObtemNumeroColisoesParedeEsquerdaPacman(tPacman* pacman) {
    return pacman->nColisoesParedeEsquerda;
}

int ObtemNumeroMovimentosDireitaPacman(tPacman* pacman) {
    return pacman->nMovimentosDireita;
}

int ObtemNumeroFrutasComidasDireitaPacman(tPacman* pacman) {
    return pacman->nFrutasComidasDireita;
}

int ObtemNumeroColisoesParedeDireitaPacman(tPacman* pacman) {
    return pacman->nColisoesParedeDireita;
}

int ObtemNumeroMovimentosSignificativosPacman(tPacman* pacman) {
    return pacman->nMovimentosSignificativos;
}

int ObtemPontuacaoAtualPacman(tPacman* pacman) {
    return (pacman->nFrutasComidasBaixo + pacman->nFrutasComidasCima + pacman->nFrutasComidasDireita + pacman->nFrutasComidasEsquerda);
}