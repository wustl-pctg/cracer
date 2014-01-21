#!/usr/bin/env Rscript
require(gplots)
#options(scipen=10)

args <- commandArgs(trailingOnly = TRUE)
if (is.na(args[1])) {
	 logFile <- "../logs/multi.out"
} else {
	logFile <- args[1]
}
if (is.na(args[2])) {
	 outFile <- "../graphs/multi.pdf"
} else {
	outFile <- args[2]
}

data <- read.csv(file=logFile, header=TRUE, sep=",")
subset <- data[data$P > 0,]
pdf(file=outFile, width=8, height=6)

y_range <- range(0, subset$a, subset$b, subset$c, subset$d,
                 subset$e, subset$f, subset$g, subset$h,
                 subset$i, subset$j, subset$k)

par(mar=c(5.1,6.1,4.1,2.1), mgp=c(4,1,0)) # Set Margins

plot(subset$P,subset$a, type="o", col="blue", ylim=y_range,
     axes = FALSE, ann = FALSE)
lines(subset$P, subset$b, type="o", pch=15, lty=1, col="red")
lines(subset$P, subset$c, type="o", pch=16, lty=1, col="green")
lines(subset$P, subset$d, type="o", pch=17, lty=1, col="purple")
lines(subset$P, subset$e, type="o", pch=18, lty=1, col="black")
lines(subset$P, subset$f, type="o", pch=19, lty=1, col="cyan")
lines(subset$P, subset$g, type="o", pch=20, lty=2, col="blue")
lines(subset$P, subset$h, type="o", pch=21, lty=2, col="red")
lines(subset$P, subset$i, type="o", pch=22, lty=2, col="green")
lines(subset$P, subset$j, type="o", pch=23, lty=2, col="purple")
lines(subset$P, subset$k, type="o", pch=24, lty=2, col="black")

axis(1, at=subset$P, lab=subset$P)
axis(2, las=1)

title(main=paste("Throughput vs. Cores"),
      col.main="red", font.main=4, cex.main=2.0)
title(xlab="Cores", col.lab=rgb(0,0.5,0), cex.lab=2.0)
title(ylab="Throughput (ops/ms)", col.lab=rgb(0,0.5,0), cex.lab=2.0)

legend("topright",
       c("0", "10", "25", "50", "100", "150", "200", "300", "500", "750", "1000"),
       cex=1.0, col=c("blue","red", "green", "purple","black","cyan","blue",
                    "red","green","purple","black"),
       pch=c(21,14:24), lty=c(1,1,1,1,1,1,2,2,2,2,2),
       inset=.05)


dev.off()
