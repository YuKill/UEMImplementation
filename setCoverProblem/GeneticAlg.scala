package scG

import scala.collection.mutable.HashMap
import scala.util.Random

class GeneticAlg(t: Table, st_pop: Int, mut: Float) {
  private var startPop: Int = _
  private var curPop: Int = 0
  var best: Person = _
  private var worst: Person = _
  private var totalW: Float = _
  private var table: Table = _
  var child: Person = _
  private var rate: Float = _
  private var mutPossibility: Float = _
  private var parents: Array[Int] = new Array[Int](2)
  private var population: Array[Person] = _
  
  private def isNotComplete(p: Person): Boolean = {
    return (p.hashLin.size < table.getNLines)
  }
  
  def genPerson(collection: Array[Array[Float]], newP: Person): Unit = {
    var col = 0
    val rand = Random
    while (isNotComplete(newP)) {
      col = rand.nextInt(collection.size)
      col = collection(col)(0).toInt - 1
      if (newP.hashGen.get(col) == None ) {
        newP.addGen(table.getLines(), col)
      }
    }
  }
  
  def genPopulation(): Unit = {
    while (curPop < startPop) {
      var p = new Person
      genPerson(table.getLines, p)
      population(curPop) = p
      curPop += 1
    }
  }
  
  private def checkContains(winners: HashMap[Int, Int], p: Int): Boolean = {
    for ((k, v) <- winners) {
      if (v == p) return true;
    }
    return false;
  }
  
  private def selectWinners(winners: HashMap[Int, Int]): Unit = {
    var perc: Float = 0
    var rF: Float = 0
    var r: Int = 0
    
    while (winners.size < 4) {
      r = Random.nextInt(curPop)
      rF = Random.nextFloat
      perc = population(r).weight/totalW
      if (!checkContains(winners, r) && perc <= rF) // é um winner
        winners.put(winners.size, r)
    }
  }
  
  private def genParents(): Unit = {
    val winners: HashMap[Int, Int] = new HashMap
    var bestF: Float = 0
    var r: Int = 0
    (0 to 1) foreach { i =>
      selectWinners(winners)
      bestF = Float.MaxValue
      for ((k, v) <- winners) {
        if (population(v).weight < bestF) {
          if (i == 0 || parents(0) != v) { 
            bestF = population(v).weight
            parents(i) = v
          }
        }
      }
    }
  }
  
  private def mkListGen(list: Array[Array[Float]], p: Person, start: Int): Int = {
    var s = start
    for((x: Int, y: Int) <- p.hashGen) {
      list(s) = table.getColumn(x)
      s += 1
    }
    return s
  }
  
  def doCrossover(): Unit = {
    genParents()
    val size = population(parents(0)).hashGen.size + population(parents(1)).hashGen.size
    val list: Array[Array[Float]] = new Array[Array[Float]](size)
    
    var start = 0
    start = mkListGen(list, population(parents(0)), start)
    mkListGen(list, population(parents(1)), start)
    
    child = new Person
    genPerson(list, child)
  }
  
  def geraVizinhos(s0: Person): Array[Person] = {
    var array: Array[Person] = new Array[Person](10)
    (0 until 10) foreach { i =>
      var newSol = s0.cloneMeth
      doMutation(1, newSol)
      array(i) = newSol
    }
    return array
  }
  
  def doVND(s0: Person, r: Int): Person = {
    var p = s0
    var p2: Person = null
    var k = 0
    while (k < r) {
      p2 = findBest(geraVizinhos(p));
      if (p2.weight < p.weight) {
        p = p2
        k = 0
      } else {
        k += 1
      }
    }
    return p
  }
  
  def doMutation(rate: Float, child: Person): Unit = {
    var rand = Random.nextFloat
    var qtd = Random.nextInt(child.hashGen.size/2) + 1
    val mutated: HashMap[Int, Int] = new HashMap
    
    if (rand <= rate) {
      while (qtd > 0) {
        for ((k: Int, v: Int) <- child.hashGen) {
          if (qtd > 0 && mutated.get(k) == None) {
            var x = Random.nextFloat
            if (x <= mutPossibility) {
              mutated.put(k, v)
              qtd -= 1
            }
          }
        }
      }
      mutated.foreach { case (k, v) =>
          child.remGen(table.getLines, k)
      }
      genPerson(table.getLines, child)
    }
  }
  
  def mixPopulation(r: Int): Unit = {
    child = doVND(child, r)
    if (child.weight < worst.weight) {
      var newPop = new Array[Person](population.size+1)
      newPop(0) = child
      population.copyToArray(newPop, 1)
      population = newPop
      curPop += 1
    }
  }
  
  def getCurPop(): Int = {
    return curPop
  }
  
  def findBest(pop: Array[Person]): Person = {
    var w = Float.MaxValue
    var bestP: Person = null
    for (p <- pop) {
      if (w > p.weight) {
        w = p.weight
        bestP = p
      }
    }
    return bestP
  }
  
  def getData(): Unit = {
    var wBest = Float.MaxValue
    var wWorst: Float = 0
    totalW = 0
    population.foreach {
      p =>
      if (wBest > p.weight) {
        wBest = p.weight
        best = p
      }
      if (wWorst < p.weight) {
        wWorst = p.weight
        worst = p
      }
      totalW += p.weight
    }
  }
  
  def getAverage(): Unit = {
    print("Média: " + totalW/curPop)
    println
  }
  
  def getBest(): Unit = {
    print("BEST: " + best.weight)
    println
  }
  
  def getWorst(): Unit = {
    print("WORST: " + worst.weight)
    println
  }
  
  def printPopulation(): Unit = {
    var c = 1
    population.foreach { p => 
      println("PERSON " + c)
      c += 1
      p.printPerson
    }
  }
  
  private def inicializar(): Unit = {
    table = t
    startPop = st_pop
    population = new Array[Person](startPop)
    mutPossibility = mut
  }
  
  inicializar()
}
