/*
 * 算法工厂
 */

#ifndef CALGORITHMFACTORY_H
#define CALGORITHMFACTORY_H

#include "csort.h"

enum ALGORITHM_TYPE{BUBBLE, QUICK};

class CAlgorithmFactory
{
private :
    ALGORITHM_TYPE m_type;

public:
    CAlgorithmFactory(ALGORITHM_TYPE type);
    CSort *NewAlgorithm();
};

#endif // CALGORITHMFACTORY_H
