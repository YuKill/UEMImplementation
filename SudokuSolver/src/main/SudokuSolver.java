/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package main;

import java.util.Arrays;
import psr.PSR;
import psr.Sudoku;

/**
 *
 * @author yukiosir
 */
public class SudokuSolver {

    /**
     * THE MAIN PROGRAM
     */
    public static String path = "";
    public static String algorithm = "";
    public static boolean solved = true;
    public static boolean showSolution = false;
    public static long Cycles;
    public static long MaxCycles = 10000;

    public static void main(String[] args) {
        if (!processArgs(args)) {
            return;
        }

        long start = System.currentTimeMillis();
        switch (algorithm.toLowerCase()) {
            case "backtrack":
                runBacktrack();
                break;
            case "a-star":
                runAStar();
                break;
        }
        long end = System.currentTimeMillis();
        double time = ((double) end - start) / 1000.0;

        String[] pathSplit = path.split("/");
        String filename = pathSplit[pathSplit.length - 1];
        String result = "solved";
        if (!solved) {
            result = "unsolved";
        }

        System.out.println(filename);
        System.out.println(result);
        System.out.println(time);
        System.out.println(Cycles);
    }

// ----------------------------------------- Run Algorithms
    public static void runBacktrack() {
        Sudoku sudoku = new Sudoku(path);
        PSR psr = new PSR(sudoku);
        psr.MAX_CYCLES = MaxCycles;
        
        psr.run();
        if (showSolution) {
            psr.showSolution();
        }

        solved = psr.solved;
        Cycles = psr.Cycles;
    }
    
    public static void runAStar() {
        
        sudokua.Sudoku sk = new sudokua.Sudoku();
        sk = sk.makeTabuleiro(path);
        int max = sk.getOrdem()*sk.getOrdem();
        int maxCiclos = (int) MaxCycles;
        int maxPQ = 300000;
        int nSave = 1000;
        
        Cycles = sk.resolve(sk, max, maxCiclos, maxPQ, nSave);
        if (showSolution) {
            sk.printSudoku(sk.best, max);
        }
        
        solved = sk.ehSolucao(sk.best, max);
    }

// ----------------------------------------- Processing Arguments
    public static boolean processArgs(String[] args) {
        if (args.length < 2) {
            showHelp();
            return false;
        }

        int pathPos = Arrays.asList(args).indexOf("-i");
        int algorithmPos = Arrays.asList(args).indexOf("-a");
        int showSolutionPos = Arrays.asList(args).indexOf("-v");
        int maxCyclesPos = Arrays.asList(args).indexOf("-c");

        if (pathPos == -1 || algorithmPos == -1) {
            showHelp();
            return false;
        }

        try {
            path = args[pathPos + 1];
            algorithm = args[algorithmPos + 1];            
        } catch (IndexOutOfBoundsException ex) {
            showHelp();
            return false;
        }
        
        try {
            if (maxCyclesPos != -1)
                MaxCycles = Long.parseLong(args[maxCyclesPos + 1]);
        } catch (IndexOutOfBoundsException ex) {
        }

        if (!algorithm.equals("backtrack") && !algorithm.equals("genetic")
                && !algorithm.equals("a-star")) {
            showHelp();
            return false;
        }

        if (showSolutionPos != -1) {
            showSolution = true;
        }

        return true;
    }

    public static void showHelp() {
        System.out.println("Usage: ./solve.sh [arguments] [options]");
        System.out.println("Arguments:");
        System.out.println(" -i : specify the input file");
        System.out.println(" -a : specify the algorithm to be used");
        System.out.println("Algorithms:");
        System.out.println(" a-star    : use a-star search algorithm to solve");
        System.out.println(" genetic   : use genetic algorithm to solve");
        System.out.println(" backtrack : use backtrack to solve");
        System.out.println("Options:");
        System.out.println(" -v : shows the solution found(if found) or its closest answer");
        System.out.println(" -c : specify the maximum cycles");
        System.out.println();
        System.out.println("Examples:");
        System.out.println("./solve.sh -i sudokuFile -a genetic -v");
        System.out.println("\tUses the genetic algorithm to try to solve the file 'sudokuFile'"
                + "and at the end, it shows the answer it found");
    }

}
