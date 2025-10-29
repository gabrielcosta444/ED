#include "objeto.hpp"


// Construtor padrão
Objeto::Objeto() : id(-1), x(0.0), y(0.0), largura(0.0) {}

// Construtor parametrizado
Objeto::Objeto(int id, double x, double y, double largura)
    : id(id), x(x), y(y), largura(largura) {}

// Getters
int Objeto::getId() const { return id; }
double Objeto::getX() const { return x; }
double Objeto::getY() const { return y; }
double Objeto::getLargura() const { return largura; }

// Setters
void Objeto::setPosicao(double novoX, double novoY) {
    x = novoX;
    y = novoY;
}

void Objeto::setLargura(double novaLargura) {
    largura = novaLargura;
}

// Métodos auxiliares
double Objeto::inicio() const {
    return x - largura / 2.0;
}

double Objeto::fim() const {
    return x + largura / 2.0;
}

