// NOME: KARINA DE OLIVEIRA KRISAN RA: 3020100204
// NOME: GUILHERME KERTES DA COSTA RA: 3022103487
// NOME: GABRIEL VINICIUS VIEIRA POSTBIEGEL RA: 3021104392


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int id;
    float preco;
    char nome[50];
    char origem[3];
    char destino[3];
} Produto;

typedef struct {
    Produto produtos[10];
} Palete;

typedef struct {
    char cidade[50];
} Ordem;

typedef struct {
    int numero;
    Palete paletes[10];
    Ordem ordem;
    int tempo_carga_descarga;
    float valor_total;
    int tempo_inspecao_produto;
    int tempo_total_doca;
} Caminhao;

#define TAMANHO_FILA 10

typedef struct {
    Caminhao elementos[TAMANHO_FILA];
    int frente, tras;
} Fila;

typedef struct {
    Produto elementos[10];
    int topo;
} Pilha;

void inicializarFila(Fila *fila) {
    fila->frente = -1;
    fila->tras = -1;
}

int estaVazia(Fila *fila) {
    return (fila->frente == -1);
}

int estaCheia(Fila *fila) {
    return ((fila->tras + 1) % TAMANHO_FILA == fila->frente);
}

void enfileirar(Fila *fila, Caminhao caminhao) {
    if (!estaCheia(fila)) {
        if (estaVazia(fila)) {
            fila->frente = 0;
        }
        fila->tras = (fila->tras + 1) % TAMANHO_FILA;
        fila->elementos[fila->tras] = caminhao;
    } else {
        printf("A fila está cheia. Não é possível enfileirar mais caminhões.\n");
    }
}

Caminhao desenfileirar(Fila *fila) {
    Caminhao caminhao;
    if (!estaVazia(fila)) {
        caminhao = fila->elementos[fila->frente];
        if (fila->frente == fila->tras) {
            fila->frente = -1;
            fila->tras = -1;
        } else {
            fila->frente = (fila->frente + 1) % TAMANHO_FILA;
        }
        return caminhao;
    } else {
        printf("A fila está vazia. Não é possível desenfileirar caminhões.\n");
        Caminhao caminhaoVazio = {0};
        return caminhaoVazio;
    }
}

int compararPrecos(const void *a, const void *b) {
    Produto *produtoA = (Produto *)a;
    Produto *produtoB = (Produto *)b;
    if (produtoA->preco < produtoB->preco) {
        return -1;
    } else if (produtoA->preco > produtoB->preco) {
        return 1;
    } else {
        return 0;
    }
}

Produto *encontrarProdutoPorNome(Caminhao caminhao, const char *nome) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (strcmp(caminhao.paletes[i].produtos[j].nome, nome) == 0) {
                return &caminhao.paletes[i].produtos[j];
            }
        }
    }
    return NULL;
}

#define TAMANHO_PILHA 10

void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int estaVaziaPilha(Pilha *pilha) {
    return (pilha->topo == -1);
}

int estaCheiaPilha(Pilha *pilha) {
    return (pilha->topo == TAMANHO_PILHA - 1);
}

void empilhar(Pilha *pilha, Produto produto) {
    if (!estaCheiaPilha(pilha)) {
        pilha->topo++;
        pilha->elementos[pilha->topo] = produto;
    } else {
        printf("A pilha está cheia. Não é possível empilhar mais produtos.\n");
    }
}

Produto desempilhar(Pilha *pilha) {
    Produto produto;
    if (!estaVaziaPilha(pilha)) {
        produto = pilha->elementos[pilha->topo];
        pilha->topo--;
        return produto;
    } else {
        printf("A pilha está vazia. Não é possível desempilhar produtos.\n");
        Produto produtoVazio = {0};
        return produtoVazio;
    }
}

void logCaminhaoDesenfileirado(FILE *logFile, int numero) {
    fprintf(logFile, "Caminhão desenfileirado: %d\n", numero);
}

Produto gerarProduto(int id, char cidades[5][3]) {
    Produto produto;
    produto.id = id; // Garantindo ID único

    float min_price = 10.0;
    float max_price = 1000.0;
    float price_range = max_price - min_price;
    float step = price_range / 10.0;
    produto.preco = min_price + step * (rand() % 10);

    int cidadeOrigemIndex = rand() % 5;
    int cidadeDestinoIndex;

    do {
        cidadeDestinoIndex = rand() % 5;
    } while (cidadeDestinoIndex == cidadeOrigemIndex);

    char nomesProdutos[10][20] = {"TABLET", "OVERBOARD", "CELULAR", "RELOGIO", "FONE DE OUVIDO", "CARREGADOR", "MEMORIA RAM", "NOTEBOOK", "PLACA DE VIDEO", "GABINETE"};
    int nomeIndex = rand() % 10;
    strcpy(produto.nome, nomesProdutos[nomeIndex]);
    strcpy(produto.origem, cidades[cidadeOrigemIndex]);
    strcpy(produto.destino, cidades[cidadeDestinoIndex]);

    return produto;
}

void preencherPalete(Palete *paletes, char cidades[5][3]) {
    for (int i = 0; i < 10; i++) {
        paletes->produtos[i] = gerarProduto(i + 1, cidades);
    }
}

Ordem gerarOrdem(char cidades[5][3]) {
    Ordem ordem;
    int cidadeIndex = rand() % 5;
    strcpy(ordem.cidade, cidades[cidadeIndex]);
    return ordem;
}

void criarLog(Caminhao caminhoes[10]) {
    FILE *logFile = fopen("log.txt", "w");

    if (logFile == NULL) {
        printf("Erro ao abrir o arquivo de log.\n");
        return;
    }

    fprintf(logFile, "Log de Dados:\n");

    for (int i = 0; i < 10; i++) {
        fprintf(logFile, "Caminhão %d\n", caminhoes[i].numero);
        fprintf(logFile, "Tempo de carga/descarga: %d minutos\n", caminhoes[i].tempo_carga_descarga);
        fprintf(logFile, "Tempo de inspecao do produto: %d minutos\n", caminhoes[i].tempo_inspecao_produto);
        fprintf(logFile, "Tempo total na doca: %d minutos\n", caminhoes[i].tempo_total_doca);

        for (int j = 0; j < 10; j++) {
            for (int k = 0; k < 10; k++) {
                fprintf(logFile, "Produtos: %s\n", caminhoes[i].paletes[j].produtos[k].nome);
                fprintf(logFile, "ID: %d\n", caminhoes[i].paletes[j].produtos[k].id);
                fprintf(logFile, "Preço: R$%.2f\n", caminhoes[i].paletes[j].produtos[k].preco);
                fprintf(logFile, "Origem: %s\n", caminhoes[i].paletes[j].produtos[k].origem);
                fprintf(logFile, "Destino: %s\n", caminhoes[i].paletes[j].produtos[k].destino);
                fprintf(logFile, "Valor do Produto a ser Inspecionado: R$%.2f\n", caminhoes[i].paletes[j].produtos[k].preco);
                fprintf(logFile, "\n");
            }
        }
        logCaminhaoDesenfileirado(logFile, caminhoes[i].numero);
    }

    fclose(logFile);
    printf("Log de dados gerado com sucesso (arquivo 'log.txt').\n");
}

// Função de comparação para qsort
int compararCaminhoes(const void *a, const void *b) {
    Caminhao *caminhaoA = (Caminhao *)a;
    Caminhao *caminhaoB = (Caminhao *)b;
    if (caminhaoA->valor_total < caminhaoB->valor_total) {
        return -1;
    } else if (caminhaoA->valor_total > caminhaoB->valor_total) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    srand(time(NULL));

    char cidades[5][3] = {"SP", "RJ", "SC", "MG", "DF"};

    Caminhao caminhoes[10];
    int tempo_total_dia = 600;

    for (int i = 0; i < 10; i++) {
        caminhoes[i].numero = i + 1;
        caminhoes[i].ordem = gerarOrdem(cidades);

        for (int j = 0; j < 10; j++) {
            preencherPalete(&caminhoes[i].paletes[j], cidades);
            caminhoes[i].tempo_carga_descarga = rand() % tempo_total_dia;
            caminhoes[i].valor_total += caminhoes[i].paletes[j].produtos[0].preco;
            caminhoes[i].tempo_inspecao_produto = rand() % tempo_total_dia;
            caminhoes[i].tempo_total_doca += caminhoes[i].tempo_carga_descarga;

            // Implementação da lógica de horas extras
            if (caminhoes[i].tempo_carga_descarga > tempo_total_dia) {
                int horasExtras = caminhoes[i].tempo_carga_descarga - tempo_total_dia;
                printf("Caminhão %d realizou %d minutos extras de carga/descarga.\n", caminhoes[i].numero, horasExtras);
            }
        }
    }

    // Ajustar IDs específicos
    for (int i = 0; i < 10; i++) {
        caminhoes[i].paletes[0].produtos[0].id = 1;   // Tablet
        caminhoes[i].paletes[0].produtos[2].id = 3;   // Memoria Ram
    }

    // Ordenar os caminhões por valor total transportado
    qsort(caminhoes, 10, sizeof(Caminhao), compararCaminhoes);

    // Imprimir os caminhões ordenados
    printf("Caminhões ordenados por valor total transportado:\n");
    for (int i = 0; i < 10; i++) {
        printf("Caminhão %d - Valor Total Transportado: R$%.2f\n", caminhoes[i].numero, caminhoes[i].valor_total);
    }

    criarLog(caminhoes);

    Fila fila;
    inicializarFila(&fila);

    for (int i = 0; i < 10; i++) {
        enfileirar(&fila, caminhoes[i]);
    }

    Caminhao caminhaoDesenfileirado = desenfileirar(&fila);
    printf("Caminhão desenfileirado: %d\n", caminhaoDesenfileirado.numero);

    // Encontrar caminhão que passou mais tempo na doca
    int caminhaoMaisTempoDoca = 0;
    for (int i = 1; i < 10; i++) {
        if (caminhoes[i].tempo_total_doca > caminhoes[caminhaoMaisTempoDoca].tempo_total_doca) {
            caminhaoMaisTempoDoca = i;
        }
    }

    printf("Caminhão que passou mais tempo na doca: %d\n", caminhoes[caminhaoMaisTempoDoca].numero);

    // Identificar caminhões com produtos acima de R$ 900,00
    for (int i = 0; i < 10; i++) {
        int countProdutosAcima900 = 0;
        for (int j = 0; j < 10; j++) {
            for (int k = 0; k < 10; k++) {
                if (caminhoes[i].paletes[j].produtos[k].preco > 900.0) {
                    countProdutosAcima900++;
                }
            }
        }
        if (countProdutosAcima900 >= 10) {
            printf("Caminhão %d possui pelo menos 10 produtos acima de R$ 900,00.\n", caminhoes[i].numero);
        }
    }

    return 0;
}
