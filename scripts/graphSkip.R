#!/usr/bin/env Rscript
require(gplots)
#options(scipen=10)

args <- commandArgs(trailingOnly = TRUE)
if (is.na(args[1])) {
	 logFile <- "../logs/skiplist1-16-19-24.log"
} else {
	logFile <- args[1]
}
if (is.na(args[2])) {
	 outFile <- "../graphs/skip.pdf"
} else {
	outFile <- args[2]
}

data <- read.csv(file=logFile, header=TRUE, sep=",")
subset <- data[data$P > 0,]
pdf(file=outFile, width=8, height=6)

y_range <- range(0, subset$FC, subset$BatchSeq, subset$BatchPar)

par(mar=c(5.1,6.1,4.1,2.1), mgp=c(4,1,0)) # Set Margins

plot(subset$P,subset$BatchSeq, type="o", col="blue", ylim=y_range,
     axes = FALSE, ann = FALSE)
lines(subset$P, subset$BatchPar, type="o", pch=22, lty=2, col="red")
lines(subset$P, subset$FC, type="o", pch=23, lty=3, col="green")
axis(1, at=subset$P, lab=subset$P)
axis(2, las=1)

title(main=paste("Throughput vs. Cores"),
      col.main="red", font.main=4, cex.main=2.0)
title(xlab="Cores", col.lab=rgb(0,0.5,0), cex.lab=2.0)
title(ylab="Throughput (ops/ms)", col.lab=rgb(0,0.5,0), cex.lab=2.0)

legend("topright",
       c("Batch (Sequential)", "Batch (Parallel)", "Flat Combining"),
       cex=1.0, col=c("blue","red", "green"), pch=21:23, lty=1:3,
       inset=.05)


dev.off()
