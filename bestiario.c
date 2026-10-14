#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "bestiario.h"

//------------------------------------------------------------
// Função auxiliar para comparar nomes de monstros, ignorando maiúsculas e minúsculas

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

#define TOTAL_CHAVES_MONSTRO 38

//------------------------------------------------------------
// Lista de chaves obrigatórias para cada monstro

static const char *chavesMonstro[TOTAL_CHAVES_MONSTRO] = {
    "id",
    "nome",
    "descricao",
    "frequencia",
    "numeroEncontrado",
    "covil",
    "climas",
    "terrenos",
    "organizacao",
    "atividade",
    "dieta",
    "inteligencia",
    "tesouro",
    "ordem",
    "moralidade",
    "ca",
    "movimentos",
    "dv",
    "thac0",
    "ataques",
    "ataquesEspeciais",
    "defesasEspeciais",
    "resistenciaMagia",
    "tamanho",
    "altura",
    "moral",
    "classificacaoMoral",
    "xp",
    "poderBase",
    "modDia",
    "modNoite",
    "modTerreno",
    "bonusPorAdicional",
    "limiteGrupoGrande",
    "bonusGrupoGrande",
    "descricaoCombate",
    "habitatSociedade",
    "descricaoEcologia"
};

//------------------------------------------------------------
// Função auxiliar para obter o índice de uma chave na lista de chaves obrigatórias

static int indiceChaveMonstro(const char *chave)
{
    if (chave == NULL) {
        return -1;
    }

    for (int i = 0; i < TOTAL_CHAVES_MONSTRO; i++) {

        if (strcmp(chave, chavesMonstro[i]) == 0) {
            return i;
        }
    }

    return -1;
}

//------------------------------------------------------------
// Função para carregar o bestiário a partir de um arquivo

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

    unsigned char chavesEncontradas[TOTAL_CHAVES_MONSTRO] = {0};

    int numeroLinha = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        
        numeroLinha++;
        // Verifica se a linha cabe no buffer
        size_t tamanho = strlen(linha);

        if (tamanho > 0 && linha[tamanho - 1] != '\n' && !feof(arquivo)) {

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

            memset(chavesEncontradas, 0, sizeof(chavesEncontradas));

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
            
            // Verifica se a capacidade do vetor foi excedida
            if (contador >= capacidade) {
                printf("Erro: capacidade do bestiario excedida!\n");

                fclose(arquivo);
                return 0;
            }

            // Verifica se todas as chaves obrigatorias apareceram
            for (int i = 0; i < TOTAL_CHAVES_MONSTRO; i++)
            {

                if (!chavesEncontradas[i])
                {

                    printf("Erro na linha %d: campo obrigatorio '%s' ausente!\n", numeroLinha, chavesMonstro[i]);

                    fclose(arquivo);
                    return 0;
                }
            }

            // Valida todos os atributos do monstro
            if (!validaMonstro(&monstroAtual))
            {

                printf("Erro: ficha do monstro invalida!\n");

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

        int indice = indiceChaveMonstro(chave);

        // Verifica se a chave existe
        if (indice == -1)
        {

            printf("Erro na linha %d: chave desconhecida '%s'!\n",
                   numeroLinha, chave);

            fclose(arquivo);
            return 0;
        }

        // Verifica se a chave ja apareceu neste monstro
        if (chavesEncontradas[indice])
        {

            printf("Erro na linha %d: chave '%s' repetida!\n", numeroLinha, chave);

            fclose(arquivo);
            return 0;
        }

        // Processa o atributo
        if (!processarCampoMonstro(&monstroAtual, chave, valor)) {

            printf("Erro na linha %d: valor invalido para '%s': %s\n", numeroLinha, chave, valor);

            fclose(arquivo);
            return 0;
        }
        chavesEncontradas[indice] = 1;
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

//------------------------------------------------------------
// Função para salvar o bestiário em um arquivo

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

//------------------------------------------------------------
// Função para exibir os detalhes de um monstro

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

//------------------------------------------------------------
// Função para buscar um monstro pelo nome

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


//------------------------------------------------------------
// Função para filtrar monstros por frequência

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

//------------------------------------------------------------
// Função para filtrar monstros por terreno

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

//------------------------------------------------------------
// Função para filtrar monstros por clima

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


//------------------------------------------------------------
// Função para filtrar monstros por período de atividade

int insereMonstro(Bestiario *b, Monstro monstro)
{
    // Verifica se o bestiario existe
    if (b == NULL) {
        printf("Erro: bestiario invalido!\n");
        return 0;
    }

    // Verifica se a quantidade esta correta
    if (b->quantidade < 0 || b->quantidade > MAX_MONSTROS) {

        printf("Erro: quantidade de monstros invalida!\n");
        return 0;
    }

    // Verifica se existe espaco
    if (b->quantidade == MAX_MONSTROS) {

        printf("Erro: bestiario cheio!\n");
        return 0;
    }

    // Verifica se a ficha e valida
    if (!validaMonstro(&monstro)) {

        printf("Erro: ficha do monstro invalida!\n");
        return 0;
    }

    // Verifica se o ID ja existe
    for (int i = 0; i < b->quantidade; i++) {

        if (b->monstros[i].id == monstro.id) {

            printf("Erro: ja existe um monstro com ID %d!\n", monstro.id);

            return 0;
        }
    }

    // Insere o monstro na proxima posicao livre
    b->monstros[b->quantidade] = monstro;

    // Atualiza a quantidade
    b->quantidade++;

    return 1;
}

//------------------------------------------------------------
// Função para remover um monstro do bestiário

int removeMonstro(Bestiario *b, int id)
{
    if (b == NULL) {
        printf("Erro: bestiario invalido!\n");
        return 0;
    }

    if (b->quantidade < 0 || b->quantidade > MAX_MONSTROS) {

        printf("Erro: quantidade invalida!\n");
        return 0;
    }

    if (id <= 0) {
        printf("Erro: ID invalido!\n");
        return 0;
    }

    // Procura a posicao do monstro
    int posicao = -1;

    for (int i = 0; i < b->quantidade; i++) {

        if (b->monstros[i].id == id) {
            posicao = i;
            break;
        }
    }

    // Verifica se encontrou
    if (posicao == -1) {
        printf("Erro: monstro nao encontrado!\n");
        return 0;
    }

    // Desloca os monstros para a esquerda
    for (int i = posicao; i < b->quantidade - 1; i++) {

        b->monstros[i] = b->monstros[i + 1];
    }

    // Diminui a quantidade
    b->quantidade--;

    // Limpa a ultima posicao utilizada
    b->monstros[b->quantidade] = (Monstro){0};

    return 1;
}

//------------------------------------------------------------
// Função para alterar os dados de um monstro existente

int alteraMonstro(Bestiario *b, int id, Monstro novosDados)
{
    if (b == NULL) {
        printf("Erro: bestiario invalido!\n");
        return 0;
    }

    if (b->quantidade < 0 || b->quantidade > MAX_MONSTROS) {

        printf("Erro: quantidade de monstros invalida!\n");
        return 0;
    }

    if (id <= 0) {
        printf("Erro: ID informado invalido!\n");
        return 0;
    }

    // Procura o monstro que sera alterado
    int posicao = -1;

    for (int i = 0; i < b->quantidade; i++) {

        if (b->monstros[i].id == id) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
        printf("Erro: monstro nao encontrado!\n");
        return 0;
    }

    // Verifica se a nova ficha e valida
    if (!validaMonstro(&novosDados)) {
        printf("Erro: os novos dados sao invalidos!\n");
        return 0;
    }

    // Verifica se o novo ID pertence a outro monstro
    for (int i = 0; i < b->quantidade; i++) {

        if (i != posicao &&
            b->monstros[i].id == novosDados.id) {

            printf("Erro: o ID %d ja pertence a outro monstro!\n", novosDados.id);

            return 0;
        }
    }

    // Substitui a ficha somente depois de validar tudo
    b->monstros[posicao] = novosDados;

    return 1;
}

//------------------------------------------------------------
// Função para verificar se um monstro é ativo em um determinado período

int ativoNoPeriodo(const Monstro *m, Atividade periodo)
{
    if (m == NULL) {
        return 0;
    }

    if (periodo < DIURNO || periodo > QUALQUER_HORARIO) {
        return 0;
    }

    if (m->atividade < DIURNO || m->atividade > QUALQUER_HORARIO) {
        return 0;
    }

    // Monstros ativos em qualquer horario
    // podem aparecer tanto de dia quanto de noite.
    if (m->atividade == QUALQUER_HORARIO) {
        return 1;
    }

    // Consulta por qualquer horario:
    // aceita todos os periodos validos.
    if (periodo == QUALQUER_HORARIO) {
        return 1;
    }

    // Verifica se os periodos sao iguais.
    return m->atividade == periodo;
}

//------------------------------------------------------------
// Função para filtrar monstros por período de atividade

void filtrarMonstrosPorAtividade(const Monstro monstros[], int quantidade, Atividade periodo)
{
    if (monstros == NULL || quantidade < 0) {
        printf("Erro: dados invalidos!\n");
        return;
    }

    if (periodo < DIURNO || periodo > QUALQUER_HORARIO) {
        printf("Erro: periodo invalido!\n");
        return;
    }

    int encontrados = 0;

    printf("\n===== MONSTROS POR ATIVIDADE =====\n");
    printf("Periodo: %s\n\n", atividadeParaTexto(periodo));

    for (int i = 0; i < quantidade; i++) {

        if (ativoNoPeriodo(&monstros[i], periodo)) {

            printf("ID: %d | Nome: %s | Atividade: %s\n", monstros[i].id, monstros[i].nome, atividadeParaTexto(monstros[i].atividade));

            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum monstro encontrado.\n");
    }

    printf("\nTotal encontrado: %d\n", encontrados);
}

//------------------------------------------------------------
// Função auxiliar para salvar a ficha de um monstro em um arquivo

static int salvarFichaMonstro(FILE *arquivo, const Monstro *m)
{
    char dado[64];
    if (fprintf(arquivo,
        "[MONSTRO]\n"
        "id=%d\nnome=%s\ndescricao=%s\nfrequencia=%s\n",
        m->id, m->nome, m->descricao,
        frequenciaParaTexto(m->frequencia)) < 0) return 0;

    dadoParaTexto(m->numeroEncontrado, dado, sizeof dado);
    if (fprintf(arquivo, "numeroEncontrado=%s\ncovil=%d,%d\nclimas=",
        dado, m->covil.minimo, m->covil.maximo) < 0) return 0;

    for (int i = 0; i < m->quantidadeClimas; i++)
        if (fprintf(arquivo, "%s%s", i ? "," : "", climaParaTexto(m->climas[i])) < 0) return 0;

    if (fprintf(arquivo, "\nterrenos=") < 0) return 0;
    for (int i = 0; i < m->quantidadeTerrenos; i++)
        if (fprintf(arquivo, "%s%s", i ? "," : "", terrenoParaTexto(m->terrenos[i])) < 0) return 0;

    if (fprintf(arquivo,
        "\norganizacao=%s\natividade=%s\ndieta=%s\ninteligencia=%d\ntesouro=",
        organizacaoParaTexto(m->organizacao), atividadeParaTexto(m->atividade),
        dietaParaTexto(m->dieta), m->inteligencia) < 0) return 0;

    if (m->quantidadeTesouros == 0 && fprintf(arquivo, "NENHUM") < 0) return 0;
    for (int i = 0; i < m->quantidadeTesouros; i++) {
        const Tesouro *t = &m->tesouros[i];
        if (fprintf(arquivo, "%s%c", i ? ";" : "", t->codigo) < 0) return 0;
        if (t->quantidade != 1 && fprintf(arquivo, "x%d", t->quantidade) < 0) return 0;
    }

    if (fprintf(arquivo,
        "\nordem=%s\nmoralidade=%s\nca=%d\nmovimentos=",
        ordemParaTexto(m->tendencia.ordem), moralParaTexto(m->tendencia.moral),
        m->ca) < 0) return 0;

    for (int i = 0; i < m->quantidadeMovimentos; i++) {
        const Movimento *mov = &m->movimentos[i];
        if (fprintf(arquivo, "%s%s:%d", i ? "|" : "",
            tipoMovimentoParaTexto(mov->tipo), mov->velocidade) < 0) return 0;
        if (mov->tipo == VOO && fprintf(arquivo, ":%c", mov->manobrabilidade) < 0) return 0;
    }

    dadoParaTexto(m->dv, dado, sizeof dado);
    if (fprintf(arquivo, "\ndv=%s\nthac0=%d\nataques=", dado, m->thac0) < 0) return 0;

    for (int i = 0; i < m->quantidadeAtaques; i++) {
        dadoParaTexto(m->ataques[i].dano, dado, sizeof dado);
        if (fprintf(arquivo, "%s%s:%s", i ? "|" : "", m->ataques[i].nome, dado) < 0) return 0;
    }

    if (fprintf(arquivo, "\nataquesEspeciais=") < 0) return 0;
    if (m->quantidadeAtaquesEspeciais == 0 && fprintf(arquivo, "NENHUM") < 0) return 0;
    for (int i = 0; i < m->quantidadeAtaquesEspeciais; i++) {
        const HabilidadeEspecial *h = &m->ataquesEspeciais[i];
        if (fprintf(arquivo, "%s%s:%d:%s", i ? "|" : "", h->nome,
            h->valor, h->descricao) < 0) return 0;
    }

    if (fprintf(arquivo, "\ndefesasEspeciais=") < 0) return 0;
    if (m->quantidadeDefesasEspeciais == 0 && fprintf(arquivo, "NENHUMA") < 0) return 0;
    for (int i = 0; i < m->quantidadeDefesasEspeciais; i++) {
        const HabilidadeEspecial *h = &m->defesasEspeciais[i];
        if (fprintf(arquivo, "%s%s:%d:%s", i ? "|" : "", h->nome,
            h->valor, h->descricao) < 0) return 0;
    }

    if (fprintf(arquivo,
        "\nresistenciaMagia=%d\ntamanho=%s\naltura=%.6g,%.6g\n"
        "moral=%d\nclassificacaoMoral=%s\nxp=%d\npoderBase=%d\n"
        "modDia=%d\nmodNoite=%d\nmodTerreno=",
        m->resistenciaMagia, tamanhoParaTexto(m->tamanho),
        (double)m->altura.minimo, (double)m->altura.maximo,
        m->moral, classificacaoMoralParaTexto(m->classificacaoMoral),
        m->xp, m->poderBase, m->modDia, m->modNoite) < 0) return 0;

    if (m->quantidadeModificadoresTerreno == 0 && fprintf(arquivo, "NENHUM") < 0) return 0;
    for (int i = 0; i < m->quantidadeModificadoresTerreno; i++) {
        const ModificadorTerreno *mod = &m->modificadoresTerreno[i];
        if (fprintf(arquivo, "%s%s:%d", i ? "," : "",
            terrenoParaTexto(mod->terreno), mod->modificador) < 0) return 0;
    }

    if (fprintf(arquivo,
        "\nbonusPorAdicional=%d\nlimiteGrupoGrande=%d\nbonusGrupoGrande=%d\n"
        "descricaoCombate=%s\nhabitatSociedade=%s\ndescricaoEcologia=%s\n"
        "[/MONSTRO]\n\n",
        m->bonusPorAdicional, m->limiteGrupoGrande, m->bonusGrupoGrande,
        m->descricaoCombate, m->habitatSociedade, m->descricaoEcologia) < 0) return 0;

    return 1;
}

//------------------------------------------------------------
// Função para salvar o bestiário em um arquivo

int salvaBestiario(const Bestiario *b, const char *nomeArquivo)
{
    if (b == NULL || nomeArquivo == NULL || nomeArquivo[0] == '\0' || b->quantidade < 0 || b->quantidade > MAX_MONSTROS) return 0;

    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo para salvar");
        return 0;
    }
    int sucesso = 1;
    for (int i = 0; i < b->quantidade; i++) {
        if (!salvarFichaMonstro(arquivo, &b->monstros[i])) {
            sucesso = 0;
            break;
        }
    }
    if (fclose(arquivo) != 0) sucesso = 0;
    if (!sucesso) printf("Erro durante a gravacao do bestiario!\n");
    return sucesso;
}

//------------------------------------------------------------
// Função para verificar se um monstro vive em um determinado terreno

int viveNoTerreno(const Monstro *m, Terreno terreno)
{
    if (m == NULL) {
        return 0;
    }

    if (terreno < FLORESTA || terreno > URBANO) {
        return 0;
    }

    if (m->quantidadeTerrenos < 0 || m->quantidadeTerrenos > MAX_TERRENOS) {
        return 0;
    }

    for (int i = 0; i < m->quantidadeTerrenos; i++) {

        if (m->terrenos[i] == terreno) {
            return 1;
        }
    }

    return 0;
}

//------------------------------------------------------------
// Função para listar monstros por terreno

int listaMonstrosPorTerreno(const Bestiario *b, Terreno terreno, Monstro *resultado[], int capacidadeResultado)
{
    if (b == NULL || resultado == NULL) {
        return -1;
    }

    if (terreno < FLORESTA || terreno > URBANO) {
        return -1;
    }

    if (capacidadeResultado < 0 || b->quantidade < 0 || b->quantidade > MAX_MONSTROS) {
        return -1;
    }

    int encontrados = 0;

    for (int i = 0; i < b->quantidade; i++) {

        if (viveNoTerreno(&b->monstros[i], terreno)) {

            if (encontrados >= capacidadeResultado) {
                return -1;
            }

            resultado[encontrados] = (Monstro *)&b->monstros[i];

            encontrados++;
        }
    }

    return encontrados;
}

//------------------------------------------------------------
// Inicializa o bestiario

void inicializaBestiario(Bestiario *b)
{
    if (b == NULL) {
        return;
    }

    *b = (Bestiario){0};
}

//------------------------------------------------------------
// Carrega os monstros utilizando a estrutura Bestiario
int carregaBestiario(Bestiario *b, const char *nomeArquivo)
{
    if (b == NULL || nomeArquivo == NULL) {
        return 0;
    }

    inicializaBestiario(b);

    return carregarBestiario( nomeArquivo, b->monstros, MAX_MONSTROS, &b->quantidade);
}

//------------------------------------------------------------
// Consulta um monstro sem permitir modificacoes

const Monstro *consultaMonstroPorId(const Bestiario *b, int id)
{
    if (b == NULL || id <= 0) {
        return NULL;
    }

    if (b->quantidade < 0 || b->quantidade > MAX_MONSTROS) { return NULL;
    }

    for (int i = 0; i < b->quantidade; i++) {

        if (b->monstros[i].id == id) {
            return &b->monstros[i];
        }
    }

    return NULL;
}

//------------------------------------------------------------
// Consulta um monstro sem permitir modificacoes

Monstro *buscaMonstroPorId(Bestiario *b, int id)
{
    if (b == NULL || id <= 0) {
        return NULL;
    }

    return buscarMonstroPorId(b->monstros, b->quantidade, id);
}

//----------------------------------------------------------
// Consulta um monstro sem permitir modificacoes

Monstro *buscaMonstroPorNome(Bestiario *b, const char *nome)
{
    if (b == NULL || nome == NULL) {
        return NULL;
    }

    return buscarMonstroPorNome(b->monstros, b->quantidade, nome);
}

//----------------------------------------------------------
// Lista todos os monstros do bestiario

void listaMonstros(const Bestiario *b)
{
    if (b == NULL) {
        return;
    }

    listarMonstros(b->monstros, b->quantidade);
}

//----------------------------------------------------------
// Exibe os detalhes de um monstro

void exibeMonstro(const Monstro *m)
{
    mostrarMonstro(m);
}