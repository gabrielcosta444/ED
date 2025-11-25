#include "minheap.hpp"

/*
 * Construtor padrão.
 * Cria uma heap vazia sem alocar memória inicial.
 * A capacidade será alocada sob demanda (lazy allocation).
 */
MinHeap::MinHeap()
    : chaves(nullptr),
      valores(nullptr),
      tamanho(0),
      capacidade(0) {}

/*
 * Construtor com capacidade inicial.
 * Aloca vetores de chaves (double) e valores (int),
 * utilizados nos nós da heap.
 */
MinHeap::MinHeap(int capacidadeInicial)
    : chaves(nullptr),
      valores(nullptr),
      tamanho(0),
      capacidade(capacidadeInicial) {
    if (capacidadeInicial > 0) {
        chaves = new double[capacidadeInicial];
        valores = new int[capacidadeInicial];
    }
}

/*
 * Destrutor.
 * Libera os vetores dinâmicos usados pela heap.
 */
MinHeap::~MinHeap() {
    delete[] chaves;
    delete[] valores;
}

/*
 * Retorna true se não há elementos armazenados.
 */
bool MinHeap::vazio() const {
    return tamanho == 0;
}

/*
 * Retorna o número de elementos atualmente na heap.
 */
int MinHeap::getTamanho() const {
    return tamanho;
}

/*
 * Troca a posição de dois elementos da heap.
 * Operação auxiliar utilizada em subir() e descer().
 */
void MinHeap::trocar(int i, int j) {
    double ck = chaves[i];
    chaves[i] = chaves[j];
    chaves[j] = ck;

    int cv = valores[i];
    valores[i] = valores[j];
    valores[j] = cv;
}

/*
 * Sobe um elemento recém-inserido até restaurar
 * a propriedade de min-heap.
 */
void MinHeap::subir(int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (chaves[i] < chaves[pai]) {
            trocar(i, pai);
            i = pai;
        } else {
            break;
        }
    }
}

/*
 * Desce o elemento na raiz (ou após remoção)
 * até que a propriedade de min-heap seja restaurada.
 */
void MinHeap::descer(int i) {
    while (true) {
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;
        int menor = i;

        if (esq < tamanho && chaves[esq] < chaves[menor]) {
            menor = esq;
        }
        if (dir < tamanho && chaves[dir] < chaves[menor]) {
            menor = dir;
        }
        if (menor != i) {
            trocar(i, menor);
            i = menor;
        } else {
            break;
        }
    }
}

/*
 * Insere um novo par (chave, valor) na heap.
 * Se necessário, realoca os vetores dobrando a capacidade.
 */
void MinHeap::inserir(double chave, int valor) {
    // realocação quando a capacidade está cheia
    if (tamanho == capacidade) {
        int novaCap = (capacidade == 0 ? 1 : capacidade * 2);
        double* novasChaves = new double[novaCap];
        int* novosValores = new int[novaCap];

        for (int i = 0; i < tamanho; ++i) {
            novasChaves[i] = chaves[i];
            novosValores[i] = valores[i];
        }

        delete[] chaves;
        delete[] valores;
        chaves = novasChaves;
        valores = novosValores;
        capacidade = novaCap;
    }

    // insere no final e faz subir
    int i = tamanho++;
    chaves[i] = chave;
    valores[i] = valor;
    subir(i);
}

/*
 * Obtém o mínimo sem removê-lo.
 * Retorna false se a heap estiver vazia.
 */
bool MinHeap::obterMinimo(double& chave, int& valor) const {
    if (tamanho == 0) return false;
    chave = chaves[0];
    valor = valores[0];
    return true;
}

/*
 * Remove o elemento mínimo (raiz).
 * Move o último elemento para a raiz e aplica descer().
 */
bool MinHeap::removerMinimo(double& chave, int& valor) {
    if (tamanho == 0) return false;

    chave = chaves[0];
    valor = valores[0];

    // substitui topo pelo último elemento
    chaves[0] = chaves[tamanho - 1];
    valores[0] = valores[tamanho - 1];
    --tamanho;

    if (tamanho > 0) {
        descer(0);
    }
    return true;
}

/*
 * Esvazia a heap sem liberar memória,
 * permitindo reaproveitar a capacidade atual.
 */
void MinHeap::limpar() {
    tamanho = 0;
}
