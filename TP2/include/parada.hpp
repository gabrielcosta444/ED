#ifndef PARADA_HPP
#define PARADA_HPP

#include "demanda.hpp"

class Demanda;

// Tipo de parada em uma corrida: embarque ou desembarque
enum TipoParada {
    PARADA_EMBARQUE = 0,
    PARADA_DESEMBARQUE
};

// Classe que representa uma parada da rota (origem ou destino de uma demanda)
class Parada {
private:
    Ponto ponto;          // Coordenadas da parada
    TipoParada tipo;      // Embarque ou desembarque
    Demanda* demanda;     // Demanda associada a esta parada

public:
    Parada();                                      // Construtor padrão
    Parada(const Ponto& p, TipoParada tipo_, Demanda* demanda_); // Construtor completo

    Ponto getPonto() const;                        // Obtém coordenadas da parada
    TipoParada getTipo() const;                    // Obtém o tipo da parada
    Demanda* getDemanda() const;                   // Obtém a demanda associada

    void setPonto(const Ponto& p);                 // Define posição da parada
    void setTipo(TipoParada t);                    // Define o tipo de parada
    void setDemanda(Demanda* d);                   // Associa uma demanda
};

#endif // PARADA_HPP
