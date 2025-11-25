#ifndef CORRIDA_HPP
#define CORRIDA_HPP

#include "demanda.hpp"
#include "parada.hpp"
#include "trecho.hpp"
#include <iosfwd> // std::ostream

class Corrida {
private:
    Demanda** demandas;
    int qtdDemandas;
    int capacidadeDemandas;

    Parada** paradas;
    int qtdParadas;
    int capacidadeParadas;

    Trecho** trechos;
    int qtdTrechos;
    int capacidadeTrechos;

    double distanciaTotal;
    double tempoInicio;
    double tempoFim;

public:
    explicit Corrida(int capacidadeInicialDemandas);
    ~Corrida();

    void adicionarDemanda(Demanda* d);

    int getQtdDemandas() const;
    Demanda* getDemanda(int idx) const;

    // Rota básica: origens na ordem, depois destinos na mesma ordem
    void construirRotaBasica();

    // Cria trechos entre paradas consecutivas e soma distânciaTotal
    void calcularTrechosEDistancia();

    // Atributos de tempo
    void setTempoInicio(double t);
    void setTempoFim(double t);
    double getTempoInicio() const;
    double getTempoFim() const;

    // Acesso à estrutura da corrida
    double getDistanciaTotal() const;
    int getNumeroParadas() const;
    Parada* getParada(int idx) const;
    Trecho* getTrecho(int idx) const;

    // Impressão da linha de saída
    void imprimirSaida(std::ostream& out) const;
};

#endif // CORRIDA_HPP
