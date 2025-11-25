#include "escalonador.hpp"
#include <iostream>

/*
 * Construtor do Escalonador.
 * - Inicializa o MinHeap usado para ordenar eventos por tempo.
 * - Aloca o vetor dinâmico de eventos.
 * - Aloca o vetor de corridas (somente para liberar no destrutor).
 * - Define valor inicial do relógio e parâmetro gama (velocidade).
 */
Escalonador::Escalonador(int capacidadeInicialCorridas, double gama_)
    : heap(capacidadeInicialCorridas),
      eventos(nullptr),
      qtdEventos(0),
      capacidadeEventos(capacidadeInicialCorridas * 2),
      corridas(nullptr),
      qtdCorridas(0),
      capacidadeCorridas(capacidadeInicialCorridas),
      relogio(0.0),
      gama(gama_) {

    if (capacidadeEventos > 0) {
        eventos = new Evento[capacidadeEventos];
    }
    if (capacidadeCorridas > 0) {
        corridas = new Corrida*[capacidadeCorridas];
    }
}

/*
 * Destrutor:
 * Libera todas as corridas armazenadas e os vetores dinâmicos internos.
 */
Escalonador::~Escalonador() {
    if (corridas) {
        for (int i = 0; i < qtdCorridas; ++i) {
            delete corridas[i];
        }
    }
    delete[] corridas;
    delete[] eventos;
}

/*
 * Adiciona um evento ao escalonador:
 * - Realoca vetor se necessário.
 * - Coloca o evento no vetor e insere sua chave (tempo) no MinHeap.
 * - Retorna o índice onde o evento foi armazenado.
 */
int Escalonador::adicionarEvento(const Evento& e) {
    if (qtdEventos == capacidadeEventos) {
        int novaCap = (capacidadeEventos == 0 ? 1 : capacidadeEventos * 2);
        Evento* novoVet = new Evento[novaCap];
        for (int i = 0; i < qtdEventos; ++i) {
            novoVet[i] = eventos[i];
        }
        delete[] eventos;
        eventos = novoVet;
        capacidadeEventos = novaCap;
    }

    int idx = qtdEventos;
    eventos[qtdEventos++] = e;
    heap.inserir(e.tempo, idx);
    return idx;
}

/*
 * Registra uma corrida no escalonador e agenda seu primeiro evento.
 * - Armazena a corrida apenas para fins de desalocação posterior.
 * - Define o tempo de início como o tempo da primeira demanda.
 * - Agenda o primeiro evento de parada (primeiro embarque).
 */
void Escalonador::adicionarCorrida(Corrida* corrida) {
    // Armazena corrida
    if (qtdCorridas == capacidadeCorridas) {
        int novaCap = (capacidadeCorridas == 0 ? 1 : capacidadeCorridas * 2);
        Corrida** novoVet = new Corrida*[novaCap];
        for (int i = 0; i < qtdCorridas; ++i) {
            novoVet[i] = corridas[i];
        }
        delete[] corridas;
        corridas = novoVet;
        capacidadeCorridas = novaCap;
    }
    corridas[qtdCorridas++] = corrida;

    // Primeira parada ocorre no tempo da solicitação da demanda 0
    Demanda* d0 = corrida->getDemanda(0);
    double t0 = d0->getTempoSolicitacao();
    corrida->setTempoInicio(t0);

    // Agenda evento para a parada 0
    Evento e;
    e.tempo = t0;
    e.tipo = EVENTO_PARADA;
    e.corrida = corrida;
    e.indiceParada = 0;

    adicionarEvento(e);
}

/*
 * Executa a simulação:
 * - Remove sempre o próximo evento do MinHeap (menor tempo).
 * - Atualiza o relógio.
 * - Para eventos de parada:
 *      - Se houver próxima parada, agenda deslocamento até ela.
 *      - Caso contrário, fecha a corrida, define tempo final
 *        e marca todas as demandas como concluídas.
 * - Ao finalizar a corrida, imprime a saída no formato solicitado.
 */
void Escalonador::simularEImprimir() {
    double tempo;
    int idxEvento;

    // Processa eventos em ordem temporal crescente
    while (heap.removerMinimo(tempo, idxEvento)) {
        relogio = tempo;

        Evento& ev = eventos[idxEvento];
        Corrida* c = ev.corrida;

        if (ev.tipo == EVENTO_PARADA) {
            int idxP = ev.indiceParada;
            int numParadas = c->getNumeroParadas();

            // Se existe próxima parada, agenda deslocamento até ela
            if (idxP + 1 < numParadas) {
                Trecho* trecho = c->getTrecho(idxP);
                double dist = trecho->getDistancia();
                double deltaT = (gama > 0.0) ? (dist / gama) : 0.0;

                Evento prox;
                prox.tempo = tempo + deltaT;
                prox.tipo = EVENTO_PARADA;
                prox.corrida = c;
                prox.indiceParada = idxP + 1;

                adicionarEvento(prox);
            } else {
                // Última parada: fim da corrida
                c->setTempoFim(tempo);

                // Marca todas as demandas da corrida como concluídas
                int qd = c->getQtdDemandas();
                for (int i = 0; i < qd; ++i) {
                    Demanda* d = c->getDemanda(i);
                    d->setEstado(DEMANDA_CONCLUIDA);
                }
                
                c->imprimirSaida(std::cout);
            }
        }
    }
}
