#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "bestiario.h"

int compararNomes(const char *nome1, const char *nome2)
{
    if (nome1 == NULL || nome2 == NULL) {
        return 0;
    }

    while (*nome1 != '\0' && *nome2 != '\0') {

        if (tolower((unsigned char)*nome1) != tolower((unsigned char)*nome2)) {

            return 0;
        }

        nome1++;
        nome2++;
    }

    return *nome1 == '\0' && *nome2 == '\0';
}

int carregarBestiario(const char *nomeArquivo, Monstro monstros[], int capacidade, int *quantidade)
{
    if (nomeArquivo == NULL || monstros == NULL || quantidade == NULL || capacidade <= 0) {

        return 0;
    }

    *quantidade = 0;

    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 0;
    }

    char linha[2048];
    char chave[100];
    char valor[2048];

    int dentroMonstro = 0;
    int contador = 0;

    Monstro monstroAtual = {0};

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        // Verifica se a linha cabe no buffer
        size_t tamanho = strlen(linha);

        if (tamanho > 0 &&
            linha[tamanho - 1] != '\n' && !feof(arquivo)) {

            int caractere;

            while ((caractere = fgetc(arquivo)) != '\n' && caractere != EOF) {
            }

            printf("Erro: linha muito longa!\n");

            fclose(arquivo);
            return 0;
        }

        removerQuebraLinha(linha);

        // Ignora linhas vazias e comentarios
        if (linha[0] == '\0' || linha[0] == '#') {
            continue;
        }

        // Inicio do registro
        if (strcmp(linha, "[MONSTRO]") == 0) {

            if (dentroMonstro) {
                printf("Erro: monstro anterior nao encerrado!\n");

                fclose(arquivo);
                return 0;
            }

            monstroAtual = (Monstro){0};

            dentroMonstro = 1;

            continue;
        }

        // Fim do registro
        if (strcmp(linha, "[/MONSTRO]") == 0) {

            if (!dentroMonstro) {
                printf("Erro: fechamento inesperado!\n");

                fclose(arquivo);
                return 0;
            }

            if (contador >= capacidade) {
                printf("Erro: capacidade do bestiario excedida!\n");

                fclose(arquivo);
                return 0;
            }

            // Verifica os campos obrigatorios basicos
            if (monstroAtual.id <= 0 || monstroAtual.nome[0] == '\0') {

                printf("Erro: monstro sem ID ou nome valido!\n");

                fclose(arquivo);
                return 0;
            }

            // Verifica IDs repetidos
            for (int i = 0; i < contador; i++) {

                if (monstros[i].id == monstroAtual.id) {

                    printf("Erro: ID %d duplicado!\n", monstroAtual.id);

                    fclose(arquivo);
                    return 0;
                }
            }

            // Armazena o monstro no vetor
            monstros[contador] = monstroAtual;

            contador++;

            dentroMonstro = 0;

            continue;
        }

        if (!dentroMonstro) {
            printf("Erro: atributo fora de um monstro!\n");

            fclose(arquivo);
            return 0;
        }

        // Separa chave e valor
        if (!separarChaveValor(linha, chave, sizeof(chave), valor, sizeof(valor))) {

            printf("Erro ao interpretar linha: %s\n", linha);

            fclose(arquivo);
            return 0;
        }

        // Processa o atributo
        if (!processarCampoMonstro(&monstroAtual, chave, valor)) {

            printf("Erro ao processar: %s = %s\n", chave, valor);

            fclose(arquivo);
            return 0;
        }
    }

    if (ferror(arquivo)) {
        printf("Erro durante a leitura!\n");

        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);

    if (dentroMonstro) {
        printf("Erro: ultimo monstro nao encerrado!\n");
        return 0;
    }

    *quantidade = contador;

    return 1;
}


void listarMonstros(const Monstro monstros[], int quantidade)
{
    if (monstros == NULL || quantidade <= 0) {
        printf("Nenhum monstro cadastrado!\n");
        return;
    }

    printf("\n========== LISTA DE MONSTROS ==========\n");

    for (int i = 0; i < quantidade; i++) {

        printf("\nID: %d\n", monstros[i].id);

        printf("Nome: %s\n", monstros[i].nome);

        printf("Frequencia: %s\n", frequenciaParaTexto(monstros[i].frequencia));

        printf("-----------------------------------\n");
    }

    printf("\nTotal de monstros: %d\n", quantidade);
}


Monstro *buscarMonstroPorId(Monstro monstros[], int quantidade, int id)
{
    if (monstros == NULL || quantidade <= 0 || id <= 0) {
        return NULL;
    }

    for (int i = 0; i < quantidade; i++) {

        if (monstros[i].id == id) {
            return &monstros[i];
        }
    }

    return NULL;
}


Monstro *buscarMonstroPorNome(Monstro monstros[], int quantidade, const char *nome)
{
    if (monstros == NULL || quantidade <= 0 || nome == NULL || nome[0] == '\0') {

        return NULL;
    }

    for (int i = 0; i < quantidade; i++) {

        if (compararNomes(monstros[i].nome, nome)) {
            return &monstros[i];
        }
    }

    return NULL;
}


void filtrarMonstrosPorFrequencia(const Monstro monstros[], int quantidade, Frequencia frequencia)
{
    if (monstros == NULL || quantidade <= 0) {
        printf("Nenhum monstro cadastrado!\n");
        return;
    }

    int encontrados = 0;

    printf("\n===== MONSTROS POR FREQUENCIA =====\n");
    printf("Frequencia: %s\n", frequenciaParaTexto(frequencia));

    for (int i = 0; i < quantidade; i++) {

        if (monstros[i].frequencia == frequencia) {

            printf("\nID: %d\n", monstros[i].id);
            printf("Nome: %s\n", monstros[i].nome);

            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("\nNenhum monstro encontrado!\n");
    }

    printf("\nTotal encontrado: %d\n", encontrados);
}


void filtrarMonstrosPorTerreno(const Monstro monstros[], int quantidade, Terreno terreno)
{
    if (monstros == NULL || quantidade <= 0) {
        printf("Nenhum monstro cadastrado!\n");
        return;
    }

    int encontrados = 0;

    printf("\n===== MONSTROS POR TERRENO =====\n");
    printf("Terreno: %s\n", terrenoParaTexto(terreno));

    for (int i = 0; i < quantidade; i++) {

        for (int j = 0; j < monstros[i].quantidadeTerrenos; j++) {

            if (monstros[i].terrenos[j] == terreno) {

                printf("\nID: %d\n", monstros[i].id);
                printf("Nome: %s\n", monstros[i].nome);

                encontrados++;

                break;
            }
        }
    }

    if (encontrados == 0) {
        printf("\nNenhum monstro encontrado!\n");
    }

    printf("\nTotal encontrado: %d\n", encontrados);
}


void filtrarMonstrosPorClima(const Monstro monstros[], int quantidade, Clima clima)
{
    if (monstros == NULL || quantidade <= 0) {
        printf("Nenhum monstro cadastrado!\n");
        return;
    }

    int encontrados = 0;

    printf("\n===== MONSTROS POR CLIMA =====\n");
    printf("Clima: %s\n", climaParaTexto(clima));

    for (int i = 0; i < quantidade; i++) {

        for (int j = 0; j < monstros[i].quantidadeClimas; j++) {

            if (monstros[i].climas[j] == clima) {

                printf("\nID: %d\n", monstros[i].id);
                printf("Nome: %s\n", monstros[i].nome);

                encontrados++;

                break;
            }
        }
    }

    if (encontrados == 0) {
        printf("\nNenhum monstro encontrado!\n");
    }

    printf("\nTotal encontrado: %d\n", encontrados);
}