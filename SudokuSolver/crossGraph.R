#! /usr/bin/Rscript

cmdLine <- commandArgs(T)

rad <- "results"
benchs <- c("FTP", "9x9")
algorithm <- c("backtrack", "a-star", "genetic")


readFile <- function(file) {
  rawData <- read.table(file)
  dimnames(rawData) <- list(1:nrow(rawData),
                            c("Sudoku", "Status", "Time", "Cycles"))
  as.matrix(rawData[, c("Sudoku", cmdLine[1])])
}

totalData <- 1:2
for (bench in benchs) {
  for (alg in algorithm) {
    filename <- paste(rad, bench, "-", alg, sep="")
    print(filename)
    totalData <- rbind2(totalData, readFile(filename))
  }
  totalData <- totalData[-1,]
  algorithm <- rep(algorithm, each=nrow(totalData)/3)
  totalData <- cbind2(algorithm, totalData)

  dimnames(totalData) <- list(1:nrow(totalData),
                              c("Algorithm", "Sudoku", cmdLine[1]))
  ggplot(totalData, aes_string(x="Sudoku", y=cmdLine[1], colour="Algorithm")) +
    geom_point() +
    coord_flip()

  ggsave(paste(bench, ".png", sep=""))
}


