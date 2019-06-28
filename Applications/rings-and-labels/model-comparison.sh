#!/usr/bin/env bash

APP=/home-local/dantu1/GIT/PhD/dev/Experiments/cmake-build-release/Applications/rings-and-labels/model-flow

BASE_FOLDER=/home-local/dantu1/GIT/PhD/dev/Experiments/Applications/rings-and-labels/output


model()
{
    shape=$1
    method=$2
    mode=$3

    $APP ${BASE_FOLDER}/flows/$shape/probe/L5 ${BASE_FOLDER}/model/$shape/$method/$mode $method $mode
}

model square concavities probe&
model square concavities improve&

model square convexities probe&
model square convexities improve&

#model flower concavities probe
#model flower concavities improve
#
#model flower convexities probe
#model flower convexities improve