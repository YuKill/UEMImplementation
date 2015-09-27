#! /usr/bin/Rscript

readSource <- function(path) {
  RawData <- 
    read.table(path, as.is=F, colClasses=c("character", rep("numeric", 5)))

  colnames(RawData) <- 
    c("N", "Machines", "LUTime", "Time", "SendTime", "RecvTime"); 

  RawData
}
