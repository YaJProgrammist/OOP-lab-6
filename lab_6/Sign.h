#ifndef LAB2_SIGN_H
#define LAB2_SIGN_H

class Sign { //Класс оператора
    char sig; //Символ оператора
    int prior; //Приоритет оператора
public:
    Sign(char newSig, int newPrior) : sig(newSig), prior(newPrior) {}
    char getSig();
    int getPrior();
};


#endif //LAB2_SIGN_H
