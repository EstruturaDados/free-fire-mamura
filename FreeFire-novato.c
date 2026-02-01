#include <stdio.h>
#include <string.h>

#define MAX_ITENS 10
#define TAM_NOME  30
#define TAM_TIPO  20

typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
    int ativo; // 1 = existe no inventário, 0 = vazio
} Item;

/* Remove \n do fgets */
void removerQuebraLinha(char s[]) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') {
        s[n - 1] = '\0';
    }
}

/* Limpa o buffer do stdin (após scanf) */
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* Conta quantos itens ativos existem */
int contarAtivos(Item mochila[]) {
    int count = 0;
    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ativo == 1) count++;
    }
    return count;
}

/* Retorna o primeiro índice livre, ou -1 se cheio */
int acharPosicaoLivre(Item mochila[]) {
    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ativo == 0) return i;
    }
    return -1;
}

/* Procura item pelo nome (comparação exata), retorna índice ou -1 */
int buscarPorNome(Item mochila[], const char nome[]) {
    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ativo == 1 && strcmp(mochila[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

void listarItens(Item mochila[]) {
    int total = contarAtivos(mochila);

    printf("\n================= INVENTARIO DA MOCHILA =================\n");
    if (total == 0) {
        printf("Mochila vazia.\n");
        printf("=========================================================\n");
        return;
    }

    printf("%-3s | %-30s | %-20s | %-10s\n", "#", "NOME", "TIPO", "QTD");
    printf("----+--------------------------------+----------------------+-----------\n");

    int idx = 1;
    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ativo == 1) {
            printf("%-3d | %-30s | %-20s | %-10d\n",
                   idx, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            idx++;
        }
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
    limparBuffer();                 // garante que o fgets vai ler certo
    fgets(mochila[pos].nome, TAM_NOME, stdin);
    removerQuebraLinha(mochila[pos].nome);

    printf("Tipo do item: ");
    fgets(mochila[pos].tipo, TAM_TIPO, stdin);
    removerQuebraLinha(mochila[pos].tipo);

    printf("Quantidade: ");
    scanf("%d", &mochila[pos].quantidade);

    if (mochila[pos].quantidade < 0) {
        mochila[pos].quantidade = 0; // simples, evita negativo
    }

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
        printf("Item '%s' nao encontrado.\n", nome);
        return;
    }

    mochila[pos].ativo = 0;
    mochila[pos].nome[0] = '\0';
    mochila[pos].tipo[0] = '\0';
    mochila[pos].quantidade = 0;

    printf("Item removido com sucesso!\n");
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
        printf("\n=========== DESAFIO CODIGO DA ILHA (NOVATO) ===========\n");
        printf("1 - Adicionar item\n");
        printf("2 - Remover item (por nome)\n");
        printf("3 - Listar itens\n");
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
