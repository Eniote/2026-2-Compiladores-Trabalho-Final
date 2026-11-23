#ifndef PARSER_LL1_EXPORTAR_DOT_H
#define PARSER_LL1_EXPORTAR_DOT_H

#include <string>

#include "arvore_sintatica.h"

// Grava a árvore sintática no formato DOT do Graphviz.
// Para gerar a imagem:  dot -Tpng arvore.dot -o arvore.png
// Devolve false se não conseguiu abrir o arquivo.
bool exportarArvoreDot(const NoArvore& raiz, const std::string& caminhoDoArquivo);

#endif // PARSER_LL1_EXPORTAR_DOT_H
