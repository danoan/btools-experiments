plot "gs_0.txt" using 1:($2/$3) with lines title "GS=1", \
     "gs_1.txt" using 1:($2/$3) with lines title "GS=0.9", \
     "gs_2.txt" using 1:($2/$3) with lines title "GS=0.8", \
     "gs_3.txt" using 1:($2/$3) with lines title "GS=0.7", \
     "gs_4.txt" using 1:($2/$3) with lines title "GS=0.6", \
     "gs_5.txt" using 1:($2/$3) with lines title "GS=0.5", \
     "gs_6.txt" using 1:($2/$3) with lines title "GS=0.4", \
     "gs_7.txt" using 1:($2/$3) with lines title "GS=0.3", \
     "gs_8.txt" using 1:($2/$3) with lines title "GS=0.2", \
     "gs_9.txt" using 1:($2/$3) with lines title "GS=0.1"



plot "gs_0.txt" using 1:4 with lines title "GS=1", \
     "gs_1.txt" using 1:4 with lines title "GS=0.9", \
     "gs_2.txt" using 1:4 with lines title "GS=0.8", \
     "gs_3.txt" using 1:4 with lines title "GS=0.7", \
     "gs_4.txt" using 1:4 with lines title "GS=0.6", \
     "gs_5.txt" using 1:4 with lines title "GS=0.5", \
     "gs_6.txt" using 1:4 with lines title "GS=0.4", \
     "gs_7.txt" using 1:4 with lines title "GS=0.3", \
     "gs_8.txt" using 1:4 with lines title "GS=0.2", \
     "gs_9.txt" using 1:4 with lines title "GS=0.1"



plot "gs_0.txt" using 1:4 with points title "GS=1", \
     "gs_2.txt" using 1:4 with points title "GS=0.8", \
     "gs_5.txt" using 1:4 with points title "GS=0.5", \
     "gs_8.txt" using 1:4 with points title "GS=0.2", \
     "gs_9.txt" using 1:4 with points title "GS=0.1"