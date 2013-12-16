#!/usr/bin/env Rscript
require(gplots)
#options(scipen=10)

args <- commandArgs(trailingOnly = TRUE)
if (is.na(args[1])) {
	 logFile <- "../logs/overhead.350.log"
} else {
	logFile <- args[1]
}
if (is.na(args[2])) {
	 outFile <- "../graphs/overhead.350.dickens.pdf"
} else {
	outFile <- args[2]
}

data <- read.csv(file=logFile, header=TRUE, sep=",")
pdf(file=outFile, width=8, height=6)

y_range <- range(0, data$FC, data$TERM)

par(mar=c(5.1,6.1,4.1,2.1), mgp=c(4,1,0)) # Set Margins

plot(data$NPROC, data$TERM, type="o", col="red", ylim=y_range,
     axes = FALSE, ann = FALSE)
lines(data$NPROC, data$FC, type="o", pch=22, lty=2, col="blue")
#lines(data$NPROC, data$RAW, type="o", pch=22, lty=2, col="red")
axis(1, at=data$NPROC, lab=data$NPROC)
axis(2, las=1)

title(main=paste("Stack Throughput vs. Nproc"),
      col.main="red", font.main=4, cex.main=2.0)
title(xlab="Nproc", col.lab=rgb(0,0.5,0), cex.lab=2.0)
title(ylab="Throughput (ds ops/ms)", col.lab=rgb(0,0.5,0), cex.lab=2.0)

legend("topright",
       c("Separate batch termination", "Flat combining"),
       cex=1.0, col=c("red","blue"),
       pch=21:22, lty=1:2, inset=.05)


dev.off()
