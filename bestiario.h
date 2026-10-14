#ifndef BESTIARIO_H
#define BESTIARIO_H

#include "monstro.h"

#define MAX_MONSTROS 100

typedef struct {
    Monstro monstros[MAX_MONSTROS];
    int quantidade;
} Bestiario;

int carregarBestiario(const char *nomeArquivo, Monstro monstros[], int capacidade, int *quantidade);
int insereMonstro(Bestiario *b, Monstro monstro);
int removeMonstro(Bestiario *b, int id);
int alteraMonstro(Bestiario *b, int id, Monstro novosDados);
int ativoNoPeriodo(const Monstro *m, Atividade periodo);
int salvaBestiario(const Bestiario *b, const char *nomeArquivo);
int viveNoTerreno(const Monstro *m, Terreno terreno);
int listaMonstrosPorTerreno(const Bestiario *b, Terreno terreno, Monstro *resultado[], int capacidadeResultado);
int carregaBestiario(Bestiario *b, const char *nomeArquivo);

void listarMonstros(const Monstro monstros[], int quantidade);
void filtrarMonstrosPorFrequencia(const Monstro monstros[], int quantidade, Frequencia frequencia);
void filtrarMonstrosPorTerreno( const Monstro monstros[], int quantidade, Terreno terreno);
void filtrarMonstrosPorClima(const Monstro monstros[], int quantidade, Clima clima);
void filtrarMonstrosPorAtividade(const Monstro monstros[], int quantidade, Atividade periodo);
void inicializaBestiario(Bestiario *b);
void listaMonstros(const Bestiario *b);
void exibeMonstro(const Monstro *m);

Monstro *buscarMonstroPorId(Monstro monstros[], int quantidade, int id);
Monstro *buscarMonstroPorNome(Monstro monstros[], int quantidade, const char *nome);
Monstro *buscaMonstroPorId(Bestiario *b, int id);
Monstro *buscaMonstroPorNome(Bestiario *b, const char *nome);

const Monstro *consultaMonstroPorId(const Bestiario *b, int id);

#endif