#!/usr/bin/env Rscript

args <- commandArgs(trailingOnly = TRUE)
if (is.na(args[1])) {
    outFile <- "graphs/tests.pdf"
} else {
    outFile <- args[1]
}
if (is.na(args[2])) {
    nproc <- 8
} else {
    nproc <- as.numeric(args[2])
}
t1 <- paste("test.",nproc,".0.0.s.log",sep="")
t2 <- paste("test.",nproc,".0.0.p.log",sep="")
t3 <- paste("test.",nproc,".0.100.s.log",sep="")
t4 <- paste("test.",nproc,".50.100.p.log",sep="")
t5 <- paste("test.",nproc,".50.100.s.log",sep="")

d1 <- read.csv(file=t1, header=TRUE, sep=",")
d2 <- read.csv(file=t2, header=TRUE, sep=",")
d3 <- read.csv(file=t3, header=TRUE, sep=",")
d4 <- read.csv(file=t4, header=TRUE, sep=",")
d5 <- read.csv(file=t5, header=TRUE, sep=",")
pdf(file=outFile)

y_range <- range(0, d1$BATCH, d2$BATCH, d3$BATCH, d4$BATCH, d5$BATCH)

plot(d1$WORKTIME, d1$BATCH, type="o", col="blue", ylim=y_range,
     axes = FALSE, ann = FALSE)
lines(d2$WORKTIME, d2$BATCH, type="o", pch=22, lty=2, col="red")
lines(d3$WORKTIME, d2$BATCH, type="o", pch=23, lty=3, col="green")
lines(d4$WORKTIME, d2$BATCH, type="o", pch=24, lty=4, col="orange")
lines(d5$WORKTIME, d2$BATCH, type="o", pch=25, lty=5, col="purple")
axis(1, at=d1$WORKTIME, lab=d1$WORKTIME)
axis(2, las=1, at=10000*0:y_range[2])

title(main="Execution Time vs. Work",
      col.main="red", font.main=4)
title(xlab="Work", col.lab=rgb(0,0.5,0))
title(ylab="Time", col.lab=rgb(0,0.5,0))
legend("topleft", y_range[2], c("0,0,s","0,0,p","0,100,s","50,100,p","50,100,s"), cex=0.8,
       col=c("blue","red","green","orange","purple"), pch=21:25, lty=1:5);

dev.off()
