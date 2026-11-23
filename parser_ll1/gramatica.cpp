#include "gramatica.h"

Simbolo simboloTerminal(TipoToken terminal) {
    return {true, terminal, NaoTerminal::E};
}

Simbolo simboloNaoTerminal(NaoTerminal naoTerminal) {
    return {false, TipoToken::FIM, naoTerminal};
}

std::string nomeDoNaoTerminal(NaoTerminal naoTerminal) {
    switch (naoTerminal) {
        case NaoTerminal::E:       return "E";
        case NaoTerminal::E_LINHA: return "E'";
        case NaoTerminal::T:       return "T";
        case NaoTerminal::T_LINHA: return "T'";
        case NaoTerminal::F:       return "F";
        case NaoTerminal::F_LINHA: return "F'";
        case NaoTerminal::P:       return "P";
    }
    return "?";
}

std::string nomeDoSimbolo(const Simbolo& simbolo) {
    return simbolo.ehTerminal ? simboloDoTerminal(simbolo.terminal)
                              : nomeDoNaoTerminal(simbolo.naoTerminal);
}

namespace {

std::vector<Producao> criarProducoes() {
    const Simbolo E      = simboloNaoTerminal(NaoTerminal::E);
    const Simbolo ELinha = simboloNaoTerminal(NaoTerminal::E_LINHA);
    const Simbolo T      = simboloNaoTerminal(NaoTerminal::T);
    const Simbolo TLinha = simboloNaoTerminal(NaoTerminal::T_LINHA);
    const Simbolo F      = simboloNaoTerminal(NaoTerminal::F);
    const Simbolo FLinha = simboloNaoTerminal(NaoTerminal::F_LINHA);
    const Simbolo P      = simboloNaoTerminal(NaoTerminal::P);

    const Simbolo mais     = simboloTerminal(TipoToken::MAIS);
    const Simbolo menos    = simboloTerminal(TipoToken::MENOS);
    const Simbolo vezes    = simboloTerminal(TipoToken::VEZES);
    const Simbolo dividido = simboloTerminal(TipoToken::DIVIDIDO);
    const Simbolo potencia = simboloTerminal(TipoToken::POTENCIA);
    const Simbolo abrePar  = simboloTerminal(TipoToken::ABRE_PAR);
    const Simbolo fechaPar = simboloTerminal(TipoToken::FECHA_PAR);
    const Simbolo num      = simboloTerminal(TipoToken::NUM);

    return {
        { 1, NaoTerminal::E,       {T, ELinha}              },  // E  → T E'
        { 2, NaoTerminal::E_LINHA, {mais, T, ELinha}        },  // E' → + T E'
        { 3, NaoTerminal::E_LINHA, {menos, T, ELinha}       },  // E' → - T E'
        { 4, NaoTerminal::E_LINHA, {}                       },  // E' → ε
        { 5, NaoTerminal::T,       {F, TLinha}              },  // T  → F T'
        { 6, NaoTerminal::T_LINHA, {vezes, F, TLinha}       },  // T' → * F T'
        { 7, NaoTerminal::T_LINHA, {dividido, F, TLinha}    },  // T' → / F T'
        { 8, NaoTerminal::T_LINHA, {}                       },  // T' → ε
        { 9, NaoTerminal::F,       {P, FLinha}              },  // F  → P F'
        {10, NaoTerminal::F_LINHA, {potencia, F}            },  // F' → ^ F
        {11, NaoTerminal::F_LINHA, {}                       },  // F' → ε
        {12, NaoTerminal::P,       {abrePar, E, fechaPar}   },  // P  → ( E )
        {13, NaoTerminal::P,       {num}                    },  // P  → num
    };
}

} // namespace

const Producao& obterProducao(int numero) {
    static const std::vector<Producao> producoes = criarProducoes();
    return producoes.at(numero - 1);
}

std::string producaoComoTexto(const Producao& producao) {
    std::string texto = nomeDoNaoTerminal(producao.ladoEsquerdo) + " →";
    if (producao.ladoDireito.empty()) {
        return texto + " ε";
    }
    for (const Simbolo& simbolo : producao.ladoDireito) {
        texto += " " + nomeDoSimbolo(simbolo);
    }
    return texto;
}
