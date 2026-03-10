import java.awt.*;
import java.io.File;
import java.util.List;
import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;

public class TelaMenuPrincipal extends JFrame {

    private File arquivoUsuario;
    private String nomeUsuario;
    
    private Autenticacao auth = new Autenticacao();

    // Constantes visuais
    private final Color COR_FUNDO = new Color(99, 107, 47);
    private final Font FONTE_BOTAO = new Font("Arial", Font.BOLD, 25);
    private final Dimension TAMANHO_BOTAO = new Dimension(500, 70);

    // Construtor
    public TelaMenuPrincipal(File arquivoUsuario, String nomeUsuario) {
        super("Termo - Menu Principal");
        this.arquivoUsuario = arquivoUsuario;
        this.nomeUsuario = nomeUsuario;
        configurarJanela();
        inicializarComponentes();
    }

    // Realiza as configuracoes iniciais da janela
    private void configurarJanela() {
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setExtendedState(JFrame.MAXIMIZED_BOTH); // Tela Cheia
        setLayout(new GridBagLayout()); // Centraliza o painel
        getContentPane().setBackground(COR_FUNDO);
    }

    // Inicializa os componentes da interface
    private void inicializarComponentes() {
        JPanel panelMenu = new JPanel();
        panelMenu.setLayout(new BoxLayout(panelMenu, BoxLayout.Y_AXIS));
        panelMenu.setOpaque(false);

        // Titulo de boas-vindas e nome do usuario
        JLabel lblTitulo = new JLabel("Bem-vindo, " + nomeUsuario + "!");
        lblTitulo.setFont(new Font("Arial", Font.BOLD, 50));
        lblTitulo.setForeground(Color.WHITE);
        lblTitulo.setAlignmentX(Component.CENTER_ALIGNMENT);

        // Botao Iniciar novo jogo
        JButton btnJogar = criarBotao("Iniciar Novo Jogo");
        btnJogar.addActionListener(e -> acaoIniciarNovoJogo());

        // Botao Voltar para partida em andamento
        JButton btnContinuar = criarBotao("Voltar para partida em andamento");
        btnContinuar.addActionListener(e -> acaoContinuarJogo());

        // Botao Ver estatisticas
        JButton btnStats = criarBotao("Ver Estatísticas");
        btnStats.addActionListener(e -> mostrarEstatisticasPessoais());

        // Botao Placar de Lideres
        JButton btnRanking = criarBotao("Placar de Líderes");
        btnRanking.addActionListener(e -> mostrarRankingTop5());

        // Botao de Logout
        JButton btnSair = criarBotao("Logout");
        btnSair.addActionListener(e -> {
            new TelaLogin().setVisible(true);
            this.dispose();
        });

        // Adicao ao Painel
        panelMenu.add(lblTitulo);
        panelMenu.add(Box.createVerticalStrut(60));
        panelMenu.add(btnJogar);
        panelMenu.add(Box.createVerticalStrut(20));
        panelMenu.add(btnContinuar);
        panelMenu.add(Box.createVerticalStrut(20));
        panelMenu.add(btnStats);
        panelMenu.add(Box.createVerticalStrut(20));
        panelMenu.add(btnRanking);
        panelMenu.add(Box.createVerticalStrut(20));
        panelMenu.add(btnSair);

        add(panelMenu);
    }

    // Metodo auxiliar para padronizar os botoes
    private JButton criarBotao(String texto) {
        JButton btn = new JButton(texto);
        btn.setFont(FONTE_BOTAO);
        btn.setPreferredSize(TAMANHO_BOTAO);
        btn.setMaximumSize(TAMANHO_BOTAO);
        btn.setAlignmentX(Component.CENTER_ALIGNMENT);
        btn.setBackground(Color.WHITE);
        btn.setForeground(COR_FUNDO);
        return btn;
    }

    // Administra a logica para iniciar uma nova partida
    private void acaoIniciarNovoJogo() {
        // Verifica se ja existe um jogo salvo antes de começar um novo
        if (Salvamento.temJogoSalvo(nomeUsuario)) {
            int escolha = JOptionPane.showConfirmDialog(this,
                    "Você tem uma partida em andamento!\nIniciar um novo jogo contará como DERROTA na atual.\nDeseja continuar?",
                    "Abandonar Partida?", JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE);
            
            if (escolha == JOptionPane.YES_OPTION) {
                // Se o usuario confirmou desistencia
                MotorDoJogo motorTemp = new MotorDoJogo(arquivoUsuario);
                motorTemp.contabilizarDerrota();
                Salvamento.apagarJogoSalvo(nomeUsuario);
                abrirTelaJogo();
            }
        }
        // Sem nao houver save pendente, inicia outra partida direto
        else {
            abrirTelaJogo();
        }
    }

    // Verifica o retorno para uma partida em andamento
    private void acaoContinuarJogo() {
        if (Salvamento.temJogoSalvo(nomeUsuario)) {
            // Carrega os dados do arquivo temporario
            List<String> dados = Salvamento.carregarJogo(nomeUsuario);
            int tentativasUsadas = dados.size() - 1; 
            
            int escolha = JOptionPane.showConfirmDialog(this, 
                "Retomar jogo com " + tentativasUsadas + "/6 tentativas usadas?",
                "Retomar Jogo", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE);
                
            if (escolha == JOptionPane.YES_OPTION) {
                TelaJogo jogo = new TelaJogo(arquivoUsuario, nomeUsuario);
                jogo.setVisible(true);
                jogo.restaurarPartida(dados); // Reconstroi o tabuleiro
                this.dispose();
            }
        } 
        else {
            JOptionPane.showMessageDialog(this, "Não há partida em andamento!");
        }
    }

    // Troca da tela do menu para a tela do jogo
    private void abrirTelaJogo() {
        new TelaJogo(arquivoUsuario, nomeUsuario).setVisible(true);
        this.dispose();
    }

    // Exibe uma janela com o desempenho do jogador logado
    private void mostrarEstatisticasPessoais() {
        MotorDoJogo motorTemp = new MotorDoJogo(arquivoUsuario);
        String mensagem = "<html><body style='width: 250px; text-align: center'>" +
                "<h1 style='color: #636B2F'>Suas Estatísticas</h1><hr>" +
                "<h2>Jogos: " + motorTemp.getJogosFeitos() + "</h2>" +
                "<h2 style='color: green'>Vitórias: " + motorTemp.getPartidasGanhas() + "</h2>" +
                "<h2 style='color: red'>Derrotas: " + motorTemp.getPartidasPerdidas() + "</h2><br>" +
                "<h3 style='color: #FF8C00'>Seq. Atual: " + motorTemp.getSequenciaAtual() + "</h3>" +
                "<h3 style='color: #FFD700'>Melhor Seq: " + motorTemp.getMelhorSequencia() + "</h3>" +
                "</body></html>";
        JOptionPane.showMessageDialog(this, mensagem, "Seu Histórico", JOptionPane.PLAIN_MESSAGE);
    }

    // Exibe o ranking dos 5 melhores jogadores em uma tabela organizada
    private void mostrarRankingTop5() {
        List<String[]> ranking = auth.getTop5Ranking();

        if (ranking.isEmpty()) {
            JOptionPane.showMessageDialog(this, "Ainda não há jogadores registrados!");
            return;
        }

        // Colunas da Tabela
        String[] colunas = {"#", "Nome", "Vitórias", "Jogos", "Derrotas", "Seq. Atual", "Melhor Seq."};
        String[][] dados = new String[ranking.size()][7];
        
        // Converte a lista de Strings para uma matriz compativel
        for (int i = 0; i < ranking.size(); i++) {
            String[] r = ranking.get(i);
            dados[i][0] = (i + 1) + "º"; // Posicao
            dados[i][1] = r[0]; // Nome
            dados[i][2] = r[2]; // Vitórias
            dados[i][3] = r[1]; // Jogos
            dados[i][4] = r[3]; // Derrotas
            dados[i][5] = r[4]; // Seq
            dados[i][6] = r[5]; // Melhor
        }

        // Cria o visual da tabela
        JTable tabela = new JTable(dados, colunas);
        tabela.setRowHeight(30);
        tabela.setFont(new Font("Arial", Font.PLAIN, 14));
        tabela.getTableHeader().setFont(new Font("Arial", Font.BOLD, 14));
        tabela.getTableHeader().setBackground(new Color(255, 215, 0));
        
        // Centraliza o texto
        DefaultTableCellRenderer centerRenderer = new DefaultTableCellRenderer();
        centerRenderer.setHorizontalAlignment(JLabel.CENTER);
        for(int x=0; x<7; x++){
            tabela.getColumnModel().getColumn(x).setCellRenderer(centerRenderer);
        }

        // Adiciona barra de rolagem
        JScrollPane scrollPane = new JScrollPane(tabela);
        scrollPane.setPreferredSize(new Dimension(650, 200)); // Tamanho ajustado para 5 linhas

        JPanel panelPopup = new JPanel(new BorderLayout());
        JLabel lblTop = new JLabel("TOP 5 JOGADORES (Por Vitórias)");
        lblTop.setFont(new Font("Arial", Font.BOLD, 18));
        lblTop.setHorizontalAlignment(SwingConstants.CENTER);
        lblTop.setBorder(BorderFactory.createEmptyBorder(0, 0, 10, 0));
        
        panelPopup.add(lblTop, BorderLayout.NORTH);
        panelPopup.add(scrollPane, BorderLayout.CENTER);

        JOptionPane.showMessageDialog(this, panelPopup, "Placar de Líderes", JOptionPane.PLAIN_MESSAGE);
    }
}