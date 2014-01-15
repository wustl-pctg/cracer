#!/usr/bin/env Rscript
require(gplots)

args <- commandArgs(trailingOnly = TRUE)
specifier <- paste(system("date +%j", intern=TRUE), ".",
                   system("hostname", intern=TRUE))
specifier <- gsub(" ", "", specifier, fixed=TRUE)

logFile <- paste("../logs/overhead.", specifier, ".log")
if (!is.na(args[1])) logFile <- args[1]

outFile <- paste("../graphs/overhead.", specifier, ".pdf")
if (!is.na(args[2])) outFile <- args[2]

data <- read.csv(file=logFile, header=TRUE, sep=",")
pdf(file=outFile, width=8, height=6)

y_range <- range(0, data$FC, data$RAW, data$COMP,
                 data$SWITCHCOMP, data$SWITCHNOCOMP)

par(mar=c(5.1,6.1,4.1,2.1), mgp=c(4,1,0)) # Set Margins

plot(data$NPROC, data$FC, type="o", col="red", ylim=y_range,
     axes = FALSE, ann = FALSE)
lines(data$NPROC, data$RAW, type="o", pch=22, lty=2, col="blue")
lines(data$NPROC, data$COMP, type="o", pch=23, lty=3, col="green")
lines(data$NPROC, data$SWITCHCOMP, type="o", pch=24, lty=4, col="purple")
lines(data$NPROC, data$SWITCHNOCOMP, type="o",
      pch=25, lty=5, col="orange")

axis(1, at=data$NPROC, lab=data$NPROC)
axis(2, las=1)

title(main=paste("Stack Throughput vs. Nproc"),
      col.main="red", font.main=4, cex.main=2.0)
title(xlab="Nproc", col.lab=rgb(0,0.5,0), cex.lab=2.0)
title(ylab="Throughput (ds ops/ms)", col.lab=rgb(0,0.5,0), cex.lab=2.0)

legend("topright",
       c("Flat combining", "Raw", "Compaction",
         "Switch (raw)", "Switch (compaction)"),
       cex=1.0, col=c("red","blue","green","purple","orange"),
       pch=21:25, lty=1:5, inset=.05)


dev.off()
