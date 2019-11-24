#include "csortalgorithm.h"

CSortAlgorithm::CSortAlgorithm(CAlgorithmFactory * pFactory)
    :pAlgorithm(pFactory->NewAlgorithm())
{

}

CSortAlgorithm::~CSortAlgorithm()
{
    delete pAlgorithm;
    pAlgorithm = nullptr;
}

int *CSortAlgorithm::Run(int *pArray, int nArrayLength)
{
    if(pAlgorithm)
        return pAlgorithm->Sort(pArray, nArrayLength);
    return nullptr;
}
