/*
 * 排序算法
 */

#ifndef CSORTALGORITHM_H
#define CSORTALGORITHM_H

#include "csort.h"
#include "calgorithmfactory.h"

class CSortAlgorithm
{
private:
    CSort *pAlgorithm;
public:
    CSortAlgorithm(CAlgorithmFactory * pFactory);
    virtual ~CSortAlgorithm();
    int *Run(int * pArray, int nArrayLength);
};

#endif // CSORTALGORITHM_H
