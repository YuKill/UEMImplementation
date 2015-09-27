/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package psr;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author yukiosir
 */
public class Sudoku {

    private int m[][];
    private int order;

    public Sudoku(String file) {
        try {

            BufferedReader br = new BufferedReader(new FileReader(file));
            String line = br.readLine();
            int i = 1;

            order = Integer.parseInt(line);
            m = new int[getOrderSq() + 1][getOrderSq() + 1];
            while ((line = br.readLine()) != null) {
                String numbers[] = line.split(" ");
                for (int j = 0; j < numbers.length; j++) {
                    m[i][j + 1] = Integer.parseInt(numbers[j]);
                }
                i++;
            }

        } catch (FileNotFoundException ex) {
            ex.printStackTrace();
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    public int getOrderSq() {
        return order * order;
    }

    public void printSudoku() {
        for (int line[] : m) {
            for (int n : line) {
                System.out.print(n + " ");
            }
            System.out.println();
        }
    }

    public ArrayList<String> getLineString(int i) {
        ArrayList<String> lineString = new ArrayList<>();
        for (int j = 1; j < getOrderSq() + 1; j++) {
            lineString.add("line:" + i + "col:" + j);
        }
        return lineString;
    }

    public ArrayList<String> getLineString(PSRVar var) {
        ArrayList<String> lineString = new ArrayList<>();
        for (int j = 1; j < getOrderSq() + 1; j++) {
            if (var.column != j) {
                lineString.add("line:" + var.line + "col:" + j);
            }
        }
        return lineString;
    }

    public ArrayList<String> getColumnString(int i) {
        ArrayList<String> colString = new ArrayList<>();
        for (int j = 1; j < getOrderSq() + 1; j++) {
            colString.add("line:" + j + "col:" + i);
        }
        return colString;
    }

    public ArrayList<String> getColumnString(PSRVar var) {
        ArrayList<String> colString = new ArrayList<>();
        for (int j = 1; j < getOrderSq() + 1; j++) {
            if (var.line != j) {
                colString.add("line:" + j + "col:" + var.column);
            }
        }
        return colString;
    }

    public ArrayList<String> getBlockString(int i) {
        ArrayList<String> blkString = new ArrayList<>();
        int line = ((i / order) * order) + 1,
                col = ((i % order) * order) + 1;
        for (int j = 0; j < getOrderSq(); j++) {
            int l = line + (j / order),
                    c = col + (j % order);
            blkString.add("line:" + l + "col:" + c);
        }
        return blkString;
    }

    public ArrayList<String> getBlockString(PSRVar var) {
        ArrayList<String> blkString = new ArrayList<>();
        int line = getLineFromBlock(var.block),
                col = getColFromBlock(var.block);
        for (int j = 0; j < getOrderSq(); j++) {
            int l = line + (j / order),
                    c = col + (j % order);
            if (var.line != l || var.column != c) {
                blkString.add("line:" + l + "col:" + c);
            }
        }
        return blkString;
    }
    
    public int getLineFromBlock(int b) {
        return ((b / order) * order) + 1;
    }
    
    public int getColFromBlock(int b) {
        return ((b % order) * order) + 1;
    }
    
    public int getBlock(int line, int col) {
        int l = line-1, c = col-1;
        return (l/order)*order + (c/order);
    }
    
    public int getNInBlock(int line, int col) {
        int b = getBlock(line, col);
        int baseLine = getLineFromBlock(b),
                baseCol = getColFromBlock(b);
        return (line-baseLine)*order + (col-baseCol);
    }
    
    public void byBlock(DoEveryElementInterface dEE) {
        for (int i = 1; i < getOrderSq()+1; i++) {
            for (int j = 1; j < getOrderSq()+1; j++) {
                dEE.execute(i, j, getBlock(i, j), getNInBlock(i, j), m[i][j]);
            }
        }
    }

    public interface DoEveryElementInterface {
        public void execute(int line, int col, int block, int nInBlock, int n);
    }
}
