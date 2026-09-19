/*  Ítallo Kauê Barbosa Santos   16839029
    Natália Yumi Watanabi        13725566
*/

#include <stdio.h>
#include "funcionalidades.h"

int main() {
    int opcao;
    if (scanf("%d", &opcao) != 1) return 0;
    
    switch(opcao) {
        case 1: Funcionalidade_1(); break;
        case 2: Funcionalidade_2(); break;
        case 3: Funcionalidade_3(); break;
        case 4: Funcionalidade_4(); break;
        case 5: Funcionalidade_5(); break; 
        case 6: Funcionalidade_6(); break;
        case 7: Funcionalidade_7(); break;
    }
    return 0;
}