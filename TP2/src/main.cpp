#include <iostream>
#include <iomanip>
#include <cmath>

#include "demanda.hpp"
#include "corrida.hpp"
#include "escalonador.hpp"

using namespace std;

/*
 * Calcula a distância euclidiana entre dois pontos do plano.
 * Essa função é usada tanto para distâncias individuais das demandas
 * quanto para a rota compartilhada construída para um grupo.
 */
double distPontos(const Ponto& a, const Ponto& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

/*
 * Calcula a distância total percorrida por uma rota compartilhada de um grupo.
 * O modelo adotado é:
 *  - visitar todas as origens na ordem em que aparecem no grupo
 *  - depois visitar os destinos na mesma ordem
 *
 * Esse padrão corresponde exatamente ao modelo de rota usado na classe Corrida.
 */
double distanciaRota(const Demanda* demandas, const int* grupo, int tamGrupo) {
    if (tamGrupo == 0) return 0.0;

    double d = 0.0;
    Ponto anterior = demandas[grupo[0]].getOrigem();

    // sequência de origens
    for (int i = 1; i < tamGrupo; ++i) {
        Ponto atual = demandas[grupo[i]].getOrigem();
        d += distPontos(anterior, atual);
        anterior = atual;
    }

    // última origem -> primeiro destino
    Ponto primeiroDestino = demandas[grupo[0]].getDestino();
    d += distPontos(anterior, primeiroDestino);
    anterior = primeiroDestino;

    // sequência de destinos
    for (int i = 1; i < tamGrupo; ++i) {
        Ponto atual = demandas[grupo[i]].getDestino();
        d += distPontos(anterior, atual);
        anterior = atual;
    }

    return d;
}

/*
 * Soma das distâncias individuais das demandas:
 * É a distância que os passageiros gastariam se fossem transportados sozinhos.
 * Usado para calcular a eficiência λ do compartilhamento.
 */
double distanciaIndividualTotal(const Demanda* demandas, const int* grupo, int tamGrupo) {
    double soma = 0.0;
    for (int i = 0; i < tamGrupo; ++i) {
        const Demanda& d = demandas[grupo[i]];
        soma += distPontos(d.getOrigem(), d.getDestino());
    }
    return soma;
}

/*
 * Função principal:
 * 1) Lê parâmetros globais do sistema (η, γ, δ, α, β, λ).
 * 2) Lê todas as demandas.
 * 3) Executa o algoritmo guloso de agrupamento baseado nas restrições:
 *       - janela de tempo δ
 *       - distância entre origens α
 *       - distância entre destinos β
 *       - eficiência mínima λ
 *       - capacidade η
 * 4) Para cada grupo encontrado, monta uma Corrida,
 *    constrói a rota e agenda no Escalonador.
 * 5) Executa a simulação e imprime o resultado no formato especificado.
 */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int eta;            // capacidade do veículo
    double gama;        // velocidade de deslocamento
    double delta;       // janela máxima para combinar corridas
    double alfa;        // limite de distância entre origens
    double beta;        // limite de distância entre destinos
    double lambda;      // eficiência mínima exigida
    int numDemandas;    // número total de solicitações

    // Caso entrada inesperada, encerra silenciosamente
    if (!(cin >> eta)) {
        return 0;
    }

    cin >> gama >> delta >> alfa >> beta >> lambda >> numDemandas;

    // Vetor de todas as demandas
    Demanda* demandas = new Demanda[numDemandas];

    // Leitura das demandas
    for (int i = 0; i < numDemandas; ++i) {
        int id;
        double tempo;
        Ponto origem, destino;

        cin >> id >> tempo
            >> origem.x >> origem.y
            >> destino.x >> destino.y;

        demandas[i] = Demanda(id, tempo, origem, destino);
    }

    // marcação de demandas já agrupadas
    bool* usada = new bool[numDemandas];
    for (int i = 0; i < numDemandas; ++i) {
        usada[i] = false;
    }

    // vetor temporário para montar cada grupo
    int* grupo = new int[eta];

    Escalonador escalonador(numDemandas, gama);

    cout << fixed << setprecision(2);

    // algoritmo de agrupamento guloso baseado nas restrições (δ, α, β, λ, η)
    for (int i = 0; i < numDemandas; ++i) {
        if (usada[i]) continue;

        Demanda& c0 = demandas[i];

        int tamGrupo = 0;
        grupo[tamGrupo++] = i;
        usada[i] = true;

        // tenta adicionar cj ao grupo de c0
        for (int j = i + 1; j < numDemandas; ++j) {
            Demanda& cj = demandas[j];

            // restrição da janela temporal δ
            if (cj.getTempoSolicitacao() - c0.getTempoSolicitacao() >= delta) {
                break;
            }

            if (usada[j]) continue;
            if (tamGrupo >= eta) break; // capacidade cheia

            // restrições espaciais α e β
            bool ok = true;
            for (int g = 0; g < tamGrupo; ++g) {
                const Demanda& dExistente = demandas[grupo[g]];
                double distOrig = distPontos(cj.getOrigem(), dExistente.getOrigem());
                double distDest = distPontos(cj.getDestino(), dExistente.getDestino());
                if (distOrig > alfa || distDest > beta) {
                    ok = false;
                    break;
                }
            }
            if (!ok) {
                break;
            }

            // verifica eficiência λ com cj incluída
            grupo[tamGrupo] = j;
            int novoTam = tamGrupo + 1;

            double distRota = distanciaRota(demandas, grupo, novoTam);
            double distInd = distanciaIndividualTotal(demandas, grupo, novoTam);

            double eficiencia = (distRota > 0.0) ? (distInd / distRota) : 1.0;

            if (eficiencia <= lambda) {
                break;
            }

            // adiciona a nova demanda ao grupo
            tamGrupo = novoTam;
            usada[j] = true;
        }

        // monta a Corrida correspondente ao grupo encontrado
        Corrida* corrida = new Corrida(tamGrupo);
        for (int g = 0; g < tamGrupo; ++g) {
            corrida->adicionarDemanda(&demandas[grupo[g]]);
        }

        corrida->construirRotaBasica();
        corrida->calcularTrechosEDistancia();

        escalonador.adicionarCorrida(corrida);
    }

    escalonador.simularEImprimir();

    delete[] grupo;
    delete[] usada;
    delete[] demandas;

    return 0;
}
