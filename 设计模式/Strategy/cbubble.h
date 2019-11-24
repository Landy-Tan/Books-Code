/*
 * 冒泡排序
 */

#ifndef CBUBBLE_H
#define CBUBBLE_H

#include "csort.h"

class CBubble : public CSort
{
public:
    virtual int *Sort(int *pnArray, int nArrayLength);
};

#endif // CBUBBLE_H
