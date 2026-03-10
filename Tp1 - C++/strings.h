#include <string>
#include <iostream>
#include <limits>
#include <unordered_map>

using namespace std;

string removerAcentos(const string& s) {
    static const unordered_map<char32_t, char> mapa = {
        // a
        {U'á', 'a'}, {U'à', 'a'}, {U'â', 'a'}, {U'ã', 'a'}, {U'ä', 'a'}, {U'Á', 'a'}, {U'À', 'a'}, {U'Â', 'a'}, {U'Ã', 'a'}, {U'Ä', 'a'},
        // e
        {U'é', 'e'}, {U'è', 'e'}, {U'ê', 'e'}, {U'ë', 'e'}, {U'É', 'e'}, {U'È', 'e'}, {U'Ê', 'e'}, {U'Ë', 'e'},
        // i
        {U'í', 'i'}, {U'ì', 'i'}, {U'î', 'i'}, {U'ï', 'i'}, {U'Í', 'i'}, {U'Ì', 'i'}, {U'Î', 'i'}, {U'Ï', 'i'},
        // o
        {U'ó', 'o'}, {U'ò', 'o'}, {U'ô', 'o'}, {U'õ', 'o'}, {U'ö', 'o'}, {U'Ó', 'o'}, {U'Ò', 'o'}, {U'Ô', 'o'}, {U'Õ', 'o'}, {U'Ö', 'o'},
        // u
        {U'ú', 'u'}, {U'ù', 'u'}, {U'û', 'u'}, {U'ü', 'u'}, {U'Ú', 'u'}, {U'Ù', 'u'}, {U'Û', 'u'}, {U'Ü', 'u'},
        // c
        {U'ç', 'c'}, {U'Ç', 'c'}
    };

    string out;
    out.reserve(s.size());

    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = s[i];

        if (c < 128) {
            // ASCII normal
            out.push_back(c);
            i++;
        } else {
            // UTF-8 multibyte (2 bytes para acentos latinos)
            if (i + 1 < s.size()) {
                char32_t unicode = ((c & 0x1F) << 6) | (s[i+1] & 0x3F);
                auto it = mapa.find(unicode);
                if (it != mapa.end())
                    out.push_back(it->second);
                else {
                    // caractere desconhecido: ignora ou copia
                    // out.push_back('?');
                }
                i += 2;
            } else {
                i++; // byte perdido
            }
        }
    }

    return out;
}
#include <algorithm>
// #include <cctype>

string toLower(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c){ return tolower(c); });
    return result;
}

void limparBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

template <typename T>
T getStr() {
    T str;
    limparBuffer();
    cin >> str;
    return str;
}

template <typename T>
T getFormattedStr(){
    T str;

    // se proximo char for enter, limpa o buffer
    if (cin.peek() == '\n')
        limparBuffer();

    getline(cin, str);
    return toLower(removerAcentos(str));
}