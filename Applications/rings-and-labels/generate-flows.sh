#!/usr/bin/env bash

FLOW_APP=/home-local/dantu1/GIT/PhD/BTools/build-release/modules/Applications/shape-flow/shape-flow

generate_flow()
{
    shape=$1
    method=$2
    gs=$3

    OUTPUT_FOLDER=/home-local/dantu1/GIT/PhD/dev/Experiments/Applications/rings-and-labels/output/flows/h$gs/$shape/$method

    $FLOW_APP -r5 -S$shape -h$gs -m$method -q1.0 -t0 -g0 -l1 -pdouble -i100 $OUTPUT_FOLDER/L1&
    $FLOW_APP -r5 -S$shape -h$gs -m$method -q1.0 -t0 -g0 -l2 -pdouble -i100 $OUTPUT_FOLDER/L2&
    $FLOW_APP -r5 -S$shape -h$gs -m$method -q1.0 -t0 -g0 -l3 -pdouble -i100 $OUTPUT_FOLDER/L3&
    $FLOW_APP -r5 -S$shape -h$gs -m$method -q1.0 -t0 -g0 -l4 -pdouble -i100 $OUTPUT_FOLDER/L4&
    $FLOW_APP -r5 -S$shape -h$gs -m$method -q1.0 -t0 -g0 -l5 -pdouble -i300 $OUTPUT_FOLDER/L5&
}


generate_flow square probe 0.5
generate_flow square improve 0.5
generate_flow flower probe 0.5
generate_flow flower improve 0.5

