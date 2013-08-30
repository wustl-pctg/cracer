#!/usr/bin/env Rscript
require(gplots)

args <- commandArgs(trailingOnly = TRUE)
if (is.na(args[1])) {
	 logFile <- "param.log"
} else {
	logFile <- args[1]
}
if (is.na(args[2])) {
	 outFile <- "graphs/param.pdf"
} else {
	outFile <- args[2]
}
if (is.na(args[3])) {
	 operations <- 100000
} else {
	operations <- as.numeric(args[3])
}

data <- read.csv(file=logFile, header=TRUE, sep=",")
subset <- data[data$OPS == operations,]
pdf(file=outFile)

y_range <- range(0, subset$BATCH, subset$LOCKS)

plot(subset$WORKTIME, subset$BATCH, type="o", col="blue", ylim=y_range,
									axes = FALSE, ann = FALSE)
lines(subset$WORKTIME, subset$LOCKS, type="o", pch=22, lty=2, col="red")
lab <- seq(0, y_range[2], ceiling(signif(y_range[2],digits=2))/10)
axis(1, at=subset$WORKTIME, lab=subset$WORKTIME)
axis(2, at=lab, lab=lab/1000)
title(main=paste("Execution Time vs. Work with ", formatC(operations, format="d"), " Operations"),
      col.main="red", font.main=4)
title(xlab="Work (ms)", col.lab=rgb(0,0.5,0))
title(ylab="Time (s)", col.lab=rgb(0,0.5,0))

## legend("bottomright", c("batch", "locks"), cex=0.8,
##        col=c("blue","red"), pch=21:22, lty=1:2)
smartlegend(x="right", y="bottom", c("batch", "locks"), cex=0.8,
            col=c("blue","red"), pch=21:22, lty=1:2)

dev.off()
