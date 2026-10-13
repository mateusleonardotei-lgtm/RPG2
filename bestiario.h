#ifndef BESTIARIO_H
#define BESTIARIO_H

#include "monstro.h"

#define MAX_MONSTROS 50

typedef struct {
    Monstro monstros[MAX_MONSTROS];
    int quantidade;
} Bestiario;

int carregarBestiario(const char *nomeArquivo, Monstro monstros[], int capacidade, int *quantidade);

void listarMonstros(const Monstro monstros[], int quantidade);
void filtrarMonstrosPorFrequencia(const Monstro monstros[], int quantidade, Frequencia frequencia);
void filtrarMonstrosPorTerreno( const Monstro monstros[], int quantidade, Terreno terreno);
void filtrarMonstrosPorClima(const Monstro monstros[], int quantidade, Clima clima);

Monstro *buscarMonstroPorId( Monstro monstros[], int quantidade, int id);
Monstro *buscarMonstroPorNome(Monstro monstros[], int quantidade, const char *nome);

#endif