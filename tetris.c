#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ----------------------- ESTRUTURA DA PEÇA -----------------------
typedef struct {
    char nome;
    int id;
} Peca;


// ------------------------- FILA CIRCULAR --------------------------
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->quantidade = 0;
}

void enqueue(Fila *f, Peca p) {
    if (f->quantidade == TAM_FILA) return;
    f->itens[f->tras] = p;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->quantidade++;
}

Peca dequeue(Fila *f) {
    if (f->quantidade == 0) {
        Peca vazio = {'-', -1};
        return vazio;
    }
    Peca p = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->quantidade--;
    return p;
}

Peca acessarFrente(Fila *f) {
    if (f->quantidade == 0) {
        Peca vazio = {'-', -1};
        return vazio;
    }
    return f->itens[f->frente];
}


// --------------------------- PILHA ------------------------------
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) { return p->topo == -1; }
int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }

void push(Pilha *p, Peca x) {
    if (!pilhaCheia(p)) p->itens[++p->topo] = x;
}

Peca pop(Pilha *p) {
    if (pilhaVazia(p)) {
        Peca vazio = {'-', -1};
        return vazio;
    }
    return p->itens[p->topo--];
}

Peca topoPilha(Pilha *p) {
    if (pilhaVazia(p)) {
        Peca v = {'-', -1};
        return v;
    }
    return p->itens[p->topo];
}


// ------------------------ GERAR PEÇA -----------------------------
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}


// ------------------------- EXIBIR ESTADO -------------------------
void exibirFila(Fila *f) {
    printf("\nFila de peças:\n");
    int i = f->frente;
    for (int c = 0; c < f->quantidade; c++) {
        printf("[%c %d] ", f->itens[i].nome, f->itens[i].id);
        i = (i + 1) % TAM_FILA;
    }
    printf("\n");
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base):\n");
    if (pilhaVazia(p)) {
        printf("(vazia)\n\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n\n");
}


// ------------------------ TROCA SIMPLES --------------------------
void trocarFrenteComTopo(Fila *f, Pilha *p) {
    if (f->quantidade == 0 || pilhaVazia(p)) {
        printf("Troca impossível! Fila ou pilha vazia.\n");
        return;
    }

    int idx = f->frente;

    Peca temp = f->itens[idx];
    f->itens[idx] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("Ação: troca feita entre a peça da frente da fila e o topo da pilha.\n");
}


// ------------------------ TROCA MÚLTIPLA -------------------------
void trocarTres(Fila *f, Pilha *p) {
    if (f->quantidade < 3 || p->topo < 2) {
        printf("Troca múltipla impossível! São necessárias 3 peças em ambos.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int posFila = (f->frente + i) % TAM_FILA;
        int posPilha = p->topo - i;

        Peca temp = f->itens[posFila];
        f->itens[posFila] = p->itens[posPilha];
        p->itens[posPilha] = temp;
    }

    printf("Ação: troca realizada entre os 3 primeiros da fila e os 3 da pilha!\n");
}


// ------------------------------ MAIN ------------------------------
int main() {
    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);
    srand(time(NULL));

    int idAtual = 0;

    // Fila sempre inicia cheia
    for (int i = 0; i < TAM_FILA; i++)
        enqueue(&fila, gerarPeca(idAtual++));

    int opcao;

    do {
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("Opções:\n");
        printf("1 - Jogar peça da fila\n");
        printf("2 - Reservar peça da fila para a pilha\n");
        printf("3 - Usar peça da pilha\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Trocar 3 da fila com 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = dequeue(&fila);
                printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                enqueue(&fila, gerarPeca(idAtual++));
            } break;

            case 2: {
                if (!pilhaCheia(&pilha)) {
                    Peca p = dequeue(&fila);
                    printf("Peça reservada: [%c %d]\n", p.nome, p.id);
                    push(&pilha, p);
                    enqueue(&fila, gerarPeca(idAtual++));
                } else {
                    printf("A pilha está cheia! Não é possível reservar.\n");
                }
            } break;

            case 3: {
                Peca usada = pop(&pilha);
                if (usada.id != -1)
                    printf("Peça usada: [%c %d]\n", usada.nome, usada.id);
                enqueue(&fila, gerarPeca(idAtual++));
            } break;

            case 4:
                trocarFrenteComTopo(&fila, &pilha);
                break;

            case 5:
                trocarTres(&fila, &pilha);
                break;

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}

