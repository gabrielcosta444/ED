#ifndef MINHEAP_HPP
#define MINHEAP_HPP

// Estrutura de MinHeap para gerenciar eventos em ordem crescente de tempo.
// Armazena pares (chave, valor), onde a chave é usada como critério de prioridade.
class MinHeap {
private:
    double* chaves;   // Vetor das chaves (prioridades)
    int* valores;     // Vetor dos valores associados a cada chave
    int tamanho;      // Quantidade atual de elementos
    int capacidade;   // Capacidade máxima alocada

    // Funções auxiliares para restaurar a propriedade de heap
    void subir(int i);       // Corrige a posição após inserção
    void descer(int i);      // Corrige a posição após remoção
    void trocar(int i, int j); // Troca dois elementos do heap

public:
    MinHeap();                         // Construtor padrão
    explicit MinHeap(int capacidadeInicial); // Construtor com capacidade definida
    ~MinHeap();                        // Destrutor

    bool vazio() const;                // Verifica se o heap está vazio
    int getTamanho() const;            // Retorna o número de elementos

    void inserir(double chave, int valor);  // Insere um novo par (chave, valor)
    bool obterMinimo(double& chave, int& valor) const; // Lê o menor elemento sem remover
    bool removerMinimo(double& chave, int& valor);      // Remove o elemento de menor chave

    void limpar();                     // Remove todos os elementos
};

#endif // MINHEAP_HPP
