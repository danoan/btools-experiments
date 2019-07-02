#!/usr/bin/env bash

FLOW_APP=/home-local/dantu1/GIT/PhD/BTools/build-release/modules/Applications/shape-flow/shape-flow

generate_flow()
{
    shape=$1
    method=$2
    gs=$3
    radius=$4

    i=1
    while [ $i -le $radius ]
    do
        OUTPUT_FOLDER=/home-local/dantu1/GIT/PhD/dev/Experiments/Applications/rings-and-labels/output/flows/h$gs/radius-$radius/$shape/$method
        $FLOW_APP -r$radius -S$shape -h$gs -m$method -q1.0 -t0 -g0 -l$i -pdouble -i100 $OUTPUT_FOLDER/L$i&
        i=$[$i+1]
    done

    wait

}

collection_flow()
{
    shape=$1
    gs=$2

    generate_flow $shape probe $gs 3
    generate_flow $shape improve $gs 3

    generate_flow $shape probe $gs 5
    generate_flow $shape improve $gs 5

    generate_flow $shape probe $gs 7
    generate_flow $shape improve $gs 7

    generate_flow $shape probe $gs 9
    generate_flow $shape improve $gs 9

}

collection_flow square 1.0
collection_flow flower 1.0

collection_flow flower 0.5
collection_flow square 0.5

collection_flow square 0.25
collection_flow flower 0.25


