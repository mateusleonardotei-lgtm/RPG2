#ifndef MONSTRO_H
#define MONSTRO_H

#include <stddef.h>

#define MAX_TERRENOS 8
#define MAX_CLIMAS 5
#define MAX_MOVIMENTOS 5
#define MAX_ATAQUES 10
#define MAX_HABILIDADES 10
#define MAX_TESOUROS 20
#define MAX_MOD_TERRENOS 8


typedef struct {
    int quantidade;
    int faces;
    int modificador;
} Dado;

typedef enum {
    MUITO_RARO,
    RARO,
    INCOMUM,
    COMUM,
    MUITO_COMUM
} Frequencia;

typedef enum {
    DIURNO,
    NOTURNO,
    QUALQUER_HORARIO
} Atividade;

typedef enum {
    FLORESTA,
    DESERTO,
    MONTANHA,
    PANTANO,
    PLANICIE,
    CAVERNA,
    RUINAS,
    URBANO
} Terreno;

typedef enum {
    QUALQUER,
    ARIDO,
    TROPICAL,
    TEMPERADO,
    FRIO,
    POLAR
} Clima;

typedef enum {
    SOLITARIO,
    TRIBO,
    COLONIA,
    CIRCULO,
    CLA,
    COMPANHIA,
    BANDO,
    MATILHA,
    NINHO,
    ENXAME,
    CASAL,
    GUARDA,
    HORDA,
    CORTE
} Organizacao;

typedef enum {
    CARNIVORO,
    HERBIVORO,
    ONIVORO,
    NAO_SE_ALIMENTA,
    PARASITA
} Dieta;

typedef enum {
    LEAL,
    NEUTRO_ORDEM,
    CAOTICO
} EixoOrdem;

typedef enum {
    BOM,
    NEUTRO_MORAL,
    MAU
} EixoMoral;

typedef struct {
    EixoOrdem ordem;
    EixoMoral moral;
} Tendencia;

typedef enum {
    PEQUENO,
    MEDIO,
    GRANDE,
    ENORME
} Tamanho;

typedef enum {
    INSTAVEL,
    MEDIO_MORAL,
    ESTAVEL,
    CAMPEAO,
    FANATICO
} ClassificacaoMoral;

typedef enum {
    TERRESTRE,
    NATACAO,
    VOO,
    ESCAVACAO
} TipoMovimento;

typedef struct {
    TipoMovimento tipo;
    int velocidade;
    char manobrabilidade;
} Movimento;

typedef struct {
    char nome[50];
    Dado dano;
} Ataque;

typedef struct {
    char nome[50];
    int valor;
    char descricao[200];
} HabilidadeEspecial;

typedef struct {
    int minimo;
    int maximo;
} IntervaloInteiro;

typedef struct {
    float minimo;
    float maximo;
} IntervaloReal;

typedef struct {
    char codigo;
    int quantidade;
} Tesouro;

typedef struct {
    Terreno terreno;
    int modificador;
} ModificadorTerreno;

typedef struct {

    int id;
    char nome[50];
    char descricao[500];

    int ca;
    Dado dv;
    int thac0;
    int resistenciaMagia;

    Frequencia frequencia;
    Dado numeroEncontrado;

    Clima climas[MAX_CLIMAS];
    int quantidadeClimas;

    Terreno terrenos[MAX_TERRENOS];
    int quantidadeTerrenos;

    Organizacao organizacao;
    Atividade atividade;
    Dieta dieta;

    int inteligencia;

    Tendencia tendencia;

    Tamanho tamanho;

    int moral;
    ClassificacaoMoral classificacaoMoral;

    Movimento movimentos[MAX_MOVIMENTOS];
    int quantidadeMovimentos;

    Ataque ataques[MAX_ATAQUES];
    int quantidadeAtaques;

    HabilidadeEspecial ataquesEspeciais[MAX_HABILIDADES];
    int quantidadeAtaquesEspeciais;

    HabilidadeEspecial defesasEspeciais[MAX_HABILIDADES];
    int quantidadeDefesasEspeciais;

    IntervaloInteiro covil;
    IntervaloReal altura;

    int xp;

    Tesouro tesouros[MAX_TESOUROS];
    int quantidadeTesouros;

    int poderBase;

    int modDia;
    int modNoite;

    ModificadorTerreno modificadoresTerreno[MAX_MOD_TERRENOS];
    int quantidadeModificadoresTerreno;

    int bonusPorAdicional;
    int limiteGrupoGrande;
    int bonusGrupoGrande;

    char descricaoCombate[500];
    char habitatSociedade[500];
    char descricaoEcologia[500];
} Monstro;

void dadoParaTexto(Dado dado, char *destino, int tamanho);
void mostrarMonstro(const Monstro *monstro);
void mostrarAtaque(const Ataque *ataque);
void mostrarHabilidadeEspecial(const HabilidadeEspecial *habilidade);
void mostrarTesouro(const Tesouro *tesouro);
void mostrarModificadorTerreno(const ModificadorTerreno *modificador);
void removerQuebraLinha(char *texto);

int converteTextoDado(const char *texto, Dado *dado);
int validaDado(Dado dado);
int converteTextoFrequencia(const char *texto, Frequencia *frequencia);
int converteTextoAtividade(const char *texto, Atividade *atividade);
int converteTextoTerreno(const char *texto, Terreno *terreno);
int converteListaTerrenos(const char *texto, Terreno terrenos[], int *quantidade);
int converteTextoClima(const char *texto, Clima *clima);
int converteListaClimas(const char *texto, Clima climas[], int *quantidade);
int converteTextoOrganizacao(const char *texto, Organizacao *organizacao);
int converteTextoDieta(const char *texto, Dieta *dieta);
int converteTextoOrdem(const char *texto, EixoOrdem *ordem);
int converteTextoMoral(const char *texto, EixoMoral *moral);
int converteTextoTamanho(const char *texto, Tamanho *tamanho);
int converteTextoClassificacaoMoral( const char *texto,ClassificacaoMoral *classificacao);
int converteTextoTipoMovimento(const char *texto, TipoMovimento *tipoMovimento);
int converteTextoMovimento(const char *texto, Movimento *movimento);
int converteListaMovimentos( const char *texto, Movimento movimentos[], int *quantidade);
int converteTextoAtaque(const char *texto, Ataque *ataque);
int converteListaAtaques(const char *texto, Ataque ataques[], int *quantidade);
int converteTextoHabilidadeEspecial(const char *texto, HabilidadeEspecial *habilidade);
int converteListaHabilidadesEspeciais(const char *texto, HabilidadeEspecial habilidades[], int *quantidade);
int converteTextoIntervaloInteiro(const char *texto, IntervaloInteiro *intervalo);
int converteTextoIntervaloReal(const char *texto, IntervaloReal *intervalo);
int converteTextoTesouro(const char *texto, Tesouro *tesouro);
int converteListaTesouros(const char *texto, Tesouro tesouros[], int *quantidade);
int converteTextoModificadorTerreno(const char *texto, ModificadorTerreno *modificador);
int converteListaModificadoresTerreno(const char *texto, ModificadorTerreno modificadores[], int *quantidade);
int copiarTextoSeguro(char *destino, size_t capacidade, const char *origem);
int separarChaveValor(const char *linha, char *chave, size_t capacidadeChave, char *valor, size_t capacidadeValor);
int testarLeituraBestiario(const char *nomeArquivo);
int processarCampoMonstro(Monstro *monstro, const char *chave, const char *valor);


const char *atividadeParaTexto(Atividade atividade);
const char *frequenciaParaTexto(Frequencia frequencia); 
const char *terrenoParaTexto(Terreno terreno);
const char *climaParaTexto(Clima clima);
const char *organizacaoParaTexto(Organizacao organizacao);
const char *dietaParaTexto(Dieta dieta);
const char *ordemParaTexto(EixoOrdem ordem);
const char *moralParaTexto(EixoMoral moral);
const char *tamanhoParaTexto(Tamanho tamanho);
const char *classificacaoMoralParaTexto( ClassificacaoMoral classificacao);
const char *tipoMovimentoParaTexto(TipoMovimento tipoMovimento);

#endif // MONSTRO_H