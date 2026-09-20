# 2026-2-Compiladores-Trabalho-Final
Implementação de um compilador para uma linguagem simplicicada

## Enunciado do trabalho
- [x] Escolher uma LP simplificada que tenha BNF 
  - Iremos usar C-Minus, a especifação da linguagem pode ser encontrada no apendice A do livro da disciplina
- [ ] Definir um subconjunto de regras de produção (mínimo 5) para:
- [ ] Definir ERs para cada token do subconjunto selecionado
- [ ] Executar o processo NFA ⇾ DFA ⇾ DFA mínimo sem usar ferramenta que automatize tudo
- [ ] Implementar o Scanner
- [ ] Gerar os códigos para o subconjunto selecionado (válidos e inválidos
- [ ] Usar Flex ou equivalente para gerar um Scanner (analisador) para a BNF completa
- [ ] Gerar arquivos com códigos sem erro e com erro
- [ ] Mostrar como fica a saída com o Scanner gerado

## Next
- [ ] Definir todas as regras no formato do lexer
- [ ] Escolher o subconjunto que sera usado para a parte manual
  - O que é considerado uma regra?
- [ ] Estudar o processo para a parte manual (tem no livro :p)



## Pastas do projeto
- common_lib ⇾ pode ser usado para colocar bibliotecas compatilhadas pelos outros subprojetos. 
- diagrams ⇾ guarda as imagens e arquivos .dot usados dos diagramas para a primeira parte do projeto
- env_test ⇾ um pequeno programa gerado por IA para confirmar que o anbiente esta configurado corretamente. Se esse programa rodar o resto do projeto em cpp deve rodar
- no_lexer ⇾ a primeira parte do projeto em que separamos um subconjunto da BNF para criar manualmente o AFD minimo e implementa-lo em cpp. Aque esta guardado o programa em cpp e um .md com mais detalhes
- with_lexer ⇾ a segunda parte do projeto onde implementamos o resto da BNF o input e output desse subprojeto deve ficar na root para que possa ser usado pelas etapas seguintes
    

## Como configurar e executar o projeto

### No windows com o CLion

#### Para configurar
1. Clone o repositorio e abra com o clion
2. na root do projeto aprete com o botao direito no CMakeList.txt e selecione a `Reload CMake Project`
3. teste o ambiente rodando o env_test

#### Para rodar
%%TBD%%


### Dependencias Externas
- [winflexbison](https://github.com/lexxmark/winflexbison/releases/tag/v2.5.25) ⇾ a biblioteca do flex(o cmakefile esta configurado para adiciona-lo altomaticamente)
- [graphviz](https://graphviz.org/download/) ⇾ para criar imagens dos diagramas apartir de um arquivo .dot (não é necessaio para rodar o codigo)

## Disclamer de Uso de IA
Qualquer uso de IA durante o projeto sera discriminado aqui.

IA foi usada se e somente se estiver discriminado a baixo, ou seja, 
se algo (como esse read-me) não estiver explicitamente discriminado a baixo,
não teve IA envolvina na criação, e se ouve IA na criação ele sera discriminado a baixo

### Configuração Inicial do projeto
O gemini foi usado para criar os arquivos de configuração CMakeList.txt na root e 
e para orientar na configuração inicial do ambiente

- Setup do projeto ⇾ https://share.gemini.google/3U0FFGr8NAUt

### Configuração do Graphviz localmente
O Graphviz é uma ferrametne que usa arquivos .dot e uma linguagem propria para gerar diagramas. Daria pra usar o Graphviz na web mas preferimos deixar tudo no mesmo lugar. Nessa parte a IA foi usada para descobrir a existencia do Graphviz e auxiliar na configuração do ambiente para usa-lo local e automaticamente
- O Chat ⇾ https://share.gemini.google/2fnpySTJOkkF


## Referencias

### Especificação do C Minus
A especificação pode ser encontrada no apendice A do livro Compiler Construction: Principle and practices by Kenneth c. Louden

### Como usar o flex
- [Documentation](https://web.mit.edu/gnu/doc/html/flex_1.html)
- [Part 1: Tutorial on Lex/yacc](https://youtu.be/54bo1qaHAfk)
- [a flex manual](https://westes.github.io/flex/manual/Indices.html#Indices)