#include "analisador_lexico.h"

std::string nomeDoToken(TipoToken tipo) {
    switch (tipo) {
        case TipoToken::NUM:       return "NUM";
        case TipoToken::MAIS:      return "MAIS";
        case TipoToken::MENOS:     return "MENOS";
        case TipoToken::VEZES:     return "VEZES";
        case TipoToken::DIVIDIDO:  return "DIVIDIDO";
        case TipoToken::POTENCIA:  return "POTENCIA";
        case TipoToken::ABRE_PAR:  return "ABRE_PAR";
        case TipoToken::FECHA_PAR: return "FECHA_PAR";
        case TipoToken::FIM:       return "FIM";
    }
    return "?";
}

std::string simboloDoTerminal(TipoToken tipo) {
    switch (tipo) {
        case TipoToken::NUM:       return "num";
        case TipoToken::MAIS:      return "+";
        case TipoToken::MENOS:     return "-";
        case TipoToken::VEZES:     return "*";
        case TipoToken::DIVIDIDO:  return "/";
        case TipoToken::POTENCIA:  return "^";
        case TipoToken::ABRE_PAR:  return "(";
        case TipoToken::FECHA_PAR: return ")";
        case TipoToken::FIM:       return "$";
    }
    return "?";
}

namespace {

bool ehDigito(char c) {
    return c >= '0' && c <= '9';
}

bool ehEspaco(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

// Operadores e parênteses têm exatamente um caractere,
// então cada um deles já é um token completo.
bool ehOperadorOuParentese(char c, TipoToken& tipo) {
    switch (c) {
        case '+': tipo = TipoToken::MAIS;      return true;
        case '-': tipo = TipoToken::MENOS;     return true;
        case '*': tipo = TipoToken::VEZES;     return true;
        case '/': tipo = TipoToken::DIVIDIDO;  return true;
        case '^': tipo = TipoToken::POTENCIA;  return true;
        case '(': tipo = TipoToken::ABRE_PAR;  return true;
        case ')': tipo = TipoToken::FECHA_PAR; return true;
        default:  return false;
    }
}

// ---------------------------------------------------------------------------
// AFD que reconhece o token num.
//
//   Expressão regular:  num = digito+ ( '.' digito+ )?
//
//   início -> Q0 --digito--> Q1_INTEIRO --'.'--> Q2_PONTO --digito--> Q3_DECIMAL
//                            Q1_INTEIRO --digito--> Q1_INTEIRO
//                            Q3_DECIMAL --digito--> Q3_DECIMAL
//
//   Estados de aceitação: Q1_INTEIRO e Q3_DECIMAL.
//   Qualquer transição não listada leva ao estado MORTO.
//
//   O desenho do autômato está em diagrams/afd_num.dot.
// ---------------------------------------------------------------------------
enum class EstadoNum { Q0, Q1_INTEIRO, Q2_PONTO, Q3_DECIMAL, MORTO };

EstadoNum transicao(EstadoNum estado, char c) {
    switch (estado) {
        case EstadoNum::Q0:
            return ehDigito(c) ? EstadoNum::Q1_INTEIRO : EstadoNum::MORTO;
        case EstadoNum::Q1_INTEIRO:
            if (ehDigito(c)) return EstadoNum::Q1_INTEIRO;
            if (c == '.')    return EstadoNum::Q2_PONTO;
            return EstadoNum::MORTO;
        case EstadoNum::Q2_PONTO:
            return ehDigito(c) ? EstadoNum::Q3_DECIMAL : EstadoNum::MORTO;
        case EstadoNum::Q3_DECIMAL:
            return ehDigito(c) ? EstadoNum::Q3_DECIMAL : EstadoNum::MORTO;
        case EstadoNum::MORTO:
            return EstadoNum::MORTO;
    }
    return EstadoNum::MORTO;
}

bool ehEstadoDeAceitacao(EstadoNum estado) {
    return estado == EstadoNum::Q1_INTEIRO || estado == EstadoNum::Q3_DECIMAL;
}

// Caracteres fora da tabela ASCII ocupam mais de um byte em UTF-8.
// Esta função devolve o caractere inteiro que começa na posição i,
// para que a mensagem de erro mostre o símbolo certo (ex.: '−').
std::string caractereUtf8Em(const std::string& texto, size_t i) {
    size_t fim = i + 1;
    while (fim < texto.size() && (static_cast<unsigned char>(texto[fim]) & 0xC0) == 0x80) {
        fim++;
    }
    return texto.substr(i, fim - i);
}

void registrarErro(ResultadoLexico& resultado, const std::string& mensagem, int coluna) {
    resultado.sucesso = false;
    resultado.mensagemDeErro = mensagem;
    resultado.colunaDoErro = coluna;
}

} // namespace

ResultadoLexico analisarLexicamente(const std::string& expressao) {
    ResultadoLexico resultado{true, {}, "", 0};

    size_t i = 0;
    while (i < expressao.size()) {
        const char c = expressao[i];
        const int coluna = static_cast<int>(i) + 1;

        // 1) Espaços em branco apenas separam tokens.
        if (ehEspaco(c)) {
            i++;
            continue;
        }

        // 2) Operadores e parênteses.
        TipoToken tipoDoOperador;
        if (ehOperadorOuParentese(c, tipoDoOperador)) {
            resultado.tokens.push_back({tipoDoOperador, std::string(1, c), coluna});
            i++;
            continue;
        }

        // 3) Números: simula o AFD enquanto houver transição válida.
        if (ehDigito(c)) {
            EstadoNum estado = EstadoNum::Q0;
            size_t j = i;
            while (j < expressao.size()) {
                EstadoNum proximo = transicao(estado, expressao[j]);
                if (proximo == EstadoNum::MORTO) {
                    break;
                }
                estado = proximo;
                j++;
            }

            const std::string lexema = expressao.substr(i, j - i);

            // O AFD parou. Se parou em estado de aceitação, o número está completo.
            // O único estado de não aceitação alcançável aqui é Q2_PONTO
            // (leu "123." e o próximo caractere não é dígito).
            if (!ehEstadoDeAceitacao(estado)) {
                registrarErro(resultado,
                              "número malformado '" + lexema +
                              "': faltam dígitos depois do '.' (ex.: 3.0)",
                              static_cast<int>(j) + 1);
                return resultado;
            }

            resultado.tokens.push_back({TipoToken::NUM, lexema, coluna});
            i = j;
            continue;
        }

        // 4) Qualquer outro caractere é erro léxico.
        //    Alguns casos comuns recebem uma mensagem mais específica.
        if (c == ',') {
            registrarErro(resultado,
                          "',' não é separador decimal nesta linguagem; use '.' (ex.: 3.14)",
                          coluna);
        } else if (c == '.') {
            registrarErro(resultado,
                          "'.' inesperado: o separador decimal aparece uma única vez "
                          "e precisa de dígitos antes e depois (ex.: 0.5, 3.14)",
                          coluna);
        } else {
            registrarErro(resultado,
                          "caractere inválido '" + caractereUtf8Em(expressao, i) + "'",
                          coluna);
        }
        return resultado;
    }

    // Fim da entrada: acrescenta o marcador $ usado pelo analisador sintático.
    resultado.tokens.push_back({TipoToken::FIM, "$", static_cast<int>(expressao.size()) + 1});
    return resultado;
}
