#!/usr/bin/env Rscript

# Filled Kernel Density Plot
## d <- density(univariateData)
## plot(d, main="Kernel Density of blah balh")
## polydon(d, col="red", border="blue")

#inverse.rle(list(lengths=COUNTS, values=HEADER))

args <- commandArgs(trailingOnly = TRUE)
if (is.na(args[1])) {
	 logFile <- "sizes.log"
} else {
	logFile <- args[1]
}
if (is.na(args[2])) {
	 outFile <- "graphs/sizes.pdf"
} else {
	outFile <- args[2]
}

data <- read.csv(file=logFile, header=TRUE, sep=",", check.names=FALSE)
h <- inverse.rle(list(lengths=data[1,], values=as.numeric(colnames(data))))
f <- factor(h)

pdf(file=outFile)
plot(f, col = "darkslategray4", border = "seashell3", space=0)
title(main = paste("Histogram of Batch Sizes (",length(h)," batches)", sep=""),
      col.main="red", font.main=4)
title(xlab="Batch Sizes", col.lab=rgb(0,0.5,0))
title(ylab="Frequency", col.lab=rgb(0,0.5,0))
box()

## Other plotting options:
## plot(density(h))
## hist(h)
    
dev.off()
