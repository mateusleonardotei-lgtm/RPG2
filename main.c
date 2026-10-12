#include <stdio.h>
#include "monstro.h"
// TESTES LEGAIS
int main()
{
    Monstro monstros[MAX_MONSTROS];

    int quantidade = 0;

    printf("===== CARREGANDO BESTIARIO =====\n");

    if (!carregarBestiario("bestiario.txt", monstros, MAX_MONSTROS, &quantidade)) {

        printf("Erro ao carregar bestiario!\n");

        return 1;
    }

    printf("\nBestiario carregado com sucesso!\n");

    printf("Total de monstros: %d\n", quantidade);

    printf("\n===== MONSTROS CARREGADOS =====\n");

    for (int i = 0; i < quantidade; i++) {

        printf("\nMonstro %d\n", i + 1);

        printf("ID: %d\n", monstros[i].id);

        printf("Nome: %s\n", monstros[i].nome);

        printf("Frequencia: %s\n", frequenciaParaTexto(monstros[i].frequencia));

        printf("Poder base: %d\n", monstros[i].poderBase);
    }

    return 0;
}