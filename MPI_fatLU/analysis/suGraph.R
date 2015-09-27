#! /usr/bin/Rscript

require("ggplot2")
source("util.R")

RawData <- readSource("../runs")

MachineNumber  <- c( 1, 2, 4, 8, 16)
OverallSpeedUp <- c( 0, 0, 0, 0, 0)

OverallSpeedUp[1] <- mean(RawData[RawData$Machines == 1, "Time"])
for (I in 2:length(MachineNumber)) {
  OverallSpeedUp[I] <- OverallSpeedUp[1] / 
        mean(RawData[RawData$Machines == MachineNumber[I], "Time"])   
}

GraphTable <- data.frame(MachineNumber[-1], OverallSpeedUp[-1])
colnames(GraphTable) <- c("Processos", "SpeedUp")

ggplot(GraphTable, aes(x=Processos, y=SpeedUp)) +
  geom_point(size=5) +
  geom_line() +
  geom_segment(alpha=0.2, aes(x=1, y=1, xend=17, yend=17), colour="black", size=1, linetype="dashed") + 
  scale_y_continuous(breaks=c(0, 2, 4, 6, 8, 16), limits=c(0, 18)) +
  scale_x_continuous(breaks=c(0, 2, 4, 6, 8, 16), limits=c(0, 18))
  
ggsave("OverallSpeedUp.png")
