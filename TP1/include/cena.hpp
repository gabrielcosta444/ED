#ifndef CENA_HPP
#define CENA_HPP

#include <cstdio>   
#include <string>   

class Cena {
private:
    struct Intervalo {
        double a; // início 
        double b; // fim 
    };

    struct ItemSaida {
        int objetoId;
        double a;
        double b;
    };

    // Cobertura (união de intervalos), sempre ordenada por a e sem sobreposição
    Intervalo* cobertura_;
    int nCob_;
    int capCob_;

    // Itens visíveis acumulados para imprimir
    ItemSaida* itens_;
    int nItens_;
    int capItens_;

    // Gestão de capacidade 
    void ensureCobCap(int need);
    void ensureItensCap(int need);

    // Insere [seg] na cobertura_
    void inserirCobertura(const Intervalo& seg);

    // Subtrai cobertura_ de 'seg' e retorna uma lista de pedaços visíveis em 'out'.
    // Retorna o número de pedaços escritos e garante que n <= maxOut.
    int subtrairCobertura(const Intervalo& seg, Intervalo* out, int maxOut) const;

    // Ordena cópia dos itens por início (insertion)
    static void ordenarPorId(ItemSaida* v, int n);

public:
    Cena();
    ~Cena();

    // Adiciona um objeto por seu intervalo [inicio, fim]
    void adicionarObjeto(int objetoId, double inicio, double fim);

    // Emite as linhas no formato exigido
    void gravarCenaPorId(FILE* fp, int tempo) const;

};

#endif 
