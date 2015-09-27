package scG

import scala.io.Source._

object main {
  def main(args: Array[String]): Unit = {
    val path = args(0)
    val pop = args(1).toInt
    val rate = args(5).toFloat
    val mutPossibility = args(6).toFloat
    val r = args(7).toInt
    
    
    val arqv = new Table(path)
    val gen = new GeneticAlg(arqv, pop, mutPossibility)
    
    var bestOld: Float = 0
    var bestNew: Float = 0
    gen.genPopulation
//    gen.printPopulation
    
    val cur: Long = System.currentTimeMillis
    while (conditionNoBetter(cur, args(4).toInt, bestOld, bestNew) || conditionArg(args, gen, cur)) {
      gen.getData
//      gen.getBest
//      gen.getWorst
//      gen.getAverage
//      println("----------------------------------")
      bestOld = bestNew
      bestNew = gen.best.weight
      
      gen.doCrossover
      gen.doMutation(rate, gen.child)
      gen.mixPopulation(r)
    }
    
    gen.getData
//    println
//    println
//    println
//    println("----------------------------------")
    gen.getBest
//    gen.getWorst
//    gen.getAverage
//    println("----------------------------------")
//    arqv.printAll
    print("Linhas: ")
    gen.best.printPerson
    arqv.closeFile
    
    println("TEMPO: " + countTime(cur) + "s")
  }
  
  def conditionArg(args: Array[String], gen: GeneticAlg, cur: Long): Boolean = {
    args(2) match {
      case "-t" => return countTime(cur) <= args(3).toInt
      case "-p" => return gen.getCurPop <= args(3).toInt
      case _ => return false
    }
  }
  
  def conditionNoBetter(cur: Long, time: Int, old: Float, novo: Float): Boolean = {
    var tmp = cur
    if (old != novo) tmp = getTime
    return countTime(tmp) <= time
  }
  
  def getTime(): Long = {
    return System.currentTimeMillis
  }
  
  def countTime(first: Long): Double = {
    var tempo: Double = (getTime - first).toDouble
    return tempo/1000
  }
}
