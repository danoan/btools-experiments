#!/usr/bin/env bash

PROJECT_FOLDER=/home-local/dantu1/GIT/PhD/Experiments
DCF_LP=${PROJECT_FOLDER}/cmake-build-debug/Applications/dcf-lp/dcf-lp

OUTPUT_FOLDER=${PROJECT_FOLDER}/output/dcf-lp

#$DCF_LP -o boundary -a boundary -s square -i 2 ${OUTPUT_FOLDER}/square/boundary-boundary&
#
#$DCF_LP -o boundary -a around -l 1 -s square -i 10 ${OUTPUT_FOLDER}/square/boundary-around-l1&
#$DCF_LP -o boundary -a around -l 2 -s square -i 20 ${OUTPUT_FOLDER}/square/boundary-around-l2&
#$DCF_LP -o boundary -a around -l 3 -s square -i 40 ${OUTPUT_FOLDER}/square/boundary-around-l3&

#wait

$DCF_LP -o boundary -a boundary -s flower -i 2 ${OUTPUT_FOLDER}/flower/boundary-boundary&

$DCF_LP -o boundary -a around -l 1 -s flower -i 5 ${OUTPUT_FOLDER}/flower/boundary-around-l1&
wait

$DCF_LP -o boundary -a around -l 2 -s flower -i 10 ${OUTPUT_FOLDER}/flower/boundary-around-l2&
$DCF_LP -o boundary -a around -l 3 -s flower -i 10 ${OUTPUT_FOLDER}/flower/boundary-around-l3&