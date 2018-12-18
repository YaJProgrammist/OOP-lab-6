#ifndef LAB2_ALGEBRAIC_EXPRESSIONS_H
#define LAB2_ALGEBRAIC_EXPRESSIONS_H

#pragma once

#include <string>
#include <iostream>
#include <stack>
#include "Elem.h"
#include "Sign.h"
#include "Error.h"

using namespace std;

class Algebraic_expressions { //Класс алгебраического выражения одной переменной
    string fx; //Исходная строка
    char x; //Переменная
    void skipSpace(int &i); //Пропустить несколько пробелов подряд в исходной строке
    void skipSpaceBack(int &i); //Пропустить несколько пробелов подряд в исходной строке в обратном направлении
public:
    Algebraic_expressions(); //Конструкор
    explicit Algebraic_expressions(string newFX); //Конструкор превращения
    string getString(); // Получить строку-выражение
    Elem smallLetter(int i); //Обрабатывает маленькую латинскую букву в выражении
    Elem digit(int& i, int curNum); //Обрабатывает цифру в выражении
    int getOperPrior(int i); //Получение приоритета оператора
    void oper(int i, stack<Elem>& elements, stack<Sign>& Signs); //Обрабатывает оператор в выражении
    void calcTwoElems(stack<Elem>& elements, stack<Sign>& Signs); //Совершает операцию между двумя верхними элементами стека
    bool properOper(char lastOper, int i); //Проверяет, корректно ли использовать оператор curOper после оператора lastOper
    Elem checkSmallLetter(char &state, int i); //Проверить допустимость буквенного символа и запустить его обработку
    void checkOper(int i, char &state, char &lastOper, int &brNum, stack<Elem> &elements, stack<Sign> &signs); // Проверить допустимость оператора и запустить его обработку
    void parseExpr(stack<Elem> &elements, stack<Sign> &signs); //Проходимся по выражению и разбираем его на элементы и операторы, попутно совершая некоторые из операций
    void swapSigns(Algebraic_expressions &res, int i); //Поменять '-' на '+' и '+' на '-'
    void setFirstSign(Algebraic_expressions &res, int &i); //Определить первый знак выражения с противоположным знаком
    Algebraic_expressions operator ~(); //Упростить выражение и получить из него строку
    Algebraic_expressions operator -(); //Изменить знак выражения
    Algebraic_expressions operator +(Algebraic_expressions algExpr); //Сложить два выражения
    Algebraic_expressions operator -(Algebraic_expressions algExpr); //Вычесть второе выражение из первого
    void repComWithDots(string &str); //Заменить запятые в строке на точки
    friend ostream& operator << (ostream& os, Algebraic_expressions& algExpr); //Вывести выражение
};


#endif //LAB2_ALGEBRAIC_EXPRESSIONS_H
