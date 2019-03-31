#ifndef EXPERIMENTS_MULTIINDEX_H
#define EXPERIMENTS_MULTIINDEX_H

#include <vector>
#include <assert.h>
#include <algorithm>

namespace Experiments
{
    template<typename TIndex>
    class MultiIndex
    {
    public:
        typedef TIndex Index;
        typedef MultiIndex<Index> Self;

        typedef std::vector<Index> Container;
        typedef typename Container::const_iterator ConstIterator;

        Self& operator<<(const Index& element)
        {
            this->elements.push_back(element);
            std::sort( this->elements.begin(),this->elements.end() );

            return *this;
        }

        bool operator<(const Self& other) const
        {
            if(this->size()!=other.size()) return this->size()<other.size();

            ConstIterator it1 = this->elements.begin();
            ConstIterator it2 = other.elements.begin();

            while(*it1==*it2 && it1!=this->elements.end())
            {
                ++it1;
                ++it2;
            }

            if(it1==this->elements.end())
            {
                --it1;
                --it2;
            }

            return *it1 < *it2;
        }


        size_t size() const{return elements.size();}
        ConstIterator begin() const{return elements.begin();}
        ConstIterator end() const{return elements.end();}

    private:
        Container elements;

    };


}

#endif //EXPERIMENTS_MULTIINDEX_H
