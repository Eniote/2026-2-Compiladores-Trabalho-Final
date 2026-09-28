//
// Created by luanl on 2026-08-24.
//

#include <regex>

using namespace std;

enum alfabeto {
    I,       // 0 "i"
    F,       // 1 "f"
    OPAR_CHAR,    // 2 open parenteses "("
    CPAR_CHAR,    // 3 close parenteses ")"
    EQ_CHAR,      // 4 "="
    DIGIT,   // 5 [0-9]
    LETTER,  // 6 [A-z]
    WHITE,   // 7 [ \n\t]
    ERROR    // 8 Anything not in the list
};

enum token_type {
    NUMBER,
    ID,
    IF,
    OPAR,
    CPAR,
    EQ
};

//q0 é o estado de erro
//se

bool isLetter(string c) {
    return regex_match(c, regex("[A-z_]"));
}

bool isDigit(string c) {
    return regex_match(c, regex("[0-9]"));
}

bool isBlank(char c) {
    return regex_match(c, regex("([ \n\t])"));
}
// yeap, I now realize I could've just used regex_match for the whole think
// but where is the fun in that!


alfabeto charToAlfabeto(char c) {
    switch (c) {
        case 'i': return I;
        case 'f': return F;
        case '(': return OPAR_CHAR;
        case ')': return CPAR_CHAR;
        case '=': return EQ_CHAR;
        default:
            if (isLetter(c)) return LETTER;
            if (isDigit(c)) return DIGIT;
            if (isBlank(c)) return WHITE;
            return ERROR;
    }
}

// bool compute(string tape) {
//     int state{1}, oldState{0};
//     int afd[8][10] = {
//         // q0 q1 q2 q3 q4 q5 q6 q7 q8 q9
//         { 0, 2, 0, 6, 0, 0, 6, 0, 0, 0},         // i
//         { 0, 0, 3, 6, 0, 0, 6, 0, 0, 0},         // f
//         { 0, 9, 0, 0, 0, 0, 0, 0, 0, 0},         // (
//         { 0, 5, 0, 0, 0, 0, 0, 0, 0, 0},         // )
//         { 0, 7, 0, 0, 0, 0, 0, 8, 0, 0},         // =
//         { 0, 4, 0, 6, 4, 0, 6, 0, 0, 0},         // digito
//         { 0, 6, 0, 6, 0, 0, 6, 0, 0, 0},         // letra
//         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}          // blank
//     };
//
//     for (char head: tape) {
//         alfabeto a = charToAlfabeto(head);
//         switch (a) {
//             case I:      oldState = state; state = afd[0][state]; break;
//             case F:      oldState = state; state = afd[1][state]; break;
//             case OPAR_CHAR:   oldState = state; state = afd[2][state]; break;
//             case CPAR_CHAR:   oldState = state; state = afd[3][state]; break;
//             case EQ_CHAR:     oldState = state; state = afd[4][state]; break;
//             case DIGIT:  oldState = state; state = afd[5][state]; break;
//             case LETTER: oldState = state; state = afd[6][state]; break;
//             case WHITE:  oldState = state; state = afd[7][state]; break;
//             default: return ERROR; break;
//         }
//
//         switch (oldState) {
//             case 3: tape.begin tape.
//             case 4:
//             case 5:
//             case 6:
//             case 8:
//             case 9:
//         }
//         return oldState;
//     }
// }

int main() {



    return 0;
}