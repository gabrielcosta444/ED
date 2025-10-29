#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "objeto.hpp"
#include "cena.hpp"


// ===== Armazenamento manual de objetos (sem STL) =====
struct ObjStore {
    Objeto* v;
    int n;
    int cap;

    ObjStore(): v(NULL), n(0), cap(0) {}

    ~ObjStore() { delete[] v; }

    void ensure(int need) {
        if (need <= cap) return;
        int novo = cap > 0 ? cap : 16;
        while (novo < need) novo <<= 1;
        Objeto* nv = new(std::nothrow) Objeto[novo];
        if (!nv) return; // defensivo
        for (int i = 0; i < n; ++i) nv[i] = v[i];
        delete[] v;
        v = nv;
        cap = novo;
    }

    int findIndexById(int id) const {
        for (int i = 0; i < n; ++i) if (v[i].getId() == id) return i;
        return -1;
    }

    void add(const Objeto& obj) {
        // não duplica id; se já existir, substitui
        int idx = findIndexById(obj.getId());
        if (idx >= 0) { v[idx] = obj; return; }
        ensure(n + 1);
        v[n++] = obj;
    }
};

// ===== Ordenação por Y (insertion sort, estável, sem STL) =====
static inline bool vemAntes(const Objeto& a, const Objeto& b) {
    return a.getY() < b.getY();
}
void insertionSortPorY(Objeto* v, int n) {
    for (int i = 1; i < n; ++i) {
        Objeto aux = v[i];
        int j = i - 1;
        while (j >= 0 && !vemAntes(v[j], aux)) {
            v[j + 1] = v[j];
            --j;
        }
        v[j + 1] = aux;
    }
}

// ===== Gerar a cena no tempo t =====
void gerarCenaNoTempo(const ObjStore& store, int tempo) {
    if (store.n <= 0) return;

    // Faz uma cópia local para ordenar sem mexer no store
    Objeto* arr = new(std::nothrow) Objeto[store.n];
    if (!arr) return;
    for (int i = 0; i < store.n; ++i) arr[i] = store.v[i];

    insertionSortPorY(arr, store.n);

    Cena cena;
    // Processa do "mais na frente" para o "mais ao fundo"
    for (int i = 0; i < store.n; ++i) {
        double ini = arr[i].inicio();
        double fim = arr[i].fim();
        if (fim > ini) {
            cena.adicionarObjeto(arr[i].getId(), ini, fim);
        }
    }
    // Grava em stdout no formato exigido
    cena.gravarCenaPorId(stdout, tempo);

    delete[] arr;
}

// ===== Leitor de entrada =====
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    ObjStore store;

    std::string line;
    while (std::getline(std::cin, line)) {
        // ignora linhas vazias
        std::string s = line;
        // tira espaços iniciais
        size_t p = s.find_first_not_of(" \t\r\n");
        if (p == std::string::npos) continue;
        if (p > 0) s.erase(0, p);

        char tipo = 0;
        std::istringstream iss(s);
        iss >> tipo;
        if (!iss) continue;

        if (tipo == 'O') {
            int id; double x, y, larg;
            if (!(iss >> id >> x >> y >> larg)) continue;
            if (larg <= 0.0) continue; // ignora degenerado
            Objeto obj(id, x, y, larg);
            store.add(obj);
        }
        else if (tipo == 'M') {
            int tempoDummy, id; double x, y;
            if (!(iss >> tempoDummy >> id >> x >> y)) continue;
            int idx = store.findIndexById(id);
            if (idx >= 0) {
                store.v[idx].setPosicao(x, y);
            }
            // Se o objeto não existir ainda, é entrada inválida;
        }
        else if (tipo == 'C') {
            int tempo;
            if (!(iss >> tempo)) continue;
            gerarCenaNoTempo(store, tempo);
        }
        // Outros tipos de linha: ignora
    }

    return 0;
}
