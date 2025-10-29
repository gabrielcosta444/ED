#ifndef OBJETO_HPP
#define OBJETO_HPP

#include <string>

class Objeto {
private:
    int id;          // Identificador do objeto
    double x;        // Posição x (centro)
    double y;        // Posição y (profundidade)
    double largura;  // Largura do objeto (meio à esquerda e meio à direita)

public:
    // Construtor
    Objeto();
    Objeto(int id, double x, double y, double largura);

    // Getters
    int getId() const;
    double getX() const;
    double getY() const;
    double getLargura() const;

    // Setters / Atualizações
    void setPosicao(double novoX, double novoY);
    void setLargura(double novaLargura);

    // Métodos auxiliares
    double inicio() const; // retorna x - largura/2
    double fim() const;    // retorna x + largura/2

};

#endif
