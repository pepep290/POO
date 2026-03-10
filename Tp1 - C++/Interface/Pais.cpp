#include "Pais.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;


string eventoToString(int evento);

Pais::Pais(string cod, string n) :  codigo(cod),nome(n) {}
Pais::~Pais(){};
string Pais::getCodigo(){
    return codigo;
}
vector<Atleta *> Pais::getAtletas(){
    return atletas;
}

string Pais::getNome(){
    return nome;
}
void Pais::setNome(string nome){
    this->nome = nome;
}

vector<Medalha *> Pais::getMedalhas(Medalha *){
    vector<Medalha *> medalhas;
    for (auto atleta : atletas) {
        vector<Medalha *> atletaMedalhas = atleta->getMedalhas();
        for (auto medalha : atletaMedalhas) {
            medalhas.push_back(medalha);
        }
    }
    return medalhas;
}
void Pais::setAtletas(vector<Atleta *> NovosAtletas){
    atletas = NovosAtletas;
}
void Pais::addAtleta(Atleta * atleta){
    atletas.push_back(atleta);
}

int Pais::getClassificacao(int ano, int evento){
    int classificacao = 0;
    for (auto atleta : atletas) {
        for (auto medalha : atleta->getMedalhas()) {
            if (medalha->ano == ano && medalha->modalidade->evento == evento) {
                if (medalha->tipo == "ouro") classificacao += 3;
                else if (medalha->tipo == "prata") classificacao += 2;
                else if (medalha->tipo == "bronze") classificacao += 1;
            }
        }
    }
    return classificacao;
}
void Pais::exibirQuadroMedalhas(int ano, int evento) {
    // Descobrir o maior nome para ajustar a coluna automaticamente
    size_t larguraNome = 0;
    for (auto atleta : atletas) {
        larguraNome = max(larguraNome, atleta->nome.size());
    }

    // Largura mínima da coluna
    if (larguraNome < 7) larguraNome = 7; // "Atleta "

    // Criar divisória
    auto linha = string(larguraNome + 26, '-');

    cout << linha << "\n";

    // Cabeçalhos com alinhamento
    cout << left << setw(larguraNome) << "Atleta" << " | "
         << "Ouro " << " | "
         << "Prata " << " | " << "Bronze" << "\n";

    cout << linha << "\n";

    // Linhas da tabela
    for (auto atleta : atletas) {
        int ouro = 0, prata = 0, bronze = 0;
        for (auto medalha : atleta->getMedalhas()) {
            if (medalha->ano == ano && medalha->modalidade->evento == evento) {
                if (medalha->tipo == "ouro") ouro++;
                else if (medalha->tipo == "prata") prata++;
                else if (medalha->tipo == "bronze") bronze++;
            }
        }
        cout << left << setw(larguraNome) << atleta->nome << " | "
             << right << setw(4) << ouro << " | "
             << right << setw(4) << prata << " | "
             << right << setw(4) << bronze << "\n";
    }

    cout << linha << "\n";
}


string eventoToString(int evento){
        switch (evento) {
            case 0:
                return "Olimpíadas";
            case 1:
                return "Olimpiadas de Inverno";
            case 2:
                return "Paraolimpíadas";
            default:
                throw "Evento Desconhecido";
        }
    }