#!/usr/bin/env bash

gp_save()
{
	printf "set size 1.0, 0.6;
		set terminal postscript portrait enhanced mono dashed lw 1 \"Helvetica\" 20;
		set key right center;
		set output \"my-plot.ps\";
		replot;
		set terminal x11;
		set size 1,1;"
}

gp_plot_config()
{
    printf "set title '$1';
    set yrange [-0.1:1.1]; 
    set xlabel 'Iterations';
    set ylabel 'Perc. Unlabeled';"
}


gp_add_plot()
{
    printf "'$1' u 1:2 w l title '$2',"
}

gp_last_plot()
{
	printf "'$1' u 1:2 w l title '$2';"
}

create_multiplot()
{
	fileoutput=$1;shift;
	plottitle="$1";shift;

	buffer="$(gp_plot_config "$plottitle")plot "
	i=0
	num_plots=`expr ${#} / 2 - 1`

	while [ ${i} -lt ${num_plots} ]
	do
		subplotDataFile=$1
		subplotTitle=$2
		buffer="${buffer}$(gp_add_plot $subplotDataFile $subplotTitle)"
		shift; shift;

		i=`expr $i + 1`
	done

	if [ $num_plots -eq 0 ]
	then
		buffer="${buffer}$(gp_last_plot $1 $2)"
	else
		buffer="${buffer}$(gp_last_plot $1 $2)"
	fi

	buffer="${buffer}$(gp_save)"


	`gnuplot -e "$buffer"`
	`mv my-plot.ps ${fileoutput}`
}

BASE_FOLDER=$(realpath $1)
PLOTS_OUTPUT=${BASE_FOLDER}/plots-out
mkdir -p $PLOTS_OUTPUT

model_plot()
{
	shape=$1
	mode=$2
	method=$3
    	gs=$4

	OUTPUT_PLOT=${PLOTS_OUTPUT}/plot-model-$shape-$mode-$method.eps
	create_multiplot $OUTPUT_PLOT "Unlabeled" "${BASE_FOLDER}/output/model/h$gs/$shape/$method/$mode/level-1.txt" "m=1" \
	"${BASE_FOLDER}/output/model/h$gs/$shape//$method/$mode/level-2.txt" "m=2" \
	"${BASE_FOLDER}/output/model/h$gs/$shape/$method/$mode/level-3.txt" "m=3" \
	"${BASE_FOLDER}/output/model/h$gs/$shape/$method/$mode/level-4.txt" "m=4" \
	"${BASE_FOLDER}/output/model/h$gs/$shape/$method/$mode/level-5.txt" "m=5"
}


model_plot square concavities probe 0.5
model_plot square convexities probe 0.5
model_plot flower concavities probe 0.5
model_plot flower convexities probe 0.5
