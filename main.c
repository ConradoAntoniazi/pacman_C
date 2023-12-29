#include "tJogo.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
    tJogo* jogo;

    if (argc <= 1) { //erro e quebra prematura do codigo
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado\n");
        return 0;
    }

    jogo = CriaJogo(argv[1]);
    RealizaJogo(jogo);
    GeraArquivosGerais(jogo);
    DesalocaJogo(jogo);

    return 0;
}