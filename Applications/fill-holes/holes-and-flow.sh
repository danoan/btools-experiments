#!/usr/bin/env bash

PROJECT_FOLDER=/home-local/dantu1/GIT/PhD/dev/Experiments
HOLES_APP=$PROJECT_FOLDER/cmake-build-release/Applications/fill-holes/fill-holes
FLOW_APP=/home-local/dantu1/GIT/PhD/BTools/build-release/modules/Applications/shape-flow/shape-flow
SUMMARY_APP=/home-local/dantu1/GIT/PhD/BTools/build-release/modules/Applications/summary-flow/summary-flow

OUTPUT_FOLDER=$PROJECT_FOLDER/Applications/fill-holes/output

$HOLES_APP 5 80 15 $OUTPUT_FOLDER/square-holes.pgm
$FLOW_APP -i100 -r6 -p double -q1.0 -t0 -g0 -l-1 -S${OUTPUT_FOLDER}/square-holes.pgm ${OUTPUT_FOLDER}/square-holes
$SUMMARY_APP ${OUTPUT_FOLDER}/square-holes summary_flow 10 eps
