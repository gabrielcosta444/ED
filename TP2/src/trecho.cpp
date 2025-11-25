#include "trecho.hpp"
#include <cmath>

/*
 Construtor padrão: cria um trecho vazio,
 sem paradas associadas e com distância zero.
 */
Trecho::Trecho()
    : inicio(nullptr),
      fim(nullptr),
      distancia(0.0),
      tempo(0.0),
      tipo(TRECHO_DESLOCAMENTO) {}

/*
 * Construtor completo: recebe duas paradas e o tipo do trecho.
 * Calcula automaticamente a distância Euclidiana entre início e fim.
 */
Trecho::Trecho(Parada* inicio_, Parada* fim_, TipoTrecho tipo_)
    : inicio(inicio_),
      fim(fim_),
      distancia(0.0),
      tempo(0.0),
      tipo(tipo_) {
    if (inicio && fim) {
        Ponto a = inicio->getPonto();
        Ponto b = fim->getPonto();
        double dx = a.x - b.x;
        double dy = a.y - b.y;
        distancia = std::sqrt(dx * dx + dy * dy);
    }
}

/*
 * Acessores básicos.
 */
Parada* Trecho::getInicio() const {
    return inicio;
}

Parada* Trecho::getFim() const {
    return fim;
}

double Trecho::getDistancia() const {
    return distancia;
}

double Trecho::getTempo() const {
    return tempo;
}

TipoTrecho Trecho::getTipo() const {
    return tipo;
}

/*
 * Modificadores básicos.
 */
void Trecho::setDistancia(double d) {
    distancia = d;
}

void Trecho::setTempo(double t) {
    tempo = t;
}

void Trecho::setTipo(TipoTrecho t) {
    tipo = t;
}
