import javax.swing.*;

public class Main {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            // Instancia a classe TelaLogin
            TelaLogin login = new TelaLogin();
            login.setVisible(true); // Mostra a tela de login para iniciar o programa
        });
    }
}