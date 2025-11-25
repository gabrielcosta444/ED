#include "corrida.hpp"
#include <iostream>

// Construtor: inicializa vetores dinâmicos de demandas, paradas e trechos.
// Cada capacidade inicial é derivada do número de demandas previsto.
Corrida::Corrida(int capacidadeInicialDemandas)
    : demandas(nullptr),
      qtdDemandas(0),
      capacidadeDemandas(capacidadeInicialDemandas),
      paradas(nullptr),
      qtdParadas(0),
      capacidadeParadas(2 * capacidadeInicialDemandas),
      trechos(nullptr),
      qtdTrechos(0),
      capacidadeTrechos(2 * capacidadeInicialDemandas),
      distanciaTotal(0.0),
      tempoInicio(0.0),
      tempoFim(0.0) {

    if (capacidadeDemandas > 0) {
        demandas = new Demanda*[capacidadeDemandas];
    }
    if (capacidadeParadas > 0) {
        paradas = new Parada*[capacidadeParadas];
    }
    if (capacidadeTrechos > 0) {
        trechos = new Trecho*[capacidadeTrechos];
    }
}

// Destrutor: libera todas as estruturas alocadas dinamicamente
Corrida::~Corrida() {
    if (paradas) {
        for (int i = 0; i < qtdParadas; ++i) {
            delete paradas[i];
        }
    }
    if (trechos) {
        for (int i = 0; i < qtdTrechos; ++i) {
            delete trechos[i];
        }
    }
    delete[] demandas;
    delete[] paradas;
    delete[] trechos;
}

// Adiciona uma nova demanda ao vetor, expandindo a capacidade se necessário
void Corrida::adicionarDemanda(Demanda* d) {
    if (qtdDemandas == capacidadeDemandas) {
        int novaCap = (capacidadeDemandas == 0 ? 1 : capacidadeDemandas * 2);
        Demanda** novoVet = new Demanda*[novaCap];

        for (int i = 0; i < qtdDemandas; ++i) {
            novoVet[i] = demandas[i];
        }

        delete[] demandas;
        demandas = novoVet;
        capacidadeDemandas = novaCap;
    }

    demandas[qtdDemandas++] = d;
    d->setCorrida(this); // associa a demanda a esta corrida
}

// Retorna a quantidade de demandas da corrida
int Corrida::getQtdDemandas() const {
    return qtdDemandas;
}

// Retorna uma demanda pelo índice
Demanda* Corrida::getDemanda(int idx) const {
    if (idx < 0 || idx >= qtdDemandas) return nullptr;
    return demandas[idx];
}

// Constrói a rota básica: primeiro embarques, depois desembarques
void Corrida::construirRotaBasica() {
    // limpa paradas antigas
    for (int i = 0; i < qtdParadas; ++i) {
        delete paradas[i];
    }
    qtdParadas = 0;

    // inserção das paradas de embarque
    for (int i = 0; i < qtdDemandas; ++i) {
        Demanda* d = demandas[i];
        Parada* p = new Parada(d->getOrigem(), PARADA_EMBARQUE, d);
        paradas[qtdParadas++] = p;
    }

    // inserção das paradas de desembarque
    for (int i = 0; i < qtdDemandas; ++i) {
        Demanda* d = demandas[i];
        Parada* p = new Parada(d->getDestino(), PARADA_DESEMBARQUE, d);
        paradas[qtdParadas++] = p;
    }
}

// Cria os trechos consecutivos da rota e calcula a distância total
void Corrida::calcularTrechosEDistancia() {
    // limpa trechos antigos
    for (int i = 0; i < qtdTrechos; ++i) {
        delete trechos[i];
    }
    qtdTrechos = 0;
    distanciaTotal = 0.0;

    if (qtdParadas <= 1) return;

    // cria trechos entre cada par de paradas consecutivas
    for (int i = 0; i < qtdParadas - 1; ++i) {
        Trecho* t = new Trecho(paradas[i], paradas[i + 1], TRECHO_DESLOCAMENTO);
        trechos[qtdTrechos++] = t;
        distanciaTotal += t->getDistancia();
    }
}

// Define o tempo de início da corrida
void Corrida::setTempoInicio(double t) {
    tempoInicio = t;
}

// Define o tempo de término
void Corrida::setTempoFim(double t) {
    tempoFim = t;
}

double Corrida::getTempoInicio() const {
    return tempoInicio;
}

double Corrida::getTempoFim() const {
    return tempoFim;
}

double Corrida::getDistanciaTotal() const {
    return distanciaTotal;
}

int Corrida::getNumeroParadas() const {
    return qtdParadas;
}

// Acesso a paradas e trechos
Parada* Corrida::getParada(int idx) const {
    if (idx < 0 || idx >= qtdParadas) return nullptr;
    return paradas[idx];
}

Trecho* Corrida::getTrecho(int idx) const {
    if (idx < 0 || idx >= qtdTrechos) return nullptr;
    return trechos[idx];
}

// Imprime a saída final da corrida no formato especificado pelo enunciado
void Corrida::imprimirSaida(std::ostream& out) const {
    out << tempoFim << " " << distanciaTotal << " " << qtdParadas;

    for (int i = 0; i < qtdParadas; ++i) {
        Ponto p = paradas[i]->getPonto();
        out << " " << p.x << " " << p.y;
    }
    out << "\n";
}
