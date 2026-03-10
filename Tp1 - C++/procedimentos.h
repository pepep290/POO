#ifndef PROCEDIMENTOS_H
#define PROCEDIMENTOS_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <limits>
#include "strings.h"
#include "Pessoa.h"
#include "Atleta.h"
#include "Pais.h"
#include "Treinador.h"
#include "Modalidade.h"
#include "Medalha.h"



bool associarPessoaPais(Pessoa *pessoa, vector<Pais *> &paises) {
    string nomeBusca;
    cout << "Informe o NOME do país (ex: Brasil): ";
    nomeBusca = getFormattedStr<string>();
    for (Pais *p : paises) {
        if (p->getNome() == nomeBusca) {
            // Vincula na classe Pessoa (Atleta ou Treinador)
            pessoa->setPais(p);
            // Se for Atleta, adiciona na lista do país
            Atleta *atleta = dynamic_cast<Atleta *>(pessoa);
            if (atleta != nullptr) {
                p->addAtleta(atleta);
            }
            cout << "Vínculo realizado com sucesso!" << endl;
            return true;
        }
    }
    cout << "ERRO: O país '" << nomeBusca << "' não foi encontrado no sistema." << endl;
    return false;
}

void CadastrarPais(vector<Pais *> &paises) {
    cout << "Insira o nome do país: ";
    string nome = getFormattedStr<string>();
    for (auto pais : paises)
        if (pais->nome == nome) {
            cout << "Esse país já existe" << endl;
            return;
        }
    cout << "Insira o código do país: ";
    string codigo = getFormattedStr<string>();
    for (auto pais : paises)
        if (pais->getCodigo() == nome) {
            cout << "Esse código já existe" << endl;
            return;
        }
    Pais *novoPais = new Pais(codigo, nome);
    paises.push_back(novoPais);
}

void cadastrarAtleta(vector<Atleta *> &atletas, vector<Pais *> &paises, vector<Modalidade *> &modalidades) {
    string nome, genero, codigoModalidade;
    int anoNascimento, participacoes;
    char opcao;
    cout << "Nome do Atleta: ";
    nome = getFormattedStr<string>();
    cout << "Gênero: ";
    genero = getFormattedStr<string>();
    cout << "Ano de Nascimento: ";
    cin >> anoNascimento;
    cout << "Número de participações em Olimpíadas: ";
    cin >> participacoes;
    limparBuffer();
    Atleta *novoAtleta = new Atleta(nome, genero, anoNascimento, participacoes);
    if (!associarPessoaPais(novoAtleta, paises)) {
        cout << "O cadastro foi cancelado pois o país é obrigatório." << endl;
        delete novoAtleta;
        return;
    }
    do {
        cout << "Informe o código da modalidade do atleta: ";
        codigoModalidade = getFormattedStr<string>();
        bool modEncontrada = false;
        for (Modalidade *m : modalidades) {
            if (m->codigo == codigoModalidade) {
                novoAtleta->setModalidade(m);
                modEncontrada = true;
                cout << "Modalidade '" << m->nome << "' adicionada ao atleta." << endl;
                break;
            }
        }
        if (!modEncontrada) {
            cout << "ERRO: Modalidade não encontrada (nenhuma associação feita)." << endl;
        }
        cout << "Deseja adicionar outra modalidade para este atleta? (s/n): ";
        cin >> opcao;
        limparBuffer();
    } while (opcao == 's' || opcao == 'S');
    atletas.push_back(novoAtleta);
    cout << "\nAtleta cadastrado com sucesso!" << endl;
}

void cadastrarTreinador(vector<Treinador *> &treinadores, vector<Pais *> &paises, vector<Modalidade *> &modalidades, vector<Atleta *> &atletas) {
    string nome, codigoModalidade, nomeAtletaBusca;
    char opcao;
    cout << "Nome do Treinador: ";
    nome = getFormattedStr<string>();
    Treinador *novoTreinador = new Treinador(nome);
    if (!associarPessoaPais(novoTreinador, paises)) {
        cout << "Cadastro cancelado pois o país é obrigatório." << endl;
        delete novoTreinador;
        return;
    }
    do {
        cout << "Informe o código da modalidade do treinador: " << endl;
        codigoModalidade = getFormattedStr<string>();

        bool modEncontrada = false;
        for (Modalidade *m : modalidades) {
            if (m->codigo == codigoModalidade)
            {
                novoTreinador->setModalidade(m); // Herança de Pessoa
                modEncontrada = true;
                cout << "Modalidade '" << m->nome << "' adicionada." << endl;
                break;
            }
        }
        if (!modEncontrada)
            cout << "ERRO: Modalidade não encontrada." << endl;
        cout << "Adicionar outra modalidade? (s/n): ";
        cin >> opcao;
        limparBuffer();
    } while (opcao == 's' || opcao == 'S');
    do
    {
        cout << "Deseja vincular um atleta a este treinador? (s/n): ";
        cin >> opcao;
        limparBuffer();
        if (opcao == 's' || opcao == 'S')
        {
            cout << "Informe o NOME do atleta a ser vinculado: ";
            nomeAtletaBusca = getFormattedStr<string>();
            bool atletaEncontrado = false;
            for (Atleta *a : atletas)
            {
                if (a->nome == nomeAtletaBusca)
                {
                    novoTreinador->vincularAtleta(a);
                    cout << "Atleta '" << a->getNome() << "' vinculado ao treinador." << endl;
                    atletaEncontrado = true;
                    break;
                }
            }
            if (!atletaEncontrado)
            {
                cout << "ERRO: Atleta não encontrado na lista cadastrada." << endl;
            }
        }
    } while (opcao == 's' || opcao == 'S');
    treinadores.push_back(novoTreinador);
    cout << "\nTreinador cadastrado com sucesso!" << endl;
}

void criaModalidade(vector<Modalidade *> &modalidade)
{
    string nome;
    string codigo;
    int evento;
    cout << "Digite o nome: ";
    limparBuffer();
    nome = getFormattedStr<string>();
    if (nome.empty())
    {
        cout << "Nome não pode ser vazio!\n";
        return;
    }
    cout << "Digite o codigo: ";
    codigo = getFormattedStr<string>();
    for (auto &mod : modalidade)
    {
        if (mod->codigo == codigo)
        {
            cout << "Codigo ja existe!\n";
            return;
        }
    }

    cout << "Digite o evento (0-Olimpíadas, 1-Olimpíadas Inverno, 2-Paralimpíadas): ";
    cin >> evento;

    // Validar evento
    if (evento < 0 || evento > 2)
    {
        cout << "Evento invalido! Use 0, 1 ou 2.\n";
        return;
    }
    Modalidade *nmodalidade = new Modalidade(nome, codigo, evento);
    modalidade.push_back(nmodalidade);
}

void premiarAtletaComMedalha(vector<Atleta *> &atletas, vector<Medalha *> &medalhas)
{
    string nome;
    cout << "Insira o nome do atleta: ";
    nome = getFormattedStr<string>();

    Atleta *atletaPremiado = nullptr;

    bool atletaEncontrado = false;
    for (auto atleta : atletas)
        if (atleta->nome == nome)
        {
            atletaEncontrado = true;
            atletaPremiado = atleta;
            break;
        }

    if (!atletaEncontrado)
    {
        cout << "Atleta nao encontrado";
        return;
    }
    cout << "Insira o tipo da medalha: ";
    string tipo = getFormattedStr<string>();
    cout << "Insira o nome da modalidade: ";
    string modalidade = getFormattedStr<string>();
    cout << "Insira o ano da conquista: ";
    int ano;
    cin >> ano;
    limparBuffer();

    for (auto medalha = medalhas.begin(); medalha != medalhas.end();)
    {
        if ((*medalha)->ano == ano && (*medalha)->modalidade->nome == modalidade && (*medalha)->tipo == tipo) {
            auto novaMedalha = *medalha;
            medalhas.erase(medalha);
            atletaPremiado->setMedalhas(novaMedalha);
            return;
        } else {
            ++medalha;
        }
    }
    cout << "ERRO: Medalha nao encontrada." << endl;
    return;
}

void criaMedalha(vector<Medalha *> &medalhas, vector<Modalidade *> &modalidades)
{
    string tipo;
    int ano;
    cout << "\nDigite o tipo (ouro, prata, bronze): ";
    tipo = getFormattedStr<string>();

    // Validar tipo
    while (tipo != "ouro" && tipo != "prata" && tipo != "bronze") {
        cout << "Tipo invalido! Use 'ouro', 'prata' ou 'bronze'.\n";
        cout << "Digite o tipo (ouro, prata, bronze): ";
        tipo = getFormattedStr<string>();
    }
    // Ano
    cout << "Digite o ano da conquista: ";

    cin >> ano;
    // Listar modalidades disponíveis
    if (modalidades.empty())
    {
        cout << "Nenhuma modalidade cadastrada! Crie uma modalidade primeiro.\n";
        return;
    }

    cout << "\nModalidades disponiveis: \n";
    for (size_t i = 0; i < modalidades.size(); i++) {
        cout << i + 1 << " - " << modalidades[i]->nome << endl;
    }
    int escolha;
    cout << "\nEscolha o numero da modalidade: ";
    cin >> escolha;
    limparBuffer();
    if (escolha < 1 || escolha > static_cast<int>(modalidades.size()))
    {
        cout << "Escolha invalida!\n";
        return;
    }

    Modalidade *modalidadeEscolhida = modalidades[escolha - 1];

    // Criar medalha
    Medalha *novaMedalha = new Medalha(tipo, ano, modalidadeEscolhida);
    medalhas.push_back(novaMedalha);

    cout << "\n Medalha de " << tipo << " criada com sucesso!\n";
    cout << "   Modalidade: " << modalidadeEscolhida->nome << endl;
    cout << "   Ano: " << ano << endl;
}

void listarAtletas(vector<Atleta *> &atletas, vector<Modalidade *> &modalidades)
{
    limparBuffer();
    string codigo;
    cout << "Digite o código da modalidade: ";
    codigo = getFormattedStr<string>();
    Modalidade *modSelecionada = nullptr;

    for (Modalidade *m : modalidades)
    {
        if (m->codigo == codigo)
        {
            modSelecionada = m;
            break;
        }
    }

    if (modSelecionada == nullptr)
    {
        cout<<"modalidade não encontrada!"<<endl;
        return;
    }
    cout << "\nAtletas da modalidade -> " << modSelecionada->nome << " \n";

    bool encontrou = false;

    for (Atleta* a : atletas) {
        vector<Modalidade*> mods = a->getModalidade();
        for (Modalidade* m : mods) {
            if (m == modSelecionada) {
                cout << " - " << a->nome << endl;
                encontrou = true;
                break;
            }
        }
    }

    if (!encontrou) {
        cout << "Nenhum atleta cadastrado nesta modalidade." << endl;
    }
}

#include <iomanip>
void exibirQuadroDeMedalhas(vector<Pais *> &paises){
    struct linhaPais{
        string nome;
        int ouro;
        int prata;
        int bronze;
        int total;

        bool operator<(const linhaPais &outro) const {
            if (total != outro.total) return total > outro.total; // descendente por medalhas
            return nome < outro.nome; // ascendente por nome
        }
    };
    set<linhaPais> linhas;

    for (auto pais : paises){
        linhaPais linha;
        linha.nome = pais->nome;
        linha.ouro = 0;
        linha.prata = 0;
        linha.bronze = 0;
        linha.total = 0;
        for (auto atleta : pais->getAtletas())
            for (auto medalha : atleta->getMedalhas()){
                if (medalha->tipo == "ouro") linha.ouro++;
                else if (medalha->tipo == "prata") linha.prata++;
                else if (medalha->tipo == "bronze") linha.bronze++;
            }
        linha.total = linha.ouro + linha.prata + linha.bronze;
        linhas.insert(linha);
    }

    cout << left
        << setw(17) << "País"
        << "| " << setw(8) << "Ouro"
        << "| " << setw(8) << "Prata"
        << "| " << setw(8) << "Bronze"
        << "| " << setw(8) << "Total"
        << endl;

    for (const auto linha : linhas)
        cout << left
            << setw(16) << linha.nome
            << "| " << setw(8) << linha.ouro
            << "| " << setw(8) << linha.prata
            << "| " << setw(8) << linha.bronze
            << "| " << setw(8) << linha.total
            << endl;
}

void salvarEmArquivo(const string& nome, const string& conteudo) {
    ofstream arq(nome);
    if (!arq) {
        cout << "ERRO ao criar arquivo!\n";
        return;
    }
    arq << conteudo;
    arq.close();
    cout << "Arquivo salvo como: " << nome << endl;
}

void gerarRelatoriosEEstatisticas(vector<Atleta*> &atletas, vector<Pais*> &paises, vector<Medalha*> &medalhas)
{
    int opcao;
    char salvar;

    do {
        cout << "\n===== RELATÓRIOS E ESTATÍSTICAS =====\n";
        cout << "1 - Número total de medalhas por país\n";
        cout << "2 - Classificação dos países com mais medalhas\n";
        cout << "3 - Atletas com mais medalhas\n";
        cout << "4 - Atletas com mais participações\n";
        cout << "5 - Modalidades com mais atletas\n";
        cout << "6 - Anos em que países mais ganharam medalhas\n";
        cout << "0 - Voltar\n";
        cout << "Escolha: ";
        cin >> opcao;
        limparBuffer();

        string relatorio = "";

        switch (opcao)
        {
        case 1: {
            relatorio += "=== TOTAL DE MEDALHAS POR PAÍS ===\n";
            for (Pais* p : paises) {
                int total = 0;
                for (Atleta* a : p->getAtletas()) {
                    total += a->getMedalhas().size();
                }
                relatorio += p->nome + ": " + to_string(total) + " medalhas\n";
            }
            cout << relatorio;
            break;
        }

        case 2: {
            relatorio += "=== RANKING DE PAÍSES POR MEDALHAS ===\n";
            vector<pair<Pais*, int>> ranking;

            for (Pais* p : paises) {
                int total = 0;
                for (Atleta* a : p->getAtletas()) {
                    total += a->getMedalhas().size();
                }
                ranking.push_back({p, total});
            }

            sort(ranking.begin(), ranking.end(),
                [](auto &a, auto &b){ return a.second > b.second; });

            for (auto &r : ranking) {
                relatorio += r.first->nome + ": " + to_string(r.second) + " medalhas\n";
            }

            cout << relatorio;
            break;
        }

        case 3: {
            relatorio += "=== ATLETAS COM MAIS MEDALHAS ===\n";
            vector<pair<Atleta*, int>> ranking;

            for (Atleta* a : atletas) {
                ranking.push_back({a, (int)a->getMedalhas().size()});
            }

            sort(ranking.begin(), ranking.end(),
                [](auto&a, auto&b){ return a.second > b.second; });

            for (auto &r : ranking) {
                relatorio += r.first->nome + " - " + to_string(r.second) + " medalhas\n";
            }

            cout << relatorio;
            break;
        }

        case 4: {
            relatorio += "=== ATLETAS COM MAIS PARTICIPAÇÕES ===\n";
            vector<pair<Atleta*, int>> ranking;

            for (Atleta* a : atletas) {
                ranking.push_back({a, a->getParticipacoes()});
            }

            sort(ranking.begin(), ranking.end(),
                [](auto&a, auto&b){ return a.second > b.second; });

            for (auto &r : ranking) {
                relatorio += r.first->nome + " - " + to_string(r.second) + " participações\n";
            }

            cout << relatorio;
            break;
        }

        case 5: {
            relatorio += "=== MODALIDADES COM MAIS ATLETAS ===\n";
            map<Modalidade*, int> contador;

            for (Atleta* a : atletas) {
                for (Modalidade* m : a->getModalidade()) {
                    contador[m]++;
                }
            }

            vector<pair<Modalidade*, int>> ranking(contador.begin(), contador.end());

            sort(ranking.begin(), ranking.end(),
                [](auto&a, auto&b){ return a.second > b.second; });

            for (auto &r : ranking) {
                relatorio += r.first->nome + " - " + to_string(r.second) + " atletas\n";
            }

            cout << relatorio;
            break;
        }

        case 6: {
            relatorio += "=== ANOS EM QUE PAÍSES MAIS GANHARAM MEDALHAS ===\n";

            for (Pais* p : paises) {
                map<int, int> anoContador;
                for (Atleta* a : p->getAtletas()) {
                    for (Medalha* m : a->getMedalhas()) {
                        anoContador[m->ano]++;
                    }
                }

                relatorio += "\nPaís: " + p->nome + "\n";

                for (auto &ac : anoContador) {
                    relatorio += "  Ano " + to_string(ac.first) + ": " +
                        to_string(ac.second) + " medalhas\n";
                }
            }

            cout << relatorio;
            break;
        }

        case 0:
            return;

        default:
            cout << "Opcao invalida!\n";
            continue;
        }

        // Perguntar se salva em arquivo
        cout << "\nDeseja salvar este relatório em arquivo .txt? (s/n): ";
        cin >> salvar;

        if (salvar == 's' || salvar == 'S') {
            string nomeArquivo;
            cout << "Nome do arquivo (sem .txt): ";
            cin >> nomeArquivo;
            salvarEmArquivo(nomeArquivo + ".txt", relatorio);
        }

    } while (opcao != 0);
}


// void gerarRelatoriosEEstatisticas( vector<Atleta *> &atletas, vector<Pais *> &paises, vector<Medalha *> medalhas){

// }




// Salva os dados em arquivo .csv
void salvarDados(vector<Atleta *> &atletas, vector<Medalha *> &medalhas,
                vector<Modalidade *> &modalidades, vector<Pais *> &paises, vector<Treinador *> &treinadores){
    // abrir o arquivo
    cout<< "Pretende salvar os dados em um arquivo .csv? (s/n): "<<endl;
    char opcao;
    cin>>opcao;
    if (opcao == 'n' || opcao == 'N')
        return;

    cout<<"Insira o nome do arquivo: "<<endl;
    string nomeArquivo;
    cin>>nomeArquivo;

    if (nomeArquivo.empty())
        return;

    nomeArquivo += ".csv";
    ofstream arquivo(nomeArquivo);

    //cabecalho de atletas
    arquivo << "nome,genero,ano,pais,participacoes" << endl;
    // escrever os dados no arquivo
    for (auto atleta : atletas) {
        arquivo << atleta->nome << "," << atleta->genero << "," << atleta->anoNascimento << ","
        << atleta->getPais()->getNome() << "," << atleta->getParticipacoes() << endl;
    }

    arquivo << endl;
    // cabecalho de medalhas
    arquivo << "tipo,ano,modalidade" << endl;
    // escrever os dados no arquivo
    for (auto medalha : medalhas) {
        arquivo << medalha->tipo << "," << medalha->ano << "," << medalha->modalidade->nome << endl;
    }

    arquivo << endl;
    // cabecalho de modalidades
    arquivo << "nome,codigo,evento" << endl;
    // escrever os dados no arquivo
    for (auto modalidade : modalidades) {
        arquivo << modalidade->nome << "," << modalidade->codigo << "," << modalidade->evento << endl;
    }

    arquivo << endl;
    // cabecalho de paises
    arquivo << "codigo,nome" << endl;
    // escrever os dados no arquivo
    for (auto pais : paises) {
        arquivo << pais->getCodigo() << "," << pais->getNome() << endl;
    }

    arquivo << endl;
    // cabecalho de treinadores
    arquivo << "nome,genero,ano,pais" << endl;
    // escrever os dados no arquivo
    for (auto treinador : treinadores) {
        arquivo << treinador->nome << "," << treinador->genero << "," << treinador->anoNascimento << ","
        << treinador->getPais()->getNome() << endl;
    }

    arquivo<<endl;
    // imprimir lista modalidade de atletas
    arquivo<<"atleta,quantidade de modalidades,modalidades ->"<<endl;
    for (auto atleta : atletas){
        arquivo<<atleta->getNome()<<","<<atleta->getModalidade().size()<<",";
        for (auto modalidade : atleta->getModalidade()){
            arquivo<<modalidade->nome<<",";
        }
        arquivo<<endl;
    }

    arquivo<<endl;
    // imprimir lista de medalhas de atletas
    arquivo<<"atleta,quantidade de medalhas,medalhas ->"<<endl;
    for (auto atleta : atletas){
        arquivo<<atleta->getNome()<<","<<atleta->getMedalhas().size()<<",";
        for (auto medalha : atleta->getMedalhas()){
            arquivo<<medalha->tipo<<",";
        }
        arquivo<<endl;
    }


    arquivo<<endl;
    // imprimir lista de atletas de um pais
    arquivo<<"nome do pais,quantidade de atletas,atletas ->"<<endl;
    for (auto pais : paises){
        arquivo<<pais->getNome()<<","<<pais->getAtletas().size()<<",";
        auto atletas = pais->getAtletas();
        for (auto atleta : atletas){
            arquivo<<atleta->getNome()<<",";
        }
        arquivo<<endl;
    }

    arquivo<<endl;
    // imprimir lsita de atletas de um treinador
    arquivo<<"nome do treinador,quantidade de atletas,atletas ->"<<endl;
    for (auto treinador : treinadores){
        arquivo<<treinador->getNome()<<","<<treinador->getAtletas().size()<<",";
        auto atletas = treinador->getAtletas();
        for (auto atleta : atletas){
            arquivo<<atleta->getNome()<<",";
        }
        arquivo<<endl;
    }
    arquivo.close();
}

void carregarDados(vector<Atleta *> &atletas, vector<Medalha *> &medalhas,
              vector<Modalidade *> &modalidades, vector<Pais *> &paises, vector<Treinador *> &treinadores){

    cout << "Pretende carregar os dados de um arquivo .csv? (s/n): " << endl;
    char opcao;
    cin >> opcao;
    if (opcao == 'n' || opcao == 'N')
        return;

    cout << "Insira o nome do arquivo: " << endl;
    string nomeArquivo;
    cin >> nomeArquivo;

    if (nomeArquivo.empty())
        return;

    nomeArquivo += ".csv";
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    string linha;

    // ====== CARREGAR ATLETAS ======
    getline(arquivo, linha); // Pular cabeçalho "nome,genero,ano,pais,participacoes"
    while (getline(arquivo, linha)) {
        if (linha.empty()) break; // Linha vazia indica fim da seção

        stringstream ss(linha);
        string nome, genero, anoStr, nomePais, participacoesStr;

        getline(ss, nome, ',');
        getline(ss, genero, ',');
        getline(ss, anoStr, ',');
        getline(ss, nomePais, ',');
        getline(ss, participacoesStr, ',');

        int ano = stoi(anoStr);
        int participacoes = stoi(participacoesStr);

        Atleta* atleta = new Atleta(nome, genero, ano, participacoes);

        // Buscar país pelo nome
        for (auto pais : paises) {
            if (pais->getNome() == nomePais) {
                atleta->setPais(pais);
                break;
            }
        }

        atletas.push_back(atleta);
    }

    // ====== CARREGAR MEDALHAS ======
    getline(arquivo, linha); // Pular cabeçalho "tipo,ano,modalidade"
    while (getline(arquivo, linha)) {
        if (linha.empty()) break;

        stringstream ss(linha);
        string tipo, anoStr, nomeModalidade;

        getline(ss, tipo, ',');
        getline(ss, anoStr, ',');
        getline(ss, nomeModalidade, ',');

        int ano = stoi(anoStr);

        Modalidade* modalidade = nullptr;
        for (auto mod : modalidades) {
            if (mod->nome == nomeModalidade) {
                modalidade = mod;
                break;
            }
        }

        Medalha* medalha = new Medalha(tipo, ano, modalidade);
        medalhas.push_back(medalha);
    }

    // ====== CARREGAR MODALIDADES ======
    getline(arquivo, linha); // Pular cabeçalho "nome,codigo,evento"
    while (getline(arquivo, linha)) {
        if (linha.empty()) break;

        stringstream ss(linha);
        string nome, codigo, eventoStr;

        getline(ss, nome, ',');
        getline(ss, codigo, ',');
        getline(ss, eventoStr, ',');

        int evento = stoi(eventoStr);

        Modalidade* modalidade = new Modalidade(nome, codigo, evento);
        modalidades.push_back(modalidade);
    }

    // ====== CARREGAR PAISES ======
    getline(arquivo, linha); // Pular cabeçalho "codigo,nome"
    while (getline(arquivo, linha)) {
        if (linha.empty()) break;

        stringstream ss(linha);
        string codigo, nome;

        getline(ss, codigo, ',');
        getline(ss, nome, ',');

        Pais* pais = new Pais(codigo, nome);
        paises.push_back(pais);
    }

    // ====== CARREGAR TREINADORES ======
    getline(arquivo, linha); // Pular cabeçalho "nome,genero,ano,pais"
    while (getline(arquivo, linha)) {
        if (linha.empty()) break;

        stringstream ss(linha);
        string nome, genero, anoStr, nomePais;

        getline(ss, nome, ',');
        getline(ss, genero, ',');
        getline(ss, anoStr, ',');
        getline(ss, nomePais, ',');

        int ano = stoi(anoStr);

        Treinador* treinador = new Treinador(nome);
        treinador->genero = genero;
        treinador->anoNascimento = ano;

        // Buscar país pelo nome
        for (auto pais : paises) {
            if (pais->getNome() == nomePais) {
                treinador->setPais(pais);
                break;
            }
        }

        treinadores.push_back(treinador);
    }

    // ====== CARREGAR MODALIDADES DE ATLETAS ======
    getline(arquivo, linha); // Pular cabeçalho "atleta,quantidade de modalidades,modalidades ->"
    while (getline(arquivo, linha)) {
        if (linha.empty()) break;

        stringstream ss(linha);
        string nomeAtleta, quantidadeStr, nomeModalidade;

        getline(ss, nomeAtleta, ',');
        getline(ss, quantidadeStr, ',');

        // Buscar atleta pelo nome
        Atleta* atleta = nullptr;
        for (auto a : atletas) {
            if (a->getNome() == nomeAtleta) {
                atleta = a;
                break;
            }
        }

        if (atleta != nullptr) {
            // Ler todas as modalidades
            while (getline(ss, nomeModalidade, ',')) {
                // Buscar modalidade pelo nome
                for (auto mod : modalidades) {
                    if (mod->nome == nomeModalidade) {
                        atleta->setModalidade(mod);
                        break;
                    }
                }
            }
        }
    }

    // ====== CARREGAR MEDALHAS DE ATLETAS ======
    getline(arquivo, linha); // Pular cabeçalho "atleta,quantidade de medalhas,medalhas ->"
    while (getline(arquivo, linha)) {
        if (linha.empty()) break;

        stringstream ss(linha);
        string nomeAtleta, quantidadeStr, tipoMedalha;

        getline(ss, nomeAtleta, ',');
        getline(ss, quantidadeStr, ',');

        // Buscar atleta pelo nome
        Atleta* atleta = nullptr;
        for (auto a : atletas) {
            if (a->getNome() == nomeAtleta) {
                atleta = a;
                break;
            }
        }

        if (atleta != nullptr) {
            // Ler todas as medalhas (apenas os tipos)
            while (getline(ss, tipoMedalha, ',')) {
                // Buscar medalha pelo tipo no vetor de medalhas
                for (auto med : medalhas) {
                    if (med->tipo == tipoMedalha) {
                        atleta->setMedalhas(med);
                        break;
                    }
                }
            }
        }
    }

    // ====== CARREGAR ATLETAS DE PAÍSES ======
    getline(arquivo, linha); // Pular cabeçalho "nome do pais,quantidade de atletas,atletas ->"
    while (getline(arquivo, linha)) {
        if (linha.empty()) break;

        stringstream ss(linha);
        string nomePais, quantidadeStr, nomeAtleta;

        getline(ss, nomePais, ',');
        getline(ss, quantidadeStr, ',');

        // Buscar país pelo nome
        Pais* pais = nullptr;
        for (auto p : paises) {
            if (p->getNome() == nomePais) {
                pais = p;
                break;
            }
        }

        if (pais != nullptr) {
            // Ler todos os atletas
            while (getline(ss, nomeAtleta, ',')) {
                // Buscar atleta pelo nome
                for (auto a : atletas) {
                    if (a->getNome() == nomeAtleta) {
                        pais->addAtleta(a);
                        break;
                    }
                }
            }
        }
    }

    // ====== CARREGAR ATLETAS DE TREINADORES ======
    getline(arquivo, linha); // Pular cabeçalho "nome do treinador,quantidade de atletas,atletas ->"
    while (getline(arquivo, linha)) {
        if (linha.empty()) break;

        stringstream ss(linha);
        string nomeTreinador, quantidadeStr, nomeAtleta;

        getline(ss, nomeTreinador, ',');
        getline(ss, quantidadeStr, ',');

        // Buscar treinador pelo nome
        Treinador* treinador = nullptr;
        for (auto t : treinadores) {
            if (t->getNome() == nomeTreinador) {
                treinador = t;
                break;
            }
        }

        if (treinador != nullptr) {
            // Ler todos os atletas
            while (getline(ss, nomeAtleta, ',')) {
                // Buscar atleta pelo nome
                for (auto a : atletas) {
                    if (a->getNome() == nomeAtleta) {
                        treinador->vincularAtleta(a);
                        break;
                    }
                }
            }
        }
    }

    arquivo.close();
    cout << "Dados carregados com sucesso!" << endl;
}

#endif