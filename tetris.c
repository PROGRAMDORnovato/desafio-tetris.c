#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// -------------------------------------------
// Definição da struct que representa uma peça
// -------------------------------------------
typedef struct {
    char nome; // tipo da peça (I, O, T, L)
    int id;    // identificador único
} Peca;

// -------------------------------------------
// Fila circular (capacidade fixa: 5 peças)
// -------------------------------------------
#define MAX_FILA 5

typedef struct {
    Peca itens[MAX_FILA];
    int frente, tras, tamanho;
} Fila;

// Inicializa a fila circular
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->tamanho = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->tamanho == MAX_FILA;
}

// Verifica se está vazia
int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

// Insere peça no fim da fila
void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->itens[f->tras] = p;
    f->tras = (f->tras + 1) % MAX_FILA;
    f->tamanho++;
}

// Remove peça da frente da fila
Peca dequeue(Fila *f) {
    Peca removida = {'-', -1};
    if (filaVazia(f)) return removida;

    removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->tamanho--;
    return removida;
}

// Acessa a peça da frente sem remover
Peca frenteFila(Fila *f) {
    return f->itens[f->frente];
}

// -------------------------------------------
// Pilha de peças reservadas (capacidade: 3)
// -------------------------------------------
#define MAX_PILHA 3

typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

// Verifica se está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Empilha uma peça
void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return;
    p->itens[++p->topo] = x;
}

// Desempilha
Peca pop(Pilha *p) {
    if (pilhaVazia(p)) {
        Peca vazia = {'-', -1};
        return vazia;
    }
    return p->itens[p->topo--];
}

// Consulta topo
Peca topo(Pilha *p) {
    return p->itens[p->topo];
}

// -------------------------------------------
// Função que gera uma nova peça automaticamente
// -------------------------------------------
char tipos[] = {'I', 'O', 'T', 'L'};
int geradorID = 0;

Peca gerarPeca() {
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = geradorID++;
    return nova;
}

// -------------------------------------------
// Exibe estado atual da fila e pilha
// -------------------------------------------
void exibirEstado(Fila *f, Pilha *p) {
    printf("\nFila de pecas: ");
    int idx = f->frente;
    for (int i = 0; i < f->tamanho; i++) {
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }

    printf("\nPilha reserva (Topo -> Base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n");
}

// -------------------------------------------
// Menu com regras do jogo
// -------------------------------------------
void mostrarRegras() {
    printf("\n===== REGRAS DO TETRIS STACK =====\n");
    printf("1) A fila sempre tem 5 pecas.\n");
    printf("2) A pilha de reserva guarda ate 3 pecas.\n");
    printf("3) Acoes possiveis:\n");
    printf("   - Jogar a peca da frente da fila.\n");
    printf("   - Reservar: mover a peca da fila para a pilha.\n");
    printf("   - Usar peca reservada: remover do topo da pilha.\n");
    printf("   - Trocar peca atual: frente da fila ↔ topo da pilha.\n");
    printf("   - Troca em bloco: 3 da fila ↔ 3 da pilha.\n");
    printf("4) Sempre que uma peca e usada ou enviada para a pilha,\n");
    printf("   uma nova peca e criada automaticamente.\n");
    printf("5) Pecas usadas nao retornam mais ao jogo.\n\n");
}

// -------------------------------------------
// Troca peça da frente da fila com topo da pilha
// -------------------------------------------
void trocarUma(Fila *f, Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Nao ha peca na pilha para trocar!\n");
        return;
    }

    Peca temp = frenteFila(f);
    f->itens[f->frente] = topo(p);
    p->itens[p->topo] = temp;

    printf("Troca realizada entre frente da fila e topo da pilha.\n");
}

// -------------------------------------------
// Troca múltipla: 3 primeiros da fila com 3 da pilha
// -------------------------------------------
void trocarTres(Fila *f, Pilha *p) {
    if (p->topo < 2) {
        printf("A pilha nao possui 3 pecas!\n");
        return;
    }
    if (f->tamanho < 3) {
        printf("A fila nao possui 3 pecas!\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idxFila = (f->frente + i) % MAX_FILA;
        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }

    printf("Troca de bloco (3x3) realizada!\n");
}

// -------------------------------------------
// PROGRAMA PRINCIPAL
// -------------------------------------------
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial
    for (int i = 0; i < MAX_FILA; i++)
        enqueue(&fila, gerarPeca());

    int opc;

    // ---------------------------
    // Tela de boas-vindas
    // ---------------------------
    printf("=====================================\n");
    printf("  BEM-VINDO AO TETRIS STACK DELUXE!\n");
    printf("=====================================\n");
    printf("1 - Continuar para o jogo\n");
    printf("2 - Ver regras\n");
    printf("Escolha: ");
    scanf("%d", &opc);

    if (opc == 2) {
        mostrarRegras();
    }

    do {
        printf("\n========== MENU PRINCIPAL ==========\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca (fila -> pilha)\n");
        printf("3 - Usar peca reservada (pilha)\n");
        printf("4 - Trocar peca atual (fila ↔ pilha)\n");
        printf("5 - Trocar 3 pecas (fila ↔ pilha)\n");
        printf("6 - Ver estado atual\n");
        printf("7 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1: {
                Peca removida = dequeue(&fila);
                if (removida.id != -1)
                    printf("Peca jogada: [%c %d]\n", removida.nome, removida.id);

                enqueue(&fila, gerarPeca());
                break;
            }

            case 2: {
                if (pilhaCheia(&pilha)) {
                    printf("Pilha cheia! Nao e possivel reservar.\n");
                    break;
                }
                Peca removida = dequeue(&fila);
                push(&pilha, removida);
                printf("Peca [%c %d] reservada.\n", removida.nome, removida.id);

                enqueue(&fila, gerarPeca());
                break;
            }

            case 3: {
                if (pilhaVazia(&pilha)) {
                    printf("Pilha vazia! Nao ha peca para usar.\n");
                    break;
                }
                Peca usada = pop(&pilha);
                printf("Peca usada: [%c %d]\n", usada.nome, usada.id);
                break;
            }

            case 4:
                trocarUma(&fila, &pilha);
                break;

            case 5:
                trocarTres(&fila, &pilha);
                break;

            case 6:
                exibirEstado(&fila, &pilha);
                break;

            case 7:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opc != 7);

    return 0;
}
