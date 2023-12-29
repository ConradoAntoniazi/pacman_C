#include "tJogo.h"
#include <stdio.h>
#include <stdlib.h>

tInformacaoJogada AgrupaInformacoesJogada(char comando, int numColisoesParede, int numComidasPegas, int qtdUsadas) {
    tInformacaoJogada informacaoJogada;

    informacaoJogada.comando = comando;
    informacaoJogada.numColisoesParede = numColisoesParede;
    informacaoJogada.numComidasPegas = numComidasPegas;
    informacaoJogada.qtdUsada = qtdUsadas;

    return informacaoJogada;
}

tFantasma** InicializaFantasmas(tJogo* jogo) {
    tFantasma** fantasmas = (tFantasma**) malloc(sizeof(tFantasma*));
    char aparencias_fantasmas[NUM_FANTASMAS] = {'B', 'C', 'P', 'I', 'V', 'H'};
    tPosicao* pos_aux_fant = NULL;

    for(int i = 0; i < NUM_FANTASMAS; i++) {
        pos_aux_fant = ObtemPosicaoItemMapa(jogo->mapa, aparencias_fantasmas[i]);
        if(pos_aux_fant != NULL) {
            jogo->numFantasmas++;
            
            if(jogo->numFantasmas > 1) {
                fantasmas = (tFantasma**) realloc (fantasmas, jogo->numFantasmas * sizeof(tFantasma*));
            }
            fantasmas[jogo->numFantasmas - 1] = CriaFantasma(ClonaPosicao(pos_aux_fant), aparencias_fantasmas[i]);
            DesalocaPosicao(pos_aux_fant);
        }
    }

    return fantasmas;
}

void ImprimeMapaJogo(tMapa* mapa) {
    for(int i = 0; i < ObtemNumeroLinhasMapa(mapa); i++) {
        for(int j = 0; j < ObtemNumeroColunasMapa(mapa); j++) {
            tPosicao* posicao = CriaPosicao(i, j);
            printf("%c", ObtemItemMapa(mapa, posicao));
            DesalocaPosicao(posicao);
        }
        printf("\n");
    }
}

void MoveFantasmasJogo(tJogo* jogo) {
    for(int i = 0; i < jogo->numFantasmas; i++) {
        if(ObtemAparenciaFantasma(jogo->fantasmas[i]) == 'H' || ObtemAparenciaFantasma(jogo->fantasmas[i]) == 'V'){
            MoveFantasmaEspecial(jogo->fantasmas[i], jogo->mapa, jogo->pacman);

        } else {
            MoveFantasma(jogo->fantasmas[i], jogo->mapa, jogo->pacman);
        }
    }
}

void ImprimeEstadoAtualJogo(tJogo* jogo, char acao) {
    printf("Estado do jogo apos o movimento '%c':\n", acao);
    ImprimeMapaJogo(jogo->mapa);
    printf("Pontuacao: %d\n\n", ObtemPontuacaoAtualPacman(jogo->pacman));
}

void AtualizaMapa(tJogo* jogo, tPosicao* posicao_anterior_pacman) {
    
    AtualizaItemMapa(jogo->mapa, ObtemPosicaoPacman(jogo->pacman), '>');

    //atualizando a saida do pacman de um tunel
    if(ObtemTunelMapa(jogo->mapa) != NULL) {
        if(EntrouTunel(ObtemTunelMapa(jogo->mapa), posicao_anterior_pacman) && !SaoIguaisPosicao(ObtemPosicaoPacman(jogo->pacman), posicao_anterior_pacman)) AtualizaItemMapa(jogo->mapa, posicao_anterior_pacman, '@');
    }
    for(int i = 0; i < jogo->numFantasmas; i++) {
        if(EstaVivoFantasma(jogo->fantasmas[i])) {
            AtualizaItemMapa(jogo->mapa, ObtemPosicaoAtualFantasma(jogo->fantasmas[i]), ObtemAparenciaFantasma(jogo->fantasmas[i]));
        }
    }
}

void GeraInicializacao_txt(tMapa* mapa, tPacman* pacman) {
    FILE* inicializacao_txt;

    inicializacao_txt = fopen("inicializacao.txt", "w");

    for(int l = 0; l < ObtemNumeroLinhasMapa(mapa); l++) {
        for(int c = 0; c < ObtemNumeroColunasMapa(mapa); c++) {
            tPosicao* posicao = CriaPosicao(l, c);
            fprintf(inicializacao_txt, "%c", ObtemItemMapa(mapa, posicao));
            DesalocaPosicao(posicao);
        }
        fprintf(inicializacao_txt, "\n");
    }

    fprintf(inicializacao_txt, "Pac-Man comecara o jogo na linha %d e coluna %d", ObtemLinhaPosicao(ObtemPosicaoPacman(pacman)) + 1, ObtemColunaPosicao(ObtemPosicaoPacman(pacman)) + 1);
    
    fclose(inicializacao_txt);
}

tJogo* CriaJogo(char* caminhoConfig) {
    tJogo* jogo = (tJogo*) malloc(sizeof(tJogo));
    if(jogo == NULL) {
        printf("Erro na alocacao do jogo - CriaJogo()\n");
        exit(1);
    }
    jogo->numFantasmas = 0;
    jogo->mapa = CriaMapa(caminhoConfig);
    jogo->pacman = CriaPacman(ObtemPosicaoItemMapa(jogo->mapa, '>'));
    CriaTrilhaPacman(jogo->pacman, ObtemNumeroLinhasMapa(jogo->mapa), ObtemNumeroColunasMapa(jogo->mapa));
    //ja atualizando a trilha na posicao 0 (inicial)
    AtualizaTrilhaPacman(jogo->pacman);
    jogo->fantasmas = InicializaFantasmas(jogo);
    //gerando inicializacao.txt
    GeraInicializacao_txt(jogo->mapa, jogo->pacman);
    return jogo;
}

void DesalocaJogo(tJogo* jogo) {
    DesalocaPacman(jogo->pacman);
    DesalocaMapa(jogo->mapa);
    for(int i = 0; i < jogo->numFantasmas; i++) {
        DesalocaFantasma(jogo->fantasmas[i]);
    }
    free(jogo->fantasmas);
    free(jogo);
}

char LeComandoTeclado() {
    char acao;

    scanf("%*[^wasd]");
    scanf("%c", &acao);

    return acao;
}

COMANDO ConverteComando(char acao) {
    if(acao == 'a') return MOV_ESQUERDA;
    else if(acao == 'w') return MOV_CIMA;
    else if(acao == 's') return MOV_BAIXO;
    else if(acao == 'd') return MOV_DIREITA;

    return -1; //erro
}

char ConverteAcao(COMANDO comando) {
    if(comando == MOV_ESQUERDA) return 'a';
    else if(comando == MOV_CIMA) return 'w';
    else if(comando == MOV_BAIXO) return 's';
    else if(comando == MOV_DIREITA) return 'd';

    return 'x'; //erro
}

int VerificaSePerdeuJogo(tJogo* jogo, char acao, tPosicao* pos_anterior_pacman) {
    int pontuacao_final = ObtemPontuacaoAtualPacman(jogo->pacman);

    if(!EstaVivoPacman(jogo->pacman)) {
        AtualizaItemMapa(jogo->mapa, ObtemPosicaoPacman(jogo->pacman), '#');
        ImprimeEstadoAtualJogo(jogo, acao);
        printf("Game over!\nPontuacao final: %d", pontuacao_final);
        return 1;
    }
    COMANDO comando = ConverteComando(acao);
    char acaoSig[50] = "fim de jogo por encostar em um fantasma";
    //verificando se algum dos fantasma esta na mesma posicao do pacman
    for(int i = 0; i < jogo->numFantasmas; i++) {
        if(MataFantasmaPacman(jogo->pacman) > 0) {
            if(SaoIguaisPosicao(ObtemPosicaoAtualFantasma(jogo->fantasmas[i]), ObtemPosicaoPacman(jogo->pacman))) {
                MataFantasma(jogo->fantasmas[i]);
                AtualizaItemMapa(jogo->mapa, ObtemPosicaoPacman(jogo->pacman), '>');
                //zerando o atributo mata fantasma para nao poder matar outro
                ZeraMataFantasma(jogo->pacman);
                return 0;

            } else if(SaoIguaisPosicao(ObtemPosicaoAnteriorFantasma(jogo->fantasmas[i]), ObtemPosicaoPacman(jogo->pacman)) &&
                    SaoIguaisPosicao(ObtemPosicaoAtualFantasma(jogo->fantasmas[i]), pos_anterior_pacman)) {

                MataFantasma(jogo->fantasmas[i]);
                //zerando o atributo mata fantasma para nao poder matar outro
                ZeraMataFantasma(jogo->pacman);
                return 0;
            }
        }
        else {
           if(SaoIguaisPosicao(ObtemPosicaoAtualFantasma(jogo->fantasmas[i]), ObtemPosicaoPacman(jogo->pacman))) {
                InsereNovoMovimentoSignificativoPacman(jogo->pacman, comando, acaoSig);
                ImprimeEstadoAtualJogo(jogo, acao);
                printf("Game over!\nPontuacao final: %d", pontuacao_final);
                return 1;

            } else if(SaoIguaisPosicao(ObtemPosicaoAnteriorFantasma(jogo->fantasmas[i]), ObtemPosicaoPacman(jogo->pacman)) &&
                    SaoIguaisPosicao(ObtemPosicaoAtualFantasma(jogo->fantasmas[i]), pos_anterior_pacman)) {
                
                InsereNovoMovimentoSignificativoPacman(jogo->pacman, comando, acaoSig);
                AtualizaItemMapa(jogo->mapa, ObtemPosicaoPacman(jogo->pacman), ' ');
                ImprimeEstadoAtualJogo(jogo, acao);
                printf("Game over!\nPontuacao final: %d", pontuacao_final);
                return 1;
            } 
        }
    }

    return 0;
}

void RealizaJogo(tJogo* jogo) {
    char acao;
    int numComidasIniciaisJogo = ObtemQuantidadeFrutasIniciaisMapa(jogo->mapa);

    while (ObtemNumeroAtualMovimentosPacman(jogo->pacman) < ObtemNumeroMaximoMovimentosMapa(jogo->mapa)) {
        tPosicao* pos_anterior_pacman = ClonaPosicao(ObtemPosicaoPacman(jogo->pacman));

        //lendo o comando da entrada padrao
        acao = LeComandoTeclado();

        //fazendo as atualizacoes de acordo com o comando dado
        MoveFantasmasJogo(jogo);
        MovePacman(jogo->pacman, jogo->mapa, ConverteComando(acao));
        AtualizaMapa(jogo, pos_anterior_pacman);
        //verificando se o jogo acabou por contato com fantasma
        if(VerificaSePerdeuJogo(jogo, acao, pos_anterior_pacman)) {
            DesalocaPosicao(pos_anterior_pacman);
            return;
        }
        //caso a condicao anterior n tenha acontecio, o jogo continua
        ImprimeEstadoAtualJogo(jogo, acao);
        //verificando se venceu o jogo
        if(ObtemPontuacaoAtualPacman(jogo->pacman) == numComidasIniciaisJogo) {
            printf("Voce venceu!\nPontuacao final: %d\n", numComidasIniciaisJogo);
            DesalocaPosicao(pos_anterior_pacman);
            return;
        }
        DesalocaPosicao(pos_anterior_pacman);
    }
    printf("Game over!\nPontuacao final: %d", ObtemPontuacaoAtualPacman(jogo->pacman)); //numero maximo de movimentos atingidos e consequentemente game over
}

void GeraRanking_txt(tJogo* jogo) {
    FILE* ranking_txt;
    tInformacaoJogada InformacoesJogadas[4]; //4 pois so há 4 comandos (w, a, s, d)
    tInformacaoJogada aux; //usando para fazer as trocas na ordenacao

    ranking_txt = fopen("ranking.txt", "w");

    //ja ordenando em ordem alfabetica
    InformacoesJogadas[0] = AgrupaInformacoesJogada('a', ObtemNumeroColisoesParedeEsquerdaPacman(jogo->pacman), ObtemNumeroFrutasComidasEsquerdaPacman(jogo->pacman), ObtemNumeroMovimentosEsquerdaPacman(jogo->pacman));
    InformacoesJogadas[1] = AgrupaInformacoesJogada('d', ObtemNumeroColisoesParedeDireitaPacman(jogo->pacman), ObtemNumeroFrutasComidasDireitaPacman(jogo->pacman), ObtemNumeroMovimentosDireitaPacman(jogo->pacman));
    InformacoesJogadas[2] = AgrupaInformacoesJogada('s', ObtemNumeroColisoesParedeBaixoPacman(jogo->pacman), ObtemNumeroFrutasComidasBaixoPacman(jogo->pacman), ObtemNumeroMovimentosBaixoPacman(jogo->pacman));
    InformacoesJogadas[3] = AgrupaInformacoesJogada('w', ObtemNumeroColisoesParedeCimaPacman(jogo->pacman), ObtemNumeroFrutasComidasCimaPacman(jogo->pacman), ObtemNumeroMovimentosCimaPacman(jogo->pacman));

    for(int i = 0; i < 4; i++) {
        for(int j = i + 1; j < 4; j++) {
            if(InformacoesJogadas[j].numComidasPegas > InformacoesJogadas[i].numComidasPegas) {
                aux = InformacoesJogadas[i];
                InformacoesJogadas[i] = InformacoesJogadas[j];
                InformacoesJogadas[j] = aux;

            } else if(InformacoesJogadas[j].numComidasPegas == InformacoesJogadas[i].numComidasPegas &&
                    InformacoesJogadas[j].numColisoesParede < InformacoesJogadas[i].numColisoesParede) {
                    
                aux = InformacoesJogadas[i];
                InformacoesJogadas[i] = InformacoesJogadas[j];
                InformacoesJogadas[j] = aux;

            } else if(InformacoesJogadas[j].numComidasPegas == InformacoesJogadas[i].numComidasPegas &&
                    InformacoesJogadas[j].numColisoesParede == InformacoesJogadas[i].numColisoesParede &&
                    InformacoesJogadas[j].qtdUsada > InformacoesJogadas[i].qtdUsada) {
                    
                aux = InformacoesJogadas[i];
                InformacoesJogadas[i] = InformacoesJogadas[j];
                InformacoesJogadas[j] = aux;
            }
        } //fim for de dentro
    } //fim for de fora

    for(int i = 0; i < 4; i++) {
        fprintf(ranking_txt, "%c,%d,%d,%d\n", InformacoesJogadas[i].comando, InformacoesJogadas[i].numComidasPegas, InformacoesJogadas[i].numColisoesParede, InformacoesJogadas[i].qtdUsada);
    }

    fclose(ranking_txt);
}

void GeraEstatistica_txt(tJogo* jogo) {
    FILE* estatistica_txt;

    estatistica_txt = fopen("estatisticas.txt", "w");

    fprintf(estatistica_txt, "Numero de movimentos: %d\n", ObtemNumeroAtualMovimentosPacman(jogo->pacman));
    fprintf(estatistica_txt, "Numero de movimentos sem pontuar: %d\n", ObtemNumeroMovimentosSemPontuarPacman(jogo->pacman));
    fprintf(estatistica_txt, "Numero de colisoes com parede: %d\n", ObtemNumeroColisoesParedePacman(jogo->pacman));
    fprintf(estatistica_txt, "Numero de movimentos para baixo: %d\n", ObtemNumeroMovimentosBaixoPacman(jogo->pacman));
    fprintf(estatistica_txt, "Numero de movimentos para cima: %d\n", ObtemNumeroMovimentosCimaPacman(jogo->pacman));
    fprintf(estatistica_txt, "Numero de movimentos para esquerda: %d\n", ObtemNumeroMovimentosEsquerdaPacman(jogo->pacman));
    fprintf(estatistica_txt, "Numero de movimentos para direita: %d", ObtemNumeroMovimentosDireitaPacman(jogo->pacman));

    fclose(estatistica_txt);
}

void GeraResumo_txt(tJogo* jogo) {
    FILE* resumo_txt;
    tMovimento** clone_historico = ClonaHistoricoDeMovimentosSignificativosPacman(jogo->pacman);
    int nMovSig = ObtemNumeroMovimentosSignificativosPacman(jogo->pacman);

    resumo_txt = fopen("resumo.txt", "w");
    if(resumo_txt == NULL) {
        printf("Erro ao gerar o arquivo resumo.txt\n");
    }

    for(int i = 0; i < nMovSig; i++) {
        COMANDO comando = ObtemComandoMovimento(clone_historico[i]);
        fprintf(resumo_txt, "Movimento %d (%c) %s\n", ObtemNumeroMovimento(clone_historico[i]), ConverteAcao(comando), ObtemAcaoMovimento(clone_historico[i]));

    }
    if(clone_historico != NULL) {
        //desalocando o histórico clonado
        for(int j = 0; j < nMovSig; j++) {
            DesalocaMovimento(clone_historico[j]);
        }
        free(clone_historico);
    }
    
    fclose(resumo_txt);
}

void GeraArquivosGerais(tJogo* jogo) {
    GeraEstatistica_txt(jogo);
    GeraRanking_txt(jogo);
    GeraResumo_txt(jogo);
    SalvaTrilhaPacman(jogo->pacman);
}