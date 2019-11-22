#ifndef EXPERIMENTS_TAYLORENERGYINTERFACE_H
#define EXPERIMENTS_TAYLORENERGYINTERFACE_H

class TaylorEnergyInterface
{
public:
    typedef long double NumType;

    virtual NumType alpha(NumType e) const=0;

    virtual NumType alpha_prime(NumType e)  const=0;

    virtual NumType beta(NumType e)  const=0;

    virtual NumType beta_prime(NumType e)  const=0;

};

#endif //EXPERIMENTS_TAYLORENERGYINTERFACE_H
