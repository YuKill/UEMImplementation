package scG

import scala.io.BufferedSource
import scala.io.Source._

class Table(fp: String) {
  private var nLines: Int = _
  private var file: BufferedSource = _
  private var fLines: Array[Array[Float]] = _
  private val RemLin = "[a-zA-Z\\:]+[0-9\\.%\\s]*".r
  
  def getLines(): Array[Array[Float]] = {
    return this.fLines
  }
  
  def getColumn(col: Int): Array[Float] = {
    return this.fLines(col)
  }
  
  def getNLines(): Int = {
    return this.nLines
  }
  
  def printAll: Unit = {
    (0 until this.fLines.size) foreach { l =>
      (0 until this.fLines(l).length) foreach { c => 
        print(this.fLines(l)(c) + "\t")
      }
      println
    }
  }
  
  def closeFile: Unit = {
    file.close
  }
  
  def inicializar(): Unit = {
    file = fromFile(fp, "UTF-8")
    var sLines = file.mkString.split("\n")
    
    for (i <- 0 until sLines.size) {
      sLines(i) = sLines(i).replaceAll("[\\s]+$", "")
      sLines(i) = sLines(i).replaceAll("^[\\s]+", "")
    }
    this.nLines = sLines(0).split("[\\s]+")(1).toInt
    sLines = sLines.filter(value => !RemLin.pattern.matcher(value).matches)
    this.fLines = new Array[Array[Float]](sLines.size)
    for (i <- 0 until sLines.size) {
      var aux: Array[Float] = sLines(i).split("[\\s]+").map(value => value.toFloat)
      this.fLines(i) = aux
    }
  }
  
  inicializar()
}
