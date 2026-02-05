#include <stdio.h>
#include <string.h>

#define MAX_ITENS 10
#define TAM_NOME  30
#define TAM_TIPO  20

typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
    int ativo; // 1 = existe, 0 = vazio
} Item;

/* Remove \n do fgets */
void removerQuebraLinha(char s[]) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') {
        s[n - 1] = '\0';
    }
}

/* Limpa stdin após scanf */
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* Acha primeira posição livre */
int acharPosicaoLivre(Item mochila[]) {
    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ativo == 0) return i;
    }
    return -1;
}

/* Busca sequencial por nome (strcmp), retorna índice ou -1 */
int buscarPorNome(Item mochila[], const char nome[]) {
    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ativo == 1 && strcmp(mochila[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

void listarItens(Item mochila[]) {
    int existe = 0;

    printf("\n================= INVENTARIO DA MOCHILA =================\n");
    printf("%-3s | %-30s | %-20s | %-10s\n", "#", "NOME", "TIPO", "QTD");
    printf("----+--------------------------------+----------------------+-----------\n");

    int idx = 1;
    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ativo == 1) {
            existe = 1;
            printf("%-3d | %-30s | %-20s | %-10d\n",
                   idx, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            idx++;
        }
    }

    if (!existe) {
        printf("   (mochila vazia)\n");
    }

    printf("=========================================================\n");
}

void adicionarItem(Item mochila[]) {
    int pos = acharPosicaoLivre(mochila);
    if (pos == -1) {
        printf("\nMochila cheia! (limite: %d itens)\n", MAX_ITENS);
        return;
    }

    printf("\n--- ADICIONAR ITEM ---\n");

    printf("Nome do item: ");
    limparBuffer();
    fgets(mochila[pos].nome, TAM_NOME, stdin);
    removerQuebraLinha(mochila[pos].nome);

    printf("Tipo do item: ");
    fgets(mochila[pos].tipo, TAM_TIPO, stdin);
    removerQuebraLinha(mochila[pos].tipo);

    printf("Quantidade: ");
    scanf("%d", &mochila[pos].quantidade);
    if (mochila[pos].quantidade < 0) mochila[pos].quantidade = 0;

    mochila[pos].ativo = 1;
    printf("Item adicionado com sucesso!\n");
}

void removerItem(Item mochila[]) {
    char nome[TAM_NOME];

    printf("\n--- REMOVER ITEM ---\n");
    printf("Digite o nome do item para remover: ");
    limparBuffer();
    fgets(nome, TAM_NOME, stdin);
    removerQuebraLinha(nome);

    int pos = buscarPorNome(mochila, nome);

    if (pos == -1) {
        printf("Ops! Nao encontrei nenhum item chamado '%s'.\n", nome);
        return;
    }

    mochila[pos].ativo = 0;
    mochila[pos].nome[0] = '\0';
    mochila[pos].tipo[0] = '\0';
    mochila[pos].quantidade = 0;

    printf("Item removido com sucesso!\n");
}

/* NOVO: busca pelo nome e mostra detalhes */
void buscarItemDetalhado(Item mochila[]) {
    char nome[TAM_NOME];

    printf("\n--- BUSCAR ITEM ---\n");
    printf("Digite o nome do item que deseja buscar: ");
    limparBuffer();
    fgets(nome, TAM_NOME, stdin);
    removerQuebraLinha(nome);

    int encontrado = 0; // flag
    int pos = -1;

    // busca sequencial (com flag)
    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ativo == 1 && strcmp(mochila[i].nome, nome) == 0) {
            encontrado = 1;
            pos = i;
            break;
        }
    }

    if (encontrado) {
        printf("\n✅ Item encontrado!\n");
        printf("Nome      : %s\n", mochila[pos].nome);
        printf("Tipo      : %s\n", mochila[pos].tipo);
        printf("Quantidade: %d\n", mochila[pos].quantidade);
    } else {
        printf("\n❌ Item '%s' nao foi encontrado na mochila.\n", nome);
        printf("Dica: confira se digitou exatamente igual (maiúsculas/minúsculas).\n");
    }
}

int main(void) {
    Item mochila[MAX_ITENS];
    int opcao;

    // inicializa mochila vazia
    for (int i = 0; i < MAX_ITENS; i++) {
        mochila[i].ativo = 0;
        mochila[i].nome[0] = '\0';
        mochila[i].tipo[0] = '\0';
        mochila[i].quantidade = 0;
    }

    do {
        printf("\n=========== DESAFIO CODIGO DA ILHA (AVENTUREIRO) ===========\n");
        printf("1 - Adicionar item\n");
        printf("2 - Remover item (por nome)\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item por nome (NOVO)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarItem(mochila);
                break;
            case 2:
                removerItem(mochila);
                break;
            case 3:
                listarItens(mochila);
                break;
            case 4:
                buscarItemDetalhado(mochila);
                break;
            case 0:
                printf("Saindo... boa sorte na ilha!\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
