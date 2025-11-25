#include "parada.hpp"

/*
 Construtor padrão.
 */
Parada::Parada()
    : ponto{0.0, 0.0},
      tipo(PARADA_EMBARQUE),
      demanda(nullptr) {}

/*
 Construtor completo.
 */
Parada::Parada(const Ponto& p, TipoParada tipo_, Demanda* demanda_)
    : ponto(p),
      tipo(tipo_),
      demanda(demanda_) {}

/*
Retorna a coordenada espacial da parada.
 */
Ponto Parada::getPonto() const {
    return ponto;
}

/*
 * Retorna o tipo da parada:
 *  - PARADA_EMBARQUE   → início da demanda
 *  - PARADA_DESEMBARQUE → destino da demanda
 */
TipoParada Parada::getTipo() const {
    return tipo;
}

/*
 * Retorna a demanda associada a esta parada.
 * Cada parada está vinculada exatamente a uma demanda.
 */
Demanda* Parada::getDemanda() const {
    return demanda;
}

/*
 * Atualiza a posição da parada.
 */
void Parada::setPonto(const Ponto& p) {
    ponto = p;
}

/*
 * Atualiza o tipo da parada (embarque/desembarque).
 */
void Parada::setTipo(TipoParada t) {
    tipo = t;
}

/*
 * Atualiza o ponteiro para a demanda associada.
 */
void Parada::setDemanda(Demanda* d) {
    demanda = d;
}
