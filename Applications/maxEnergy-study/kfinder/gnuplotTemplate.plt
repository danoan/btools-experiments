plot "plotRatioTotalTev.txt" using 1:($2/$3) with lines title "MaxEnergy/TEV"

set arrow 1 from 0,0.01 to 1,0.01 nohead
plot "gs_0.txt" using 1:4 with points title "GS=1", \
     "gs_2.txt" using 1:4 with points title "GS=0.8", \
     "gs_5.txt" using 1:4 with points title "GS=0.5", \
     "gs_8.txt" using 1:4 with points title "GS=0.2", \
     "gs_9.txt" using 1:4 with points title "GS=0.1"
