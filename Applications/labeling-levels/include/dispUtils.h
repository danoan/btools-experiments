#ifndef EXPERIMENTS_DISPUTILS_H
#define EXPERIMENTS_DISPUTILS_H

#include <iostream>

#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/SquaredCurvatureTerm.h"

#include "energyUtils.h"

typedef SCaBOliC::Core::ODRModel ODRModel;
typedef SCaBOliC::Energy::ISQ::SquaredCurvatureTerm SquaredCurvatureTerm;

void printLabelingReport(std::ostream& ofs,
                         const ODRModel& ODR,
                         const SquaredCurvatureTerm& et,
                         bool verbose=false);

#endif //EXPERIMENTS_DISPUTILS_H
