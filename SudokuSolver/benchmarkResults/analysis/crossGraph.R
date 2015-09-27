#! /usr/bin/Rscript

cmdLine <- commandArgs(T)

require("ggplot2")
rad <- "results"
benchs <- c("FTP", "9x9")
algorithm <- c("backtrack", "a-star", "genetic")


readFile <- function(file) {
  rawData <- read.table(file)
  dimnames(rawData) <- list(1:nrow(rawData),
                            c("Sudoku", "Status", "Time", "Cycles"))
  as.matrix(rawData[, c("Sudoku", cmdLine[1])])
}

for (bench in benchs) {
  totalData <- 1:2
  for (alg in algorithm) {
    filename <- paste(rad, bench, "-", alg, sep="")
    print(filename)

    fileData <- readFile(filename)
    totalData <- rbind2(totalData, fileData)
  }
  totalData <- totalData[-1,]
  algorithmThis <- rep(algorithm, each=nrow(totalData)/3)
  print(nrow(totalData))
  print(nrow(algorithmThis))
  totalData <- cbind2(algorithmThis, totalData)

  dimnames(totalData) <- list(1:nrow(totalData),
                              c("Algorithm", "Sudoku", cmdLine[1]))
  minimum <- min(as.numeric(as.vector(totalData[, cmdLine[1]])))
  maximum <- max(as.numeric(as.vector(totalData[, cmdLine[1]])))
  totalData <- as.data.frame(totalData)

  totalData[, cmdLine[1]] <- as.numeric(totalData[, cmdLine[1]])
  ggplot(totalData, aes_string(x="Sudoku", y=cmdLine[1], colour="Algorithm")) +
    geom_point() +
    geom_line(aes(group=Algorithm)) +
    scale_y_log10() +
    coord_flip()

  ggsave(paste(bench, cmdLine[1], ".png", sep=""), width=10)
}


