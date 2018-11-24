#include "Elem.h"

Elem::Elem() {
    minDeg = 204;
    maxDeg = 0;
}

Elem::Elem(int newMinDeg, int newMaxDeg) {
    minDeg = newMinDeg;
    maxDeg = newMaxDeg;
}

void Elem::setNumsCell(int num, int val) {
    nums[num] = val;
}

bool Elem::operator == (Elem elem2) {
    int newMinDeg = min(minDeg, elem2.minDeg);
    int newMaxDeg = max(maxDeg, elem2.maxDeg);
    for (int i = newMinDeg; i <= newMaxDeg; i++) {
        if (nums[i] != elem2.nums[i]) {
            return false;
        }
    }
    return true;
}

Elem& Elem::operator = (const Elem& elem2) {
    minDeg = elem2.minDeg;
    maxDeg = elem2.maxDeg;
    for (int i = minDeg; i <= maxDeg; i++) {
        nums[i] = elem2.nums[i];
    }
    if (elem2.addDiv) {
        addDiv = elem2.addDiv;
    }
    if (elem2.addPow) {
        addPow = elem2.addPow;
    }
    addAdd.clear();
    for (int i = 0; i < (int)elem2.addAdd.size(); i++) {
        shared_ptr<Elem> addElem = elem2.addAdd[i];
        addAdd.push_back(addElem);
    }
    addSigns = elem2.addSigns;
    return *this;
}

void Elem::insertAddElems(Elem elem2) {
    for (int i = 0; i < (int)elem2.addAdd.size(); i++) {
        shared_ptr<Elem> addElem = elem2.addAdd[i];
        addAdd.push_back(addElem);
    }
}

Elem Elem::operator + (Elem elem2) {
    Elem res = *this;
    for (int i = elem2.minDeg; i <= elem2.maxDeg; i++) {
        res.nums[i] += elem2.nums[i];
    }
    res.insertAddElems(elem2);
    res.addSigns.insert(res.addSigns.end(), elem2.addSigns.begin(), elem2.addSigns.end());
    res.minDeg = min(minDeg, elem2.minDeg);
    res.maxDeg = max(maxDeg, elem2.maxDeg);
    return res;
}

Elem Elem::operator - (Elem elem2) {
    Elem res = *this;
    for (int i = elem2.minDeg; i <= elem2.maxDeg; i++) {
        res.nums[i] -= elem2.nums[i];
    }
    for (int i = 0; i < (int)elem2.addSigns.size(); i++) {
        if (elem2.addSigns[i] == '+') {
            elem2.addSigns[i] = '-';
        } else {
            elem2.addSigns[i] = '+';
        }
    }
    res.insertAddElems(elem2);
    res.addSigns.insert(res.addSigns.end(), elem2.addSigns.begin(), elem2.addSigns.end());
    res.minDeg = min(minDeg, elem2.minDeg);
    res.maxDeg = max(maxDeg, elem2.maxDeg);
    return res;
}

void Elem::multAddElems(Elem &elem2, Elem &res){
    for (int i = 0; i < (int)addAdd.size(); i++) {
        for (int j = elem2.minDeg; j <= elem2.maxDeg; j++) {
            shared_ptr<Elem> addElem = shared_ptr<Elem>(new Elem);
            *addElem = *(addAdd[i]) * make_pair(elem2.nums[j], j);
            if (addAdd[i] -> addDiv) {
                addElem -> addDiv = addAdd[i] -> addDiv;
            }
            res.addAdd.push_back(addElem);
            res.addSigns.push_back('+');
        }
        for (int j = 0; j < (int)elem2.addAdd.size(); j++) {
            shared_ptr<Elem> addElem = shared_ptr<Elem>(new Elem);
            *addElem = *(addAdd[i]) * *(elem2.addAdd[j]);
            if (addDiv && elem2.addDiv) {
                *(addElem -> addDiv) = *(addDiv) * *(elem2.addDiv);
            } else
            if (addDiv) {
                addElem -> addDiv = addDiv;
            } else
            if (elem2.addDiv) {
                addElem -> addDiv = elem2.addDiv;
            }
            res.addAdd.push_back(addElem);
            res.addSigns.push_back('+');
        }
    }
}

Elem Elem::operator * (Elem elem2) {
    Elem res(200, 0);
    for (int i = minDeg; i <= maxDeg; i++) {
        for (int j = elem2.minDeg; j <= elem2.maxDeg; j++) {
            res.nums[i + j - 100] += nums[i] * elem2.nums[j];
        }
        for (int j = 0; j < (int)elem2.addAdd.size(); j++) {
            shared_ptr<Elem> addElem = shared_ptr<Elem>(new Elem);
            *addElem = *(elem2.addAdd[j]) * make_pair(nums[i], i);
            if (elem2.addAdd[j] -> addDiv) {
                addElem -> addDiv = elem2.addAdd[i] -> addDiv;
            }
            res.addAdd.push_back(addElem);
            res.addSigns.push_back('+');
        }
    }
    multAddElems(elem2, res);
    res.minDeg = minDeg + elem2.minDeg - 100;
    res.maxDeg = maxDeg + elem2.maxDeg - 100;
    return res;
}

void Elem::newAddDel(Elem &elem2, Elem &res) {
    if (addAdd.size() == 1 && addAdd[0]->addDiv) {
        res = *this;
        *res.addAdd[0]->addDiv = elem2 * *(res.addAdd[0]->addDiv);
    } else {
        shared_ptr<Elem> newAdd = shared_ptr<Elem>(new Elem);
        *newAdd = *this;
        newAdd->addDiv = shared_ptr<Elem>(new Elem);
        *newAdd->addDiv = elem2;
        res.addAdd.push_back(newAdd);
        res.addSigns.push_back('+');
    }
}

void Elem::multDel(Elem &elem2, Elem &res) {
    shared_ptr<Elem> newAdd = shared_ptr<Elem>(new Elem);
    *newAdd = *this;
    newAdd->addDiv = shared_ptr<Elem>(new Elem);
    *newAdd->addDiv = elem2 * (*addDiv);
    res.addAdd.push_back(newAdd);
    res.addSigns.push_back('+');
}

Elem Elem::operator / (Elem elem2) {
    Elem res(200, 0);
    if (elem2.minDeg == 100 && elem2.minDeg == elem2.maxDeg && addAdd.size() == 0) {
        if (elem2.nums[100] == 0) {
            throw ZeroDivide();
        } else {
            for (int i = minDeg; i <= maxDeg; i++) {
                res.nums[i] += nums[i] / elem2.nums[100];
            }
            res.minDeg = minDeg;
            res.maxDeg = maxDeg;
        }
    } else {
        if (!addDiv) {
            newAddDel(elem2, res);
        } else {
            multDel(elem2, res);
        }
    }
    return res;
}

Elem Elem::powToNum(int pow) {
    Elem res(200, 0);
    Elem resCopy = *this;
    for (int q = 1; q < pow; q++) {
        fill(res.nums, res.nums + 205, 0);
        for (int i = (minDeg - 100)*q + 100; i <= (maxDeg - 100)*q + 100; i++) {
            for (int j = minDeg; j <= maxDeg; j++) {
                res.nums[i + j - 100] += resCopy.nums[i] * nums[j];
            }
        }
        res.minDeg = (minDeg - 100) * (q + 1) + 100;
        res.maxDeg = (maxDeg - 100) * (q + 1) + 100;
        resCopy = res;
    }
    return res;
}

void Elem::newAddPow(Elem &elem2, Elem &res) {
    if (addAdd.size() == 1 && addAdd[0]->addPow) {
        res = *this;
        *res.addAdd[0]->addPow = elem2 * *(res.addAdd[0]->addPow);
    } else {
        shared_ptr<Elem> newAdd = shared_ptr<Elem>(new Elem);
        *newAdd = *this;
        newAdd -> addPow = shared_ptr<Elem>(new Elem);
        *newAdd -> addPow = elem2;
        res.addAdd.push_back(newAdd);
        res.addSigns.push_back('+');
    }
}

void Elem::multPow(Elem &elem2, Elem &res) {
    shared_ptr<Elem> newAdd = shared_ptr<Elem>(new Elem);
    *newAdd = *this;
    newAdd -> addPow = shared_ptr<Elem>(new Elem);
    *newAdd -> addPow = elem2 * (*addPow);
    res.addAdd.push_back(newAdd);
    res.addSigns.push_back('+');
}

Elem Elem::operator ^ (Elem elem2) {
    if (elem2.minDeg == 100 && elem2.minDeg == elem2.maxDeg && (addAdd.size() != 1 || elem2.nums[100] == 0)) {
        if (elem2.nums[100] == 0) {
            Elem res(100, 100);
            res.nums[100] = 1;
            return res;
        } else {
            return powToNum((int)elem2.nums[100]);
        }
    } else {
        Elem res(200, 0);
        if (!addPow) {
            newAddPow(elem2, res);
        } else {
            multPow(elem2, res);
        }
        return res;
    }
}

Elem Elem::operator * (pair<double, int> num) {
    Elem res(minDeg + num.second - 100, maxDeg + num.second - 100);
    for (int i = minDeg; i <= maxDeg; i++) {
        res.nums[i + num.second - 100] = num.first * nums[i];
    }
    return res;
}

Elem Elem::calc(Elem elem2, char curSign) {
    if (curSign == '+') {
        return *this + elem2;
    }
    if (curSign == '-') {
        return *this - elem2;
    }
    if (curSign == '*') {
        return *this * elem2;
    }
    if (curSign == '/') {
        return *this / elem2;
    }
    if (curSign == '^') {
        return *this ^ elem2;
    }
    return *this;
}

void Elem::actualize() {
    while (nums[minDeg] == 0 && minDeg != maxDeg) {
        minDeg++;
    }
    while (nums[maxDeg] == 0 && minDeg != maxDeg) {
        maxDeg--;
    }
}

void Elem::calcSameDividers() {
    for (int i = 0; i < (int) addAdd.size(); i++) {
        shared_ptr<Elem> addElem1 = addAdd[i];
        if (addElem1->addDiv) {
            for (int j = i + 1; j < (int) addAdd.size(); j++) {
                shared_ptr<Elem> addElem2 = addAdd[j];
                if (addElem2->addDiv && *addElem1->addDiv == *addElem2->addDiv) {
                    if (addSigns[i] == addSigns[j]) {
                        *addElem1 = *addElem1 + *addElem2;
                    } else {
                        *addElem1 = *addElem1 - *addElem2;
                    }
                    addAdd.erase(addAdd.begin() + j);
                    addSigns.erase(addSigns.begin() + j);
                    j--;
                    addElem1 -> actualize();
                    if (addElem1-> minDeg == 100 && addElem1-> minDeg == addElem1-> maxDeg && addElem1 -> nums[100] == 0) {
                        addAdd.erase(addAdd.begin() + i);
                        addSigns.erase(addSigns.begin() + i);
                        i--;
                    }
                }
            }
        }
    }
}

void Elem::addOneElemToRes(string &res, int i, char x) {
    shared_ptr<Elem> addElem = addAdd[i];
    (*addElem).actualize();
    if (addElem->minDeg == addElem->maxDeg) {
        res += (*addElem).getResStr(x);
    } else {
        res += "(" + (*addElem).getResStr(x) + ")";
    }
    if (addElem->addDiv) {
        if (addElem->addDiv->minDeg == addElem->addDiv->maxDeg) {
            res += "/" + (*addElem->addDiv).getResStr(x);
        } else {
            res += "/(" + (*addElem->addDiv).getResStr(x) + ")";
        }
    } else {
        if (addElem->addPow->minDeg == addElem->addPow->maxDeg) {
            res += "^" + (*addElem->addPow).getResStr(x);
        } else {
            res += "^(" + (*addElem->addPow).getResStr(x) + ")";
        }
    }
}

void Elem::addAddElemsToRes(string& res, bool smthAdded, char x) {
    for (int i = 0; i < (int) addAdd.size(); i++) {
        if (addSigns[i] == '+') {
            if (smthAdded) {
                res += " + ";
            } else {
                smthAdded = true;
            }
        } else {
            if (smthAdded) {
                res += " - ";
            } else {
                res += "-";
                smthAdded = true;
            }
        }
        addOneElemToRes(res, i, x);
    }
}

void Elem::addBodyOfElemToRes(string &res, int i, char x, string &numToStrNum, bool smthAdded) {
    if (nums[i] < 0) {
        numToStrNum = " - " + numToStrNum;
    }
    if (nums[i] == -1) {
        res += " - ";
        if (i == 100) {
            res += "1";
        } else {
            res += x;
        }
    }
    else if (nums[i] == 1) {
        if (smthAdded) {
            res += " + ";
        }
        if (i == 100) {
            res += "1";
        } else {
            res += x;
        }
    } else {
        if (nums[i] >= 0 && smthAdded) {
            res += " + ";
        }
        res += numToStrNum;
        if (i != 100) {
            res += "*";
            res += x;
        }
    }
}

string Elem::getResStr(char x) {
    string res = "";
    bool smthAdded = false; //true если какой-либо элемент уже добавлен к строке
    //Добавление к строке слагаемых элемента каждой из степеней
    for (int i = minDeg; i <= maxDeg; i++) {
        if (nums[i] != 0) {
            string numToStrDeg, numToStrNum;
            numToStrDeg = to_string(i - 100);
            if ((int) (nums[i]) == nums[i]) {
                numToStrNum = to_string(abs((int) nums[i]));
            } else {
                numToStrNum = to_string(abs(nums[i]));
            }
            addBodyOfElemToRes(res, i, x, numToStrNum, smthAdded);
            if (i < 100) {
                res += "^(" + numToStrDeg + ")";
            } else if (i > 101) {
                res += "^" + numToStrDeg;
            }
            smthAdded = true;
        }
    }

    calcSameDividers();
    addAddElemsToRes(res, smthAdded, x);

    if (res == "") {
        res = "0";
    }
    return res;
}