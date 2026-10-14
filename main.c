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

int lerCampo(char valor[], int tamanho)
{
    if (fgets(valor, tamanho, stdin) == NULL) {
        return 0;
    }

    char *quebra = strchr(valor, '\n');

    if (quebra != NULL) {
        *quebra = '\0';
    } else if (!feof(stdin)) {
        int c;

        while ((c = getchar()) != '\n' && c != EOF) {
        }

        printf("Entrada muito longa!\n");
        return 0;
    }

    return 1;
}

void menuAlterarMonstro(Bestiario *b)
{
    if (b == NULL) {
        printf("Erro: bestiario invalido!\n");
        return;
    }

    int id = lerInteiro("\nDigite o ID do monstro: ");

    Monstro *encontrado = buscaMonstroPorId(b, id);

    if (encontrado == NULL) {
        printf("Monstro nao encontrado!\n");
        return;
    }

    // Trabalha em uma copia da ficha
    Monstro novosDados = *encontrado;

    const char *campos[] = {
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

    int totalCampos = sizeof(campos) / sizeof(campos[0]);

    int opcao;
    char valor[2048];

    do {
        printf("\n========== ALTERAR MONSTRO ==========\n");
        printf("Monstro: %s (ID: %d)\n", novosDados.nome, novosDados.id);

        printf("\n===== ATRIBUTOS =====\n");

        for (int i = 0; i < totalCampos; i++) {
            printf("%2d - %s\n", i + 1, campos[i]);
        }

        printf("\n 0 - Salvar alteracoes\n");
        printf("-1 - Cancelar\n");

        opcao = lerInteiro("\nEscolha uma opcao: ");

        if (opcao == -1) {
            printf("\nAlteracoes canceladas!\n");
            return;
        }

        if (opcao == 0) {

            if (alteraMonstro(b, id, novosDados)) {
                printf("\nMonstro alterado com sucesso!\n");
                return;
            }

            printf("\nErro ao salvar alteracoes.\n");
            printf("A ficha original foi preservada.\n");
            continue;
        }

        if (opcao < 1 || opcao > totalCampos) {
            printf("\nOpcao invalida!\n");
            continue;
        }

        int indice = opcao - 1;

        printf("\nAlterando: %s\n", campos[indice]);
        printf("Digite o novo valor: ");

        if (!lerCampo(valor, sizeof(valor))) {
            printf("Entrada invalida!\n");
            continue;
        }

        // Faz a alteracao em uma copia temporaria
        Monstro temporario = novosDados;

        if (!processarCampoMonstro(&temporario, campos[indice], valor)) {

            printf("\nValor invalido! Alteracao recusada.\n");
            continue;
        }

        // Impede a troca para um ID ja cadastrado
        if (indice == 0 && temporario.id != id) {

            Monstro *outro = buscaMonstroPorId(b, temporario.id);

            if (outro != NULL) {

                printf("\nErro: ID ja cadastrado!\n");
                continue;
            }
        }

        novosDados = temporario;

        printf("\nCampo atualizado na ficha temporaria!\n");

    } while (1);
}

void menuCadastrarMonstro(Bestiario *b)
{
    int idModelo;
    int novoId;
    int novoPoder;
    int novoXP;

    char novoNome[50];

    Monstro *modelo;
    Monstro novoMonstro;

    if (b == NULL) {
        printf("Erro: bestiario invalido!\n");
        return;
    }

    if (b->quantidade >= MAX_MONSTROS) {
        printf("Erro: bestiario cheio!\n");
        return;
    }

    printf("\n========== CADASTRAR MONSTRO ==========\n");

    printf("\nEscolha um monstro existente para usar como modelo.\n");

    listarMonstros(b->monstros, b->quantidade);

    idModelo = lerInteiro("\nDigite o ID do monstro modelo: ");

    modelo = buscarMonstroPorId(b->monstros, b->quantidade, idModelo);

    if (modelo == NULL) {
        printf("\nMonstro modelo nao encontrado!\n");
        return;
    }

    // Copia a ficha do monstro escolhido
    novoMonstro = *modelo;

    printf("\nModelo selecionado: %s\n", modelo->nome);

    novoId = lerInteiro("\nDigite o ID do novo monstro: ");

    if (novoId <= 0) {
        printf("Erro: ID deve ser positivo!\n");
        return;
    }

    if (buscarMonstroPorId(b->monstros, b->quantidade, novoId) != NULL) {

        printf("Erro: esse ID ja esta cadastrado!\n");
        return;
    }

    novoMonstro.id = novoId;

    printf("\nDigite o nome do novo monstro: ");

    if (!lerNome(novoNome, sizeof(novoNome))) {
        printf("Cadastro cancelado!\n");
        return;
    }

    strcpy(novoMonstro.nome, novoNome);

    printf("\nPoder base do modelo: %d\n", modelo->poderBase);

    novoPoder = lerInteiro("Digite o novo poder base: ");

    if (novoPoder < 0) {
        printf("Erro: poder base invalido!\n");
        return;
    }

    novoMonstro.poderBase = novoPoder;

    printf("\nXP do modelo: %d\n", modelo->xp);

    novoXP = lerInteiro("Digite o XP do novo monstro: ");

    if (novoXP < 0) {
        printf("Erro: XP invalido!\n");
        return;
    }

    novoMonstro.xp = novoXP;

    printf("\n===== NOVO MONSTRO =====\n");
    printf("ID: %d\n", novoMonstro.id);
    printf("Nome: %s\n", novoMonstro.nome);
    printf("Poder base: %d\n", novoMonstro.poderBase);
    printf("XP: %d\n", novoMonstro.xp);

    int confirmar = lerInteiro("\nConfirmar cadastro? (1-Sim / 0-Nao): ");

    if (confirmar != 1) {
        printf("\nCadastro cancelado!\n");
        return;
    }

    if (insereMonstro(b, novoMonstro)) {

        printf("\nMonstro cadastrado com sucesso!\n");
        printf("Total de monstros: %d\n", b->quantidade);

    } else {

        printf("\nErro ao cadastrar monstro!\n");
    }
}

void mostrarMenu(void)
{
    printf("\n---------- BESTIARIO RPG ----------\n");

    printf("1 - Listar todos os monstros\n");
    printf("2 - Buscar monstro por ID\n");
    printf("3 - Buscar monstro por nome\n");
    printf("4 - Exibir ficha completa por ID\n");
    printf("5 - Filtrar monstros\n");
    printf("6 - Remover monstro\n");
    printf("7 - Alterar monstro\n");
    printf("8 - Salvar bestiario\n");
    printf("9 - Cadastrar novo monstro\n");
    printf("10 - Cadastrar monstro do zero\n");
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
        printf("4 - Filtrar por atividade\n");
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

            case 4:

                printf("\n===== PERIODOS DE ATIVIDADE =====\n");

                for (int i = DIURNO; i <= QUALQUER_HORARIO; i++)
                {
                    printf("%d - %s\n", i, atividadeParaTexto((Atividade)i));
                }

                escolha = lerInteiro("Escolha o periodo: ");

                if (escolha < DIURNO || escolha > QUALQUER_HORARIO)
                {
                    printf("Periodo invalido!\n");
                    break;
                }

                filtrarMonstrosPorAtividade(bestiario->monstros, bestiario->quantidade, (Atividade)escolha);

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

void menuCadastrarMonstroCompleto(Bestiario *b)
{
    if (b == NULL) {
        printf("Erro: bestiario invalido!\n");
        return;
    }

    if (b->quantidade >= MAX_MONSTROS) {
        printf("Erro: bestiario cheio!\n");
        return;
    }

    Monstro novo = {0};

    typedef struct {
        const char *chave;
        const char *exemplo;
    } CampoCadastro;

    const CampoCadastro campos[] = {

        // IDENTIFICACAO
        {"id", "51"},
        {"nome", "Goblin Guerreiro"},
        {"descricao", "Criatura que protege sua tribo."},

        // ECOLOGIA
        {"frequencia", "COMUM"},
        {"numeroEncontrado", "2d6"},
        {"covil", "10,50"},
        {"climas", "TEMPERADO,FRIO"},
        {"terrenos", "FLORESTA,CAVERNA"},
        {"organizacao", "TRIBO"},
        {"atividade", "NOTURNO"},
        {"dieta", "ONIVORO"},
        {"inteligencia", "8"},
        {"tesouro", "K;O;Qx10;C"},
        {"ordem", "LEAL"},
        {"moralidade", "MAU"},

        // COMBATE
        {"ca", "6"},
        {"movimentos", "TERRESTRE:6"},
        {"dv", "2d8"},
        {"thac0", "19"},
        {"ataques", "ESPADA:1d8"},
        {"ataquesEspeciais", "NENHUM"},
        {"defesasEspeciais", "NENHUMA"},
        {"resistenciaMagia", "0"},
        {"tamanho", "PEQUENO"},
        {"altura", "1.0,1.5"},
        {"moral", "10"},
        {"classificacaoMoral", "MEDIO"},
        {"xp", "50"},

        // PODER DE ENCONTRO
        {"poderBase", "5"},
        {"modDia", "-1"},
        {"modNoite", "1"},
        {"modTerreno", "FLORESTA:1,CAVERNA:1"},
        {"bonusPorAdicional", "2"},
        {"limiteGrupoGrande", "5"},
        {"bonusGrupoGrande", "3"},

        // DESCRICOES
        {"descricaoCombate", "Prefere atacar em grupo."},
        {"habitatSociedade", "Vive em cavernas."},
        {"descricaoEcologia", "Consome pequenos animais."}
    };

    int totalCampos = sizeof(campos) / sizeof(campos[0]);

    char valor[2048];

    printf("\n========== CADASTRO COMPLETO ==========\n");
    printf("Preencha os campos usando os formatos indicados.\n");
    printf("Digite CANCELAR para interromper o cadastro.\n");

    for (int i = 0; i < totalCampos; i++) {

        if (i == 0) {
            printf("\n===== IDENTIFICACAO =====\n");
        } else if (i == 3) {
            printf("\n===== ECOLOGIA =====\n");
        } else if (i == 15) {
            printf("\n===== COMBATE =====\n");
        } else if (i == 28) {
            printf("\n===== PODER DE ENCONTRO =====\n");
        } else if (i == 35) {
            printf("\n===== DESCRICOES =====\n");
        }

        while (1) {

            printf("\nCampo: %s\n", campos[i].chave);
            printf("Exemplo: %s\n", campos[i].exemplo);
            printf("Valor: ");

            if (!lerCampo(valor, sizeof(valor))) {
                printf("Cadastro interrompido.\n");
                return;
            }

            if (strcmp(valor, "CANCELAR") == 0) {
                printf("Cadastro cancelado!\n");
                return;
            }

            if (!processarCampoMonstro(&novo, campos[i].chave, valor)) {
                printf("Valor invalido! Tente novamente.\n");
                continue;
            }

            // Verifica se o ID ja existe
            if (i == 0) {

                if (buscarMonstroPorId(b->monstros, b->quantidade, novo.id) != NULL) {
                    printf("Esse ID ja esta cadastrado!\n");
                    continue;
                }
            }

            break;
        }
    }

    // Valida a ficha completa
    if (!validaMonstro(&novo)) {
        printf("\nErro: ficha invalida! Cadastro cancelado.\n");
        return;
    }

    printf("\n===== RESUMO DO CADASTRO =====\n");
    printf("ID: %d\n", novo.id);
    printf("Nome: %s\n", novo.nome);
    printf("Frequencia: %s\n", frequenciaParaTexto(novo.frequencia));
    printf("Poder base: %d\n", novo.poderBase);
    printf("XP: %d\n", novo.xp);

    int confirmar = lerInteiro("\nConfirmar cadastro? (1-Sim / 0-Nao): ");

    if (confirmar != 1) {
        printf("Cadastro cancelado!\n");
        return;
    }

    if (insereMonstro(b, novo)) {

        printf("\nMonstro cadastrado com sucesso!\n");
        printf("Total: %d monstros\n", b->quantidade);

    } else {

        printf("\nErro ao inserir monstro!\n");
    }
}

int main(void)
{
    Bestiario bestiario = {0};

    Monstro *encontrado;

    int opcao;
    int id;

    char nome[50];

    printf("========== INICIANDO BESTIARIO ==========\n");

    if (!carregaBestiario(&bestiario, "bestiario.txt"))
    {
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

            case 6:

                id = lerInteiro("\nDigite o ID do monstro que deseja remover: ");

                encontrado = buscarMonstroPorId(bestiario.monstros, bestiario.quantidade, id);

                if (encontrado == NULL)
                {

                    printf("\nMonstro nao encontrado!\n");
                    break;
                }

                printf("\nMonstro selecionado:\n");
                printf("ID: %d\n", encontrado->id);
                printf("Nome: %s\n", encontrado->nome);

                int confirmar = lerInteiro("\nDeseja realmente remover? (1-Sim / 0-Nao): ");

                if (confirmar != 1)
                {

                    printf("\nRemocao cancelada!\n");
                    break;
                }

                if (removeMonstro(&bestiario, id))
                {

                    printf("\nMonstro removido com sucesso!\n");
                }
                else
                {

                    printf("\nErro ao remover monstro!\n");
                }

                break;

            case 7:
                menuAlterarMonstro(&bestiario);
                break;

            case 8:

                if (salvaBestiario(
                        &bestiario,
                        "bestiario-saida.txt"))
                {

                    printf("\nBestiario salvo com sucesso!\n");
                }
                else
                {

                    printf("\nErro ao salvar o bestiario!\n");
                }

                break;

            case 9:

                menuCadastrarMonstro(&bestiario);

                break;

            case 10:

                menuCadastrarMonstroCompleto(&bestiario);

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