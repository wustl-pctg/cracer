#takes in a filename and number of columns, first column is always x-axis
#Use the following syntax (ignore the <> operators):
#separate arges with ;
#use ' ' around string args
#gnuplot -e "arg=<arg>;string_arg='<string_arg>" plot.gnu

if (!exists("filename")) print "no file to plot!"; exit gnuplot
if (!exists("cols")) cols = 2
if (!exists("outfile")) outfile=filename
if (exists("title")) set title title
if (exists("xlabel")) set xlabel xlabel
if (exists("ylabel")) set ylabel ylabel

out = outfile.".pdf"
set xrange[0:*] #The * indicates auto range, can manually specify if needed.
set yrange[0:*]
set datafile separator  ","
set key autotitle columnhead
set terminal pdf
set output out
plot for [i=2:cols] filename using 1:i with lines