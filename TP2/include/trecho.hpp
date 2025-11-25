#ifndef TRECHO_HPP
#define TRECHO_HPP

#include "parada.hpp"

// Tipos possíveis de trecho em uma rota:
// coleta (entre paradas de embarque), entrega (entre paradas de desembarque)
// ou deslocamento geral entre pontos da rota
enum TipoTrecho {
    TRECHO_COLETA = 0,
    TRECHO_ENTREGA,
    TRECHO_DESLOCAMENTO
};

// Classe que representa um segmento da rota entre duas paradas
class Trecho {
private:
    Parada* inicio;      // Parada inicial do trecho
    Parada* fim;         // Parada final do trecho
    double distancia;    // Distância entre as paradas
    double tempo;        // Tempo estimado (não utilizado na lógica principal)
    TipoTrecho tipo;     // Natureza do trecho

public:
    Trecho();                                   // Construtor padrão
    Trecho(Parada* inicio_, Parada* fim_, TipoTrecho tipo_); // Construtor completo

    Parada* getInicio() const;                  // Acesso à parada inicial
    Parada* getFim() const;                     // Acesso à parada final

    double getDistancia() const;                // Retorna a distância armazenada
    double getTempo() const;                    // Retorna o tempo estimado
    TipoTrecho getTipo() const;                 // Retorna o tipo do trecho

    void setDistancia(double d);                // Define a distância
    void setTempo(double t);                    // Define o tempo
    void setTipo(TipoTrecho t);                 // Define o tipo do trecho
};

#endif // TRECHO_HPP
