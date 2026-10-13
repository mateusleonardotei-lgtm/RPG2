#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "bestiario.h"

int lerInteiro(const char *mensagem)
{
    char entrada[100];
    char *fim;
    long numero;

    while (1) {

        printf("%s", mensagem);

        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            return -1;
        }

        // Verifica se a entrada ultrapassou o tamanho
        if (strchr(entrada, '\n') == NULL && !feof(stdin)) {

            int caractere;

            while ((caractere = getchar()) != '\n' && caractere != EOF) {
            }

            printf("Entrada muito longa!\n");
            continue;
        }

        numero = strtol(entrada, &fim, 10);

        if (entrada == fim) {
            printf("Digite um numero valido!\n");
            continue;
        }

        // Ignora espacos e quebras de linha
        while (*fim == ' ' || *fim == '\n' || *fim == '\t' || *fim == '\r') {
            fim++;
        }

        if (*fim != '\0' || numero < INT_MIN || numero > INT_MAX) {

            printf("Digite um numero inteiro valido!\n");
            continue;
        }

        return (int)numero;
    }
}

int lerNome(char nome[], int tamanho)
{
    if (nome == NULL || tamanho <= 1) {
        return 0;
    }

    if (fgets(nome, tamanho, stdin) == NULL) {
        return 0;
    }

    // Verifica se existe quebra de linha
    char *quebra = strchr(nome, '\n');

    if (quebra != NULL) {

        *quebra = '\0';

    } else if (!feof(stdin)) {

        // Limpa o restante da entrada
        int caractere;

        while ((caractere = getchar()) != '\n' && caractere != EOF) {
        }

        printf("Nome muito longo!\n");
        return 0;
    }

    if (nome[0] == '\0') {
        printf("O nome nao pode ser vazio!\n");
        return 0;
    }

    return 1;
}

void mostrarMenu(void)
{
    printf("\n---------- BESTIARIO RPG ----------\n");

    printf("1 - Listar todos os monstros\n");
    printf("2 - Buscar monstro por ID\n");
    printf("3 - Buscar monstro por nome\n");
    printf("4 - Exibir ficha completa por ID\n");
    printf("5 - Filtrar monstros\n");
    printf("0 - Sair\n");
}

void menuFiltros(Bestiario *bestiario)
{
    int opcao;
    int escolha;

    do {

        printf("\n---------- FILTRAR MONSTROS ----------\n");

        printf("1 - Filtrar por frequencia\n");
        printf("2 - Filtrar por terreno\n");
        printf("3 - Filtrar por clima\n");
        printf("0 - Voltar\n");

        opcao = lerInteiro("\nEscolha uma opcao: ");

        switch (opcao) {

            case 1:

                printf("\n===== FREQUENCIAS =====\n");

                for (int i = MUITO_RARO; i <= MUITO_COMUM; i++) {

                    printf("%d - %s\n", i, frequenciaParaTexto((Frequencia)i));
                }

                escolha = lerInteiro("Escolha a frequencia: ");

                if (escolha < MUITO_RARO || escolha > MUITO_COMUM) {

                    printf("Frequencia invalida!\n");
                    break;
                }

                filtrarMonstrosPorFrequencia(bestiario->monstros, bestiario->quantidade, (Frequencia)escolha);

                break;

            case 2:

                printf("\n===== TERRENOS =====\n");

                for (int i = FLORESTA; i <= URBANO; i++) {

                    printf("%d - %s\n", i, terrenoParaTexto((Terreno)i));
                }

                escolha = lerInteiro("Escolha o terreno: ");

                if (escolha < FLORESTA || escolha > URBANO) {

                    printf("Terreno invalido!\n");
                    break;
                }

                filtrarMonstrosPorTerreno(bestiario->monstros, bestiario->quantidade, (Terreno)escolha);

                break;

            case 3:

                printf("\n===== CLIMAS =====\n");

                for (int i = QUALQUER; i <= POLAR; i++) {

                    printf("%d - %s\n", i, climaParaTexto((Clima)i));
                }

                escolha = lerInteiro("Escolha o clima: ");

                if (escolha < QUALQUER || escolha > POLAR) {

                    printf("Clima invalido!\n");
                    break;
                }

                filtrarMonstrosPorClima(bestiario->monstros, bestiario->quantidade, (Clima)escolha);

                break;

            case 0:

                printf("\nVoltando ao menu principal...\n");

                break;


            case -1:

                return;


            default:

                printf("\nOpcao invalida!\n");

                break;
        }

    } while (opcao != 0);
}

int main(void)
{
    Bestiario bestiario = {0};

    Monstro *encontrado;

    int opcao;
    int id;

    char nome[50];

    printf("========== INICIANDO BESTIARIO ==========\n");

    if (!carregarBestiario("bestiario.txt", bestiario.monstros, MAX_MONSTROS, &bestiario.quantidade)) {

        printf("Erro ao carregar o bestiario!\n");

        return 1;
    }

    printf("\nBestiario carregado com sucesso!\n");

    printf("Total de monstros: %d\n", bestiario.quantidade);

    do {

        mostrarMenu();

        opcao = lerInteiro("\nEscolha uma opcao: ");

        switch (opcao) {

            case 1:

                listarMonstros(bestiario.monstros, bestiario.quantidade);

                break;

            case 2:

                id = lerInteiro("\nDigite o ID do monstro: ");

                encontrado = buscarMonstroPorId(bestiario.monstros, bestiario.quantidade, id);

                if (encontrado == NULL) {

                    printf("\nMonstro nao encontrado!\n");

                } else {

                    printf("\n===== MONSTRO ENCONTRADO =====\n");

                    printf("ID: %d\n", encontrado->id);

                    printf("Nome: %s\n", encontrado->nome);

                    printf("Frequencia: %s\n", frequenciaParaTexto( encontrado->frequencia));
                }

                break;

            case 3:

                printf("\nDigite o nome do monstro: ");

                if (!lerNome(nome, sizeof(nome))) {

                    printf("Nao foi possivel realizar a busca!\n");

                    break;
                }

                encontrado = buscarMonstroPorNome(bestiario.monstros, bestiario.quantidade, nome);

                if (encontrado == NULL) {

                    printf("\nMonstro nao encontrado!\n");

                } else {

                    printf("\n===== MONSTRO ENCONTRADO =====\n");

                    printf("ID: %d\n", encontrado->id);

                    printf("Nome: %s\n", encontrado->nome);

                    printf("Frequencia: %s\n",frequenciaParaTexto( encontrado->frequencia));
                }

                break;

            case 4:

                id = lerInteiro("\nDigite o ID do monstro: ");

                encontrado = buscarMonstroPorId(bestiario.monstros, bestiario.quantidade, id);

                if (encontrado == NULL) {

                    printf("\nMonstro nao encontrado!\n");

                } else {

                    mostrarMonstro(encontrado);
                }

                break;

            case 5:

                menuFiltros(&bestiario);

                break;

            case 0:

                printf("\nEncerrando o bestiario...\n");

                break;


            case -1:

                printf("\nEntrada encerrada.\n");

                break;

            default:

                printf("\nOpcao invalida! Tente novamente.\n");

                break;
        }

    } while (opcao != 0 && opcao != -1);

    return 0;
}