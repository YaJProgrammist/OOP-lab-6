#ifndef LAB2_ELEM_H
#define LAB2_ELEM_H

#pragma once

#include <string>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <memory>
#include <vector>
#include "Error.h"

using namespace std;

class Elem { //Класс элемента алгебраического выражения (число, переменная или их сумма/разность)
    void insertAddElems(Elem elem2); //Добавить в вектор указателей на добавочные элементы первого элемента указатели из вектора второго элемента
    int minDeg, maxDeg; //Минимальная и максимальная числовая степень в элементе
    double nums[205] = {0}; //Коэффициенты каждой из степеней переменной (в 100 ячейке - коэффициент для x^0)
    shared_ptr<Elem> addDiv = nullptr, addPow = nullptr; //Указатель на элемент - делитель/показатель степени числа
    vector<shared_ptr<Elem>> addAdd; //Вектор указателей на добавочные элементы
    vector<char> addSigns; //Вектор знаков каждого добавочого символа
public:
    Elem(); //Конструктор
    Elem(int newMinDeg, int newMaxDeg); //Конструктор
    void setNumsCell(int num, int val); //Задать значение num-той ячейки массива nums равным val
    void multAddElems(Elem &elem2, Elem &res); //Умножить добавочные элементы данного элемента на второй элемент
    Elem powToNum(int pow); //Возведение данного элемента в степень, выраженную числом
    void newAddDel(Elem &elem2, Elem &res); //Для элемента задается делитель, после чего элемент добавляется к результату
    void multDel(Elem &elem2, Elem &res); //Домножить делитель элемента на elem2, добавить элемент к результату
    void newAddPow(Elem &elem2, Elem &res); //Для элемента задается показатель степени, после чего элемент добавляется к результату
    void multPow(Elem &elem2, Elem &res); //Домножить показатель степени элемента на elem2, добавить элемент к результату
    Elem& operator = (const Elem& elem2); //Присвоить второе выражение первому
    bool operator == (Elem elem2); //Сравнить два выражения
    Elem operator + (Elem elem2); //Сложить два элемента
    Elem operator - (Elem elem2); //Вычесть второй элемент из первого
    Elem operator * (Elem elem2); //Найти произведение двух элементов
    Elem operator / (Elem elem2); //Найти частное от деления первого элемента на второй
    Elem operator ^ (Elem elem2); //Возвести первый элемент в степень, заданную вторым элементом
    Elem operator * (pair<double, int> num); //Домножить данный элемент на элемент степени num.second с коэффициентом num.first
    Elem calc(Elem elem2, char curSign); //Выполить над двумя элементами операцию curSign
    void calcSameDividers(); //Складывание/вычитание добавочных элементов с одинаковыми элементами-делителями
    void addOneElemToRes(string &res, int i, char x); //Добавление i-го добавочного элемента к строке
    void addBodyOfElemToRes(string &res, int i, char x, string &numToStrNum, bool smthAdded); //Добавление "тела" элемента к результату
    void addAddElemsToRes(string& res, bool smthAdded, char x); //Добавление к строке добавочных элементов
    void actualize(); //Актуализация наименьшего и наибольшего показателей степеней:
    string getResStr(char x); //Получить из элемента строку, принимая за переменную символ x
};


#endif //LAB2_ELEM_H
