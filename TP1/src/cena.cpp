#include "cena.hpp"
#include <cstring>   
#include <new>       
#include <cmath>     
#include <cstdio>

// Helpers numéricos simples 
static inline double dmin(double a, double b) { return (a < b) ? a : b; }
static inline double dmax(double a, double b) { return (a > b) ? a : b; }

// Construtor/Destrutor/Reset
Cena::Cena()
    : cobertura_(NULL), nCob_(0), capCob_(0),
      itens_(NULL), nItens_(0), capItens_(0) {
    capCob_ = 8;
    cobertura_ = new (std::nothrow) Intervalo[capCob_];
    nCob_ = 0;

    capItens_ = 16;
    itens_ = new (std::nothrow) ItemSaida[capItens_];
    nItens_ = 0;
}

Cena::~Cena() {
    delete[] cobertura_;
    delete[] itens_;
}


// Gestão de capacidade
void Cena::ensureCobCap(int need) {
    if (need <= capCob_) return;
    int novoCap = capCob_ > 0 ? capCob_ : 8;
    while (novoCap < need) novoCap <<= 1;
    Intervalo* novo = new (std::nothrow) Intervalo[novoCap];
    if (!novo) return;
    for (int i = 0; i < nCob_; ++i) novo[i] = cobertura_[i];
    delete[] cobertura_;
    cobertura_ = novo;
    capCob_ = novoCap;
}

void Cena::ensureItensCap(int need) {
    if (need <= capItens_) return;
    int novoCap = capItens_ > 0 ? capItens_ : 16;
    while (novoCap < need) novoCap <<= 1;
    ItemSaida* novo = new (std::nothrow) ItemSaida[novoCap];
    if (!novo) return;
    for (int i = 0; i < nItens_; ++i) novo[i] = itens_[i];
    delete[] itens_;
    itens_ = novo;
    capItens_ = novoCap;
}


// Inserção na cobertura com merge
void Cena::inserirCobertura(const Intervalo& segOrig) {
    if (segOrig.b <= segOrig.a) return;

    Intervalo seg = segOrig;
    if (seg.b < seg.a) { double t = seg.a; seg.a = seg.b; seg.b = t; } // normaliza a ≤ b por precaução.
    if (nCob_ == 0) {
        ensureCobCap(1);
        cobertura_[0] = seg;
        nCob_ = 1;
        return;
    }

    // encontra a posição de inserção por início a (mantém ordenação).
    int pos = 0;
    while (pos < nCob_ && cobertura_[pos].a <= seg.a) pos++; 

    // abre espaço e insere o novo intervalo.
    ensureCobCap(nCob_ + 1);
    for (int i = nCob_; i > pos; --i) cobertura_[i] = cobertura_[i - 1]; 
    cobertura_[pos] = seg;
    nCob_++;

    // merge linear: varre em ordem, funde sobreposições e escreve o resultado por cima do próprio vetor
    int w = 0;
    for (int r = 0; r < nCob_; ++r) {
        if (w == 0) {
            cobertura_[w++] = cobertura_[r];
        } else {
            Intervalo& prev = cobertura_[w - 1];
            Intervalo cur = cobertura_[r];
            if (cur.a <= prev.b) {
                if (cur.b > prev.b) prev.b = cur.b;
            } else {
                cobertura_[w++] = cur;
            }
        }
    }
    nCob_ = w;
}


// Subtração: [seg] \ cobertura_
int Cena::subtrairCobertura(const Intervalo& segOrig, Intervalo* out, int maxOut) const {
    if (maxOut <= 0) return 0;
    double a = segOrig.a;
    double b = segOrig.b;
    if (b <= a) return 0;
    if (b < a) { double t = a; a = b; b = t; }

    int k = 0; // num de pedaços visíveis
    double cursor = a; // marca onde ainda falta cobrir dentro de [a,b]

    // pula intervalos irrelevantes; cobertura está ordenada, então dá para sair cedo.
    for (int i = 0; i < nCob_; ++i) {
        const Intervalo& c = cobertura_[i];
        if (c.b <= a) continue;
        if (c.a >= b) break;  

    // achou um buraco visível entre cursor e o começo do intervalo coberto c.a; recorta [cursor, min(c.a,b)]
        if (cursor < c.a) {
            double ini = cursor;
            double fim = dmin(c.a, b);
            if (ini < fim && k < maxOut) {
                out[k].a = ini;
                out[k].b = fim;
                ++k;
            }
        }

    // avança cursor para depois do trecho coberto; se passou de b, terminou.
        if (c.b > cursor) cursor = c.b;
        if (cursor >= b) break;
    }

    // pega o resto final (se houver).
    if (cursor < b && k < maxOut) {
        out[k].a = cursor;
        out[k].b = b;
        ++k;
    }

    return k;
}


// Ordena por id
void Cena :: ordenarPorId(ItemSaida* v, int n) {
    for (int i = 1; i < n; ++i) {
        ItemSaida aux = v[i];
        int j = i - 1;
        while (j >= 0 && v[j].objetoId > aux.objetoId) {
            v[j + 1] = v[j];
            --j;
        }
        v[j + 1] = aux;
    }
}



// Adiciona um novo objeto à cena, considerando as partes visíveis
void Cena::adicionarObjeto(int objetoId, double inicio, double fim) {
    if (fim < inicio) { double t = inicio; inicio = fim; fim = t; }
    if (fim <= inicio) return; // Ignora intervalos degenerados

    // Cria o intervalo correspondente ao objeto
    Intervalo seg; 
    seg.a = inicio; 
    seg.b = fim;

    // Estima número máximo de intervalos resultantes (partes visíveis)
    int maxOut = nCob_ + 1;
    if (maxOut < 1) maxOut = 1;

    Intervalo* temp = new (std::nothrow) Intervalo[maxOut];
    if (!temp) return; 

    // Calcula as partes do novo intervalo que ainda não estão cobertas
    int k = subtrairCobertura(seg, temp, maxOut);

    // Se houver partes visíveis, adiciona cada uma à lista de saída
    if (k > 0) {
        ensureItensCap(nItens_ + k); 
        for (int i = 0; i < k; ++i) {
            if (temp[i].b > temp[i].a) { 
                itens_[nItens_].objetoId = objetoId;
                itens_[nItens_].a = temp[i].a;
                itens_[nItens_].b = temp[i].b;
                ++nItens_;
            }
        }
    }

    delete[] temp;
    inserirCobertura(seg);
}




// Grava por id crescente
void Cena::gravarCenaPorId(FILE* fp, int tempo) const {
    if (!fp || nItens_ <= 0) return;

    ItemSaida* aux = new (std::nothrow) ItemSaida[nItens_];
    if (!aux) return;

    for (int i = 0; i < nItens_; ++i)
        aux[i] = itens_[i];

    ordenarPorId(aux, nItens_);

    for (int i = 0; i < nItens_; ++i)
        std::fprintf(fp, "S %d %d %.2f %.2f\n",
                     tempo, aux[i].objetoId, aux[i].a, aux[i].b);

    delete[] aux;
}

