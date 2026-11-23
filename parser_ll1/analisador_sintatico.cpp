#include "analisador_sintatico.h"

#include <algorithm>

#include "gramatica.h"
#include "tabela_ll1.h"

namespace {

// Cada item da pilha guarda o símbolo da gramática e o nó da árvore que
// corresponde a ele. Assim, quando um não-terminal é expandido, os filhos
// são pendurados no nó certo; e quando um terminal é casado, o lexema é
// gravado na folha certa. O $ do fundo da pilha não tem nó (nullptr).
struct ItemDaPilha {
    Simbolo simbolo;
    NoArvore* no;
};

std::string pilhaComoTexto(const std::vector<ItemDaPilha>& pilha) {
    std::string texto;
    for (const ItemDaPilha& item : pilha) {
        if (!texto.empty()) texto += " ";
        texto += nomeDoSimbolo(item.simbolo);
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

// Preenche os campos de erro do resultado e descarta a árvore incompleta.
void registrarErro(ResultadoSintatico& resultado, PassoDoParse& passo,
                   int codigo, const std::string& origem, const Token& atual) {
    resultado.aceita = false;
    resultado.codigoDoErro = codigo;
    resultado.colunaDoErro = atual.coluna;
    resultado.mensagemDeErro = "e" + std::to_string(codigo) + ": " + descricaoDoErro(codigo) +
                               " (encontrado: " + descreverToken(atual) + ")";
    resultado.arvore.reset();

    passo.acao = "ERRO e" + std::to_string(codigo) + " — " + origem;
    resultado.passos.push_back(passo);
}

} // namespace

ResultadoSintatico analisarSintaticamente(const std::vector<Token>& tokens) {
    ResultadoSintatico resultado{false, {}, nullptr, 0, "", 0};

    // A raiz da árvore é o símbolo inicial E.
    resultado.arvore = std::make_unique<NoArvore>();
    resultado.arvore->rotulo = nomeDoNaoTerminal(NaoTerminal::E);

    // Configuração inicial: pilha = $ E, entrada = w $
    std::vector<ItemDaPilha> pilha;
    pilha.push_back({simboloTerminal(TipoToken::FIM), nullptr});
    pilha.push_back({simboloNaoTerminal(NaoTerminal::E), resultado.arvore.get()});

    size_t posicao = 0;  // índice do lookahead (próximo token não consumido)

    while (true) {
        const ItemDaPilha topo = pilha.back();
        const Token& lookahead = tokens[posicao];

        PassoDoParse passo{static_cast<int>(resultado.passos.size()) + 1,
                           pilhaComoTexto(pilha),
                           entradaComoTexto(tokens, posicao),
                           ""};

        if (topo.simbolo.ehTerminal) {
            // ---------------------------------------------------------------
            // Caso 1: topo da pilha é terminal (ou $).
            // Ele precisa ser igual ao lookahead.
            // ---------------------------------------------------------------
            if (topo.simbolo.terminal == lookahead.tipo) {
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

                topo.no->lexema = lookahead.lexema;
                pilha.pop_back();
                posicao++;
                continue;
            }

            // Terminal no topo diferente do lookahead.
            // Nesta gramática só ')' e '$' podem chegar a esta situação, porque
            // todos os outros terminais (+ - * / ^ ( num) só são empilhados quando
            // a produção foi escolhida justamente por eles serem o lookahead.
            const int codigo = (topo.simbolo.terminal == TipoToken::FECHA_PAR)
                               ? ERRO_PARENTESE_NAO_FECHADO
                               : ERRO_SIMBOLO_EXCEDENTE;
            registrarErro(resultado, passo, codigo,
                          "topo '" + nomeDoSimbolo(topo.simbolo) + "' ≠ entrada '" +
                          simboloDoTerminal(lookahead.tipo) + "'",
                          lookahead);
            return resultado;
        }

        // -------------------------------------------------------------------
        // Caso 2: topo da pilha é não-terminal A.
        // Consulta M[A, lookahead] para decidir qual produção usar.
        // -------------------------------------------------------------------
        const EntradaTabela entrada = consultarTabela(topo.simbolo.naoTerminal, lookahead.tipo);
        const std::string celula = "M[" + nomeDoSimbolo(topo.simbolo) + ", " +
                                   simboloDoTerminal(lookahead.tipo) + "]";

        if (entrada.tipo == TipoEntrada::ERRO) {
            registrarErro(resultado, passo, entrada.numero, celula, lookahead);
            return resultado;
        }

        const Producao& producao = obterProducao(entrada.numero);
        passo.acao = producaoComoTexto(producao);
        resultado.passos.push_back(passo);

        // Substitui A pelo lado direito da produção.
        pilha.pop_back();

        if (producao.ladoDireito.empty()) {
            // A → ε: nada é empilhado; na árvore, A ganha uma folha ε.
            topo.no->adicionarFilho("ε");
            continue;
        }

        // Cria os filhos na ordem da esquerda para a direita...
        std::vector<NoArvore*> nosDosFilhos;
        for (const Simbolo& simbolo : producao.ladoDireito) {
            nosDosFilhos.push_back(topo.no->adicionarFilho(nomeDoSimbolo(simbolo)));
        }
        // ...mas empilha da direita para a esquerda, para que o símbolo mais
        // à esquerda fique no topo e seja processado primeiro.
        for (size_t i = producao.ladoDireito.size(); i-- > 0;) {
            pilha.push_back({producao.ladoDireito[i], nosDosFilhos[i]});
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
