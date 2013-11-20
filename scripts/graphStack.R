#!/usr/bin/env Rscript
require(gplots)
#options(scipen=10)

args <- commandArgs(trailingOnly = TRUE)
if (is.na(args[1])) {
	 logFile <- "../logs/stackBench80_20.log"
} else {
	logFile <- args[1]
}
if (is.na(args[2])) {
	 outFile <- "../graphs/stack.pdf"
} else {
	outFile <- args[2]
}

data <- read.csv(file=logFile, header=TRUE, sep=",")
subset <- data[data$PROCS > 0,]
pdf(file=outFile, width=8, height=6)

y_range <- range(0, subset$BATCH_NO_STEAL, subset$FC_80_20)

par(mar=c(5.1,6.1,4.1,2.1), mgp=c(4,1,0)) # Set Margins

plot(subset$PROCS,subset$BATCH_NO_STEAL, type="o", col="blue", ylim=y_range,
									axes = FALSE, ann = FALSE)
lines(subset$PROCS, subset$FC_80_20, type="o", pch=22, lty=2, col="red")
axis(1, at=subset$PROC, lab=subset$PROC)
## tiks = seq(0,40000,5000);
## axis(2, at=tiks, lab=tiks)
axis(2, las=1)

title(main=paste("Throughput vs. Cores"),
      col.main="red", font.main=4, cex.main=2.0)
title(xlab="Cores", col.lab=rgb(0,0.5,0), cex.lab=2.0)
title(ylab="Throughput (ops/ms)", col.lab=rgb(0,0.5,0), cex.lab=2.0)

legend("topright", c("Batch (Sequential)", "Flat Combining"), cex=1.0,
            col=c("blue","red"), pch=21:22, lty=1:2, inset=.05)


dev.off()
