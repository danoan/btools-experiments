#!/usr/bin/env bash

PROJECT_FOLDER=/home-local/dantu1/GIT/PhD/Experiments
FLOW_APP_FOLDER=/home-local/dantu1/GIT/PhD/BTools

source ${FLOW_APP_FOLDER}/exp/utils/utils.sh


FLOW_APP=${FLOW_APP_FOLDER}/cmake-build-debug/Applications/flow/flow
OUTPUT_FOLDER=$PROJECT_FOLDER/output/graphs

rm -rf $OUTPUT_FOLDER
mkdir -p $OUTPUT_FOLDER

MAX_IT=50



$FLOW_APP -r 3 -i $MAX_IT -a 0 -c 0 -s 0 -p double-opt -n 4 -l 0 -q 1 -t 0 -g 0 -m improve -o  improve-only-opt $OUTPUT_FOLDER/improve&
$FLOW_APP -r 3 -i $MAX_IT -a 0 -c 0 -s 0 -p double -n 4 -l -2 -q 1 -t 0 -g 0 -m improve -o  improve-two-levels-plus-opt $OUTPUT_FOLDER/improve&
$FLOW_APP -r 3 -i $MAX_IT -a 0 -c 0 -s 0 -p double -n 4 -l -1 -q 1 -t 0 -g 0 -m improve -o  improve-one-level-plus-opt $OUTPUT_FOLDER/improve&
#$FLOW_APP -r 3 -i $MAX_IT -a 0 -c 0 -s 0 -p double -n 4 -l 3 -q 1 -t 0 -g 0 -m improve  px-double-around $OUTPUT_FOLDER/improve&

wait

$FLOW_APP -r 3 -i $MAX_IT -a 0 -c 0 -s 0 -p double-opt -n 4 -l 0 -q 1 -t 0 -g 0 -m probe -o  probe-only-opt $OUTPUT_FOLDER/probe&
$FLOW_APP -r 3 -i $MAX_IT -a 0 -c 0 -s 0 -p double -n 4 -l -2 -q 1 -t 0 -g 0 -m probe -o  probe-two-levels-plus-opt $OUTPUT_FOLDER/probe&
$FLOW_APP -r 3 -i $MAX_IT -a 0 -c 0 -s 0 -p double -n 4 -l -1 -q 1 -t 0 -g 0 -m probe -o  probe-one-level-plus-opt $OUTPUT_FOLDER/probe&
#$FLOW_APP -r 3 -i $MAX_IT -a 0 -c 0 -s 0 -p double -n 4 -l 3 -q 1 -t 0 -g 0 -m probe  px-double-around-probe $OUTPUT_FOLDER/probe&

wait

mkdir -p ${OUTPUT_FOLDER}/compare-probe-improve-only-opt
mkdir -p ${OUTPUT_FOLDER}/compare-probe-improve-one-level
mkdir -p ${OUTPUT_FOLDER}/compare-probe-improve-two-levels

cp -R ${OUTPUT_FOLDER}/improve/improve-only-opt ${OUTPUT_FOLDER}/compare-probe-improve-only-opt/improve-only-opt
cp -R ${OUTPUT_FOLDER}/probe/probe-only-opt ${OUTPUT_FOLDER}/compare-probe-improve-only-opt/probe-only-opt

cp -R ${OUTPUT_FOLDER}/improve/improve-one-level-plus-opt ${OUTPUT_FOLDER}/compare-probe-improve-one-level/improve-one-level-plus-opt
cp -R ${OUTPUT_FOLDER}/probe/probe-one-level-plus-opt ${OUTPUT_FOLDER}/compare-probe-improve-one-level/probe-one-level-plus-opt

cp -R ${OUTPUT_FOLDER}/improve/improve-two-levels-plus-opt ${OUTPUT_FOLDER}/compare-probe-improve-two-levels/improve-two-levels-plus-opt
cp -R ${OUTPUT_FOLDER}/probe/probe-two-levels-plus-opt ${OUTPUT_FOLDER}/compare-probe-improve-two-levels/probe-two-levels-plus-opt

create_plots ${OUTPUT_FOLDER}/improve Elastica 0
create_plots ${OUTPUT_FOLDER}/probe Elastica 0
create_plots ${OUTPUT_FOLDER}/probe Unlabeled 2

create_plots ${OUTPUT_FOLDER}/compare-probe-improve-only-opt Elastica 0
create_plots ${OUTPUT_FOLDER}/compare-probe-improve-one-level Elastica 0
create_plots ${OUTPUT_FOLDER}/compare-probe-improve-two-levels Elastica 0


