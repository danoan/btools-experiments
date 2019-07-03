#!/usr/bin/env bash

APP=/home-local/dantu1/GIT/PhD/dev/Experiments/cmake-build-release/Applications/rings-and-labels/model-flow

BASE_FOLDER=/home-local/dantu1/GIT/PhD/dev/Experiments/Applications/rings-and-labels/output


model()
{
    shape=$1
    method=$2
    mode=$3
    gs=$4
    radius=$5

    $APP ${BASE_FOLDER}/flows/h$gs/radius-$radius/$shape/probe/L$radius ${BASE_FOLDER}/model/h$gs/radius-$radius/$shape/$method/$mode $method $mode $radius
}

collection_model()
{
    shape=$1
    gs=$2

    model $shape probe concavities $gs 3&
    model $shape probe convexities $gs 3&

    model $shape probe concavities $gs 5&
    model $shape probe convexities $gs 5&

    wait

    model $shape probe concavities $gs 7&
    model $shape probe convexities $gs 7&

    model $shape probe concavities $gs 9&
    model $shape probe convexities $gs 9&

    wait
}

collection_model square 1.0
collection_model flower 1.0

collection_model square 0.5
collection_model flower 0.5

collection_model square 0.25
collection_model flower 0.25

