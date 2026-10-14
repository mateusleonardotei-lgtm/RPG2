#include <stdio.h>
#include "monstro.h"
#include <string.h>

//------------------------------------------------------------
// Função para validar um dado

int validaDado(Dado dado)
{
    if (dado.quantidade <= 0)
    {
        return 0;
    }

    if (dado.faces <= 0)
    {
        return 0;
    }

    return 1;
}

//------------------------------------------------------------
// Função para validar um monstro

int converteTextoDado(const char *texto, Dado *dado)
{
    int quantidade, faces, modificador = 0;
    char sinal;
    int caracteresLidos = 0;

    if (texto == NULL || dado == NULL)
    {
        return 0;
    }

    int resultado = sscanf(texto, "%dd%d%c%d%n", &quantidade, &faces, &sinal, &modificador, &caracteresLidos);

    if (resultado == 4)
    {
        if (sinal != '+' && sinal != '-')
        {
            return 0;
        }

        if (sinal == '-')
        {
            modificador = -modificador;
        }
    }
    else
    {
        modificador = 0;

        resultado = sscanf(texto, "%dd%d%n", &quantidade, &faces, &caracteresLidos);

        if (resultado != 2)
        {
            return 0;
        }
    }

    if (texto[caracteresLidos] != '\0')
    {
        return 0;
    }

    Dado temporario;

    temporario.quantidade = quantidade;
    temporario.faces = faces;
    temporario.modificador = modificador;

    if (!validaDado(temporario))
    {
        return 0;
    }

    *dado = temporario;

    return 1;
}

//------------------------------------------------------------
// dadoParaTexto: Converte um dado para uma representação em texto

void dadoParaTexto(Dado dado, char *destino, int tamanho)
{
    if (destino == NULL || tamanho <= 0)
    {
        return;
    }

    if (!validaDado(dado))
    {
        snprintf(destino, tamanho, "INVALIDO");
        return;
    }

    if (dado.modificador > 0)
    {
        snprintf(destino, tamanho, "%dd%d+%d", dado.quantidade, dado.faces, dado.modificador);
    }
    else if (dado.modificador < 0)
    {
        snprintf(destino, tamanho, "%dd%d%d", dado.quantidade, dado.faces, dado.modificador);
    }
    else
    {
        snprintf(destino, tamanho, "%dd%d", dado.quantidade, dado.faces);
    }
}

//------------------------------------------------------------
// textofrequencia: Converte uma frequência para uma representação em texto

int converteTextoFrequencia(const char *texto, Frequencia *frequencia)
{
    if (texto == NULL || frequencia == NULL)
    {
        return 0;
    }

    if (strcmp(texto, "MUITO_RARO") == 0)
    {
        *frequencia = MUITO_RARO;
    }
    else if (strcmp(texto, "RARO") == 0)
    {
        *frequencia = RARO;
    }
    else if (strcmp(texto, "INCOMUM") == 0)
    {
        *frequencia = INCOMUM;
    }
    else if (strcmp(texto, "COMUM") == 0)
    {
        *frequencia = COMUM;
    }
    else if (strcmp(texto, "MUITO_COMUM") == 0)
    {
        *frequencia = MUITO_COMUM;
    }
    else
    {
        return 0;
    }

    return 1;
}

//------------------------------------------------------------
// frequenciaParaTexto: Converte uma frequência para uma representação em texto

const char *frequenciaParaTexto(Frequencia frequencia)
{
    switch (frequencia)
    {
    case MUITO_RARO:
        return "MUITO_RARO";

    case RARO:
        return "RARO";

    case INCOMUM:
        return "INCOMUM";

    case COMUM:
        return "COMUM";

    case MUITO_COMUM:
        return "MUITO_COMUM";

    default:
        return "INVALIDO";
    }
}

//

int converteTextoAtividade(const char *texto, Atividade *atividade)
{
    if (texto == NULL || atividade == NULL)
    {
        return 0;
    }

    if (strcmp(texto, "DIURNO") == 0)
    {
        *atividade = DIURNO;
    }
    else if (strcmp(texto, "NOTURNO") == 0)
    {
        *atividade = NOTURNO;
    }
    else if (strcmp(texto, "QUALQUER_HORARIO") == 0)
    {
        *atividade = QUALQUER_HORARIO;
    }
    else
    {
        return 0;
    }

    return 1;
}

const char *atividadeParaTexto(Atividade atividade)
{
    switch (atividade)
    {
    case DIURNO:
        return "DIURNO";

    case NOTURNO:
        return "NOTURNO";

    case QUALQUER_HORARIO:
        return "QUALQUER_HORARIO";

    default:
        return "INVALIDO";
    }
}

int converteTextoTerreno(const char *texto, Terreno *terreno)
{
    if (texto == NULL || terreno == NULL)
    {
        return 0;
    }

    if (strcmp(texto, "FLORESTA") == 0)
    {
        *terreno = FLORESTA;
    }
    else if (strcmp(texto, "DESERTO") == 0)
    {
        *terreno = DESERTO;
    }
    else if (strcmp(texto, "MONTANHA") == 0)
    {
        *terreno = MONTANHA;
    }
    else if (strcmp(texto, "PANTANO") == 0)
    {
        *terreno = PANTANO;
    }
    else if (strcmp(texto, "PLANICIE") == 0)
    {
        *terreno = PLANICIE;
    }
    else if (strcmp(texto, "CAVERNA") == 0)
    {
        *terreno = CAVERNA;
    }
    else if (strcmp(texto, "RUINAS") == 0)
    {
        *terreno = RUINAS;
    }
    else if (strcmp(texto, "URBANO") == 0)
    {
        *terreno = URBANO;
    }
    else
    {
        return 0;
    }

    return 1;
}

const char *terrenoParaTexto(Terreno terreno)
{
    switch (terreno)
    {
    case FLORESTA:
        return "FLORESTA";

    case DESERTO:
        return "DESERTO";

    case MONTANHA:
        return "MONTANHA";

    case PANTANO:
        return "PANTANO";

    case PLANICIE:
        return "PLANICIE";

    case CAVERNA:
        return "CAVERNA";

    case RUINAS:
        return "RUINAS";

    case URBANO:
        return "URBANO";

    default:
        return "INVALIDO";
    }
}

int converteListaTerrenos(const char *texto, Terreno terrenos[], int *quantidade)
{
    if (texto == NULL || terrenos == NULL || quantidade == NULL)
    {
        return 0;
    }

    char copia[200];

    if (strlen(texto) >= sizeof(copia))
    {
        return 0;
    }

    strcpy(copia, texto);

    int total = 0;

    char *parte = strtok(copia, ",");

    while (parte != NULL)
    {

        if (total >= MAX_TERRENOS)
        {
            return 0;
        }

        Terreno terreno;

        if (!converteTextoTerreno(parte, &terreno))
        {
            return 0;
        }

        terrenos[total] = terreno;
        total++;

        parte = strtok(NULL, ",");
    }

    if (total == 0)
    {
        return 0;
    }

    *quantidade = total;

    return 1;
}

int converteTextoClima(const char *texto, Clima *clima)
{
    if (texto == NULL || clima == NULL)
    {
        return 0;
    }

    if (strcmp(texto, "QUALQUER") == 0)
    {
        *clima = QUALQUER;
    }
    else if (strcmp(texto, "ARIDO") == 0)
    {
        *clima = ARIDO;
    }
    else if (strcmp(texto, "TROPICAL") == 0)
    {
        *clima = TROPICAL;
    }
    else if (strcmp(texto, "TEMPERADO") == 0)
    {
        *clima = TEMPERADO;
    }
    else if (strcmp(texto, "FRIO") == 0)
    {
        *clima = FRIO;
    }
    else if (strcmp(texto, "POLAR") == 0)
    {
        *clima = POLAR;
    }
    else
    {
        return 0;
    }

    return 1;
}

const char *climaParaTexto(Clima clima)
{
    switch (clima)
    {
    case QUALQUER:
        return "QUALQUER";

    case ARIDO:
        return "ARIDO";

    case TROPICAL:
        return "TROPICAL";

    case TEMPERADO:
        return "TEMPERADO";

    case FRIO:
        return "FRIO";

    case POLAR:
        return "POLAR";

    default:
        return "INVALIDO";
    }
}

int converteListaClimas(const char *texto, Clima climas[], int *quantidade)
{
    if (texto == NULL || climas == NULL || quantidade == NULL)
    {
        return 0;
    }

    char copia[200];

    if (strlen(texto) >= sizeof(copia))
    {
        return 0;
    }

    strcpy(copia, texto);

    int total = 0;

    char *parte = strtok(copia, ",");

    while (parte != NULL)
    {

        if (total >= MAX_CLIMAS)
        {
            return 0;
        }

        Clima clima;

        if (!converteTextoClima(parte, &clima))
        {
            return 0;
        }

        // QUALQUER deve aparecer sozinho
        if (clima == QUALQUER && (total > 0 || strchr(texto, ',') != NULL))
        {
            return 0;
        }

        // Nao permite combinar outro clima com QUALQUER
        if (total > 0 && climas[0] == QUALQUER)
        {
            return 0;
        }

        // Verifica se o clima ja foi adicionado
        for (int i = 0; i < total; i++)
        {
            if (climas[i] == clima)
            {
                return 0;
            }
        }

        climas[total] = clima;
        total++;

        parte = strtok(NULL, ",");
    }

    if (total == 0)
    {
        return 0;
    }

    *quantidade = total;

    return 1;
}

int converteTextoOrganizacao(const char *texto, Organizacao *organizacao)
{
    if (texto == NULL || organizacao == NULL)
    {
        return 0;
    }

    if (strcmp(texto, "SOLITARIO") == 0)
    {
        *organizacao = SOLITARIO;
    }
    else if (strcmp(texto, "TRIBO") == 0)
    {
        *organizacao = TRIBO;
    }
    else if (strcmp(texto, "COLONIA") == 0)
    {
        *organizacao = COLONIA;
    }
    else if (strcmp(texto, "CIRCULO") == 0)
    {
        *organizacao = CIRCULO;
    }
    else if (strcmp(texto, "CLA") == 0)
    {
        *organizacao = CLA;
    }
    else if (strcmp(texto, "COMPANHIA") == 0)
    {
        *organizacao = COMPANHIA;
    }
    else if (strcmp(texto, "BANDO") == 0)
    {
        *organizacao = BANDO;
    }
    else if (strcmp(texto, "MATILHA") == 0)
    {
        *organizacao = MATILHA;
    }
    else if (strcmp(texto, "NINHO") == 0)
    {
        *organizacao = NINHO;
    }
    else if (strcmp(texto, "ENXAME") == 0)
    {
        *organizacao = ENXAME;
    }
    else if (strcmp(texto, "CASAL") == 0)
    {
        *organizacao = CASAL;
    }
    else if (strcmp(texto, "GUARDA") == 0)
    {
        *organizacao = GUARDA;
    }
    else if (strcmp(texto, "HORDA") == 0)
    {
        *organizacao = HORDA;
    }
    else if (strcmp(texto, "CORTE") == 0)
    {
        *organizacao = CORTE;
    }
    else
    {
        return 0;
    }

    return 1;
}

const char *organizacaoParaTexto(Organizacao organizacao)
{
    switch (organizacao)
    {

    case SOLITARIO:
        return "SOLITARIO";

    case TRIBO:
        return "TRIBO";

    case COLONIA:
        return "COLONIA";

    case CIRCULO:
        return "CIRCULO";

    case CLA:
        return "CLA";

    case COMPANHIA:
        return "COMPANHIA";

    case BANDO:
        return "BANDO";

    case MATILHA:
        return "MATILHA";

    case NINHO:
        return "NINHO";

    case ENXAME:
        return "ENXAME";

    case CASAL:
        return "CASAL";

    case GUARDA:
        return "GUARDA";

    case HORDA:
        return "HORDA";

    case CORTE:
        return "CORTE";

    default:
        return "INVALIDO";
    }
}

int converteTextoDieta(const char *texto, Dieta *dieta)
{
    if (texto == NULL || dieta == NULL)
    {
        return 0;
    }

    if (strcmp(texto, "CARNIVORO") == 0)
    {
        *dieta = CARNIVORO;
    }
    else if (strcmp(texto, "HERBIVORO") == 0)
    {
        *dieta = HERBIVORO;
    }
    else if (strcmp(texto, "ONIVORO") == 0)
    {
        *dieta = ONIVORO;
    }
    else if (strcmp(texto, "NAO_SE_ALIMENTA") == 0)
    {
        *dieta = NAO_SE_ALIMENTA;
    }
    else if (strcmp(texto, "PARASITA") == 0)
    {
        *dieta = PARASITA;
    }
    else
    {
        return 0;
    }

    return 1;
}

const char *dietaParaTexto(Dieta dieta)
{
    switch (dieta)
    {

    case CARNIVORO:
        return "CARNIVORO";

    case HERBIVORO:
        return "HERBIVORO";

    case ONIVORO:
        return "ONIVORO";

    case NAO_SE_ALIMENTA:
        return "NAO_SE_ALIMENTA";

    case PARASITA:
        return "PARASITA";

    default:
        return "INVALIDO";
    }
}

int converteTextoOrdem(const char *texto, EixoOrdem *ordem)
{
    if (texto == NULL || ordem == NULL)
    {
        return 0;
    }

    if (strcmp(texto, "LEAL") == 0)
    {
        *ordem = LEAL;
    }
    else if (strcmp(texto, "NEUTRO_ORDEM") == 0)
    {
        *ordem = NEUTRO_ORDEM;
    }
    else if (strcmp(texto, "CAOTICO") == 0)
    {
        *ordem = CAOTICO;
    }
    else
    {
        return 0;
    }

    return 1;
}

const char *ordemParaTexto(EixoOrdem ordem)
{
    switch (ordem)
    {

    case LEAL:
        return "LEAL";

    case NEUTRO_ORDEM:
        return "NEUTRO_ORDEM";

    case CAOTICO:
        return "CAOTICO";

    default:
        return "INVALIDO";
    }
}

int converteTextoMoral(const char *texto, EixoMoral *moral)
{
    if (texto == NULL || moral == NULL)
    {
        return 0;
    }

    if (strcmp(texto, "BOM") == 0)
    {
        *moral = BOM;
    }
    else if (strcmp(texto, "NEUTRO_MORAL") == 0)
    {
        *moral = NEUTRO_MORAL;
    }
    else if (strcmp(texto, "MAU") == 0)
    {
        *moral = MAU;
    }
    else
    {
        return 0;
    }

    return 1;
}

const char *moralParaTexto(EixoMoral moral)
{
    switch (moral)
    {

    case BOM:
        return "BOM";

    case NEUTRO_MORAL:
        return "NEUTRO_MORAL";

    case MAU:
        return "MAU";

    default:
        return "INVALIDO";
    }
}

int converteTextoTamanho(const char *texto, Tamanho *tamanho)
{
    if (texto == NULL || tamanho == NULL)
    {
        return 0;
    }

    if (strcmp(texto, "PEQUENO") == 0)
    {
        *tamanho = PEQUENO;
    }
    else if (strcmp(texto, "MEDIO") == 0)
    {
        *tamanho = MEDIO;
    }
    else if (strcmp(texto, "GRANDE") == 0)
    {
        *tamanho = GRANDE;
    }
    else if (strcmp(texto, "ENORME") == 0)
    {
        *tamanho = ENORME;
    }
    else
    {
        return 0;
    }

    return 1;
}

const char *tamanhoParaTexto(Tamanho tamanho)
{
    switch (tamanho)
    {

    case PEQUENO:
        return "PEQUENO";

    case MEDIO:
        return "MEDIO";

    case GRANDE:
        return "GRANDE";

    case ENORME:
        return "ENORME";

    default:
        return "INVALIDO";
    }
}

int converteTextoClassificacaoMoral(
    const char *texto,
    ClassificacaoMoral *classificacao)
{
    if (texto == NULL || classificacao == NULL)
    {
        return 0;
    }

    if (strcmp(texto, "INSTAVEL") == 0)
    {
        *classificacao = INSTAVEL;
    }
    else if (strcmp(texto, "MEDIO") == 0)
    {
        *classificacao = MEDIO_MORAL;
    }
    else if (strcmp(texto, "ESTAVEL") == 0)
    {
        *classificacao = ESTAVEL;
    }
    else if (strcmp(texto, "CAMPEAO") == 0)
    {
        *classificacao = CAMPEAO;
    }
    else if (strcmp(texto, "FANATICO") == 0)
    {
        *classificacao = FANATICO;
    }
    else
    {
        return 0;
    }

    return 1;
}

const char *classificacaoMoralParaTexto(
    ClassificacaoMoral classificacao)
{
    switch (classificacao)
    {

    case INSTAVEL:
        return "INSTAVEL";

    case MEDIO_MORAL:
        return "MEDIO";

    case ESTAVEL:
        return "ESTAVEL";

    case CAMPEAO:
        return "CAMPEAO";

    case FANATICO:
        return "FANATICO";

    default:
        return "INVALIDO";
    }
}

void mostrarMonstro(const Monstro *monstro)
{
    if (monstro == NULL)
    {
        printf("Monstro invalido!\n");
        return;
    }

    char dadoTexto[50];

    printf("\n---- MONSTRO ----\n");

    printf("ID: %d\n", monstro->id);
    printf("Nome: %s\n", monstro->nome);
    printf("Descricao: %s\n", monstro->descricao);

    printf("Frequencia: %s\n", frequenciaParaTexto(monstro->frequencia));

    dadoParaTexto(monstro->numeroEncontrado, dadoTexto, sizeof(dadoTexto));

    printf("Numero encontrado: %s\n", dadoTexto);

    printf("Climas: ");

    for (int i = 0; i < monstro->quantidadeClimas; i++)
    {
        if (i > 0)
        {
            printf(", ");
        }

        printf("%s", climaParaTexto(monstro->climas[i]));
    }

    printf("\nTerrenos: ");

    for (int i = 0; i < monstro->quantidadeTerrenos; i++)
    {
        if (i > 0)
        {
            printf(", ");
        }

        printf("%s", terrenoParaTexto(monstro->terrenos[i]));
    }

    printf("\nOrganizacao: %s\n", organizacaoParaTexto(monstro->organizacao));

    printf("Atividade: %s\n", atividadeParaTexto(monstro->atividade));

    printf("Dieta: %s\n", dietaParaTexto(monstro->dieta));

    printf("Inteligencia: %d\n", monstro->inteligencia);

    printf("Ordem: %s\n", ordemParaTexto(monstro->tendencia.ordem));

    printf("Moralidade: %s\n", moralParaTexto(monstro->tendencia.moral));

    printf("Tamanho: %s\n", tamanhoParaTexto(monstro->tamanho));

    printf("Moral: %d\n", monstro->moral);

    printf("Classificacao moral: %s\n", classificacaoMoralParaTexto(monstro->classificacaoMoral));

    printf("\n===== ATRIBUTOS DE COMBATE =====\n");

    printf("Classe de armadura: %d\n", monstro->ca);

    dadoParaTexto(monstro->dv, dadoTexto, sizeof(dadoTexto));

    printf("Dados de vida: %s\n", dadoTexto);

    printf("THAC0: %d\n", monstro->thac0);

    printf("Resistencia a magia: %d%%\n", monstro->resistenciaMagia);

    printf("\n===== MOVIMENTOS =====\n");

    for (int i = 0; i < monstro->quantidadeMovimentos; i++)
    {

        Movimento movimento = monstro->movimentos[i];

        printf("%s: %d", tipoMovimentoParaTexto(movimento.tipo), movimento.velocidade);

        if (movimento.tipo == VOO)
        {
            printf(" (Manobrabilidade: %c)", movimento.manobrabilidade);
        }

        printf("\n");
    }

    printf("\n===== ATAQUES =====\n");

    for (int i = 0; i < monstro->quantidadeAtaques; i++)
    {

        printf("\nAtaque %d:\n", i + 1);

        mostrarAtaque(&monstro->ataques[i]);
    }

    printf("\n===== ATAQUES ESPECIAIS =====\n");

if (monstro->quantidadeAtaquesEspeciais == 0) {
    printf("Nenhum ataque especial.\n");
}

for (int i = 0; i < monstro->quantidadeAtaquesEspeciais; i++)
{

    printf("\nAtaque especial %d:\n", i + 1);

    mostrarHabilidadeEspecial(&monstro->ataquesEspeciais[i]);
}

printf("\n===== DEFESAS ESPECIAIS =====\n");

if (monstro->quantidadeDefesasEspeciais == 0) {
    printf("Nenhuma defesa especial.\n");
}

for (int i = 0; i < monstro->quantidadeDefesasEspeciais; i++) {

    printf("\nDefesa especial %d:\n", i + 1);

    mostrarHabilidadeEspecial(&monstro->defesasEspeciais[i]);
}

printf("\n===== HABITAT E RECOMPENSAS =====\n");

printf("Covil: %d a %d\n",monstro->covil.minimo, monstro->covil.maximo);

printf("Altura: %.2f a %.2f\n",monstro->altura.minimo, monstro->altura.maximo);

printf("Experiencia (XP): %d\n", monstro->xp);

printf("\n===== TESOUROS =====\n");

if (monstro->quantidadeTesouros == 0) {
    printf("Nenhum tesouro.\n");
}

for (int i = 0; i < monstro->quantidadeTesouros; i++) {

    printf("\nTesouro %d:\n", i + 1);

    mostrarTesouro(&monstro->tesouros[i]);
}

printf("\n===== MODIFICADORES DE PODER =====\n");

printf("Poder base: %d\n", monstro->poderBase);

printf("Modificador dia: %+d\n", monstro->modDia);

printf("Modificador noite: %+d\n", monstro->modNoite);

printf("Bonus por adicional: %d\n", monstro->bonusPorAdicional);

printf("Limite grupo grande: %d\n", monstro->limiteGrupoGrande);

printf("Bonus grupo grande: %d\n", monstro->bonusGrupoGrande);

printf("\n===== MODIFICADORES POR TERRENO =====\n");

for (int i = 0; i < monstro->quantidadeModificadoresTerreno; i++)
{

    mostrarModificadorTerreno(&monstro->modificadoresTerreno[i]);
}

printf("\n===== INFORMACOES DESCRITIVAS =====\n");

printf("\nDescricao do combate:\n%s\n",monstro->descricaoCombate);

printf("\nHabitat e sociedade:\n%s\n",monstro->habitatSociedade);

printf("\nDescricao da ecologia:\n%s\n",monstro->descricaoEcologia);

printf("--------------------------------\n");
}

int converteTextoTipoMovimento(const char *texto, TipoMovimento *tipo)
{
    if (texto == NULL || tipo == NULL)
    {
        return 0;
    }

    if (strcmp(texto, "TERRESTRE") == 0)
    {
        *tipo = TERRESTRE;
    }
    else if (strcmp(texto, "NATACAO") == 0)
    {
        *tipo = NATACAO;
    }
    else if (strcmp(texto, "VOO") == 0)
    {
        *tipo = VOO;
    }
    else if(strcmp(texto, "ESCAVACAO") == 0)
    {
        *tipo = ESCAVACAO;
    }
    else
    {
        return 0;
    }

    return 1;
}

const char *tipoMovimentoParaTexto(TipoMovimento tipo)
{
    switch (tipo)
    {

    case TERRESTRE:
        return "TERRESTRE";

    case NATACAO:
        return "NATACAO";

    case VOO:
        return "VOO";
    
    case ESCAVACAO:
        return "ESCAVACAO";
    default:
        return "INVALIDO";
    }
}

int converteTextoMovimento(const char *texto, Movimento *movimento)
{
    if (texto == NULL || movimento == NULL)
    {
        return 0;
    }

    char tipoTexto[50];
    int velocidade;
    char manobrabilidade = '\0';
    int caracteresLidos = 0;

    int quantidade = sscanf(texto, "%49[^:]:%d:%c%n", tipoTexto, &velocidade, &manobrabilidade, &caracteresLidos);

    if (quantidade != 3)
    {

        caracteresLidos = 0;

        // Tenta ler sem manobrabilidade,
        quantidade = sscanf(texto, "%49[^:]:%d%n", tipoTexto, &velocidade, &caracteresLidos);

        if (quantidade != 2)
        {
            return 0;
        }

        manobrabilidade = '\0';
    }

    if (texto[caracteresLidos] != '\0')
    {
        return 0;
    }

    TipoMovimento tipo;

    if (!converteTextoTipoMovimento(tipoTexto, &tipo))
    {
        return 0;
    }

    if (velocidade <= 0)
    {
        return 0;
    }

    if (tipo == VOO)
    {

        if (manobrabilidade < 'A' || manobrabilidade > 'E')
        {
            return 0;
        }
    }
    else
    {

        if (manobrabilidade != '\0')
        {
            return 0;
        }
    }

    movimento->tipo = tipo;
    movimento->velocidade = velocidade;
    movimento->manobrabilidade = manobrabilidade;

    return 1;
}

int converteListaMovimentos(const char *texto, Movimento movimentos[], int *quantidade)
{
    if (texto == NULL || movimentos == NULL || quantidade == NULL)
    {
        return 0;
    }

    char copia[300];
    Movimento temporarios[MAX_MOVIMENTOS];
    int contador = 0;

    if (strlen(texto) >= sizeof(copia))
    {
        return 0;
    }

    if (texto[0] == '\0')
    {
        return 0;
    }

    strcpy(copia, texto);

    char *parte = strtok(copia, "|");

    while (parte != NULL)
    {

        if (contador >= MAX_MOVIMENTOS)
        {
            return 0;
        }

        if (!converteTextoMovimento(parte, &temporarios[contador]))
        {
            return 0;
        }

        // Verifica se o tipo de movimento já foi cadastrado
        for (int i = 0; i < contador; i++)
        {
            if (temporarios[i].tipo == temporarios[contador].tipo)
            {
                return 0;
            }
        }

        contador++;

        parte = strtok(NULL, "|");
    }

    if (contador == 0)
    {
        return 0;
    }

    // Verifica separadores vazios, como ||, | no início ou no final
    int tamanho = strlen(texto);

    if (texto[0] == '|' || texto[tamanho - 1] == '|')
    {
        return 0;
    }

    for (int i = 1; i < tamanho; i++)
    {
        if (texto[i] == '|' && texto[i - 1] == '|')
        {
            return 0;
        }
    }

    // Copia os movimentos somente depois de validar todos
    for (int i = 0; i < contador; i++)
    {
        movimentos[i] = temporarios[i];
    }

    *quantidade = contador;

    return 1;
}

void mostrarAtaque(const Ataque *ataque)
{
    if (ataque == NULL) {
        printf("Ataque invalido!\n");
        return;
    }

    char danoTexto[50];

    dadoParaTexto( ataque->dano, danoTexto, sizeof(danoTexto));

    printf("Nome: %s\n", ataque->nome);
    printf("Dano: %s\n", danoTexto);
}


int converteTextoAtaque(const char *texto, Ataque *ataque)
{
    if (texto == NULL || ataque == NULL) {
        return 0;
    }

    char nome[50];
    char danoTexto[50];

    Dado dano;

    int caracteresLidos = 0;

    int resultado = sscanf(
        texto,
        "%49[^:]:%49s%n",
        nome,
        danoTexto,
        &caracteresLidos
    );

    if (resultado != 2) {
        return 0;
    }

    if (texto[caracteresLidos] != '\0') {
        return 0;
    }

    if (!converteTextoDado(danoTexto, &dano)) {
        return 0;
    }

    strcpy(ataque->nome, nome);
    ataque->dano = dano;

    return 1;
}


int converteListaAtaques(const char *texto, Ataque ataques[], int *quantidade)
{
    if (texto == NULL || ataques == NULL || quantidade == NULL) {
        return 0;
    }

    if (texto[0] == '\0') {
        return 0;
    }

    char copia[500];
    Ataque temporarios[MAX_ATAQUES];

    int contador = 0;

    if (strlen(texto) >= sizeof(copia)) {
        return 0;
    }

    int tamanho = strlen(texto);

    // Verifica separadores invalidos
    if (texto[0] == '|' || texto[tamanho - 1] == '|') {
        return 0;
    }

    for (int i = 1; i < tamanho; i++) {
        if (texto[i] == '|' && texto[i - 1] == '|') {
            return 0;
        }
    }

    strcpy(copia, texto);

    char *parte = strtok(copia, "|");

    while (parte != NULL) {

        if (contador >= MAX_ATAQUES) {
            return 0;
        }

        if (!converteTextoAtaque(parte, &temporarios[contador])) {
            return 0;
        }

        contador++;

        parte = strtok(NULL, "|");
    }

    if (contador == 0) {
        return 0;
    }

    // Copia somente depois de validar todos
    for (int i = 0; i < contador; i++) {
        ataques[i] = temporarios[i];
    }

    *quantidade = contador;

    return 1;
}


int converteTextoHabilidadeEspecial(const char *texto, HabilidadeEspecial *habilidade)
{
    if (texto == NULL || habilidade == NULL) {
        return 0;
    }

    char nome[50];
    char descricao[200];
    int valor;

    int caracteresLidos = 0;

    //%49[^:] Lê o nome até encontrar ":"
    //%d Lê o valor inteiro
    //%199[^\n]	Lê a descrição até o fim da linha
    //%n Registra quantos caracteres foram lidos
    int resultado = sscanf(texto, "%49[^:]:%d:%199[^\n]%n", nome, &valor, descricao,&caracteresLidos);

    if (resultado != 3) {
        return 0;
    }

    if (texto[caracteresLidos] != '\0') {
        return 0;
    }

    if (valor < 0) {
        return 0;
    }

    strcpy(habilidade->nome, nome);
    habilidade->valor = valor;
    strcpy(habilidade->descricao, descricao);

    return 1;
}


void mostrarHabilidadeEspecial(const HabilidadeEspecial *habilidade)
{
    if (habilidade == NULL) {
        printf("Habilidade invalida!\n");
        return;
    }

    printf("Nome: %s\n", habilidade->nome);

    printf("Valor: %d\n", habilidade->valor);

    printf("Descricao: %s\n", habilidade->descricao);
}


int converteListaHabilidadesEspeciais(const char *texto, HabilidadeEspecial habilidades[], int *quantidade)
{
    if (texto == NULL || habilidades == NULL || quantidade == NULL) {
        return 0;
    }

    // Monstro sem habilidades especiais
    if (strcmp(texto, "NENHUM") == 0 || strcmp(texto, "NENHUMA") == 0) {

        *quantidade = 0;
        return 1;
    }

    if (texto[0] == '\0') {
        return 0;
    }

    char copia[2500];
    HabilidadeEspecial temporarias[MAX_HABILIDADES];

    int contador = 0;
    size_t tamanho = strlen(texto);

    if (tamanho >= sizeof(copia)) {
        return 0;
    }

    // Verifica separadores invalidos
    if (texto[0] == '|' || texto[tamanho - 1] == '|') {
        return 0;
    }

    for (size_t i = 1; i < tamanho; i++) {
        if (texto[i] == '|' && texto[i - 1] == '|') {
            return 0;
        }
    }

    strcpy(copia, texto);

    char *parte = strtok(copia, "|");

    while (parte != NULL) {

        if (contador >= MAX_HABILIDADES) {
            return 0;
        }

        if (!converteTextoHabilidadeEspecial(parte, &temporarias[contador])) {
            return 0;
        }

        contador++;

        parte = strtok(NULL, "|");
    }

    if (contador == 0) {
        return 0;
    }

    // Copia somente depois de validar todas as habilidades
    for (int i = 0; i < contador; i++) {
        habilidades[i] = temporarias[i];
    }

    *quantidade = contador;

    return 1;
}


int converteTextoIntervaloInteiro(const char *texto, IntervaloInteiro *intervalo)
{
    if (texto == NULL || intervalo == NULL) {
        return 0;
    }

    int minimo, maximo;
    int caracteresLidos = 0;

    int resultado = sscanf(texto, "%d,%d%n", &minimo, &maximo, &caracteresLidos
    );

    if (resultado != 2) {
        return 0;
    }

    if (texto[caracteresLidos] != '\0') {
        return 0;
    }

    if (minimo < 0 || maximo < minimo) {
        return 0;
    }

    intervalo->minimo = minimo;
    intervalo->maximo = maximo;

    return 1;
}


int converteTextoIntervaloReal(const char *texto, IntervaloReal *intervalo)
{
    if (texto == NULL || intervalo == NULL) {
        return 0;
    }

    float minimo, maximo;
    int caracteresLidos = 0;

    int resultado = sscanf( texto, "%f,%f%n", &minimo, &maximo, &caracteresLidos);

    if (resultado != 2) {
        return 0;
    }

    if (texto[caracteresLidos] != '\0') {
        return 0;
    }

    if (minimo < 0 || maximo < minimo) {
        return 0;
    }

    intervalo->minimo = minimo;
    intervalo->maximo = maximo;

    return 1;
}


int converteTextoTesouro(const char *texto, Tesouro *tesouro)
{
    if (texto == NULL || tesouro == NULL) {
        return 0;
    }

    if (texto[0] == '\0') {
        return 0;
    }

    char codigo = texto[0];
    int quantidade = 1;

    // Verifica se o codigo e uma letra maiuscula
    if (codigo < 'A' || codigo > 'Z') {
        return 0;
    }

    // Tesouro sem quantidade explicita
    if (texto[1] == '\0') {

        tesouro->codigo = codigo;
        tesouro->quantidade = 1;

        return 1;
    }

    // Tesouro com quantidade
    if (texto[1] != 'x') {
        return 0;
    }

    int caracteresLidos = 0;

    if (sscanf(texto + 2, "%d%n", &quantidade, &caracteresLidos) != 1) {

        return 0;
    }

    if (texto[2 + caracteresLidos] != '\0') {
        return 0;
    }

    if (quantidade <= 0) {
        return 0;
    }

    tesouro->codigo = codigo;
    tesouro->quantidade = quantidade;

    return 1;
}


int converteListaTesouros(const char *texto, Tesouro tesouros[], int *quantidade)
{
    if (texto == NULL || tesouros == NULL || quantidade == NULL) {
        return 0;
    }

    // Monstro sem tesouro
    if (strcmp(texto, "NENHUM") == 0) {
        *quantidade = 0;
        return 1;
    }

    if (texto[0] == '\0') {
        return 0;
    }

    char copia[500];
    Tesouro temporarios[MAX_TESOUROS];

    int contador = 0;

    size_t tamanho = strlen(texto);

    if (tamanho >= sizeof(copia)) {
        return 0;
    }

    // Verifica separadores invalidos
    if (texto[0] == ';' || texto[tamanho - 1] == ';') {
        return 0;
    }

    for (size_t i = 1; i < tamanho; i++) {
        if (texto[i] == ';' && texto[i - 1] == ';') {
            return 0;
        }
    }

    strcpy(copia, texto);

    char *parte = strtok(copia, ";");

    while (parte != NULL) {

        if (contador >= MAX_TESOUROS) {
            return 0;
        }

        if (!converteTextoTesouro(parte, &temporarios[contador])) {

            return 0;
        }

        contador++;

        parte = strtok(NULL, ";");
    }

    if (contador == 0) {
        return 0;
    }

    // Copia os tesouros validados
    for (int i = 0; i < contador; i++) {
        tesouros[i] = temporarios[i];
    }

    *quantidade = contador;

    return 1;
}


void mostrarTesouro(const Tesouro *tesouro)
{
    if (tesouro == NULL) {
        printf("Tesouro invalido!\n");
        return;
    }

    printf("Codigo: %c\n", tesouro->codigo);
    printf("Quantidade: %d\n", tesouro->quantidade);
}


int converteTextoModificadorTerreno(const char *texto, ModificadorTerreno *modificador)
{
    if (texto == NULL || modificador == NULL) {
        return 0;
    }

    char terrenoTexto[50];
    int valor;
    int caracteresLidos = 0;

    int resultado = sscanf(texto, "%49[^:]:%d%n", terrenoTexto, &valor, &caracteresLidos
    );

    if (resultado != 2) {
        return 0;
    }

    if (texto[caracteresLidos] != '\0') {
        return 0;
    }

    Terreno terreno;

    if (!converteTextoTerreno(terrenoTexto, &terreno)) {
        return 0;
    }

    modificador->terreno = terreno;
    modificador->modificador = valor;

    return 1;
}


int converteListaModificadoresTerreno(const char *texto, ModificadorTerreno modificadores[], int *quantidade)
{
    if (texto == NULL || modificadores == NULL || quantidade == NULL) {

        return 0;
    }

    if (texto[0] == '\0') {
        return 0;
    }

    char copia[500];

    ModificadorTerreno temporarios[MAX_MOD_TERRENOS];

    int contador = 0;

    size_t tamanho = strlen(texto);

    if (tamanho >= sizeof(copia)) {
        return 0;
    }

    // Verifica separadores invalidos
    if (texto[0] == ',' || texto[tamanho - 1] == ',') {
        return 0;
    }

    for (size_t i = 1; i < tamanho; i++) {

        if (texto[i] == ',' && texto[i - 1] == ',') {
            return 0;
        }
    }

    strcpy(copia, texto);

    char *parte = strtok(copia, ",");

    while (parte != NULL) {

        if (contador >= MAX_MOD_TERRENOS) {
            return 0;
        }

        if (!converteTextoModificadorTerreno(parte, &temporarios[contador])) {

            return 0;
        }

        // Impede terrenos repetidos
        for (int i = 0; i < contador; i++) {

            if (temporarios[i].terreno == temporarios[contador].terreno) {

                return 0;
            }
        }

        contador++;

        parte = strtok(NULL, ",");
    }

    if (contador == 0) {
        return 0;
    }

    // Copia somente apos validar todos
    for (int i = 0; i < contador; i++) {
        modificadores[i] = temporarios[i];
    }

    *quantidade = contador;

    return 1;
}


void mostrarModificadorTerreno(const ModificadorTerreno *modificador)
{
    if (modificador == NULL) {
        printf("Modificador invalido!\n");
        return;
    }

    printf("Terreno: %s\n", terrenoParaTexto(modificador->terreno));

    printf("Modificador: %+d\n", modificador->modificador);
}


int copiarTextoSeguro(char *destino, size_t capacidade, const char *origem)
{
    if (destino == NULL || origem == NULL || capacidade == 0) {
        return 0;
    }

    size_t tamanho = strlen(origem);

    if (tamanho >= capacidade) {
        return 0;
    }

    memcpy(destino, origem, tamanho + 1);

    return 1;
}


int separarChaveValor(const char *linha, char *chave, size_t capacidadeChave, char *valor, size_t capacidadeValor)
{
    if (linha == NULL || chave == NULL || valor == NULL || capacidadeChave == 0 || capacidadeValor == 0) {

        return 0;
    }

    // Procura o primeiro sinal de =
    const char *igual = strchr(linha, '=');

    if (igual == NULL) {
        return 0;
    }

    // Calcula o tamanho da chave
    size_t tamanhoChave = igual - linha;

    // Calcula o tamanho do valor
    size_t tamanhoValor = strlen(igual + 1);

    if (tamanhoChave == 0 || tamanhoChave >= capacidadeChave || tamanhoValor >= capacidadeValor) {

        return 0;
    }

    // Copia a chave
    memcpy(chave, linha, tamanhoChave);

    chave[tamanhoChave] = '\0';

    // Copia o valor
    memcpy(valor, igual + 1, tamanhoValor + 1);

    return 1;
}


void removerQuebraLinha(char *texto)
{
    if (texto == NULL) {
        return;
    }

    texto[strcspn(texto, "\r\n")] = '\0';
}


int testarLeituraBestiario(const char *nomeArquivo)
{
    if (nomeArquivo == NULL) {
        return 0;
    }

    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 0;
    }

    char linha[2048];
    char chave[100];
    char valor[2048];

    int dentroMonstro = 0;
    int quantidadeMonstros = 0;
    Monstro monstroAtual = {0}; 

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

        // Detecta linhas maiores que o buffer
        if (strchr(linha, '\n') == NULL && !feof(arquivo)) {

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

        // Inicio do monstro
        if (strcmp(linha, "[MONSTRO]") == 0) {

            if (dentroMonstro) {
                printf("Erro: monstro anterior nao foi encerrado!\n");
                fclose(arquivo);
                return 0;
            }

            dentroMonstro = 1;

            monstroAtual = (Monstro){0};

            printf("\n===== INICIO DO MONSTRO =====\n");

            continue;
        }

        // Fim do monstro
        if (strcmp(linha, "[/MONSTRO]") == 0) {

            if (!dentroMonstro) {
                printf("Erro: fechamento inesperado!\n");
                fclose(arquivo);
                return 0;
            }

            printf("\nMonstro processado:\n");

            printf("ID: %d\n", monstroAtual.id);
            printf("Nome: %s\n", monstroAtual.nome);

            printf("Frequencia: %s\n", frequenciaParaTexto(monstroAtual.frequencia));

            printf("Poder base: %d\n", monstroAtual.poderBase);

            printf("Modificador dia: %+d\n", monstroAtual.modDia);

            printf("Modificador noite: %+d\n", monstroAtual.modNoite);

            printf("Bonus por adicional: %d\n", monstroAtual.bonusPorAdicional);

            printf("Limite grupo grande: %d\n", monstroAtual.limiteGrupoGrande);

            printf("Bonus grupo grande: %d\n", monstroAtual.bonusGrupoGrande);

            printf("Descricao combate: %s\n", monstroAtual.descricaoCombate);

            dentroMonstro = 0;
            quantidadeMonstros++;

            printf("===== FIM DO MONSTRO =====\n");

            

            continue;
        }

        // Ignora atributos fora de um registro
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

        printf("Chave: %s | Valor: %s\n", chave, valor);

        if (!processarCampoMonstro(&monstroAtual, chave, valor))
        {

            printf("Erro ao processar o campo: %s\n", chave);

            fclose(arquivo);
            return 0;
        }
    }

    if (ferror(arquivo)) {
        printf("Erro durante a leitura do arquivo!\n");
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);

    if (dentroMonstro) {
        printf("Erro: ultimo monstro nao foi encerrado!\n");
        return 0;
    }

    printf("\nTotal de monstros encontrados: %d\n", quantidadeMonstros);

    return 1;
}


int processarCampoMonstro(Monstro *monstro, const char *chave, const char *valor)
{
    if (monstro == NULL || chave == NULL || valor == NULL) {
        return 0;
    }

    if (strcmp(chave, "id") == 0) {

        int id;
        int caracteresLidos = 0;

        if (sscanf(valor, "%d%n", &id, &caracteresLidos) != 1 || valor[caracteresLidos] != '\0' || id <= 0) {

            return 0;
        }

        monstro->id = id;
    }

    else if (strcmp(chave, "nome") == 0) {

        if (valor[0] == '\0') {
            return 0;
        }

        return copiarTextoSeguro(monstro->nome, sizeof(monstro->nome), valor);
    }

    else if (strcmp(chave, "descricao") == 0) {

        return copiarTextoSeguro(monstro->descricao, sizeof(monstro->descricao), valor);
    }

    else if (strcmp(chave, "frequencia") == 0) {

        return converteTextoFrequencia(valor, &monstro->frequencia);
    }

    else if (strcmp(chave, "numeroEncontrado") == 0) {

        return converteTextoDado(valor, &monstro->numeroEncontrado);
    }

    else if (strcmp(chave, "covil") == 0) {

        return converteTextoIntervaloInteiro(valor, &monstro->covil);
    }

    else if (strcmp(chave, "climas") == 0) {

        return converteListaClimas(valor, monstro->climas, &monstro->quantidadeClimas);
    }

    else if (strcmp(chave, "terrenos") == 0) {

        return converteListaTerrenos(valor, monstro->terrenos, &monstro->quantidadeTerrenos);
    }

    else if (strcmp(chave, "organizacao") == 0) {

        return converteTextoOrganizacao(valor, &monstro->organizacao);
    }

    else if (strcmp(chave, "atividade") == 0) {

        return converteTextoAtividade(valor, &monstro->atividade);
    }

    else if (strcmp(chave, "dieta") == 0) {

        return converteTextoDieta(valor, &monstro->dieta);
    }

    else if (strcmp(chave, "inteligencia") == 0)
    {
        return sscanf(valor, "%d", &monstro->inteligencia) == 1;
    }

    else if (strcmp(chave, "tesouro") == 0)
    {
        return converteListaTesouros(valor, monstro->tesouros, &monstro->quantidadeTesouros);
    }

    else if (strcmp(chave, "ordem") == 0)
    {
        return converteTextoOrdem(valor, &monstro->tendencia.ordem);
    }

    else if (strcmp(chave, "moralidade") == 0)
    {
        return converteTextoMoral(valor, &monstro->tendencia.moral);
    }

    else if (strcmp(chave, "ca") == 0)
    {
        return sscanf(valor, "%d", &monstro->ca) == 1;
    }

    else if (strcmp(chave, "movimentos") == 0)
    {
        return converteListaMovimentos(valor, monstro->movimentos, &monstro->quantidadeMovimentos);
    }

    else if (strcmp(chave, "dv") == 0)
    {
        return converteTextoDado(valor, &monstro->dv);
    }

    else if (strcmp(chave, "thac0") == 0)
    {
        return sscanf(valor, "%d", &monstro->thac0) == 1;
    }

    else if (strcmp(chave, "ataques") == 0)
    {
        return converteListaAtaques(valor, monstro->ataques, &monstro->quantidadeAtaques);
    }

    else if (strcmp(chave, "ataquesEspeciais") == 0)
    {
        return converteListaHabilidadesEspeciais(valor, monstro->ataquesEspeciais, &monstro->quantidadeAtaquesEspeciais);
    }

    else if (strcmp(chave, "defesasEspeciais") == 0)
    {
        return converteListaHabilidadesEspeciais(valor, monstro->defesasEspeciais, &monstro->quantidadeDefesasEspeciais);
    }

    else if (strcmp(chave, "resistenciaMagia") == 0)
    {
        return sscanf(valor, "%d", &monstro->resistenciaMagia) == 1;
    }

    else if (strcmp(chave, "tamanho") == 0)
    {
        return converteTextoTamanho(valor, &monstro->tamanho);
    }

    else if (strcmp(chave, "altura") == 0)
    {
        return converteTextoIntervaloReal(valor, &monstro->altura);
    }

    else if (strcmp(chave, "moral") == 0)
    {
        return sscanf(valor, "%d", &monstro->moral) == 1;
    }

    else if (strcmp(chave, "classificacaoMoral") == 0)
    {
        return converteTextoClassificacaoMoral(valor, &monstro->classificacaoMoral);
    }

    else if (strcmp(chave, "xp") == 0)
    {
        return sscanf(valor, "%d", &monstro->xp) == 1;
    }

    else if (strcmp(chave, "poderBase") == 0)
    {
        return sscanf(valor, "%d", &monstro->poderBase) == 1;
    }

    else if (strcmp(chave, "modDia") == 0)
    {
        return sscanf(valor, "%d", &monstro->modDia) == 1;
    }

    else if (strcmp(chave, "modNoite") == 0)
    {
        return sscanf(valor, "%d", &monstro->modNoite) == 1;
    }

    else if (strcmp(chave, "modTerreno") == 0)
    {
        return converteListaModificadoresTerreno(
            valor,
            monstro->modificadoresTerreno,
            &monstro->quantidadeModificadoresTerreno);
    }

    else if (strcmp(chave, "bonusPorAdicional") == 0)
    {
        return sscanf(valor, "%d", &monstro->bonusPorAdicional) == 1;
    }

    else if (strcmp(chave, "limiteGrupoGrande") == 0)
    {
        return sscanf(valor, "%d", &monstro->limiteGrupoGrande) == 1;
    }

    else if (strcmp(chave, "bonusGrupoGrande") == 0)
    {
        return sscanf(valor, "%d", &monstro->bonusGrupoGrande) == 1;
    }

    else if (strcmp(chave, "descricaoCombate") == 0)
    {
        return copiarTextoSeguro(
            monstro->descricaoCombate,
            sizeof(monstro->descricaoCombate),
            valor);
    }

    else if (strcmp(chave, "habitatSociedade") == 0)
    {
        return copiarTextoSeguro(
            monstro->habitatSociedade,
            sizeof(monstro->habitatSociedade),
            valor);
    }

    else if (strcmp(chave, "descricaoEcologia") == 0)
    {
        return copiarTextoSeguro(
            monstro->descricaoEcologia,
            sizeof(monstro->descricaoEcologia),
            valor);
    }

    else
    {
        printf("Campo desconhecido: %s\n", chave);
        return 0;
    }

    return 1;
}


int validaMonstro(const Monstro *monstro)
{
    if (monstro == NULL) {
        printf("Erro: monstro nulo!\n");
        return 0;
    }

    // IDENTIFICACAO

    if (monstro->id <= 0) {
        printf("Erro: ID deve ser positivo!\n");
        return 0;
    }

    if (monstro->nome[0] == '\0') {
        printf("Erro: nome nao pode ser vazio!\n");
        return 0;
    }

    // FREQUENCIA

    if (monstro->frequencia < MUITO_RARO || monstro->frequencia > MUITO_COMUM) {

        printf("Erro: frequencia invalida!\n");
        return 0;
    }

    // DADOS

    if (!validaDado(monstro->numeroEncontrado)) {
        printf("Erro: numero encontrado invalido!\n");
        return 0;
    }

    if (!validaDado(monstro->dv)) {
        printf("Erro: dados de vida invalidos!\n");
        return 0;
    }

    // COVIL

    if (monstro->covil.minimo < 0 || monstro->covil.maximo < monstro->covil.minimo) {

        printf("Erro: intervalo do covil invalido!\n");
        return 0;
    }

    // CLIMAS

    if (monstro->quantidadeClimas < 0 || monstro->quantidadeClimas > MAX_CLIMAS) {

        printf("Erro: quantidade de climas invalida!\n");
        return 0;
    }

    for (int i = 0; i < monstro->quantidadeClimas; i++) {

        if (monstro->climas[i] < QUALQUER || monstro->climas[i] > POLAR) {

            printf("Erro: clima invalido!\n");
            return 0;
        }
    }

    // TERRENOS

    if (monstro->quantidadeTerrenos < 1 || monstro->quantidadeTerrenos > MAX_TERRENOS) {

        printf("Erro: quantidade de terrenos invalida!\n");
        return 0;
    }

    for (int i = 0; i < monstro->quantidadeTerrenos; i++) {

        if (monstro->terrenos[i] < FLORESTA || monstro->terrenos[i] > URBANO) {

            printf("Erro: terreno invalido!\n");
            return 0;
        }
    }

    // ATIVIDADE

    if (monstro->atividade < DIURNO || monstro->atividade > QUALQUER_HORARIO) {

        printf("Erro: atividade invalida!\n");
        return 0;
    }

    // INTELIGENCIA

    if (monstro->inteligencia < 0) {
        printf("Erro: inteligencia invalida!\n");
        return 0;
    }

    // TENDENCIA

    if (monstro->tendencia.ordem < LEAL || monstro->tendencia.ordem > CAOTICO) {

        printf("Erro: eixo de ordem invalido!\n");
        return 0;
    }

    if (monstro->tendencia.moral < BOM || monstro->tendencia.moral > MAU) {

        printf("Erro: eixo moral invalido!\n");
        return 0;
    }

    // RESISTENCIA A MAGIA

    if (monstro->resistenciaMagia < 0 || monstro->resistenciaMagia > 100) {

        printf("Erro: resistencia a magia deve estar entre 0 e 100!\n");
        return 0;
    }

    // MOVIMENTOS

    if (monstro->quantidadeMovimentos < 1 || monstro->quantidadeMovimentos > MAX_MOVIMENTOS) {

        printf("Erro: quantidade de movimentos invalida!\n");
        return 0;
    }

    for (int i = 0; i < monstro->quantidadeMovimentos; i++) {

        if (monstro->movimentos[i].tipo < TERRESTRE || monstro->movimentos[i].tipo > ESCAVACAO) {

            printf("Erro: tipo de movimento invalido!\n");
            return 0;
        }

        if (monstro->movimentos[i].velocidade < 0) {

            printf("Erro: velocidade de movimento invalida!\n");
            return 0;
        }
    }

    // ATAQUES

    if (monstro->quantidadeAtaques < 1 || monstro->quantidadeAtaques > MAX_ATAQUES) {

        printf("Erro: quantidade de ataques invalida!\n");
        return 0;
    }

    for (int i = 0; i < monstro->quantidadeAtaques; i++) {

        if (monstro->ataques[i].nome[0] == '\0') {

            printf("Erro: ataque sem nome!\n");
            return 0;
        }

        if (!validaDado(monstro->ataques[i].dano)) {

            printf("Erro: dano do ataque invalido!\n");
            return 0;
        }
    }

    // HABILIDADES ESPECIAIS

    if (monstro->quantidadeAtaquesEspeciais < 0 || monstro->quantidadeAtaquesEspeciais > MAX_HABILIDADES) {

        printf("Erro: quantidade de ataques especiais invalida!\n");
        return 0;
    }

    if (monstro->quantidadeDefesasEspeciais < 0 || monstro->quantidadeDefesasEspeciais > MAX_HABILIDADES) {

        printf("Erro: quantidade de defesas especiais invalida!\n");
        return 0;
    }

    // TESOUROS

    if (monstro->quantidadeTesouros < 0 || monstro->quantidadeTesouros > MAX_TESOUROS) {

        printf("Erro: quantidade de tesouros invalida!\n");
        return 0;
    }

    // ALTURA

    if (monstro->altura.minimo < 0 || monstro->altura.maximo < monstro->altura.minimo) {

        printf("Erro: intervalo de altura invalido!\n");
        return 0;
    }

    // EXPERIENCIA

    if (monstro->xp < 0) {

        printf("Erro: XP nao pode ser negativo!\n");
        return 0;
    }

    // PODER E GRUPOS

    if (monstro->poderBase < 0) {

        printf("Erro: poder base invalido!\n");
        return 0;
    }

    if (monstro->quantidadeModificadoresTerreno < 0 ||
        monstro->quantidadeModificadoresTerreno > MAX_MOD_TERRENOS) {

        printf("Erro: quantidade de modificadores de terreno invalida!\n");
        return 0;
    }

    if (monstro->limiteGrupoGrande < 0) {

        printf("Erro: limite de grupo grande invalido!\n");
        return 0;
    }

    // ORGANIZACAO

    if (monstro->organizacao < SOLITARIO ||
        monstro->organizacao > CORTE)
    {

        printf("Erro: organizacao invalida!\n");
        return 0;
    }

    // DIETA

    if (monstro->dieta < CARNIVORO ||
        monstro->dieta > PARASITA)
    {

        printf("Erro: dieta invalida!\n");
        return 0;
    }

    // TAMANHO

    if (monstro->tamanho < PEQUENO ||
        monstro->tamanho > ENORME)
    {

        printf("Erro: tamanho invalido!\n");
        return 0;
    }

    // CLASSIFICACAO MORAL

    if (monstro->classificacaoMoral < INSTAVEL ||
        monstro->classificacaoMoral > FANATICO)
    {

        printf("Erro: classificacao moral invalida!\n");
        return 0;
    }

    // CLASSE DE ARMADURA E THAC0

    if (monstro->ca < -10 || monstro->ca > 20)
    {

        printf("Erro: classe de armadura fora da faixa adotada!\n");
        return 0;
    }

    if (monstro->thac0 < 0 || monstro->thac0 > 20)
    {

        printf("Erro: THAC0 fora da faixa adotada!\n");
        return 0;
    }

    // MORAL

    if (monstro->moral < 0 || monstro->moral > 20)
    {

        printf("Erro: moral fora da faixa adotada!\n");
        return 0;
    }

    return 1;
}
