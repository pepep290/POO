import java.awt.*;
import java.io.File;
import javax.swing.*;

public class TelaLogin extends JFrame {

    private Autenticacao auth = new Autenticacao();
    private final Color COR_FUNDO = new Color(99, 107, 47);
    
    // Variaveis de navegacao entre as paginas
    private CardLayout cardLayout;
    private JPanel painelPrincipal;

    // Construtor
    public TelaLogin() {
        super("Termo: Tela Inicial");
        configurarJanela();
        inicializarInterface();
    }

    // Realiza as configuracoes iniciais da janela
    private void configurarJanela() {
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setExtendedState(JFrame.MAXIMIZED_BOTH); // Tela Cheia
        setLayout(new BorderLayout()); // Cria layout de borda
    }

    // Inicializa a interface do programa
    private void inicializarInterface() {
        // Configura o gerenciador de telas
        cardLayout = new CardLayout();
        painelPrincipal = new JPanel(cardLayout);

        // Cria e adiciona as 3 telas necessarias para os botoes
        painelPrincipal.add(criarPainelCapa(), "Tela Inicial");
        painelPrincipal.add(criarPainelLogin(), "Login");
        painelPrincipal.add(criarPainelCadastro(), "Cadastro");

        // Define a tela inicial e adiciona a janela
        cardLayout.show(painelPrincipal, "Tela Inicial");
        add(painelPrincipal, BorderLayout.CENTER);
    }

    // Tela Inicial
    private JPanel criarPainelCapa() {
        JPanel panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
        panel.setBackground(COR_FUNDO);

        JLabel termo = new JLabel("TERMO");
        termo.setForeground(Color.WHITE);
        termo.setFont(new Font("Arial", Font.BOLD, 100));
        termo.setAlignmentX(Component.CENTER_ALIGNMENT);

        // Botao de Criar conta
        JButton btnCadastro = criarBotaoEstilizado("Criar Conta");
        btnCadastro.addActionListener(e -> cardLayout.show(painelPrincipal, "Cadastro"));

        // Botao de Entrar em conta ja existente
        JButton btnLogin = criarBotaoEstilizado("Entrar");
        btnLogin.addActionListener(e -> cardLayout.show(painelPrincipal, "Login"));

        // Botao de Sair do programa
        JButton btnSair = criarBotaoEstilizado("Sair");
        btnSair.addActionListener(e -> System.exit(0));

        // Layout
        panel.add(Box.createVerticalStrut(100));
        panel.add(termo);
        panel.add(Box.createVerticalGlue());
        panel.add(btnCadastro);
        panel.add(Box.createVerticalStrut(30));
        panel.add(btnLogin);
        panel.add(Box.createVerticalStrut(30));
        panel.add(btnSair);
        panel.add(Box.createVerticalGlue());

        return panel;
    }

    // Tela de Login
    private JPanel criarPainelLogin() {
        JPanel panel = new JPanel(new GridBagLayout());
        panel.setBackground(COR_FUNDO);
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(10, 10, 10, 10);

        // Campos para o usuario preencher
        JTextField txtUsuario = new JTextField();
        JPasswordField txtSenha = new JPasswordField();
        adicionarCampoFormulario(panel, gbc, "Usuário:", txtUsuario, 0);
        adicionarCampoFormulario(panel, gbc, "Senha:", txtSenha, 1);

        // Botoes de entrar na conta
        JButton btnEntrar = new JButton("Entrar");
        estilizarBotaoPequeno(btnEntrar);
        btnEntrar.addActionListener(e -> acaoFazerLogin(txtUsuario, txtSenha));

        // Botao de voltar para a tela inicial
        JButton btnVoltar = new JButton("Voltar");
        estilizarBotaoPequeno(btnVoltar);
        btnVoltar.addActionListener(e -> {
            limparCampos(txtUsuario, txtSenha); // Limpa as informacoes escritas pelo usuario
            cardLayout.show(painelPrincipal, "Tela Inicial");
        });

        adicionarBotoesRodape(panel, gbc, btnEntrar, btnVoltar, 2);
        return panel;
    }

    // Tela de Cadastro
    private JPanel criarPainelCadastro() {
        JPanel panel = new JPanel(new GridBagLayout());
        panel.setBackground(COR_FUNDO);
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(10, 10, 10, 10);

        // Campos para o usuario preencher
        JTextField txtNovoUsuario = new JTextField();
        JPasswordField txtNovaSenha = new JPasswordField();
        adicionarCampoFormulario(panel, gbc, "Novo Usuário:", txtNovoUsuario, 0);
        adicionarCampoFormulario(panel, gbc, "Nova Senha:", txtNovaSenha, 1);

        // Botao de Confirmar
        JButton btnConfirmar = new JButton("Confirmar");
        estilizarBotaoPequeno(btnConfirmar);
        btnConfirmar.addActionListener(e -> acaoFazerCadastro(txtNovoUsuario, txtNovaSenha));

        // Botao de Voltar para a tela inicial
        JButton btnVoltar = new JButton("Voltar");
        estilizarBotaoPequeno(btnVoltar);
        btnVoltar.addActionListener(e -> {
            limparCampos(txtNovoUsuario, txtNovaSenha); // Limpa as informacoes escritas pelo usuario
            cardLayout.show(painelPrincipal, "Tela Inicial");
        });

        adicionarBotoesRodape(panel, gbc, btnConfirmar, btnVoltar, 2);
        return panel;
    }

    // Controle da logica de login
    private void acaoFazerLogin(JTextField user, JPasswordField pass) {
        String nome = user.getText();
        String senha = new String(pass.getPassword());

        // Se for possivel entrar na conta com o nome e senha que foram fornecidos
        if (auth.autenticar(nome, senha)) {
            // Obtem o arquivo de estatisticas do usuario
            File arqStats = auth.getArquivoStats(nome);
            new TelaMenuPrincipal(arqStats, nome).setVisible(true); // Abre o menu principal do jogo e fecha a tela de login
            this.dispose();
        }
        // Se nao foi possivel entrar 
        else {
            JOptionPane.showMessageDialog(this, "Usuário ou senha incorretos!", "Erro", JOptionPane.ERROR_MESSAGE);
        }
    }

    // Controle da logica de cadastro
    private void acaoFazerCadastro(JTextField user, JPasswordField pass) {
        String nome = user.getText();
        String senha = new String(pass.getPassword());

        // Se for criar uma conta para o usuario com o nome e senha que foram fornecidos
        if (auth.cadastrar(nome, senha)) {
            JOptionPane.showMessageDialog(this, "Usuário criado com sucesso!");
            // Providencia o arquivo de estatisticas para o usuario
            File arqStats = auth.getArquivoStats(nome);
            new TelaMenuPrincipal(arqStats, nome).setVisible(true); // Abre o menu principal do jogo e fecha a tela de login
            this.dispose();
        } 
        // Se nao for possivel criar uma conta para o usuario
        else {
            JOptionPane.showMessageDialog(this, "Erro: Usuário já existe ou campos vazios!", "Erro", JOptionPane.ERROR_MESSAGE);
        }
    }

    // Metodo auxiliar de design para botao
    private JButton criarBotaoEstilizado(String texto) {
        JButton btn = new JButton(texto);
        btn.setFont(new Font("Arial", Font.BOLD, 30));
        btn.setPreferredSize(new Dimension(300, 80));
        btn.setMaximumSize(new Dimension(300, 80));
        btn.setAlignmentX(Component.CENTER_ALIGNMENT);
        btn.setForeground(COR_FUNDO);
        return btn;
    }

    // Metodo auxiliar de design para botao
    private void estilizarBotaoPequeno(JButton btn) {
        btn.setPreferredSize(new Dimension(200, 60));
        btn.setForeground(COR_FUNDO);
        btn.setFont(new Font("Arial", Font.BOLD, 22));
    }

    // Metodo auxiliar de design para adicionar campo para ser preenchido pelo usuario
    private void adicionarCampoFormulario(JPanel panel, GridBagConstraints gbc, String label, JComponent campo, int y) {
        gbc.gridx = 0; gbc.gridy = y;
        JLabel lbl = new JLabel(label);
        lbl.setFont(new Font("Arial", Font.BOLD, 40));
        lbl.setForeground(Color.WHITE);
        panel.add(lbl, gbc);

        gbc.gridx = 1;
        campo.setPreferredSize(new Dimension(300, 50));
        campo.setFont(new Font("Arial", Font.BOLD, 30));
        panel.add(campo, gbc);
    }

    // Organiza os botoes de voltar e entrar
    private void adicionarBotoesRodape(JPanel panel, GridBagConstraints gbc, JButton btn1, JButton btn2, int y) {
        gbc.gridx = 0; gbc.gridy = y; gbc.gridwidth = 2;
        gbc.anchor = GridBagConstraints.CENTER;
        JPanel botoes = new JPanel(new FlowLayout());
        botoes.setOpaque(false);
        botoes.add(btn1);
        botoes.add(btn2);
        panel.add(botoes, gbc);
    }
    
    // Metodo auxiliar para limpar os campos de texto
    private void limparCampos(JTextField t1, JTextField t2) {
        t1.setText("");
        t2.setText("");
    }
}