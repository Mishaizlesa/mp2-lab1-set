// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    elem_size=sizeof(TELEM)*8;
    if (len<0) throw "error";
    MemLen=(len+elem_size-1)/elem_size;
    pMem=new TELEM[MemLen];
    for(int i=0;i<MemLen;++i) pMem[i]=0;
    BitLen=len;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    elem_size=sizeof(TELEM)*8;
    MemLen=bf.MemLen;
    BitLen=bf.BitLen;
    pMem=new TELEM[MemLen];
    for(int i=0;i<MemLen;++i) pMem[i]=bf.pMem[i];
}

TBitField::~TBitField()
{
    delete [] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n<0 || n>BitLen) throw "error";
    return n/elem_size;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    TELEM unit = 1;
    return (unit << (n % elem_size));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("invalid index");
    int i = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[i] |= (mask);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)throw std::out_of_range("invalid index");
    int i = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[i] &= (~mask);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("invalid index");
    int i = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[i] & mask) >> (n % elem_size);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    MemLen=bf.MemLen;
    BitLen=bf.BitLen;
    pMem=new TELEM[MemLen];
    for(int i=0;i<MemLen;++i) pMem[i]=bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    bool fl=1;
    for(int i=0;i<MemLen;++i) fl&=(pMem[i]==bf.pMem[i]);
  return fl;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    bool fl=1;
    for(int i=0;i<MemLen;++i) fl&=(pMem[i]!=bf.pMem[i]);
  return fl;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int maxLen = max(BitLen, bf.BitLen);
        int minLen = min(BitLen, bf.BitLen);
        const int cntBit = elem_size;
        int minMemLen = (minLen + cntBit - 1) / cntBit;
        TBitField ans((BitLen == maxLen? *this: bf));
        for (int i = 0; i < minMemLen; i++){
            ans.pMem[i] = pMem[i] | bf.pMem[i];
        }
        return ans;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxLen = max(BitLen, bf.BitLen);
        int minLen = min(BitLen, bf.BitLen);
        const int cntBit = elem_size;
        int minMemLen = (minLen + cntBit - 1) / cntBit;
        TBitField ans(maxLen);
        for (int i = 0; i < minMemLen; i++){
            ans.pMem[i] = pMem[i] & bf.pMem[i];
        }
        return ans;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField ans(BitLen);
    int i = 0;
    const int cntBit = elem_size;
    while (i < BitLen){
        if (i + cntBit <= BitLen){
            ans.pMem[i / cntBit] = ~pMem[i / cntBit];
            i += cntBit;
        }
        else{
            int invert_bit = 1 - GetBit(i);
            if (invert_bit) ans.SetBit(i);
            else ans.ClrBit(i);
            i++;
        }
    }
    return ans;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf){
    string str;
    istr >> str;
    for (int i = 0; i < bf.BitLen; i++){
        if(str[i] == '1') bf.SetBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf){
    for (int i = 0; i < bf.BitLen; i++){
        ostr << bf.GetBit(i);
    }
    return ostr;
}
