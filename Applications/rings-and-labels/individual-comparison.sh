#!/usr/bin/env bash

$APP=/home-local/dantu1/GIT/PhD/dev/Experiments/cmake-build-release/Applications/rings-and-labels/individual-flow

BASE_FOLDER=/home-local/dantu1/GIT/PhD/dev/Experiments/Applications/rings-and-labels/output


individual()
{
    shape=$1
    mode=$2

    $APP ${BASE_FOLDER}/flows/$shape/probe/L5 ${BASE_FOLDER}/individual/$shape/probe/$mode/L1.txt 1&
    $APP ${BASE_FOLDER}/flows/$shape/probe/L5 ${BASE_FOLDER}/individual/$shape/probe/$mode/L2.txt 2&
    $APP ${BASE_FOLDER}/flows/$shape/probe/L5 ${BASE_FOLDER}/individual/$shape/probe/$mode/L3.txt 3&
    $APP ${BASE_FOLDER}/flows/$shape/probe/L5 ${BASE_FOLDER}/individual/$shape/probe/$mode/L4.txt 4
}

individual square concavities
individual square convexities
individual flower convexities
individual flower concavities