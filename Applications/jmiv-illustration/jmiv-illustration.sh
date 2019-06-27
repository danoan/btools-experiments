#!/usr/bin/env bash

PROJECT_FOLDER=/home-local/dantu1/GIT/PhD/dev/Experiments
REGIONS_APP=/home-local/dantu1/GIT/PhD/BTools/build-release/modules/Applications/regions-of-interest/regions-of-interest

OUTPUT_FOLDER=$PROJECT_FOLDER/Applications/jmiv-illustration/output
$REGIONS_APP -r5 -p single -n4 -l2 -Ssquare -h0.5 $OUTPUT_FOLDER/regions2.svg