#!/usr/bin/Rscript
# graph.R


args <- commandArgs(trailingOnly = TRUE)
if (is.na(args[1])) {
	 operations <- 90001
} else if (suppressWarnings(is.na(as.numeric(args[1])))) {
	stop("Invalid argument")
} else {
	operations <- as.numeric(args[1])
}

data <- read.csv(file="param.log.csv", header=TRUE, sep=",")
subset <- data[data$OPS == operations,]
pdf(file="test.pdf")

y_range <- range(0, subset$BATCH, subset$LOCKS)

plot(subset$WORK, subset$BATCH, type="o", col="blue", ylim=y_range,
									axes = FALSE, ann = FALSE)
lines(subset$WORK, subset$LOCKS, type="o", pch=22, lty=2, col="red")
axis(1, at=subset$WORK, lab=subset$WORK)
axis(2, las=1, at=1000*0:y_range[2])

title(main=paste("Execution Time vs. Work with ", operations, " Operations"),
														col.main="red", font.main=4)
title(xlab="Work", col.lab=rgb(0,0.5,0))
title(ylab="Time", col.lab=rgb(0,0.5,0))
legend(1, y_range[2], c("batch", "locks"), cex=0.8,
					col=c("blue","red"), pch=21:22, lty=1:2);


#axis(1, at=subset$WORK, lab=subset$WORK)
dev.off()
