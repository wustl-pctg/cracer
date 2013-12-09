#!/usr/bin/env Rscript
require(gplots)
#options(scipen=10)

args <- commandArgs(trailingOnly = TRUE)
if (is.na(args[1])) {
	 logFile <- "../logs/overhead.342.log"
} else {
	logFile <- args[1]
}
if (is.na(args[2])) {
	 outFile <- "../graphs/overhead.342.pdf"
} else {
	outFile <- args[2]
}

data <- read.csv(file=logFile, header=TRUE, sep=",")
pdf(file=outFile, width=8, height=6)

y_range <- range(0, data$FULL, data$RAW, data$COLLECT,data$FC)

par(mar=c(5.1,6.1,4.1,2.1), mgp=c(4,1,0)) # Set Margins

plot(data$NPROC, data$FULL, type="o", col="blue", ylim=y_range,
     axes = FALSE, ann = FALSE)
lines(data$NPROC, data$RAW, type="o", pch=22, lty=2, col="red")
lines(data$NPROC, data$COLLECT, type="o", pch=23, lty=3, col="green")
lines(data$NPROC, data$FC, type="o", pch=24, lty=4, col="purple")
axis(1, at=data$NPROC, lab=data$NPROC)
axis(2, las=1)

title(main=paste("Stack Throughput vs. Nproc"),
      col.main="red", font.main=4, cex.main=2.0)
title(xlab="Nproc", col.lab=rgb(0,0.5,0), cex.lab=2.0)
title(ylab="Throughput (ds ops/ms)", col.lab=rgb(0,0.5,0), cex.lab=2.0)

legend("topright",
       c("Full", "Raw mode", "Compaction (sequential)", "Flat combining"),
       cex=1.0, col=c("blue","red","green","purple"),
       pch=21:24, lty=1:4, inset=.05)


dev.off()
