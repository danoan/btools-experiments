#!/usr/bin/env bash

APP=/home-local/dantu1/GIT/PhD/dev/Experiments/cmake-build-release/Applications/rings-and-labels/individual-flow

BASE_FOLDER=/home-local/dantu1/GIT/PhD/dev/Experiments/Applications/rings-and-labels/output


individual()
{
    shape=$1
    mode=$2
    gs=$3
    radius=$4

    i=1
    while [ $i -le $radius ]
    do
        $APP ${BASE_FOLDER}/flows/h$gs/radius-$radius/$shape/probe/L$i ${BASE_FOLDER}/individual/h$gs/radius-$radius/$shape/probe/$mode/L$i.txt probe $mode $i&
        i=$[$i+1]
    done

    wait
}

collection_individual()
{
    shape=$1
    gs=$2

    individual $shape concavities $gs 3
    individual $shape convexities $gs 3

    individual $shape concavities $gs 5
    individual $shape convexities $gs 5

    individual $shape concavities $gs 7
    individual $shape convexities $gs 7

    individual $shape concavities $gs 9
    individual $shape convexities $gs 9
}

collection_individual square 1.0
collection_individual flower 1.0

collection_individual square 0.5
collection_individual flower 0.5

collection_individual square 0.25
collection_individual flower 0.25
