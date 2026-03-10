#ifndef PAIS_H
#define PAIS_H
#include "Atleta.h"
#include <vector>
class Pais
{
    string codigo;
    vector<Atleta *> atletas;

    public:
    string nome;
    Pais(string cod, string n);
    ~Pais();
    string getCodigo();
    vector<Atleta *> getAtletas();
    vector<Medalha*> getMedalhas(Medalha *);

    void setNome(string);
    string getNome();

    void setAtletas(vector<Atleta*> novaListaDeAtletas);

    void addAtleta(Atleta* novoAtleta);

    int getClassificacao(int ano, int evento);
    void exibirQuadroMedalhas(int ano, int evento);
};
#endif