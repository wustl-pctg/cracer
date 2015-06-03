#!/usr/bin/env Rscript
require(gplots)

args <- commandArgs(trailingOnly = TRUE)
if (is.na(args[1])) {
	 logFile <- "../logs/skiplist.throughput.dickens.1.23.9.43.log"
} else {
	logFile <- args[1]
}
if (is.na(args[2])) {
	 outFile <- "../graphs/multibatch.pdf"
} else {
	outFile <- args[2]
}

data <- read.csv(file=logFile, header=TRUE, sep=",")
subset <- data[data$P > 0,]
pdf(file=outFile, width=8, height=6)

y_range <- range(0, subset$n10s100b0, subset$n10s100b50,
                 subset$n1000s100b0, subset$n1000s100b50)

par(mar=c(5.1,6.1,4.1,2.1), mgp=c(4,1,0)) # Set Margins

plot(subset$P,subset$n10s100b0, type="o", col="blue", ylim=y_range,
     axes = FALSE, ann = FALSE, pch=0)
lines(subset$P, subset$n10s100b50, type="o", pch=1, lty=1, col="red")
lines(subset$P, subset$n1000s100b0, type="o", pch=2, lty=2, col="green")
lines(subset$P, subset$n1000s100b50, type="o", pch=3, lty=2, col="purple")

axis(1, at=subset$P, lab=subset$P)
axis(2, las=1)

title(main=paste("Throughput vs. Cores"),
      col.main="red", font.main=4, cex.main=2.0)
title(xlab="Cores", col.lab=rgb(0,0.5,0), cex.lab=2.0)
title(ylab="Throughput (ops/ms)", col.lab=rgb(0,0.5,0), cex.lab=2.0)

legend("topright",
       c("n10s100b0", "n10s100b50", "n1000s100b0", "n1000s100b50"),
       cex=1.0, col=c("blue","red","green","purple"),
       pch=0:3, lty=c(1,1,2,2),
       inset=.05)


dev.off()
