#!/usr/bin/env bash

APP=/home-local/dantu1/GIT/PhD/dev/Experiments/cmake-build-release/Applications/rings-and-labels/model-flow

BASE_FOLDER=/home-local/dantu1/GIT/PhD/dev/Experiments/Applications/rings-and-labels/output


model()
{
    shape=$1
    method=$2
    mode=$3
    gs=$4

    $APP ${BASE_FOLDER}/flows/h$gs/$shape/probe/L5 ${BASE_FOLDER}/model/h$gs/$shape/$method/$mode $method $mode
}

model square probe concavities 0.5&
model square improve concavities 0.5&

model square probe convexities 0.5&
model square improve convexities 0.5&

model flower probe concavities 0.5&
model flower improve concavities 0.5&

model flower probe convexities 0.5&
model flower improve convexities 0.5&
