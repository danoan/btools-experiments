#!/usr/bin/env bash

gp_save()
{
	printf "set size 1.0, 0.6;
		set terminal postscript portrait enhanced color dashed lw 2 \"Helvetica\" 20;
		set key right top;
		set output \"my-plot.ps\";
		replot;
		set terminal x11;
		set size 1,1;"
}

gp_plot_config()
{
    printf "set title '$1';
    set yrange [-0.1:0.2];
    set xlabel 'Iterations';
    set ylabel 'Pairwise Ratio';"
}


gp_add_plot()
{
    printf "'$1' u 1:3 w l title '$2',"
}

gp_last_plot()
{
	printf "'$1' u 1:3 w l title '$2';"
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

pariwise_plot()
{
	shape=$1
	mode=$2
	method=$3
    gs=$4
    radius=$5

    PLOTS_OUTPUT=${BASE_FOLDER}/output/plots/pairwise-ratio/h$gs/radius-$radius
    mkdir -p $PLOTS_OUTPUT

	OUTPUT_PLOT=${PLOTS_OUTPUT}/plot-pairwiseratio-$shape-$mode-$method.eps
	DATA_FOLDER=${BASE_FOLDER}/output/model/h$gs/radius-$radius/$shape/$method/$mode

	i=1
	PLOT_STRING=""
	while [ $i -le $radius ]
	do
	  PLOT_STRING=" ${PLOT_STRING} ${DATA_FOLDER}/level-$i.txt m=$i"
	  i=$[$i+1]
    done

	create_multiplot $OUTPUT_PLOT "Pairwise Ratio $shape(r=$radius)" $PLOT_STRING
}

pairwise_plot_collection()
{
    shape=$1
    gs=$2

    pariwise_plot $shape concavities probe $gs 3

    pariwise_plot $shape concavities probe $gs 5

    pariwise_plot $shape concavities probe $gs 7

    pariwise_plot $shape concavities probe $gs 9
}

lower_higher_level_plot()
{
	mode=$1
	method=$2
    gs=$3
    radius=$4

    PLOTS_OUTPUT=${BASE_FOLDER}/output/plots/pairwise-ratio/h$gs/radius-$radius
    mkdir -p $PLOTS_OUTPUT

	OUTPUT_PLOT=${PLOTS_OUTPUT}/plot-pairwiseratio-lowerHigher-$mode-$method.eps

    PLOT_STRING=""
	PLOT_STRING=" ${PLOT_STRING} ${BASE_FOLDER}/output/model/h$gs/radius-$radius/square/$method/$mode/level-1.txt square(m=1)"
	PLOT_STRING=" ${PLOT_STRING} ${BASE_FOLDER}/output/model/h$gs/radius-$radius/square/$method/$mode/level-$radius.txt square(m=$radius)"

	PLOT_STRING=" ${PLOT_STRING} ${BASE_FOLDER}/output/model/h$gs/radius-$radius/flower/$method/$mode/level-1.txt flower(m=1)"
	PLOT_STRING=" ${PLOT_STRING} ${BASE_FOLDER}/output/model/h$gs/radius-$radius/flower/$method/$mode/level-$radius.txt flower(m=$radius)"

	create_multiplot $OUTPUT_PLOT "Lower and Higher Rings (r=5)" $PLOT_STRING

}

#pairwise_plot_collection square 1.0
#pairwise_plot_collection flower 1.0
#
#pairwise_plot_collection square 0.5
#pairwise_plot_collection flower 0.5

#pairwise_plot_collection square 0.25
#pairwise_plot_collection flower 0.25

lower_higher_level_plot concavities probe 0.5 5
