#include <iostream>
#include "Error.h"
#include "Algebraic_expressions.h"
using namespace std;

int main() {
    // ПРОВЕРКА ПРАВИЛЬНОСТИ ВЫРАЖЕНИЯ ПРОИЗВОДИТСЯ СПРАВА НАЛЕВО
    try {
        setlocale(LC_ALL, "");
        Algebraic_expressions myAE("z/(z+2)+(z+3)/(z+1+1)+(z+4)/(2*(z+1)-z)+(z^(z+3))/(z+2)"), newAE("z^2"); //Без ошибок
        //Algebraic_expressions myAE("(((x+2)^(x+3)))^(x+2))^2"), newAE("x^2"); //Неправильно расставлены скобки
        //Algebraic_expressions myAE("(2 + x)^(1+y^2) - (x^2)"), newAE("x^2"); //Больше одной переменной
        //Algebraic_expressions myAE("((2 + x)/(3 - 3))^(1+x^2)"), newAE("x^2"); //Деление на 0
        //Algebraic_expressions myAE("((2 + x)^(3+x)+)/(1+x^2)/(x+1)"), newAE("x^2"); //Символ '+' не ожидался
        //Algebraic_expressions myAE("((2 + x)^(3+x))/(1+ш^2)/(x+1)"), newAE("x^2"); //Недопустимый символ 'ш'
        cout << "Исходное выражение: " << myAE << endl;
        cout << "Упрощенное выражение: " << ~myAE << endl;
        cout << "Выражение с противоположным знаком: " << -myAE << endl;
        cout << "Выражение, уменьшенное на " << newAE << ": " << (myAE - newAE) << endl;
        cout << "Выражение, увеличенное на " << newAE << ": " << (myAE + newAE) << endl;
    }
    catch(Error &e) {
        e.printError();
    }
}
