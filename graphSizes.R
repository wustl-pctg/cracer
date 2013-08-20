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
	 outFile <- "sizes.pdf"
} else {
	outFile <- args[2]
}

data <- read.csv(file=logFile, header=TRUE, sep=",", check.names=FALSE)
h <- inverse.rle(list(lengths=data[1,], values=as.numeric(colnames(data))))

dev.off()
