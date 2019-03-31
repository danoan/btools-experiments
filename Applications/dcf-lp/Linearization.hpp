#include "Linearization.h"

using namespace Experiments;

template<typename TIndex, typename TValue>
void Linearization<TIndex,TValue>::linearize(const Input &input)
{
    for(auto it=input.begin();it!=input.end();++it,++nextIndex)
    {
        uniqueIndexMap.insert( UniqueElement(it->first,nextIndex));
        reverseUniqueIndexMap.insert( ReverseElement(nextIndex,it->first) );

        linearizationMap.insert( LinearizationElement(nextIndex,it->second) );
    }
}

