#ifndef EXPERIMENTS_PROBESOLVER_H
#define EXPERIMENTS_PROBESOLVER_H

#include <DGtal/helpers/StdDefs.h>

#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Optimization/solver/probe/QPBOProbeSolver.h"

#include "SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/SquaredCurvatureTerm.h"
#include "SCaBOliC/Energy/model/OptimizationData.h"
#include "SCaBOliC/Energy/model/Solution.h"

#include "InputData.h"
#include "energyUtils.h"
#include "dispUtils.h"

typedef DGtal::Z2i::DigitalSet DigitalSet;

typedef SCaBOliC::Core::ODRModel ODRModel;

typedef SCaBOliC::Energy::OptimizationData::UnaryTermsMatrix UnaryTermsMatrix;
typedef SCaBOliC::Energy::OptimizationData::PairwiseTermsMatrix PairwiseTermsMatrix;
typedef SCaBOliC::Energy::OptimizationData::EnergyTable EnergyTable;
typedef SCaBOliC::Energy::Solution::LabelsVector LabelsVector;
typedef SCaBOliC::Energy::ISQ::SquaredCurvatureTerm SquaredCurvatureTerm;

typedef QPBOProbeSolver<UnaryTermsMatrix,
EnergyTable,
LabelsVector> MyQPBOProbeSolver;

DigitalSet probeSolve(std::ostream& os,
                      const InputData& input,
                      const DigitalSet& dsInput,
                      bool correction);

#endif //EXPERIMENTS_PROBESOLVER_H
