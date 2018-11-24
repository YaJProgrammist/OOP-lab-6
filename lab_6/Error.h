#ifndef LAB2_ERROR_H
#define LAB2_ERROR_H

#include<iostream>
using namespace std;

class Error { //Класс ошибок
public:
    virtual void printError(){ //Вывести ошибку
        cout << "Ошибка!" << endl;
        getchar();
    }
    void stopTheProgram() { //Завершение программы
        cout << "Пожалуйста, остановите программу." << endl;
        getchar();
    }
};

class ZeroDivide : public Error { //Класс ошибки деления на 0
public:
    void printError(){ //Вывести ошибку
        cout << "Операция деления на ноль!" << endl;
        stopTheProgram();
    }
};

class TwoOrMoreVariables : public Error { //Класс ошибки более одной переменной в выражении
    char symbol;
    int i;
public:
    TwoOrMoreVariables(char newSymbol, int newI) : symbol(newSymbol), i(newI) {}
    void printError(){ //Вывести ошибку
        cout << "Выражение содержит более одной переменной!" << endl;
        cout << "Выражение содержит ошибку в символе '" << symbol << "' (номер символа: " << i << ")!" << endl;
        stopTheProgram();
    }
};

class IncorrectBrackets : public Error { //Класс ошибки неправильной последовательности скобок
public:
    void printError(){ //Вывести ошибку
        cout << "Неправильная последовательность скобок!" << endl;
        stopTheProgram();
    }
};

class UnexpectedSymbol : public Error { //Класс ошибки символа, который не ожидался
    char symbol;
    int i;
public:
    UnexpectedSymbol(char newSymbol, int newI) : symbol(newSymbol), i(newI) {}
    void printError(){ //Вывести ошибку
        cout << "Не ожидался следующий символ: '" << symbol << "' (номер символа: " << i << ")!" << endl;
        stopTheProgram();
    }
};

class InvalidSymbol : public Error { //Класс ошибки некорректного выражения
    char symbol;
    int i;
public:
    InvalidSymbol(char newSymbol, int newI) : symbol(newSymbol), i(newI) {}
    void printError(){ //Вывести ошибку
        cout << "Недопустимый символ: '" << symbol << "' (номер символа: " << i << ")!" << endl;
        stopTheProgram();
    }
};

#endif //LAB2_ERROR_H
