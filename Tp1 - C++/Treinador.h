#ifndef TREINADOR_H
#define TREINADOR_H

#include "Pessoa.h"
#include <vector>
#include <iostream>

using namespace std;

class Atleta;

class Treinador : public Pessoa {
private:
    vector<Atleta *> atletas;
public:
    // (Herança) - Construtor: repassa os dados básicos para o construtor de Pessoa.
    Treinador(string nome);

    // Destrutor
    ~Treinador();

    // Getter
    vector<Atleta *> getAtletas();

    // Cria o vinculo entre o treinador e o atleta
    void vincularAtleta(Atleta *atleta);

    // (Polimorfismo) - exibirDados: exibe os dados herdados de Pessoa e os específicos do Treinador
    void exibirDados();
};
#endif