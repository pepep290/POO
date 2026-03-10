import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class Salvamento {

    // Salva o status atual da partida em um arquivo temporario para permitir que o usuario retorne
    public static void salvarJogo(String usuario, String palavraSecreta, List<String> tentativas) {
        // Cria um arquivo temporario
        File arquivo = new File("temp_" + usuario + ".txt");
        try (PrintWriter pw = new PrintWriter(new FileWriter(arquivo))) {
            pw.println(palavraSecreta); // Salva a palavra secreta na primeira linha
            for (String tentativa : tentativas) {
                pw.println(tentativa); // Salva todas as tentativas nas linhas seguintes
            }
        } catch (IOException e) {
            // Em caso de erro, exibe no terminal
            e.printStackTrace();
        }
    }

    // Carrega um jogo que estava em andamento
    public static List<String> carregarJogo(String usuario) {
        List<String> dados = new ArrayList<>();
        File arquivo = new File("temp_" + usuario + ".txt");

        // Se o arquivo temporario nao existe, nao havia jogo em andamento
        if (!arquivo.exists()) 
            return null;

        try (BufferedReader br = new BufferedReader(new FileReader(arquivo))) {
            String linha;
            // Faz a leitura do arquivo temporario
            while ((linha = br.readLine()) != null) {
                // Adiciona os dados lidos na lista
                dados.add(linha);
            }
        } catch (IOException e) {
            // Em caso de erro, exibe no terminal
            e.printStackTrace();
        }
        return dados;
    }

    // Verifica se existe um arquivo de save temporario
    public static boolean temJogoSalvo(String usuario) {
        return new File("temp_" + usuario + ".txt").exists();
    }

    // Exclui o arquivo de save temporario
    public static void apagarJogoSalvo(String usuario) {
        File arquivo = new File("temp_" + usuario + ".txt");
        if (arquivo.exists()) {
            arquivo.delete();
        }
    }
}