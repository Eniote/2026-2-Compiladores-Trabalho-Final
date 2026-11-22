# Questão 1 – Analisador sintático LL(1) para expressões aritméticas

Parte 2 do trabalho (Trabalho Prático: Analisador Sintático). Esta questão é independente do C-Minus.

## Enunciado

Gramática G = {V, Σ, R, S}, com V = {E, T, F, P}, Σ = {+, −, *, /, (, ), ^, num} e S = E:

```
E → E + T | E − T | T
T → T * F | T / F | F
F → P ^ F | P
P → ( E ) | num
```

Pede-se:

- **a.** um analisador léxico para os tokens, sendo `num` uma sequência de dígitos com separador de casas decimais;
- **b.** a tabela de análise sintática LL(1), incluindo os erros que podem ocorrer;
- **c.** um programa que, dada uma expressão: (i) faça a análise léxica; (ii) faça a análise sintática por tabela LL(1), manipulando pilha e cadeia de entrada; (iii) mostre os passos do parse; (iv) mostre a árvore de análise sintática completa.

### Decisões tomadas

| Ponto | Decisão |
|---|---|
| Símbolo `/` | O Σ do enunciado não lista `/`, mas a regra `T → T / F` usa. Ele foi tratado como terminal. |
| Separador decimal | **Apenas ponto** (`3.14`). A vírgula (`3,14`) é erro léxico. Assim o AFD do `num` fica mais limpo, com um único caminho para a parte decimal. |
| Formato do `num` | Precisa ter dígitos antes e depois do ponto: `3.` e `.5` são inválidos. |
| Sinal de menos | O `−` do enunciado é escrito na entrada com o hífen `-`. A gramática não tem menos unário, então `-3` é erro sintático. |
| Erros | A análise para no **primeiro erro** (léxico ou sintático), mostrando a coluna e o motivo. |
| Linguagem | C++ com CMake, como o restante do projeto. O analisador léxico foi escrito à mão a partir do AFD (sem Flex). |

---

## a. Analisador léxico

### Tokens

| Token | Símbolo na gramática | Lexema |
|---|---|---|
| `NUM` | `num` | `digito+ ( '.' digito+ )?` |
| `MAIS` | `+` | `+` |
| `MENOS` | `-` | `-` |
| `VEZES` | `*` | `*` |
| `DIVIDIDO` | `/` | `/` |
| `POTENCIA` | `^` | `^` |
| `ABRE_PAR` | `(` | `(` |
| `FECHA_PAR` | `)` | `)` |
| `FIM` | `$` | fim da entrada (inserido pelo analisador) |

Espaços, tabulações e quebras de linha só separam tokens e são descartados.

### AFD do `num`

Expressão regular: `num = digito+ ( '.' digito+ )?`

```
início → q0 --0-9--> (q1) --'.'--> q2 --0-9--> (q3)
                      q1 --0-9--> q1
                      q3 --0-9--> q3
```

- `q1` e `q3` são estados de aceitação (número inteiro e número com parte decimal).
- Qualquer transição que não aparece acima leva a um estado morto.
- O desenho do autômato está em [`../diagrams/afd_num.dot`](../diagrams/afd_num.dot).

No código (`analisador_lexico.cpp`), cada estado é um valor de `EstadoNum` e a função `transicao()` é a tabela de transições. O analisador simula o AFD enquanto existir transição válida. Quando para:

- se o estado atual é de aceitação, o lexema lido é um `NUM`;
- se parou em `q2` (leu `"3."` e não veio dígito), é erro léxico de número malformado.

### Erros léxicos

| Entrada | Mensagem |
|---|---|
| `3,14` | `',' não é separador decimal nesta linguagem; use '.'` |
| `3.` | `número malformado '3.': faltam dígitos depois do '.'` |
| `.5`, `3.1.4` | `'.' inesperado: o separador decimal aparece uma única vez e precisa de dígitos antes e depois` |
| `2 + a`, `2 % 3` | `caractere inválido 'a'` / `caractere inválido '%'` |

---

## b. Tabela LL(1)

### Passo 1: por que a gramática original não serve

Um analisador LL(1) escolhe a produção olhando só o próximo token (o *lookahead*). A gramática original tem dois problemas que impedem isso (Louden, cap. 4, seção 4.2):

1. **Recursão à esquerda** em `E → E + T` e `T → T * F`. Para expandir `E` com lookahead `num`, tanto `E → E + T` quanto `E → T` começam com `num`, e o analisador não sabe qual escolher. Pior: escolher `E → E + T` coloca `E` de novo no topo sem consumir nada, e o parse entra em laço infinito.
2. **Prefixo comum** em `F → P ^ F | P`. As duas alternativas começam com `P`, então o lookahead não distingue entre elas.

### Passo 2: eliminar a recursão à esquerda

Regra geral (recursão imediata):

```
A → A α | β      vira      A  → β A'
                           A' → α A' | ε
```

Aplicando em `E → E + T | E − T | T` (α₁ = `+ T`, α₂ = `- T`, β = `T`):

```
E  → T E'
E' → + T E' | - T E' | ε
```

Aplicando em `T → T * F | T / F | F` (α₁ = `* F`, α₂ = `/ F`, β = `F`):

```
T  → F T'
T' → * F T' | / F T' | ε
```

### Passo 3: fatorar à esquerda

Regra geral:

```
A → α β₁ | α β₂      vira      A  → α A'
                               A' → β₁ | β₂
```

Aplicando em `F → P ^ F | P` (α = `P`, β₁ = `^ F`, β₂ = ε):

```
F  → P F'
F' → ^ F | ε
```

`P → ( E ) | num` já começa com terminais diferentes e não muda.

### Gramática transformada

As produções são numeradas, e esses números aparecem na tabela:

```
 1. E  → T E'
 2. E' → + T E'
 3. E' → - T E'
 4. E' → ε
 5. T  → F T'
 6. T' → * F T'
 7. T' → / F T'
 8. T' → ε
 9. F  → P F'
10. F' → ^ F
11. F' → ε
12. P  → ( E )
13. P  → num
```

A transformação não muda a linguagem reconhecida. A precedência continua igual (`^` acima de `*` e `/`, que ficam acima de `+` e `-`), e o `^` continua associativo à direita, porque `F' → ^ F` mantém a recursão à direita. A árvore de `8 - 3 - 2` passa a "pender" para a direita (veja o exemplo abaixo). A associatividade à esquerda de `-` e `/` não se perde: ela é recuperada quando a árvore é avaliada percorrendo a cadeia de `E'` (ou `T'`) da esquerda para a direita (Louden, seção 4.2).

### Passo 4: conjuntos FIRST

FIRST(α) é o conjunto de terminais que podem começar uma cadeia derivada de α (e inclui ε se α deriva a cadeia vazia).

- `P → ( E )` começa com `(` e `P → num` começa com `num`, então **FIRST(P) = { (, num }**.
- `F → P F'`: P não deriva ε, então FIRST(F) = FIRST(P) = **{ (, num }**.
- `T → F T'`: FIRST(T) = FIRST(F) = **{ (, num }**.
- `E → T E'`: FIRST(E) = FIRST(T) = **{ (, num }**.
- `E' → + T E' | - T E' | ε`: **FIRST(E') = { +, -, ε }**.
- `T' → * F T' | / F T' | ε`: **FIRST(T') = { *, /, ε }**.
- `F' → ^ F | ε`: **FIRST(F') = { ^, ε }**.

### Passo 5: conjuntos FOLLOW

FOLLOW(A) é o conjunto de terminais que podem aparecer imediatamente à direita de A em alguma forma sentencial. Regras:

1. `$` ∈ FOLLOW(símbolo inicial).
2. Para `A → α B β`: tudo de FIRST(β), menos ε, entra em FOLLOW(B).
3. Para `A → α B β` com ε ∈ FIRST(β) (ou β vazio): tudo de FOLLOW(A) entra em FOLLOW(B).

Cálculo:

- **FOLLOW(E)**: `$` pela regra 1; em `P → ( E )`, o `)` vem depois de E (regra 2). Resultado: **{ ), $ }**.
- **FOLLOW(E')**: E' aparece no fim de `E → T E'` (regra 3, recebe FOLLOW(E)) e no fim de `E' → + T E'` (recebe FOLLOW(E'), o que não acrescenta nada). Resultado: **{ ), $ }**.
- **FOLLOW(T)**: em `E → T E'`, recebe FIRST(E') − ε = { +, - } (regra 2). Como E' deriva ε, recebe também FOLLOW(E) (regra 3). O mesmo vale para `E' → + T E'`, que acrescenta FOLLOW(E'). Resultado: **{ +, -, ), $ }**.
- **FOLLOW(T')**: T' está no fim de `T → F T'`, então recebe FOLLOW(T). Resultado: **{ +, -, ), $ }**.
- **FOLLOW(F)**: em `T → F T'`, recebe FIRST(T') − ε = { *, / }. Como T' deriva ε, recebe também FOLLOW(T). Em `F' → ^ F`, F está no fim e recebe FOLLOW(F'). Resultado: **{ *, /, +, -, ), $ }**.
- **FOLLOW(F')**: F' está no fim de `F → P F'`, então recebe FOLLOW(F). Resultado: **{ *, /, +, -, ), $ }**.
- **FOLLOW(P)**: em `F → P F'`, recebe FIRST(F') − ε = { ^ }. Como F' deriva ε, recebe também FOLLOW(F). Resultado: **{ ^, *, /, +, -, ), $ }**.

Resumo:

| Não-terminal | FIRST | FOLLOW |
|---|---|---|
| E  | `(` `num` | `)` `$` |
| E' | `+` `-` ε | `)` `$` |
| T  | `(` `num` | `+` `-` `)` `$` |
| T' | `*` `/` ε | `+` `-` `)` `$` |
| F  | `(` `num` | `*` `/` `+` `-` `)` `$` |
| F' | `^` ε | `*` `/` `+` `-` `)` `$` |
| P  | `(` `num` | `^` `*` `/` `+` `-` `)` `$` |

### Passo 6: montar a tabela

Para cada produção `A → α`:

- para cada terminal `a` em FIRST(α), coloca-se `A → α` em M[A, a];
- se ε ∈ FIRST(α), coloca-se `A → α` em M[A, b] para cada `b` em FOLLOW(A).

As células que sobram vazias são erros. Nenhuma célula recebeu duas produções, o que confirma que a gramática transformada é LL(1).

| | num | + | - | * | / | ^ | ( | ) | $ |
|---|---|---|---|---|---|---|---|---|---|
| **E**  | 1: E→TE' | e1 | e1 | e1 | e1 | e1 | 1: E→TE' | e1 | e2 |
| **E'** | e3 | 2: E'→+TE' | 3: E'→-TE' | e6 | e6 | e6 | e3 | 4: E'→ε | 4: E'→ε |
| **T**  | 5: T→FT' | e1 | e1 | e1 | e1 | e1 | 5: T→FT' | e1 | e2 |
| **T'** | e3 | 8: T'→ε | 8: T'→ε | 6: T'→*FT' | 7: T'→/FT' | e6 | e3 | 8: T'→ε | 8: T'→ε |
| **F**  | 9: F→PF' | e1 | e1 | e1 | e1 | e1 | 9: F→PF' | e1 | e2 |
| **F'** | e3 | 11: F'→ε | 11: F'→ε | 11: F'→ε | 11: F'→ε | 10: F'→^F | e3 | 11: F'→ε | 11: F'→ε |
| **P**  | 13: P→num | e1 | e1 | e1 | e1 | e1 | 12: P→(E) | e1 | e2 |

### Erros

Há dois lugares onde o analisador detecta erro:

1. **O topo da pilha é um não-terminal A** e a célula M[A, lookahead] é uma entrada de erro (e1, e2, e3, e6).
2. **O topo da pilha é um terminal** diferente do lookahead (e4, e5).

| Código | Quando ocorre | Exemplo | Mensagem |
|---|---|---|---|
| e1 | E, T, F ou P no topo; lookahead é operador ou `)` | `2 + * 3`, `()`, `-3` | falta um operando: esperava um número ou '(' |
| e2 | E, T, F ou P no topo; lookahead é `$` | `2 +`, `(` | a expressão terminou antes da hora: esperava um número ou '(' |
| e3 | E', T' ou F' no topo; lookahead é `num` ou `(` | `2 3`, `2 (3)` | falta um operador entre dois operandos |
| e4 | topo é `)` e o lookahead não é `)` | `(2 + 3` | parêntese não fechado: esperava ')' |
| e5 | topo é `$` mas ainda há entrada | `2 + 3)` | símbolo excedente: a expressão já estava completa |
| e6 | E' ou T' no topo; lookahead é um operador que não pode vir ali | (veja a observação) | operador fora de lugar |

Observações:

- **Só `)` e `$` podem causar erro do tipo 2.** Os outros terminais (`+ - * / ^ ( num`) só são empilhados por uma produção escolhida justamente porque eles eram o lookahead, então sempre casam no passo seguinte.
- **Algumas células de erro nunca são alcançadas** a partir do símbolo inicial. Por exemplo, E' só fica no topo depois que T' terminou, e T' já teria consumido `*` ou `/`, ou dado erro com `num`/`(`. Por isso, na prática, o e3 é sempre detectado na linha F', e as células e6 não aparecem. Elas foram preenchidas mesmo assim para a tabela ficar completa.

---

## c. Programa

### Algoritmo (Louden, seção 4.2)

```
pilha ← [$, E]           (topo à direita)
entrada ← tokens + $
repita:
    X ← topo da pilha;  a ← lookahead
    se X = $ e a = $:              ACEITA
    se X é terminal:
        se X = a:  desempilha X, avança na entrada         ("casa a")
        senão:     ERRO (e4 ou e5)
    se X é não-terminal:
        se M[X, a] = X → Y1 Y2 ... Yk:
            desempilha X
            empilha Yk ... Y2 Y1   (Y1 fica no topo)
        senão: ERRO (código da célula)
```

Construção da árvore: cada item da pilha guarda também o nó da árvore correspondente.

- Ao expandir `X → Y1 ... Yk`, o nó de X ganha os filhos Y1 ... Yk (na ordem) e cada Yi é empilhado junto com o seu nó.
- Em `X → ε`, o nó de X ganha uma folha `ε`.
- Ao casar um terminal, o lexema é gravado na folha correspondente.

Quando a pilha esvazia, a árvore está completa.

### Organização dos arquivos

| Arquivo | Conteúdo |
|---|---|
| `token.h` | `TipoToken` e `Token` |
| `analisador_lexico.h/.cpp` | (a) AFD do `num` e reconhecimento dos tokens |
| `gramatica.h/.cpp` | símbolos e as 13 produções da gramática transformada |
| `tabela_ll1.h/.cpp` | (b) tabela M[A, a] e códigos de erro |
| `analisador_sintatico.h/.cpp` | (c-ii, c-iii) pilha, entrada, passos do parse e montagem da árvore |
| `arvore_sintatica.h/.cpp` | (c-iv) nó da árvore e impressão no terminal |
| `exportar_dot.h/.cpp` | exportação opcional da árvore em DOT (Graphviz) |
| `main.cpp` | leitura da expressão e chamada das etapas |

### Como executar

```
parser_ll1 "EXPRESSAO"                   analisa a expressão
parser_ll1                               pede a expressão no terminal
parser_ll1 --dot ARQUIVO "EXPRESSAO"     também grava a árvore em .dot (veja abaixo)
```

**No CLion:** depois de `Reload CMake Project`, selecione a configuração `parser_ll1`. Em *Edit Configurations… → Program arguments*, coloque a expressão entre aspas, por exemplo `"2.5 * (3 + 4) ^ 2"`. Sem argumentos, o programa pede a expressão no console do Run.

**No terminal:** o executável gerado pelo MinGW do CLion precisa das DLLs do MinGW. Coloque a pasta `bin\mingw\bin` da instalação do CLion no `PATH` antes de rodar:

```powershell
$env:PATH = "<pasta do CLion>\bin\mingw\bin;$env:PATH"
.\cmake-build-debug\parser_ll1\parser_ll1.exe "2 + 3"
```

O código de saída é 0 quando a expressão é aceita e 1 quando há erro.

### Exemplo: `2 + 3`

```
Expressão: 2 + 3

== 1. Análise léxica ==

Lexema     Token      Coluna
---------- ---------- ------
2          NUM        1
+          MAIS       3
3          NUM        5
$          FIM        6

== 2. Análise sintática LL(1) ==

Passo | Pilha          | Entrada | Ação
------+----------------+---------+-------------
    1 | $ E            | 2 + 3 $ | E → T E'
    2 | $ E' T         | 2 + 3 $ | T → F T'
    3 | $ E' T' F      | 2 + 3 $ | F → P F'
    4 | $ E' T' F' P   | 2 + 3 $ | P → num
    5 | $ E' T' F' num | 2 + 3 $ | casa num (2)
    6 | $ E' T' F'     |   + 3 $ | F' → ε
    7 | $ E' T'        |   + 3 $ | T' → ε
    8 | $ E'           |   + 3 $ | E' → + T E'
    9 | $ E' T +       |   + 3 $ | casa +
   10 | $ E' T         |     3 $ | T → F T'
   11 | $ E' T' F      |     3 $ | F → P F'
   12 | $ E' T' F' P   |     3 $ | P → num
   13 | $ E' T' F' num |     3 $ | casa num (3)
   14 | $ E' T' F'     |       $ | F' → ε
   15 | $ E' T'        |       $ | T' → ε
   16 | $ E'           |       $ | E' → ε
   17 | $              |       $ | ACEITA

Resultado: expressão ACEITA.

== 3. Árvore de análise sintática ==

E
├── T
│   ├── F
│   │   ├── P
│   │   │   └── num (2)
│   │   └── F'
│   │       └── ε
│   └── T'
│       └── ε
└── E'
    ├── +
    ├── T
    │   ├── F
    │   │   ├── P
    │   │   │   └── num (3)
    │   │   └── F'
    │   │       └── ε
    │   └── T'
    │       └── ε
    └── E'
        └── ε
```

Como no livro, a pilha é mostrada com a base (`$`) à esquerda e o topo à direita, e a entrada termina sempre em `$`.

### Exemplo com erro: `2 3`

```
Passo | Pilha          | Entrada | Ação
------+----------------+---------+---------------------
    1 | $ E            |   2 3 $ | E → T E'
    2 | $ E' T         |   2 3 $ | T → F T'
    3 | $ E' T' F      |   2 3 $ | F → P F'
    4 | $ E' T' F' P   |   2 3 $ | P → num
    5 | $ E' T' F' num |   2 3 $ | casa num (2)
    6 | $ E' T' F'     |     3 $ | ERRO e3 — M[F', num]

Erro sintático na coluna 3: e3: falta um operador entre dois operandos (encontrado: '3')
```

### Exemplo: árvore de `8 - 3 - 2`

Mostra o formato da árvore depois de eliminar a recursão à esquerda: cada `-` fica num `E'` encadeado à direita.

```
E
├── T
│   ├── F
│   │   ├── P
│   │   │   └── num (8)
│   │   └── F'
│   │       └── ε
│   └── T'
│       └── ε
└── E'
    ├── -
    ├── T
    │   └── ... num (3) ...
    └── E'
        ├── -
        ├── T
        │   └── ... num (2) ...
        └── E'
            └── ε
```

### Exportação da árvore em DOT (opcional)

`parser_ll1 --dot arvore.dot "EXPRESSAO"` grava a árvore no formato do Graphviz. Para gerar a imagem:

```
dot -Tpng arvore.dot -o arvore.png
```

Não-terminais aparecem como elipses, terminais como retângulos (o `num` mostra o valor) e ε como texto.

Essa funcionalidade foi feita com IA (veja a seção "Disclamer de Uso de IA" do [README principal](../README.md)).

Para remover essa funcionalidade: apague `exportar_dot.h` e `exportar_dot.cpp`, tire `exportar_dot.cpp` do `CMakeLists.txt` desta pasta e, no `main.cpp`, remova o `#include "exportar_dot.h"`, o tratamento da opção `--dot` e o bloco `if (!arquivoDot.empty())` em `processarExpressao`.

## Referências

- LOUDEN, Kenneth C. *Compiler Construction: Principles and Practice*. Capítulo 4: análise sintática descendente, LL(1), remoção de recursão à esquerda e fatoração à esquerda, conjuntos FIRST e FOLLOW, construção da tabela LL(1) e tratamento de erros.
- [Graphviz – linguagem DOT](https://graphviz.org/doc/info/lang.html)
