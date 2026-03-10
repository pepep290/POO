import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.PlainDocument;

public class CampoTextoLimitado extends PlainDocument {
    
    // Armazena o número máximo de caracteres permitidos
    private final int limit;

    // Construtor
    public CampoTextoLimitado(int limit) {
        this.limit = limit;
    }

    // Metodo que valida o tamanho do texto antes de inserir
    @Override
    public void insertString(int offset, String str, AttributeSet attr) throws BadLocationException {
        // Se nao houver texto
        if (str == null) return;

        // Permite a insercao do texto caso nao ultrapasse o limite definido
        if ((getLength() + str.length()) <= limit) {
            super.insertString(offset, str, attr);
        }
    }
}