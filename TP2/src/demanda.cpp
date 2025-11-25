#include "demanda.hpp"

// Construtor padrão: cria uma demanda "vazia", ainda não associada a corrida
Demanda::Demanda()
    : id(-1),
      tempoSolicitacao(0.0),
      origem{0.0, 0.0},
      destino{0.0, 0.0},
      estado(DEMANDA_DEMANDADA),
      corridaAssociada(nullptr) {}

// Construtor completo: inicializa a demanda com valores fornecidos
Demanda::Demanda(int id_, double tempo_, const Ponto& origem_, const Ponto& destino_)
    : id(id_),
      tempoSolicitacao(tempo_),
      origem(origem_),
      destino(destino_),
      estado(DEMANDA_DEMANDADA),
      corridaAssociada(nullptr) {}

// Métodos de acesso simples
int Demanda::getId() const {
    return id;
}

double Demanda::getTempoSolicitacao() const {
    return tempoSolicitacao;
}

Ponto Demanda::getOrigem() const {
    return origem;
}

Ponto Demanda::getDestino() const {
    return destino;
}

EstadoDemanda Demanda::getEstado() const {
    return estado;
}

// Atualiza o estado da demanda (individual, compartilhada, concluída etc.)
void Demanda::setEstado(EstadoDemanda novoEstado) {
    estado = novoEstado;
}

// Retorna a corrida associada à demanda
Corrida* Demanda::getCorrida() const {
    return corridaAssociada;
}

// Associa a demanda a uma corrida específica
void Demanda::setCorrida(Corrida* corrida) {
    corridaAssociada = corrida;
}
