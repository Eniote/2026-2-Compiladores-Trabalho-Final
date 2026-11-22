#include "analisador_sintatico.h"

#include <algorithm>

#include "gramatica.h"
#include "tabela_ll1.h"

namespace {

std::string pilhaComoTexto(const std::vector<Simbolo>& pilha) {
    std::string texto;
    for (const Simbolo& simbolo : pilha) {
        if (!texto.empty()) texto += " ";
        texto += nomeDoSimbolo(simbolo);
    }
    return texto;
}

std::string entradaComoTexto(const std::vector<Token>& tokens, size_t posicao) {
    std::string texto;
    for (size_t i = posicao; i < tokens.size(); i++) {
        if (!texto.empty()) texto += " ";
        texto += tokens[i].lexema;
    }
    return texto;
}

std::string descreverToken(const Token& token) {
    if (token.tipo == TipoToken::FIM) {
        return "fim da expressão";
    }
    return "'" + token.lexema + "'";
}

// Largura visível de um texto UTF-8 (conta caracteres, não bytes).
// Necessário porque "→", "ε" e "├" ocupam mais de um byte.
size_t larguraVisivel(const std::string& texto) {
    size_t largura = 0;
    for (unsigned char c : texto) {
        if ((c & 0xC0) != 0x80) largura++;  // ignora bytes de continuação
    }
    return largura;
}

std::string completarComEspacos(const std::string& texto, size_t largura) {
    const size_t atual = larguraVisivel(texto);
    return texto + std::string(largura > atual ? largura - atual : 0, ' ');
}

// Preenche os campos de erro do resultado.
void registrarErro(ResultadoSintatico& resultado, PassoDoParse& passo,
                   const std::string& origem, const Token& atual) {
    resultado.aceita = false;
    resultado.colunaDoErro = atual.coluna;
    resultado.mensagemDeErro = "token inesperado (encontrado: " + descreverToken(atual) + ")";

    passo.acao = "ERRO — " + origem;
    resultado.passos.push_back(passo);
}

} // namespace

ResultadoSintatico analisarSintaticamente(const std::vector<Token>& tokens) {
    ResultadoSintatico resultado{false, {}, "", 0};

    // Configuração inicial: pilha = $ E, entrada = w $
    std::vector<Simbolo> pilha;
    pilha.push_back(simboloTerminal(TipoToken::FIM));
    pilha.push_back(simboloNaoTerminal(NaoTerminal::E));

    size_t posicao = 0;  // índice do lookahead (próximo token não consumido)

    while (true) {
        const Simbolo topo = pilha.back();
        const Token& lookahead = tokens[posicao];

        PassoDoParse passo{static_cast<int>(resultado.passos.size()) + 1,
                           pilhaComoTexto(pilha),
                           entradaComoTexto(tokens, posicao),
                           ""};

        if (topo.ehTerminal) {
            // ---------------------------------------------------------------
            // Caso 1: topo da pilha é terminal (ou $).
            // Ele precisa ser igual ao lookahead.
            // ---------------------------------------------------------------
            if (topo.terminal == lookahead.tipo) {
                if (lookahead.tipo == TipoToken::FIM) {
                    // Pilha = $ e entrada = $: a expressão foi reconhecida.
                    passo.acao = "ACEITA";
                    resultado.passos.push_back(passo);
                    resultado.aceita = true;
                    return resultado;
                }

                passo.acao = "casa " + simboloDoTerminal(lookahead.tipo);
                if (lookahead.tipo == TipoToken::NUM) {
                    passo.acao += " (" + lookahead.lexema + ")";
                }
                resultado.passos.push_back(passo);

                pilha.pop_back();
                posicao++;
                continue;
            }

            // Terminal no topo diferente do lookahead.
            registrarErro(resultado, passo,
                          "topo '" + nomeDoSimbolo(topo) + "' ≠ entrada '" +
                          simboloDoTerminal(lookahead.tipo) + "'",
                          lookahead);
            return resultado;
        }

        // -------------------------------------------------------------------
        // Caso 2: topo da pilha é não-terminal A.
        // Consulta M[A, lookahead] para decidir qual produção usar.
        // -------------------------------------------------------------------
        const EntradaTabela entrada = consultarTabela(topo.naoTerminal, lookahead.tipo);
        const std::string celula = "M[" + nomeDoSimbolo(topo) + ", " +
                                   simboloDoTerminal(lookahead.tipo) + "]";

        if (entrada.tipo == TipoEntrada::ERRO) {
            registrarErro(resultado, passo, celula + " vazia", lookahead);
            return resultado;
        }

        const Producao& producao = obterProducao(entrada.numero);
        passo.acao = producaoComoTexto(producao);
        resultado.passos.push_back(passo);

        // Substitui A pelo lado direito da produção.
        // Em A → ε nada é empilhado.
        pilha.pop_back();

        // Empilha da direita para a esquerda, para que o símbolo mais
        // à esquerda fique no topo e seja processado primeiro.
        for (size_t i = producao.ladoDireito.size(); i-- > 0;) {
            pilha.push_back(producao.ladoDireito[i]);
        }
    }
}

void imprimirPassos(const std::vector<PassoDoParse>& passos, std::ostream& saida) {
    const std::string tituloPasso = "Passo";
    const std::string tituloPilha = "Pilha";
    const std::string tituloEntrada = "Entrada";
    const std::string tituloAcao = "Ação";

    size_t larguraPasso = larguraVisivel(tituloPasso);
    size_t larguraPilha = larguraVisivel(tituloPilha);
    size_t larguraEntrada = larguraVisivel(tituloEntrada);
    for (const PassoDoParse& passo : passos) {
        larguraPasso = std::max(larguraPasso, std::to_string(passo.numero).size());
        larguraPilha = std::max(larguraPilha, larguraVisivel(passo.pilha));
        larguraEntrada = std::max(larguraEntrada, larguraVisivel(passo.entrada));
    }

    // A pilha fica alinhada à esquerda (o topo é o último símbolo) e a entrada
    // à direita, para o $ ficar sempre na mesma coluna, como no livro.
    auto alinharADireita = [](const std::string& texto, size_t largura) {
        const size_t atual = larguraVisivel(texto);
        return std::string(largura > atual ? largura - atual : 0, ' ') + texto;
    };

    saida << completarComEspacos(tituloPasso, larguraPasso) << " | "
          << completarComEspacos(tituloPilha, larguraPilha) << " | "
          << alinharADireita(tituloEntrada, larguraEntrada) << " | "
          << tituloAcao << "\n";

    size_t larguraAcao = larguraVisivel(tituloAcao);
    for (const PassoDoParse& passo : passos) {
        larguraAcao = std::max(larguraAcao, larguraVisivel(passo.acao));
    }
    saida << std::string(larguraPasso, '-') << "-+-"
          << std::string(larguraPilha, '-') << "-+-"
          << std::string(larguraEntrada, '-') << "-+-"
          << std::string(larguraAcao, '-') << "\n";

    for (const PassoDoParse& passo : passos) {
        saida << alinharADireita(std::to_string(passo.numero), larguraPasso) << " | "
              << completarComEspacos(passo.pilha, larguraPilha) << " | "
              << alinharADireita(passo.entrada, larguraEntrada) << " | "
              << passo.acao << "\n";
    }
}
