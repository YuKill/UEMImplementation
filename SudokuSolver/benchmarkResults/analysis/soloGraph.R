#! /usr/bin/Rscript

cmdLine <- commandArgs(T)

rawData <- read.table(cmdLine[1])
dimnames(rawData) <- list(1:nrow(rawData),
                          c("Benchmark", "Status", "Time", "Cycles"))
yAxis <- cmdLine[2]

rawData <- rawData[, c("Benchmark", yAxis)]

require("ggplot2")
ggplot(data=rawData, aes_string(x="Benchmark", y=yAxis, colour="Benchmark")) +
  geom_point() +
  geom_line(aes(group="Benchmark")) +
  coord_flip()


stdHeight <- 7
if (length(cmdLine) == 3)
  stdHeight <- as.numeric(cmdLine[3])

Output <- strsplit(cmdLine[1], split="/")[[1]]
Output <- Output[length(Output)]
Output <- paste(Output, yAxis, sep="-")
Output <- paste(Output, ".png", sep="")
ggsave(file=Output, height=stdHeight)
