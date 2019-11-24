#include "calgorithmfactory.h"

#include "cbubble.h"
#include "cquicksort.h"

CAlgorithmFactory::CAlgorithmFactory(ALGORITHM_TYPE type)
    :m_type(type)
{

}

CSort *CAlgorithmFactory::NewAlgorithm()
{
    switch (m_type) {
    case BUBBLE:
        return new CBubble;
        break;
    case QUICK:
        return new CQuick;
    default:
        break;
    }
}
