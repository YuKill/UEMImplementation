/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package psr;

import java.util.ArrayList;

/**
 *
 * @author yukiosir
 */
public class PSRVar {    
    public int n;
    
    public int line;
    public int column;
    public int block;
    public int nInBlock;
    
    public PSRVar(int line, int column, int block, int nInBlock) {
        this.line = line;
        this.column = column;
        this.block = block;
        this.nInBlock = nInBlock;
    }
    
    public void setValue(int n) {
        this.n = n;
    }
    
    public String getHashString() {
        return "line:" + line + "col:" + column;
    }
    
    public int getNinBlock() {
        return nInBlock;
    }
    
    public boolean isRelatedTo(PSRVar var) {
        return (var.line == line || var.column == column ||
                var.block == block);
    }
}
