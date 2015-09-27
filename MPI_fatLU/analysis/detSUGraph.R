#! /usr/bin/Rscript

require("ggplot2")
source("util.R")

RawData <- readSource("../runs")
Column  <- "Time"

NSize          <- c( 1024, 2048, 4096, 8192 )
MachineNumber  <- c( 1, 2, 4, 8, 16 )

GraphTable <- data.frame(0, 0, 0)
colnames(GraphTable) <- c("N", "Processos", "SpeedUp")

for (M in 1:length(MachineNumber)) {
  for (I in 1:length(NSize)) {
    NewRow <- c(NSize[I], MachineNumber[M],
                mean(RawData[(RawData$N == NSize[I] & 
                              RawData$Machines == MachineNumber[M]), Column]))
    GraphTable <- rbind2(GraphTable, NewRow)
  }
}

GraphTable <- GraphTable[-1, ]

for (M in MachineNumber[-1]) {
  for (I in NSize) {
    Value <- GraphTable[(GraphTable$N == I & GraphTable$Processos == M), "SpeedUp"]

    GraphTable[(GraphTable$N == I & GraphTable$Processos == M), "SpeedUp"] <-
      GraphTable[(GraphTable$N == I & GraphTable$Processos == 1), "SpeedUp"] / Value
  }
}

GraphTable <- GraphTable[GraphTable$Processos != 1, ]
GraphTable$N <- as.factor(GraphTable$N)
print(GraphTable)

ggplot(GraphTable, aes(x=Processos, y=SpeedUp, color=N)) +
  geom_point(size=5) +
  geom_line(aes(group=N)) +
  geom_segment(alpha=0.2, aes(x=1, y=1, xend=17, yend=17), colour="black", size=1, linetype="dashed") + 
  scale_y_continuous(breaks=c(0, 2, 4, 6, 8, 16), limits=c(0, 18)) +
  scale_x_continuous(breaks=c(0, 2, 4, 6, 8, 16), limits=c(0, 18))
  
ggsave("DetailedSpeedUp.png")
