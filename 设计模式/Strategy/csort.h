/*
 * 排序算法
 */

#ifndef CSORT_H
#define CSORT_H

class CSort
{
public:
    CSort();
    virtual ~CSort();
    virtual int *Sort(int *pArray, int nArrayLength) = 0;
};

#endif // CSORT_H
