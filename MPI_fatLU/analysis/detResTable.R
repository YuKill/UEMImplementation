#! /usr/bin/Rscript

source("util.R")

RawData <- readSource("../runs")

MachineNumber <- c(1, 2, 4, 8, 16)
NSize         <- c(1024, 2048, 4096, 8192)

cat("\\begin{table}[H]\n")
cat("\\caption{Resultado detalhado da execucao do algoritmo.}\n")
cat("\\label{detResTable}\n")
cat("\\begin{center}\n")
cat("\\begin{tabular}{ ||c|c|c|c|| }\n")
cat("\\hline\n")
cat("Processos & Instancia & Tempo total(s) & Overhead da comunicacao \\\\\n")
cat("\\hline\n")

for (M in MachineNumber) {
  cat(paste("\\multirow{", length(NSize), "}{*}{", M, "}", sep=""))
  for (I in NSize) {
    Filter <- (RawData$N == I & RawData$Machines == M)
    Total  <- mean(RawData[Filter, "Time"])
    Value  <- 0
    if (M != 1)
      Value  <- (mean(RawData[Filter, "RecvTime"]) +
                 mean(RawData[Filter, "SendTime"])) / Total

    cat(paste(" & ", I," & ", round(Total, 2), " & ", round(Value*100),"\\% \\\\\n", sep="")) 
    
  }
  cat("\\hline\n")
}

cat("\\end{tabular}\n")
cat("\\end{center}\n")
cat("\\end{table}\n")
