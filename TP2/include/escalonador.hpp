#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "minheap.hpp"
#include "corrida.hpp"

// Tipos de evento da simulação
enum TipoEvento {
    EVENTO_PARADA = 0
    // Poderíamos ter EVENTO_FIM, mas o último EVENTO_PARADA já é o fim.
};

struct Evento {
    double tempo;
    TipoEvento tipo;
    Corrida* corrida;
    int indiceParada; // índice da parada na corrida
};

class Escalonador {
private:
    MinHeap heap;      // min-heap de eventos (chave = tempo, valor = índice em eventos[])
    Evento* eventos;
    int qtdEventos;
    int capacidadeEventos;

    Corrida** corridas;   // só para gerenciar memória das corridas
    int qtdCorridas;
    int capacidadeCorridas;

    double relogio;
    double gama;          // velocidade (mesma para todos os veículos)

    int adicionarEvento(const Evento& e); // retorna índice do evento no array

public:
    Escalonador(int capacidadeInicialCorridas, double gama_);
    ~Escalonador();

    void adicionarCorrida(Corrida* corrida);
    void simularEImprimir();
};

#endif // ESCALONADOR_HPP
