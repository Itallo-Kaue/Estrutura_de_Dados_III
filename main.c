/*  Ítallo Kauê Barbosa Santos   16839029
    Natália Yumi Wanabi          13725566
*/

#include <stdio.h>
#include "funcionalidades.h"

int main() {
    int opcao;
    if (scanf("%d", &opcao) != 1) return 0;
    
    switch(opcao) {
        case 1: funcionalidade1(); break;
        case 2: funcionalidade2(); break;
        case 3: funcionalidade3(); break;
        case 4: funcionalidade4(); break;
        case 5: funcionalidade5(); break; 
        case 6: funcionalidade6(); break;
        case 7: funcionalidade7(); break;
    }
    return 0;
}