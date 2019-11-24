/*
 * 快速排序
 */

#ifndef CQUICKSORT_H
#define CQUICKSORT_H

#include "csort.h"

class CQuick : public CSort
{
public:
    virtual int *Sort(int *pArray, int nArrayLength);
};

#endif // CQUICKSORT_H
