#ifndef ATLETA_H
#define ATLETA_H

#include <iostream>
#include <vector>
#include "Medalha.h"
#include "Pessoa.h"
#include "Modalidade.h"

using namespace std;

class Atleta : public Pessoa {
    int participacoes;
    vector<Modalidade *> modalidades;
    vector<Medalha *> medalhas;
public:
    // (Herança) - Construtor: repassa os dados básicos para o construtor de Pessoa.
    Atleta(string nome, string genero, int ano, int participacoes);

    // Destrutor
    ~Atleta();

    // Getters
    int getParticipacoes();
    vector<Medalha*> getMedalhas();

    // Setters
    void setParticipacoes(int);
    void setMedalhas(Medalha*);

    // (Polimorfismo) - exibirDados: exibe os dados herdados de Pessoa e os específicos do Atleta.
    void exibirDados();
};
#endif