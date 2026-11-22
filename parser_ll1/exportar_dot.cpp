#include "exportar_dot.h"

#include <fstream>

namespace {

std::string escaparParaDot(const std::string& texto) {
    std::string resultado;
    for (char c : texto) {
        if (c == '"' || c == '\\') resultado += '\\';
        resultado += c;
    }
    return resultado;
}

// Percorre a árvore em pré-ordem dando um identificador único (n0, n1, ...)
// a cada nó. Devolve o identificador do nó visitado.
//   - não-terminais: elipse
//   - terminais:     retângulo (num mostra também o lexema)
//   - ε:             só o texto
int escreverNo(const NoArvore& no, int& proximoId, std::ofstream& arquivo) {
    const int id = proximoId++;

    std::string rotulo = escaparParaDot(no.rotulo);
    if (!no.lexema.empty() && no.lexema != no.rotulo) {
        rotulo += "\\n" + escaparParaDot(no.lexema);
    }

    std::string formato = "ellipse";
    if (no.filhos.empty()) {
        formato = (no.rotulo == "ε") ? "plaintext" : "box";
    }

    arquivo << "    n" << id << " [label=\"" << rotulo << "\", shape=" << formato << "];\n";

    for (const auto& filho : no.filhos) {
        const int idDoFilho = escreverNo(*filho, proximoId, arquivo);
        arquivo << "    n" << id << " -> n" << idDoFilho << ";\n";
    }
    return id;
}

} // namespace

bool exportarArvoreDot(const NoArvore& raiz, const std::string& caminhoDoArquivo) {
    std::ofstream arquivo(caminhoDoArquivo);
    if (!arquivo) {
        return false;
    }

    arquivo << "digraph ArvoreSintatica {\n";
    arquivo << "    ordering=out;\n";  // mantém os filhos na ordem da produção
    arquivo << "    node [fontname=\"Helvetica\"];\n";

    int proximoId = 0;
    escreverNo(raiz, proximoId, arquivo);

    arquivo << "}\n";
    return true;
}
