#include "arvore_sintatica.h"

NoArvore* NoArvore::adicionarFilho(const std::string& rotuloDoFilho) {
    auto filho = std::make_unique<NoArvore>();
    filho->rotulo = rotuloDoFilho;
    filhos.push_back(std::move(filho));
    return filhos.back().get();
}

namespace {

// Só o terminal num mostra o lexema, pois nos demais o lexema
// é igual ao próprio rótulo ("+", "(", ...).
std::string textoDoNo(const NoArvore& no) {
    if (no.lexema.empty() || no.lexema == no.rotulo) {
        return no.rotulo;
    }
    return no.rotulo + " (" + no.lexema + ")";
}

// O prefixo acumula as linhas verticais dos níveis de cima.
// Se o nó é o último filho, não há mais irmãos abaixo dele e a linha
// vertical daquele nível termina ("    " em vez de "│   ").
void imprimirFilhos(const NoArvore& no, const std::string& prefixo, std::ostream& saida) {
    for (size_t i = 0; i < no.filhos.size(); i++) {
        const NoArvore& filho = *no.filhos[i];
        const bool ehUltimo = (i + 1 == no.filhos.size());

        saida << prefixo << (ehUltimo ? "└── " : "├── ") << textoDoNo(filho) << "\n";
        imprimirFilhos(filho, prefixo + (ehUltimo ? "    " : "│   "), saida);
    }
}

} // namespace

void imprimirArvore(const NoArvore& raiz, std::ostream& saida) {
    saida << textoDoNo(raiz) << "\n";
    imprimirFilhos(raiz, "", saida);
}
