import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javax.swing.*;
import javax.swing.border.EmptyBorder;

public class TelaJogo extends JFrame {

    // Componentes da interface
    private JTextField[][] tabuleiro = new JTextField[6][5]; // Matriz de 6 tentativas x 5 letras
    private JLabel lblJogosFeitos, lblPartidasGanhas, lblPartidasPerdidas;
    private JLabel lblTentativas, lblSequenciaAtual, lblMelhorSequencia;
    
    // Mapa do teclado virtual
    private Map<String, JButton> mapTeclado = new HashMap<>();

    private MotorDoJogo motor;
    private int linhaAtual = 0; // Indica a linha de tentativa que o usuario esta
    private String nomeUsuarioAtual;
    private boolean jogoFinalizado = false;

    // Cores e constantes para design
    private final Color COR_FUNDO = new Color(99, 107, 47); 
    private final Color COR_SIDEBAR = new Color(70, 76, 33); 
    private final Color COR_TOPO = new Color(50, 50, 50);
    private final Color COR_TECLA_PADRAO = new Color(35, 35, 35); 
    private final Color COR_TECLA_ERRADA = Color.GRAY; 
    private final int LARGURA_BARRA_LATERAL = 220; 
    private final int TAMANHO_QUADRADO = 65;       

    // Construtor
    public TelaJogo(File arquivoUsuario, String nomeUsuario) {
        super("Termo - Jogador: " + nomeUsuario); 
        this.nomeUsuarioAtual = nomeUsuario;
        this.motor = new MotorDoJogo(arquivoUsuario);

        configurarJanela();
        inicializarInterface();
        
        // Foca na primeira celula
        SwingUtilities.invokeLater(() -> tabuleiro[0][0].requestFocus());
    }

    // Realiza as configuracoes iniciais da janela
    private void configurarJanela() {
        setDefaultCloseOperation(EXIT_ON_CLOSE); // Tela Cheia
        setExtendedState(JFrame.MAXIMIZED_BOTH); // Centraliza o painel
        setLayout(new BorderLayout());
    }

    // Inicializa a interface do programa
    private void inicializarInterface() {
        add(criarPainelEstatisticas(), BorderLayout.EAST);
        add(criarTabuleiro(), BorderLayout.CENTER);
        add(criarPainelBotoesSuperiores(), BorderLayout.NORTH);
        add(criarTecladoVirtual(), BorderLayout.SOUTH);
    }

    // Cria o painel de estatisticas em tempo real
    private JPanel criarPainelEstatisticas() {
        JPanel panel = new JPanel();
        panel.setBackground(COR_SIDEBAR); 
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
        panel.setPreferredSize(new Dimension(LARGURA_BARRA_LATERAL, 0));
        panel.setBorder(new EmptyBorder(30, 15, 20, 15)); 

        JLabel lblTitulo = new JLabel("Estatísticas");
        lblTitulo.setFont(new Font("Arial", Font.BOLD, 28));
        lblTitulo.setForeground(Color.WHITE);
        lblTitulo.setAlignmentX(Component.CENTER_ALIGNMENT);

        // Inicializa labels
        lblTentativas = criarLabelStats("Tentativas: 0/6", new Color(255, 255, 200));
        lblJogosFeitos = criarLabelStats("Jogos: " + motor.getJogosFeitos(), Color.WHITE);
        lblPartidasGanhas = criarLabelStats("Vitórias: " + motor.getPartidasGanhas(), Color.WHITE);
        lblPartidasPerdidas = criarLabelStats("Derrotas: " + motor.getPartidasPerdidas(), Color.WHITE);
        lblSequenciaAtual = criarLabelStats("Seq. Atual: " + motor.getSequenciaAtual(), new Color(255, 140, 0));
        lblMelhorSequencia = criarLabelStats("Melhor Seq: " + motor.getMelhorSequencia(), new Color(255, 215, 0));

        // Adiciona ao painel com espaçamento
        panel.add(lblTitulo);
        panel.add(Box.createVerticalStrut(20));
        panel.add(lblTentativas);
        panel.add(Box.createVerticalStrut(20));
        panel.add(lblJogosFeitos);
        panel.add(Box.createVerticalStrut(10));
        panel.add(lblPartidasGanhas);
        panel.add(Box.createVerticalStrut(10));
        panel.add(lblPartidasPerdidas);
        panel.add(Box.createVerticalStrut(20));
        panel.add(lblSequenciaAtual);
        panel.add(Box.createVerticalStrut(10));
        panel.add(lblMelhorSequencia);
        panel.add(Box.createVerticalGlue());

        return panel;
    }

    // Cria o tabuleiro do jogo
    private JPanel criarTabuleiro() {
        JPanel panelCentro = new JPanel(new GridBagLayout());
        panelCentro.setBackground(COR_FUNDO);
        
        JPanel panelGrade = new JPanel(new GridLayout(6, 5, 8, 8)); 
        panelGrade.setBackground(COR_FUNDO);
        
        Dimension dimQuad = new Dimension(TAMANHO_QUADRADO, TAMANHO_QUADRADO);

        // Cria as 30 celulas de texto (6x5)
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 5; j++) {
                JTextField cell = criarCelulaTabuleiro(dimQuad);
                tabuleiro[i][j] = cell;
                panelGrade.add(cell);
            }
        }
        // Centraliza o tabuleiro na tela
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 0; gbc.gridy = 0; gbc.anchor = GridBagConstraints.CENTER;
        panelCentro.add(panelGrade, gbc);
        return panelCentro;
    }

    private JTextField criarCelulaTabuleiro(Dimension dim) {
        JTextField cell = new JTextField();
        cell.setDocument(new CampoTextoLimitado(1));
        cell.setPreferredSize(dim); 
        cell.setMinimumSize(dim);
        cell.setMaximumSize(dim);
        cell.setHorizontalAlignment(JTextField.CENTER);
        cell.setFont(new Font("Arial", Font.BOLD, 36)); 
        cell.setBackground(Color.LIGHT_GRAY);
        cell.setBorder(BorderFactory.createLineBorder(Color.BLACK, 2));
        cell.setEditable(false); 
        cell.setFocusable(true);

        // Captura os dados digitados
        cell.addKeyListener(new KeyAdapter() {
            public void keyTyped(KeyEvent e) { 
                if (Character.isLetter(e.getKeyChar())) 
                    digitarLetra(String.valueOf(e.getKeyChar()).toUpperCase()); 
            }
            public void keyPressed(KeyEvent e) { 
                if (e.getKeyCode() == KeyEvent.VK_BACK_SPACE)
                    apagarLetra(); 
                if (e.getKeyCode() == KeyEvent.VK_ENTER)
                    processarTentativa(); 
            }
        });
        return cell;
    }

    // Cria os botoes da barra superior (Nova Partida e Sair do Jogo)
    private JPanel criarPainelBotoesSuperiores() {
        JPanel panel = new JPanel(new FlowLayout());
        panel.setBackground(COR_TOPO); 
        panel.setPreferredSize(new Dimension(getWidth(), 60));

        // Botao de Nova Partida
        JButton btnNovaPartida = new JButton("Nova Partida");
        btnNovaPartida.setFont(new Font("Arial", Font.BOLD, 16));
        btnNovaPartida.addActionListener(e -> acaoNovaPartida());

        // Botao de Sair do Jogo
        JButton btnSair = new JButton("Sair do Jogo");
        btnSair.setFont(new Font("Arial", Font.BOLD, 16));
        btnSair.addActionListener(e -> confirmarSaida());

        panel.add(btnNovaPartida);
        panel.add(Box.createHorizontalStrut(20)); 
        panel.add(btnSair);
        return panel;
    }

    // Cria o teclado virtual do jogo
    private JPanel criarTecladoVirtual() {
        JPanel panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
        panel.setBackground(COR_FUNDO); 
        panel.setBorder(new EmptyBorder(10, 10, 40, 10)); 

        // Posicionas as letras nas linhas do teclado virtual
        String[] linha1 = {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"};
        String[] linha2 = {"A", "S", "D", "F", "G", "H", "J", "K", "L"}; 
        String[] linha3 = {"Z", "X", "C", "V", "B", "N", "M"}; 

        panel.add(criarLinhaTeclas(linha1, false, false));
        panel.add(Box.createVerticalStrut(5)); 
        panel.add(criarLinhaTeclas(linha2, false, true)); 
        panel.add(Box.createVerticalStrut(5));
        panel.add(criarLinhaTeclas(linha3, true, false)); 
        return panel;
    }

    // Metodo auxiliar para criar as linhas de teclas
    private JPanel criarLinhaTeclas(String[] chaves, boolean comEnter, boolean comBackspace) {
        JPanel linha = new JPanel(new FlowLayout(FlowLayout.CENTER, 6, 0));
        linha.setOpaque(false);
        
        if (comEnter) {
            JButton btn = criarBotaoTeclado("ENTER", 100); 
            btn.addActionListener(e -> processarTentativa());
            linha.add(btn);
        }
        for (String chave : chaves) {
            JButton btn = criarBotaoTeclado(chave, 60);
            btn.addActionListener(e -> digitarLetra(chave));
            mapTeclado.put(chave, btn); 
            linha.add(btn);
        }
        if (comBackspace) {
            JButton btn = criarBotaoTeclado("<-", 80); 
            btn.addActionListener(e -> apagarLetra());
            linha.add(btn);
        }
        return linha;
    }

    // Metodo auxiliar para criar os botoes do teclado
    private JButton criarBotaoTeclado(String texto, int largura) {
        JButton btn = new JButton(texto);
        btn.setPreferredSize(new Dimension(largura, 65)); 
        btn.setFont(new Font("Arial", Font.BOLD, 20)); 
        btn.setBackground(COR_TECLA_PADRAO); 
        btn.setForeground(Color.WHITE);
        btn.setFocusable(false); 
        btn.setBorder(BorderFactory.createLineBorder(Color.BLACK));
        return btn;
    }

    // Inicia uma nova partida
    private void acaoNovaPartida() {
        // Se ja houver uma partida em andamento, verifica se o usuario realmente quer iniciar outra
        if (!jogoFinalizado) {
             int escolha = JOptionPane.showConfirmDialog(this, 
                 "Caso inicie uma nova partida, a partida atual será perdida (DERROTA).\nDeseja fazer isso?", 
                 "Abandonar?", JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE);
             
             if (escolha == JOptionPane.YES_OPTION) {
                 motor.contabilizarDerrota();
                 atualizarPlacarVisual();
                 reiniciarInterface();
             }
        } 
        // Se nao houver uma partida em andamento, inicia outra
        else {
            reiniciarInterface();
        }
    }

    // Verifica se o jogador quer mesmo sair da partida
    private void confirmarSaida() {
        String msg = "<html><body style='width: 250px; text-align: center'>" +
                "<h1 style='color: #636B2F'>Suas Estatísticas</h1><hr>" +
                "<h2>Jogos: " + motor.getJogosFeitos() + "</h2>" +
                "<h2 style='color: green'>Vitórias: " + motor.getPartidasGanhas() + "</h2>" +
                "<h2 style='color: red'>Derrotas: " + motor.getPartidasPerdidas() + "</h2><br>" +
                "<b>Deseja realmente sair e voltar ao menu?</b></body></html>";

        int escolha = JOptionPane.showConfirmDialog(this, msg, "Sair do Jogo?", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE);
        if (escolha == JOptionPane.YES_OPTION) 
            voltarAoMenu();
    }

    // Administra a volta do jogador ao menu
    private void voltarAoMenu() {
        // Se houver uma partida em andamento, salva o status da partida
        if (!jogoFinalizado) {
            List<String> tentativas = new ArrayList<>();
            for (int l = 0; l < linhaAtual; l++) {
                StringBuilder sb = new StringBuilder();
                for(int c=0; c<5; c++) sb.append(tabuleiro[l][c].getText());
                tentativas.add(sb.toString());
            }
            Salvamento.salvarJogo(nomeUsuarioAtual, motor.getPalavraSecreta(), tentativas);
        } 
        else {
            Salvamento.apagarJogoSalvo(nomeUsuarioAtual);
        }
        new TelaMenuPrincipal(new Autenticacao().getArquivoStats(nomeUsuarioAtual), nomeUsuarioAtual).setVisible(true);
        this.dispose();
    }

    // Metodo auxiliar para reiniciar a interface da partida
    private void reiniciarInterface() {
        linhaAtual = 0;
        jogoFinalizado = false;
        Salvamento.apagarJogoSalvo(nomeUsuarioAtual);
        lblTentativas.setText("Tentativas: 0/6");
        
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 5; j++) {
                tabuleiro[i][j].setText("");
                tabuleiro[i][j].setBackground(Color.LIGHT_GRAY);
                tabuleiro[i][j].setForeground(Color.BLACK);
            }
        }
        for (JButton btn : mapTeclado.values()) {
            btn.setBackground(COR_TECLA_PADRAO); 
            btn.setForeground(Color.WHITE);
        }
        tabuleiro[0][0].requestFocus();
        motor.iniciarNovaPartida();
    }

    // Insere uma letra na proxima celula vazia da linha atual
    private void digitarLetra(String letra) {
        if(jogoFinalizado) return;
        for (int j = 0; j < 5; j++) {
            if (tabuleiro[linhaAtual][j].getText().isEmpty()) {
                tabuleiro[linhaAtual][j].setText(letra);
                return;
            }
        }
    }

    // Remove a ultima letra digitada na linha atual
    private void apagarLetra() {
        if(jogoFinalizado) return;
        for (int j = 4; j >= 0; j--) {
            if (!tabuleiro[linhaAtual][j].getText().isEmpty()) {
                tabuleiro[linhaAtual][j].setText("");
                return;
            }
        }
    }

    // Analisa a tentativa e atualiza as cores
    private void processarTentativa() {
        if (jogoFinalizado) 
            return;

        // Constroi a string da tentativa lendo cada celula individualmente
        StringBuilder sb = new StringBuilder();
        for (int j = 0; j < 5; j++) {
            String letra = tabuleiro[linhaAtual][j].getText();
            // Aviso caso tenha espaços em branco
            if (letra.isEmpty()) {
                JOptionPane.showMessageDialog(this, "Preencha todas as letras!");
                return;
            }
            sb.append(letra.toUpperCase());
        }
        String tentativa = sb.toString();

        // Verifica se a tentativa existe no dicionario
        if (!motor.palavraExiste(tentativa)) {
            JOptionPane.showMessageDialog(this, "Palavra inválida!");
            return;
        }

        // Avalia a cor de cada letra da tentativa
        int[] resultados = motor.avaliarTentativa(tentativa);
        atualizarCoresInterface(tentativa, resultados);
        
        // Verifica vitoria
        boolean venceu = true;
        for (int cor : resultados) 
            if (cor != 2) 
                venceu = false;
        
        // Aumenta o contador de tentativas no painel de estatisticas
        lblTentativas.setText("Tentativas: " + (linhaAtual + 1) + "/6");

        if (venceu) { // Se o jogador venceu
            motor.contabilizarVitoria();
            atualizarPlacarVisual();
            finalizarPartida(true); 
        } 
        else if (linhaAtual == 5) { // Se era a ultima tentativa e nao venceu
            motor.contabilizarDerrota();
            atualizarPlacarVisual();
            finalizarPartida(false); 
        } 
        else { // Se nao era a ultima tentativa, mas ainda nao acertou
            linhaAtual++;
        }
    }

    // Finalizacao de uma partida
    private void finalizarPartida(boolean vitoria) {
        jogoFinalizado = true;
        Salvamento.apagarJogoSalvo(nomeUsuarioAtual);
        
        String msg = vitoria ? "Parabéns! Você acertou a palavra!" : "Que pena! A palavra era: " + motor.getPalavraSecreta();
        int tipo = vitoria ? JOptionPane.INFORMATION_MESSAGE : JOptionPane.ERROR_MESSAGE;

        Object[] opcoes = {"Voltar ao Menu", "Nova Partida"};
        int escolha = JOptionPane.showOptionDialog(this, msg, vitoria ? "Vitória!" : "Fim de Jogo", 
                JOptionPane.YES_NO_OPTION, tipo, null, opcoes, opcoes[0]);

        if (escolha == 0)  // Se quiser voltar ao menu
            voltarAoMenu();
        else if (escolha == 1) // Se quiser jogar novamente
            reiniciarInterface();
    }

    // Reconstroi o tabuleiro em caso de retorno para partida em andamento
    public void restaurarPartida(List<String> dadosSalvos) {
        if (dadosSalvos == null || dadosSalvos.isEmpty()) return;
        motor.setPalavraSecreta(dadosSalvos.get(0));

        // Refaz as tentativas salvas
        for (int i = 1; i < dadosSalvos.size(); i++) {
            String tentativa = dadosSalvos.get(i);

            // Preenche o texto visualmente
            for (int c = 0; c < 5; c++) 
                tabuleiro[linhaAtual][c].setText(String.valueOf(tentativa.charAt(c)));
    
            // Reavalia as cores das palavras
            int[] resultados = motor.avaliarTentativa(tentativa);
            atualizarCoresInterface(tentativa, resultados);
            linhaAtual++;
        }
        lblTentativas.setText("Tentativas: " + linhaAtual + "/6");
    }

    // Colore o tabuleiro e o teclado virtual de acordo com o resultado
    private void atualizarCoresInterface(String tentativa, int[] resultados) {
        for (int j = 0; j < 5; j++) {
            JTextField cell = tabuleiro[linhaAtual][j];
            cell.setForeground(Color.BLACK);
            Color cor = (resultados[j] == 2) ? Color.GREEN : (resultados[j] == 1) ? Color.YELLOW : Color.GRAY;
            cell.setBackground(cor);
            
            String letra = String.valueOf(tentativa.charAt(j));
            JButton btn = mapTeclado.get(letra);
            if (btn != null) {
                Color atual = btn.getBackground();
                if (cor == Color.GREEN) {
                    btn.setBackground(Color.GREEN);
                    btn.setForeground(Color.BLACK);
                } else if (cor == Color.YELLOW && atual != Color.GREEN) {
                    btn.setBackground(Color.YELLOW);
                    btn.setForeground(Color.BLACK);
                } else if (cor == Color.GRAY && atual != Color.GREEN && atual != Color.YELLOW) {
                    btn.setBackground(COR_TECLA_ERRADA);
                    btn.setForeground(Color.WHITE);
                }
            }
        }
    }

    // Cria a label de estatisticas
    private JLabel criarLabelStats(String texto, Color cor) {
        JLabel lbl = new JLabel(texto);
        lbl.setFont(new Font("Arial", Font.BOLD, 22)); 
        lbl.setForeground(cor);
        lbl.setAlignmentX(Component.CENTER_ALIGNMENT); 
        return lbl;
    }

    // Atualiza o placar visual de estatisticas
    private void atualizarPlacarVisual() {
        lblJogosFeitos.setText("Jogos: " + motor.getJogosFeitos());
        lblPartidasGanhas.setText("Vitórias: " + motor.getPartidasGanhas());
        lblPartidasPerdidas.setText("Derrotas: " + motor.getPartidasPerdidas());
        lblSequenciaAtual.setText("Seq. Atual: " + motor.getSequenciaAtual());
        lblMelhorSequencia.setText("Melhor Seq: " + motor.getMelhorSequencia());
    }
}