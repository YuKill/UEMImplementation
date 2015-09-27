package scG

import scala.collection.mutable.HashMap

class Person {
  var weight: Float = 0
  var hashGen: HashMap[Int, Int] = _
  var hashLin: HashMap[Int, Array[Int]] = _
  
  def addGen(collection: Array[Array[Float]], col: Int): Unit = {
    (2 until collection(col).size) foreach { x => addLin(collection(col)(x).toInt, col) }
    hashGen.put(col, 1)
    weight += collection(col)(1)
  }
  
  def remGen(collection: Array[Array[Float]], gen: Int) {
    hashGen.remove(gen)
    (2 until collection(gen).size) foreach { i => remLin(collection(gen)(i).toInt, gen) }
    weight -= collection(gen)(1)
  }
  
  def addLin(key: Int, v: Int): Unit = {
    if (hashLin.get(key) == None) {
      hashLin.put(key, new Array[Int](1))
      hashLin.get(key).get(0) = v
    } else {
      var aux = new Array[Int](hashLin.get(key).get.size+1)
      hashLin.get(key).get.copyToArray(aux, 0)
      aux(hashLin.get(key).get.size) = v
      hashLin.update(key, aux)
    }
  }
  
  def remLin(key: Int, v: Int): Unit = {
    if (hashLin.get(key).get.size == 1) {
      hashLin.remove(key)
    } else {
      var aux = new Array[Int](hashLin.get(key).get.size-1)
      (0 to aux.size) foreach { x =>
        if (hashLin.get(key).get(x) == v)
          hashLin.get(key).get(x) = hashLin.get(key).get(aux.size)
      }
      hashLin.get(key).get.copyToArray(aux)
      hashLin.put(key, aux)
    }
  }
  
  def printPerson(): Unit = {
    hashGen.foreach { case (key, v) =>
        print(key + " - ")
    }
    println
  }
  
  def printLin(): Unit = {
    hashLin.foreach { case (key, v) => 
        print("Key " + key + ":")
        (0 until v.size) foreach { i =>
          print(v(i) + " - ")
        }
    }
    println
  }
  
  def cloneMeth(): Person = {
    var newP = new Person
    newP.hashGen = hashGen.clone
    newP.hashLin = hashLin.clone
    newP.weight = weight
    return newP
  }
  
  def inicializar(): Unit = {
    hashGen = new HashMap
    hashLin = new HashMap
  }
  
  inicializar()
}
