#!/usr/bin/env bash

APP=/home-local/dantu1/GIT/PhD/dev/Experiments/cmake-build-release/Applications/rings-and-labels/individual-flow

BASE_FOLDER=/home-local/dantu1/GIT/PhD/dev/Experiments/Applications/rings-and-labels/output


individual()
{
    shape=$1
    mode=$2
    gs=$3

    $APP ${BASE_FOLDER}/flows/h$gs/$shape/probe/L1 ${BASE_FOLDER}/individual/h$gs/$shape/probe/$mode/L1.txt probe $mode 1&
    $APP ${BASE_FOLDER}/flows/h$gs/$shape/probe/L2 ${BASE_FOLDER}/individual/h$gs/$shape/probe/$mode/L2.txt probe $mode 2&
    $APP ${BASE_FOLDER}/flows/h$gs/$shape/probe/L3 ${BASE_FOLDER}/individual/h$gs/$shape/probe/$mode/L3.txt probe $mode 3&
    $APP ${BASE_FOLDER}/flows/h$gs/$shape/probe/L4 ${BASE_FOLDER}/individual/h$gs/$shape/probe/$mode/L4.txt probe $mode 4
}

individual square concavities 0.5&
individual square convexities 0.5&
individual flower convexities 0.5&
individual flower concavities 0.5
