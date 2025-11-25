#ifndef DEMANDA_HPP
#define DEMANDA_HPP

class Corrida; 

// Estrutura simples para representar um ponto no plano (origem ou destino)
struct Ponto {
    double x;
    double y;
};

// Enumeração que representa o estado atual de uma demanda
// Permite acompanhar sua evolução ao longo da simulação
enum EstadoDemanda {
    DEMANDA_DEMANDADA = 0,    // A demanda foi registrada, mas ainda não alocada
    DEMANDA_INDIVIDUAL,       // Atendida como corrida individual
    DEMANDA_COMPARTILHADA,    // Atendida em corrida compartilhada
    DEMANDA_CONCLUIDA         // Corrida finalizada
};

// Classe que modela uma demanda de transporte
// Armazena origem, destino, instante de solicitação e estado durante a simulação
class Demanda {
private:
    int id;                        // Identificador único da demanda
    double tempoSolicitacao;       // Instante em que o passageiro solicita a corrida
    Ponto origem;                  // Ponto de origem (x, y)
    Ponto destino;                 // Ponto de destino (x, y)
    EstadoDemanda estado;          // Estado atual da demanda na simulação
    Corrida* corridaAssociada;     // Ponteiro para a corrida que atende esta demanda

public:
    // Construtor padrão
    Demanda();

    // Construtor parametrizado: inicializa todos os atributos relevantes
    Demanda(int id_, double tempo_, const Ponto& origem_, const Ponto& destino_);

    // Métodos de acesso aos atributos básicos
    int getId() const;
    double getTempoSolicitacao() const;
    Ponto getOrigem() const;
    Ponto getDestino() const;

    // Acesso e alteração do estado da demanda
    EstadoDemanda getEstado() const;
    void setEstado(EstadoDemanda novoEstado);

    // Associação entre demanda e corrida correspondente
    Corrida* getCorrida() const;
    void setCorrida(Corrida* corrida);
};

#endif // DEMANDA_HPP
