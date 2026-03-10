#ifndef MODALIDADE_H
#define MODALIDADE_H

#include <iostream>

using namespace std;

class Modalidade {
public:
    string nome;
    string codigo;
    int evento; // (0-Olimpíadas, 1-Olimpíadas Inverno, 2-Paralimpíadas)

    // Construtor
    Modalidade(string n, string c, int e);

    // Exibe os Dados de Modalidade
    void exibirDados();
};
#endif