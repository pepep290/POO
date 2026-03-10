import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.text.Normalizer;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class MotorDoJogo {

    private String palavraSecreta;
    private File arquivoDoUsuario;
    private List<String> dicionario; 
    
    // Instancia as variaveis de estatistica e inicializa todas com zero
    private int jogosFeitos = 0;
    private int partidasGanhas = 0;
    private int partidasPerdidas = 0;
    private int sequenciaAtual = 0;
    private int melhorSequencia = 0;

    // Construtor
    public MotorDoJogo(File arquivoUsuario) {
        this.arquivoDoUsuario = arquivoUsuario;
        carregarDicionario(); 
        carregarEstatisticas();
        sortearPalavra();
    }

    // Realiza a leitura do arquivo palavras.txt, formata o texto e carrega para memoria
    private void carregarDicionario() {
        dicionario = new ArrayList<>();
        File arquivoPalavras = new File("palavras.txt");

        if (arquivoPalavras.exists()) {
            try {
                // Efetua a leitura de todas as linhas
                List<String> linhas = Files.readAllLines(arquivoPalavras.toPath(), StandardCharsets.UTF_8);
                for (String linha : linhas) {
                    // Remove acentos e caracteres especiais, se houver
                    String palavraLimpa = removerAcentos(linha).toUpperCase().replaceAll("[^A-Z]", "");
                    // Aceita apenas as palavras de 5 caracteres
                    if (palavraLimpa.length() == 5) {
                        dicionario.add(palavraLimpa);
                    }
                }
            } catch (IOException e) {
                // Em caso de erro, exibe no terminal
                System.out.println("Erro dicionário: " + e.getMessage());
            }
        }
    }

    // Metodo que remove acentos e caracteres especiais se houverem
    private String removerAcentos(String str) {
        return Normalizer.normalize(str, Normalizer.Form.NFD).replaceAll("[^\\p{ASCII}]", "");
    }

    // Sorteia uma palavra secreta para iniciar uma nova partida
    public void iniciarNovaPartida() {
        sortearPalavra();
    }

    // Escolhe aleatoriamente uma palavra da lista carregada
    private void sortearPalavra() {
        if (!dicionario.isEmpty()) {
            this.palavraSecreta = dicionario.get(new Random().nextInt(dicionario.size()));
        }
    }

    // Verifica se a palavra da tentativa do usuario pertence ao dicionario
    public boolean palavraExiste(String palavra) {
        String tentativaLimpa = removerAcentos(palavra).toUpperCase().replaceAll("[^A-Z]", "");
        return dicionario.contains(tentativaLimpa); // Se existir retorna true, caso contrario retorna false
    }

    // Analisa a tentativa do usuario
    public int[] avaliarTentativa(String tentativa) {
        String tentativaLimpa = removerAcentos(tentativa).toUpperCase().replaceAll("[^A-Z]", "");
        int[] resultado = new int[5]; 
        char[] secretaChars = palavraSecreta.toCharArray();
        char[] tentativaChars = tentativaLimpa.toCharArray();
        // Vetor auxiliar para marcar quais letras ja foram acertadas
        boolean[] letraJaContabilizada = new boolean[5];

        // Verifica se os caracteres estao na posicao exata (Verde)
        for (int i = 0; i < 5; i++) {
            if (tentativaChars[i] == secretaChars[i]) {
                resultado[i] = 2; // Codigo para verde
                letraJaContabilizada[i] = true; // Marca como usada
            }
        }
        // Verifica se os caracteres estao na posicao errada (Amarelo)
        for (int i = 0; i < 5; i++) {
            // So faz a verificacao se nao for verde
            if (resultado[i] != 2) { 
                for (int j = 0; j < 5; j++) {
                    // Se a letra ainda nao foi usada e bate com a letra da tentativa
                    if (!letraJaContabilizada[j] && tentativaChars[i] == secretaChars[j]) {
                        resultado[i] = 1; // Codigo para amarelo
                        letraJaContabilizada[j] = true; // Marca como usada
                        break;
                    }
                }
            }
        }
        return resultado;
    }

    // Carrega as estatisticas do arquivo local
    private void carregarEstatisticas() {
        // Define os valores como zero por segurança antes de ler o arquivo
        this.jogosFeitos = 0;
        this.partidasGanhas = 0;
        this.partidasPerdidas = 0;
        this.sequenciaAtual = 0;
        this.melhorSequencia = 0;

        // Se for um jogador novo
        if (!arquivoDoUsuario.exists()) {
            return;
        }
        
        // Tenta abrir o arquivo do usuario para leitura
        try (BufferedReader br = new BufferedReader(new FileReader(arquivoDoUsuario))) {
            String linha = br.readLine();
            if (linha != null) {
                // Divide a linha utilizando ponto e virgula como separador
                String[] dados = linha.split(";");
                // Converte as strings lidas de volta para numeros inteiros
                this.jogosFeitos = Integer.parseInt(dados[0]);
                this.partidasGanhas = Integer.parseInt(dados[1]);
                this.partidasPerdidas = Integer.parseInt(dados[2]);
                // Verifica se o arquivo possui os campos de sequencia
                if (dados.length >= 5) {
                    this.sequenciaAtual = Integer.parseInt(dados[3]);
                    this.melhorSequencia = Integer.parseInt(dados[4]);
                }
            }
        } catch (Exception e) {
            // Em caso de erro, exibe no terminal
            e.printStackTrace();
        }
    }

    // Atualiza os contadores em caso de vitoria e salva no arquivo
    public void contabilizarVitoria() {
        this.jogosFeitos++;
        this.partidasGanhas++;
        this.sequenciaAtual++;
        // Atualiza o recorde pessoal se a sequencia (streak) atual for maior que a antiga registrada
        if (this.sequenciaAtual > this.melhorSequencia) 
            this.melhorSequencia = this.sequenciaAtual;
        salvarEstatisticas();
    }

    // Atualiza os contadores em caso de derrota e salva no arquivo
    public void contabilizarDerrota() {
        this.jogosFeitos++;
        this.partidasPerdidas++;
        this.sequenciaAtual = 0; // Zera a sequencia (streak) atual
        salvarEstatisticas();
    }

    // Salva os dados atuais no arquivo texto
    public void salvarEstatisticas() {
        try (PrintWriter pw = new PrintWriter(new FileWriter(arquivoDoUsuario))) {
            // Separa cada informacao utilizando ponto e virgula
            pw.println(jogosFeitos + ";" + partidasGanhas + ";" + partidasPerdidas + ";" + sequenciaAtual + ";" + melhorSequencia);
        } catch (IOException e) {
            // Em caso de erro, exibe no terminal
            e.printStackTrace();
        }
    }

    // Getters e setters
    public String getPalavraSecreta() { return palavraSecreta; }
    public void setPalavraSecreta(String p) { this.palavraSecreta = p; }
    public int getJogosFeitos() { return jogosFeitos; }
    public int getPartidasGanhas() { return partidasGanhas; }
    public int getPartidasPerdidas() { return partidasPerdidas; }
    public int getSequenciaAtual() { return sequenciaAtual; }
    public int getMelhorSequencia() { return melhorSequencia; }
}