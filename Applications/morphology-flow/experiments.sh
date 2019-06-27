#!/usr/bin/env bash

PROJECT_FOLDER=$(realpath $1)
MORPH_APP=$PROJECT_FOLDER/cmake-build-release/Applications/morphology-flow/morphology-flow
OUTPUT_FOLDER=$PROJECT_FOLDER/Applications/morphology-flow/output

SHAPES="square flower"

for shape in $SHAPES
do
    $MORPH_APP -t ellipse -s 3 -o dilation -i 100 -S $shape -h1.0 \
    $OUTPUT_FOLDER/dilation/ellipse3/$shape&

    $MORPH_APP -t ellipse -s 5 -o dilation -i 100 -S $shape -h1.0 \
    $OUTPUT_FOLDER/dilation/ellipse5/$shape&

    $MORPH_APP -t ellipse -s 5 -o opening -i 100 -S $shape -h1.0 \
    $OUTPUT_FOLDER/opening/ellipse5/$shape&

    $MORPH_APP -t ellipse -s 3 -o opening -i 100 -S $shape -h1.0 \
    $OUTPUT_FOLDER/opening/ellipse3/$shape&

    $MORPH_APP -t ellipse -s 3 -o closing -i 100 -S $shape -h1.0 \
    $OUTPUT_FOLDER/closing/ellipse3/$shape&

    $MORPH_APP -t ellipse -s 5 -o closing -i 100 -S $shape -h1.0 \
    $OUTPUT_FOLDER/closing/ellipse5/$shape
done