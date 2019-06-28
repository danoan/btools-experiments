#!/usr/bin/env bash

gp_save()
{
	printf "set size 1.0, 0.6;
		set terminal postscript portrait enhanced color dashed lw 1 \"Helvetica\" 14;
		set output \"my-plot.ps\";
		replot;
		set terminal x11;
		set size 1,1;"
}

gp_plot_config()
{
    printf "set title '$1';
    set xlabel 'Iterations';
    set ylabel 'Unlabeled';"
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

BASE_FOLDER=/home-local/dantu1/GIT/PhD/dev/Experiments/Applications/rings-and-labels
OUTPUT_PLOT=${BASE_FOLDER}/plot.eps

create_multiplot $OUTPUT_PLOT "Unlabeled" "${BASE_FOLDER}/output/unlabeled/flower/level-1.txt" "Level1" \
"${BASE_FOLDER}/output/unlabeled/flower/level-2.txt" "Level2" \
"${BASE_FOLDER}/output/unlabeled/flower/level-3.txt" "Level3" \
"${BASE_FOLDER}/output/unlabeled/flower/level-4.txt" "Level4"

GRAPH_PLOT_APP=${SCRIPT_FOLDER}/graph-plot.sh