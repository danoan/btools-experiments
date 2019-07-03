#!/usr/bin/env bash

gp_save()
{
	printf "set size 1.0, 0.6;
		set terminal postscript portrait enhanced mono dashed lw 1 \"Helvetica\" 20;
		set key right top;
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

model_plot()
{
	shape=$1
	mode=$2
	method=$3
	gs=$4
	radius=$5

	PLOTS_OUTPUT=${BASE_FOLDER}/output/plots/unlabeled-per-iterations/h$gs/radius-$radius
	mkdir -p $PLOTS_OUTPUT

	OUTPUT_PLOT=${PLOTS_OUTPUT}/plot-model-$shape-$mode-$method.eps

	i=1
	PLOT_STRING="$OUTPUT_PLOT $shape(r=$radius)"
	while [ $i -le $radius ]
	do
	    PLOT_STRING="${PLOT_STRING} ${BASE_FOLDER}/output/model/h$gs/radius-$radius/$shape/$method/$mode/level-$i.txt m=$i"
	    i=$[$i+1]
	done


	create_multiplot ${PLOT_STRING}
}

collection_model_plot()
{
    shape=$1
    gs=$2

    model_plot $shape concavities probe $gs 3
#    model_plot $shape concavities probe $gs 5
#    model_plot $shape concavities probe $gs 7
#    model_plot $shape concavities probe $gs 9
}

individual_plot()
{
	shape=$1
	mode=$2
	method=$3
	gs=$4
	radius=$5

	PLOTS_OUTPUT=${BASE_FOLDER}/output/plots/unlabeled-per-iterations/h$gs/radius-$radius
	mkdir -p $PLOTS_OUTPUT

	OUTPUT_PLOT=${PLOTS_OUTPUT}/plot-individual-$shape-$mode-$method.eps


	PLOT_STRING="$OUTPUT_PLOT $shape(r=$radius)"
	i=0
	while [ $i -le $radius ]
	do
	    PLOT_STRING="${PLOT_STRING} ${BASE_FOLDER}/output/individual/h$gs/radius-$radius/$shape/$method/$mode/L$i.txt m=$i"
	    i=$[$i+1]
	done


	create_multiplot  ${PLOT_STRING}
}

collection_individual_plot()
{
    shape=$1
    gs=$2

    individual_plot $shape concavities probe $gs 3
    individual_plot $shape concavities probe $gs 5
    individual_plot $shape concavities probe $gs 7
    individual_plot $shape concavities probe $gs 9
}


#collection_model_plot square 1.0
#collection_model_plot flower 1.0

#collection_model_plot square 0.5
collection_model_plot flower 0.5

#collection_model_plot square 0.25
#collection_model_plot flower 0.25
