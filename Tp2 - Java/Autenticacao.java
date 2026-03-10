import java.io.*;
import java.nio.file.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class Autenticacao {
    
    // Pasta onde os arquivos de login e estatisticas sao salvos
    private final String PASTA_DADOS = "dados";

    // Construtor
    public Autenticacao() {
        File diretorio = new File(PASTA_DADOS);
        // Verifica se a pasta dados existe, caso contrario, cria
        if (!diretorio.exists()) {
            diretorio.mkdir();
        }
    }

    // Retorna o arquivo de senha do usuario
    public File getArquivoSenha(String nome) {
        String nomeLimpo = nome.trim().toLowerCase();
        return new File(PASTA_DADOS, nomeLimpo + "_login.txt");
    }

    // Retorna o arquivo de estatisticas do usuario
    public File getArquivoStats(String nome) {
        String nomeLimpo = nome.trim().toLowerCase();
        return new File(PASTA_DADOS, nomeLimpo + "_stats.txt");
    }

    // Cadastra um novo usuario e inicializa as estatisticas com zero
    public boolean cadastrar(String nome, String senha) {
        if (nome == null || nome.trim().isEmpty() || senha == null || senha.trim().isEmpty()) {
            return false;
        }
        
        File arqSenha = getArquivoSenha(nome);
        File arqStats = getArquivoStats(nome);
        
        // Se o usuario ja existe, retorna falso
        if (arqSenha.exists()) {
            return false;
        }

        try {
            // Salva a senha
            PrintWriter pwSenha = new PrintWriter(new FileWriter(arqSenha));
            pwSenha.println(senha.trim());
            pwSenha.close();
            // Inicializa as estatisticas com zero
            PrintWriter pwStats = new PrintWriter(new FileWriter(arqStats));
            pwStats.println("0;0;0;0;0"); 
            pwStats.close();
            
            return true;
        } catch (IOException e) {
            // Em caso de erro, exibe no terminal
            e.printStackTrace();
            return false;
        }
    }

    // Verifica se o usuario e senha correspondem aos dados salvos
    public boolean autenticar(String nome, String senha) {
        if (nome == null || nome.trim().isEmpty() || senha == null || senha.trim().isEmpty()) {
            return false;
        }

        File arqSenha = getArquivoSenha(nome);
        
        // Verifica se o arquivo do usuario existe
        if (!arqSenha.exists()) {
            return false; // Se nao existe, retorna falso
        }

        try (BufferedReader br = new BufferedReader(new FileReader(arqSenha))) {
            String senhaSalva = br.readLine();
            // Verifica se a senha digitada esta correta
            if (senhaSalva != null && senhaSalva.trim().equals(senha.trim())) {
                return true; // Senha correta, retorna true
            }
        } catch (IOException e) {
            // Em caso de erro, exibe no terminal
            e.printStackTrace();
        }
        return false;
    }

    // Gera o ranking de top 5 melhores jogadores
    public List<String[]> getTop5Ranking() {
        List<String[]> listaJogadores = new ArrayList<>();
        File pasta = new File(PASTA_DADOS);
        
        // Filtra apenas arquivos que terminam com "_stats.txt"
        File[] arquivos = pasta.listFiles((dir, name) -> name.endsWith("_stats.txt"));

        if (arquivos != null) {
            for (File arq : arquivos) {
                try (BufferedReader br = new BufferedReader(new FileReader(arq))) {
                    String linha = br.readLine();
                    if (linha != null) {
                        String[] stats = linha.split(";");
                        
                        String nomeArquivo = arq.getName();
                        // Obtem o nome original do jogador a partir do nome do arquivo
                        String nomeJogador = nomeArquivo.replace("_stats.txt", "");
                        nomeJogador = nomeJogador.substring(0, 1).toUpperCase() + nomeJogador.substring(1);

                        // Adiciona na lista
                        listaJogadores.add(new String[]{
                            nomeJogador, stats[0], stats[1], stats[2], stats[3], stats[4]
                        });
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }

        // Ordena a lista de acordo com o numero de vitorias
        Collections.sort(listaJogadores, new Comparator<String[]>() {
            @Override
            public int compare(String[] o1, String[] o2) {
                int vit1 = Integer.parseInt(o1[2]);
                int vit2 = Integer.parseInt(o2[2]);
                // Ordena de maneira decrescente
                return Integer.compare(vit2, vit1);
            }
        });

        // Exibe apenas os 5 melhores colocados
        if (listaJogadores.size() > 5) {
            return listaJogadores.subList(0, 5);
        } 
        // Se houver menos de 5 jogadores no total, mostra todos possiveis
        else {
            return listaJogadores;
        }
    }
}