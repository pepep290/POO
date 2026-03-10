#ifndef MEDALHA_H
#define MEDALHA_H

#include <string>
#include <iostream>

using namespace std;

class Modalidade;

class Medalha {
public:
    string tipo;           // Ex: "Ouro", "Prata", "Bronze"
    int ano;               // Ex: 2024
    Modalidade *modalidade; // Associação com a modalidade
    
    // Construtor
    Medalha(string tipo, int ano, Modalidade* modalidade = nullptr);
    
    // Destrutor
    ~Medalha();

    // Mostra dados de medalha
    void exibirDados();
};
#endif