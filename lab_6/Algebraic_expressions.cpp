#include "Algebraic_expressions.h"

Algebraic_expressions::Algebraic_expressions() {
    fx = "";
    x = ' ';
    cout << "Введите алгебраическое выражение одной переменной." << endl;
    getline(cin, fx);
    cin.clear();
}

Algebraic_expressions::Algebraic_expressions(string newFX) {
    fx = newFX;
    x = ' ';
}

string Algebraic_expressions::getString() {
    return fx;
}

ostream &operator<<(ostream &os, Algebraic_expressions &algExpr) {
    os << algExpr.getString();
    return os;
}

void Algebraic_expressions::skipSpace(int &i) {
    while (i < (int) fx.length() && fx[i] == ' ') {
        i++;
    }
}

void Algebraic_expressions::skipSpaceBack(int &i) {
    while (i >= 0 && fx[i] == ' ') {
        i--;
    }
}

Elem Algebraic_expressions::smallLetter(int i) {
    if (x == ' ') {
        x = fx[i];
    } else if (x != fx[i]) {
        throw TwoOrMoreVariables(fx[i], i);
    }
    Elem newElem(101, 101);
    newElem.setNumsCell(101, 1);
    return newElem;
}

Elem Algebraic_expressions::digit(int& i, int curNum) {
    double wholeNum = curNum;
    //Получение каждой цифры текущего числа:
    while (i - 1 >= 0) {
        curNum = fx[i - 1] - '0';
        if (curNum >= 0 && curNum <= 9) {
            wholeNum += curNum * 10;
        } else {
            break;
        }
        i--;
    }
    Elem newElem(100, 100);
    newElem.setNumsCell(100, wholeNum);
    return newElem;
}

int Algebraic_expressions::getOperPrior(int i) {
    char priors[7] = {'/', '-', ')', '+', '*', '^', '('}; //Операторы приоритетов соответственно -2 -1 0 1 2 3 4
    int res = 0;
    for ( ; res < 7; res++) {
        if (fx[i] == priors[res]) {
            break;
        }
    }
    res -= 2;
    return res;
};

void Algebraic_expressions::calcTwoElems(stack<Elem>& elements, stack<Sign>& signs){
    Elem prev1 = elements.top();
    elements.pop();
    Elem prev2 = elements.top();
    elements.pop();
    Sign prevS = signs.top();
    signs.pop();
    elements.push(prev1.calc(prev2, prevS.getSig()));
}

void Algebraic_expressions::oper(int i, stack<Elem>& elements, stack<Sign>& signs) {
    int curPrior = getOperPrior(i);

    if (fx[i] == ')') {
        signs.push({fx[i], curPrior});
    } else if (fx[i] == '(') {
        while (!signs.empty() && signs.top().getSig() != ')') {
            calcTwoElems(elements, signs);
        }
        signs.pop();
    } else {
        //Калькулируем предыдущие элементы с большим приоритетом:
        while (!signs.empty() && abs(signs.top().getPrior()) > abs(curPrior)) {
            calcTwoElems(elements, signs);
        }

        signs.push({fx[i], curPrior});
    }
    int j = i;
    j--;
    skipSpaceBack(j);
    if (fx[i] == '-' && (j < 0 || fx[j] == '(')) {
        elements.push(*(new Elem(204, 0)));
    }
}

bool Algebraic_expressions::properOper(char lastOper, int i){
    if (!strchr("+-*/()^", fx[i])) {
        throw InvalidSymbol(fx[i], i);
    }
    if (fx[i] == '(') {
        return !(lastOper == '*' || lastOper == '/' || lastOper == '^');
    }
    if (fx[i] == ')') {
        return (lastOper != '(');
    }
    return (lastOper == '(');
}

Elem Algebraic_expressions::checkSmallLetter(char &state, int i) {
    if (state != 'n' && state != 'o') {
        throw UnexpectedSymbol(fx[i], i);
    }
    state = 'l';
    return smallLetter(i);
}

void Algebraic_expressions::checkOper(int i, char &state, char &lastOper, int &brNum, stack<Elem> &elements, stack<Sign> &signs){
    if ((state != 'n' || fx[i] != ')') && state != 'd' && state != 'l' &&
        (state != 'o' || !properOper(lastOper, i))) {
        throw UnexpectedSymbol(fx[i], i);
    }
    if (fx[i] == '(') {
        brNum++;
    }
    else if (fx[i] == ')') {
        brNum--;
    }
    if (brNum > 0) {
        throw IncorrectBrackets();
    }
    oper(i, elements, signs);
    state = 'o';
    lastOper = fx[i];
}

void Algebraic_expressions::parseExpr(stack<Elem> &elements, stack<Sign> &signs) {
    char state = 'n';
    char lastOper = 'n';
    int brNum = 0;
    for (int i = fx.length() - 1; i >= 0; i--) {
        skipSpaceBack(i);
        if (i < 0) {
            break;
        }
        int curNum = fx[i] - '0';
        if (fx[i] >= 'a' && fx[i] <= 'z') {
            elements.push(checkSmallLetter(state, i));
        } else if (curNum >= 0 && curNum <= 9) {
            elements.push(digit(i, curNum));
            state = 'd';
        } else {
            checkOper(i, state, lastOper, brNum, elements, signs);
        }
    }
    if (brNum != 0) {
        throw IncorrectBrackets();
    }
    if (state == 'o' && lastOper != 'n') {
        if (lastOper == '+') {
            signs.pop();
        } else
        if (!strchr("-+(", lastOper)) {
            throw UnexpectedSymbol(lastOper, 0);
        }
    }
}

Algebraic_expressions Algebraic_expressions::operator~() {
    if (fx == "") {
        return *this;
    }

    stack<Elem> elements; //Стек элементов выражения
    stack<Sign> signs; //Стек операторов выражения

    parseExpr(elements, signs);

    //Калькулируем оставшиеся элементы
    while (!signs.empty()) {
        calcTwoElems(elements, signs);
    }

    Elem resElem = elements.top();
    resElem.actualize();
    fx = resElem.getResStr(x);
    return *this;
}

void Algebraic_expressions::swapSigns(Algebraic_expressions &res, int i) {
    if (fx[i] == '+') {
        res.fx += '-';
    }
    else if (fx[i] == '-') {
        if (i > 0) {
            res.fx += '+';
        }
    } else {
        res.fx += fx[i];
    }
}

void Algebraic_expressions::setFirstSign(Algebraic_expressions &res, int &i) {
    if (fx[i] == '-') {
        i++;
    }
    else if (fx[i] == '+') {
        res.fx = '-' + res.fx;
        i++;
    } else {
        res.fx = '-' + res.fx;
    }
}

Algebraic_expressions Algebraic_expressions::operator-() {
    int bracketsNum = 0;
    Algebraic_expressions res("");
    res.x = x;
    int i = 0;
    skipSpace(i);
    if (i >= (int) fx.length()) {
        return res;
    }
    setFirstSign(res, i);
    for ( ; i < (int) fx.length(); i++) {
        skipSpace(i);
        if (i >= (int) fx.length()) {
            break;
        }
        if (fx[i] == '(') {
            bracketsNum++;
            res.fx += fx[i];
            continue;
        }
        if (fx[i] == ')') {
            bracketsNum--;
            res.fx += fx[i];
            continue;
        }
        if (bracketsNum == 0) {
            swapSigns(res, i);
            continue;
        }
        res.fx += fx[i];
    }
    return res;
}

void Algebraic_expressions::repComWithDots(string &str) {
    for (int i = 0; i < (int)str.length(); i++){
        if (str[i] == ',') {
            str[i] = '.';
        }
    }
}

Algebraic_expressions Algebraic_expressions::operator+(Algebraic_expressions algExpr) {
    repComWithDots(fx);
    repComWithDots(algExpr.fx);
    Algebraic_expressions res(fx + "+ (" + algExpr.fx + ")");
    return ~res;
}

Algebraic_expressions Algebraic_expressions::operator-(Algebraic_expressions algExpr) {
    repComWithDots(fx);
    repComWithDots(algExpr.fx);
    Algebraic_expressions res(fx + "- (" + algExpr.fx + ")");
    return ~res;
}