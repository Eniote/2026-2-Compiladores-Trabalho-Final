# 2026-2-Compiladores-Trabalho-Final
Implementação de um compilador para uma linguagem simplicicada

## Enunciado
- [ ] Descrever o Processo de construção do AFD minimizado
  - [ ] Consolidar AFD feito membros do grupo no mesmo relatorio
- [ ] Descrever as decisoes de progeto para a implementação do automato
- [ ] Descrever o processo de construção com o gerador de analizador lexico
  - [ ] Referencias
  - [ ] Dificuldades
- [ ] Montar casos de teste
- [ ] Exportar relatorio para PDF


## Google docs com os AFDs
- [Google Docs 1]()
- [Google Docs 2]()

## Pastas do projeto
- common_files ⇾ Onde ficara os arquivos compartilhados por cada parte do projeto, desde o input até as tabelas usadas pelo compilador
- common_lib ⇾ pode ser usado para colocar bibliotecas compatilhadas pelos outros subprojetos. 
- diagrams ⇾ guarda as imagens e arquivos .dot usados dos diagramas para a primeira parte do projeto
- env_test ⇾ um pequeno programa gerado por IA para confirmar que o anbiente esta configurado corretamente. Se esse programa rodar o resto do projeto em cpp deve rodar
- no_lexer ⇾ a primeira parte do projeto em que separamos um subconjunto da BNF para criar manualmente o AFD minimo e implementa-lo em cpp. Aque esta guardado o programa em cpp e um .md com mais detalhes
- with_lexer ⇾ a segunda parte do projeto onde implementamos o resto da BNF o input e output desse subprojeto deve ficar na root para que possa ser usado pelas etapas seguintes

## Como configurar e executar o projeto

### No windows com o CLion

#### Para configurar
1. Clone o repositorio e abra com o clion
2. na root do projeto aperte com o botao direito no CMakeList.txt e selecione a `Reload CMake Project`
3. teste o ambiente rodando o env_test

#### Para rodar
- edite o arquivo de input localizado em common_files 
- escolha a build do CMake que queseja executar
- pessa para compilar e executar


### Dependencias Externas
- [winflexbison](https://github.com/lexxmark/winflexbison/releases/tag/v2.5.25) ⇾ a biblioteca do flex(o cmakefile esta configurado para adiciona-la altomaticamente)
- [graphviz](https://graphviz.org/download/) ⇾ para criar imagens dos diagramas apartir de um arquivo .dot (não é necessaio para rodar o codigo)

## Disclamer de Uso de IA
Qualquer uso de IA durante o projeto sera discriminado aqui.

IA foi usada se e somente se estiver discriminado a baixo, ou seja, 
se algo (como esse read-me) não estiver explicitamente discriminado a baixo,
não teve IA envolvina na criação, e se ouve IA na criação ele sera discriminado a baixo

Nenhum codigo gerado por IA foi usado. IA foi usado majoritariamente para pesquisa e consulda de aspectos tecnicos não relacionados diretametne com a materia

### Configuração Inicial do projeto
O gemini foi usado para criar os arquivos de configuração CMakeList.txt na root e 
e para orientar na configuração inicial do ambiente

- Setup do projeto ⇾ https://share.gemini.google/3U0FFGr8NAUt

### Configuração do Graphviz localmente
O Graphviz é uma ferrametne que usa arquivos .dot e uma linguagem propria para gerar diagramas. Daria pra usar o Graphviz na web mas preferimos deixar tudo no mesmo lugar. Nessa parte a IA foi usada para descobrir a existencia do Graphviz e auxiliar na configuração do ambiente para usa-lo local e automaticamente
- O Chat ⇾ https://share.gemini.google/2fnpySTJOkkF

### Exportação da árvore sintática em .dot (parser_ll1)
Na questão 1 da parte 2 (analisador sintático LL(1)), a funcionalidade opcional de exportar a árvore sintática no formato .dot do Graphviz, para depois gerar uma imagem, foi feita com IA. Isso inclui os arquivos `parser_ll1/exportar_dot.h` e `parser_ll1/exportar_dot.cpp` e a opção `--dot` no `parser_ll1/main.cpp`.


## Referencias

### Especificação do C Minus
A especificação pode ser encontrada no apendice A do livro Compiler Construction: Principle and practices by Kenneth c. Louden

### Como usar o flex
- [Documentation](https://web.mit.edu/gnu/doc/html/flex_1.html)
- [Part 1: Tutorial on Lex/yacc](https://youtu.be/54bo1qaHAfk)
- [a flex manual](https://westes.github.io/flex/manual/Indices.html#Indices)