#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define TAM_NOME 30
#define TAM_TIPO 20

typedef struct
{
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int prioridade;
} Componente;

/* Remove o '\n' do final da string, se existir */
void removerQuebraLinha(char *str)
{
    str[strcspn(str, "\n")] = '\0';
}

/* Limpa o buffer de entrada */
void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* Troca dois componentes de posição */
void trocarComponentes(Componente *a, Componente *b)
{
    Componente temp = *a;
    *a = *b;
    *b = temp;
}

/* Exibe todos os componentes formatados */
void mostrarComponentes(Componente componentes[], int n)
{
    int i;

    if (n == 0)
    {
        printf("\nNenhum componente cadastrado.\n");
        return;
    }

    printf("\n==============================================================\n");
    printf("%-4s | %-30s | %-20s | %-10s\n", "ID", "NOME", "TIPO", "PRIORIDADE");
    printf("--------------------------------------------------------------\n");

    for (i = 0; i < n; i++)
    {
        printf("%-4d | %-30s | %-20s | %-10d\n",
               i + 1,
               componentes[i].nome,
               componentes[i].tipo,
               componentes[i].prioridade);
    }

    printf("==============================================================\n");
}

/* Bubble Sort por nome */
long bubbleSortNome(Componente componentes[], int n)
{
    long comparacoes = 0;
    int i, j;
    int trocou;

    for (i = 0; i < n - 1; i++)
    {
        trocou = 0;

        for (j = 0; j < n - 1 - i; j++)
        {
            comparacoes++;
            if (strcmp(componentes[j].nome, componentes[j + 1].nome) > 0)
            {
                trocarComponentes(&componentes[j], &componentes[j + 1]);
                trocou = 1;
            }
        }

        if (!trocou)
        {
            break;
        }
    }

    return comparacoes;
}

/* Insertion Sort por tipo */
long insertionSortTipo(Componente componentes[], int n)
{
    long comparacoes = 0;
    int i, j;
    Componente chave;

    for (i = 1; i < n; i++)
    {
        chave = componentes[i];
        j = i - 1;

        while (j >= 0)
        {
            comparacoes++;
            if (strcmp(componentes[j].tipo, chave.tipo) > 0)
            {
                componentes[j + 1] = componentes[j];
                j--;
            }
            else
            {
                break;
            }
        }

        componentes[j + 1] = chave;
    }

    return comparacoes;
}

/* Selection Sort por prioridade */
long selectionSortPrioridade(Componente componentes[], int n)
{
    long comparacoes = 0;
    int i, j, menor;

    for (i = 0; i < n - 1; i++)
    {
        menor = i;

        for (j = i + 1; j < n; j++)
        {
            comparacoes++;
            if (componentes[j].prioridade < componentes[menor].prioridade)
            {
                menor = j;
            }
        }

        if (menor != i)
        {
            trocarComponentes(&componentes[i], &componentes[menor]);
        }
    }

    return comparacoes;
}

/* Busca binária por nome - válida somente se o vetor estiver ordenado por nome */
int buscaBinariaPorNome(Componente componentes[], int n, char nomeBuscado[], long *comparacoesBusca)
{
    int inicio = 0;
    int fim = n - 1;
    int meio;
    int resultado;

    *comparacoesBusca = 0;

    while (inicio <= fim)
    {
        meio = (inicio + fim) / 2;
        (*comparacoesBusca)++;

        resultado = strcmp(componentes[meio].nome, nomeBuscado);

        if (resultado == 0)
        {
            return meio;
        }
        else if (resultado < 0)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }
    }

    return -1;
}

/* Mede o tempo de execução de um algoritmo de ordenação */
double medirTempo(long (*algoritmo)(Componente[], int), Componente componentes[], int n, long *comparacoes)
{
    clock_t inicio, fim;

    inicio = clock();
    *comparacoes = algoritmo(componentes, n);
    fim = clock();

    return ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

/* Cadastra componentes */
void cadastrarComponentes(Componente componentes[], int *n)
{
    int i;
    int quantidade;
    char buffer[100];

    printf("\nQuantos componentes deseja cadastrar? (1 a %d): ", MAX_COMPONENTES);
    if (scanf("%d", &quantidade) != 1)
    {
        printf("Entrada invalida.\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    if (quantidade < 1 || quantidade > MAX_COMPONENTES)
    {
        printf("Quantidade invalida. Informe um valor entre 1 e %d.\n", MAX_COMPONENTES);
        return;
    }

    for (i = 0; i < quantidade; i++)
    {
        printf("\n--- Componente %d ---\n", i + 1);

        printf("Nome: ");
        fgets(componentes[i].nome, TAM_NOME, stdin);
        removerQuebraLinha(componentes[i].nome);

        printf("Tipo: ");
        fgets(componentes[i].tipo, TAM_TIPO, stdin);
        removerQuebraLinha(componentes[i].tipo);

        do
        {
            printf("Prioridade (1 a 10): ");
            fgets(buffer, sizeof(buffer), stdin);
            componentes[i].prioridade = atoi(buffer);

            if (componentes[i].prioridade < 1 || componentes[i].prioridade > 10)
            {
                printf("Prioridade invalida. Tente novamente.\n");
            }

        } while (componentes[i].prioridade < 1 || componentes[i].prioridade > 10);
    }

    *n = quantidade;
    printf("\nCadastro concluido com sucesso.\n");
}

/* Copia vetor de componentes */
void copiarComponentes(Componente destino[], Componente origem[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        destino[i] = origem[i];
    }
}

int main()
{
    Componente componentes[MAX_COMPONENTES];
    Componente copiaOrdenacao[MAX_COMPONENTES];

    int n = 0;
    int opcao;
    int ordenadoPorNome = 0;
    long comparacoes = 0;
    long comparacoesBusca = 0;
    double tempo = 0.0;
    int indiceEncontrado;

    char nomeBuscado[TAM_NOME];

    do
    {
        printf("\n================ MENU PRINCIPAL ================\n");
        printf("1. Cadastrar componentes\n");
        printf("2. Mostrar componentes cadastrados\n");
        printf("3. Ordenar por nome (Bubble Sort)\n");
        printf("4. Ordenar por tipo (Insertion Sort)\n");
        printf("5. Ordenar por prioridade (Selection Sort)\n");
        printf("6. Buscar componente-chave por nome (Busca Binaria)\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1)
        {
            printf("Entrada invalida.\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        switch (opcao)
        {
            case 1:
                cadastrarComponentes(componentes, &n);
                ordenadoPorNome = 0;
                break;

            case 2:
                mostrarComponentes(componentes, n);
                break;

            case 3:
                if (n == 0)
                {
                    printf("\nCadastre componentes antes de ordenar.\n");
                    break;
                }

                copiarComponentes(copiaOrdenacao, componentes, n);
                tempo = medirTempo(bubbleSortNome, copiaOrdenacao, n, &comparacoes);
                copiarComponentes(componentes, copiaOrdenacao, n);
                ordenadoPorNome = 1;

                printf("\nOrdenacao por nome concluida com sucesso.\n");
                printf("Comparacoes realizadas: %ld\n", comparacoes);
                printf("Tempo de execucao: %.8f segundos\n", tempo);
                mostrarComponentes(componentes, n);
                break;

            case 4:
                if (n == 0)
                {
                    printf("\nCadastre componentes antes de ordenar.\n");
                    break;
                }

                copiarComponentes(copiaOrdenacao, componentes, n);
                tempo = medirTempo(insertionSortTipo, copiaOrdenacao, n, &comparacoes);
                copiarComponentes(componentes, copiaOrdenacao, n);
                ordenadoPorNome = 0;

                printf("\nOrdenacao por tipo concluida com sucesso.\n");
                printf("Comparacoes realizadas: %ld\n", comparacoes);
                printf("Tempo de execucao: %.8f segundos\n", tempo);
                mostrarComponentes(componentes, n);
                break;

            case 5:
                if (n == 0)
                {
                    printf("\nCadastre componentes antes de ordenar.\n");
                    break;
                }

                copiarComponentes(copiaOrdenacao, componentes, n);
                tempo = medirTempo(selectionSortPrioridade, copiaOrdenacao, n, &comparacoes);
                copiarComponentes(componentes, copiaOrdenacao, n);
                ordenadoPorNome = 0;

                printf("\nOrdenacao por prioridade concluida com sucesso.\n");
                printf("Comparacoes realizadas: %ld\n", comparacoes);
                printf("Tempo de execucao: %.8f segundos\n", tempo);
                mostrarComponentes(componentes, n);
                break;

            case 6:
                if (n == 0)
                {
                    printf("\nCadastre componentes antes de realizar busca.\n");
                    break;
                }

                if (!ordenadoPorNome)
                {
                    printf("\nA busca binaria so pode ser feita apos ordenar por nome.\n");
                    printf("Use primeiro a opcao 3.\n");
                    break;
                }

                printf("\nDigite o nome do componente-chave: ");
                fgets(nomeBuscado, TAM_NOME, stdin);
                removerQuebraLinha(nomeBuscado);

                indiceEncontrado = buscaBinariaPorNome(componentes, n, nomeBuscado, &comparacoesBusca);

                if (indiceEncontrado != -1)
                {
                    printf("\nComponente-chave encontrado!\n");
                    printf("Posicao no vetor: %d\n", indiceEncontrado + 1);
                    printf("Nome: %s\n", componentes[indiceEncontrado].nome);
                    printf("Tipo: %s\n", componentes[indiceEncontrado].tipo);
                    printf("Prioridade: %d\n", componentes[indiceEncontrado].prioridade);
                    printf("Comparacoes na busca: %ld\n", comparacoesBusca);
                    printf("\n>>> Torre de fuga pronta para ativacao! <<<\n");
                }
                else
                {
                    printf("\nComponente-chave nao encontrado.\n");
                    printf("Comparacoes na busca: %ld\n", comparacoesBusca);
                }
                break;

            case 0:
                printf("\nEncerrando o sistema...\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}