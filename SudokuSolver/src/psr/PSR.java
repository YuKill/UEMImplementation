/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package psr;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.PriorityQueue;
import java.util.Random;

/**
 *
 * @author yukiosir
 */
public class PSR {

    public static final boolean DEBUG = false;

    private final PSRType type;
    private final Sudoku sudoku;

    public long Cycles = 0;
    public long MAX_CYCLES = 50000000;
    public boolean solved = false;

    private final ArrayList<PSRVar> var = new ArrayList<>();
    private PSRState curState;

    public PSR(Sudoku sudoku) {
        this.type = PSRType.BACKTRACKING;
        this.sudoku = sudoku;
    }

    public void run() {
        PSRState initState = init();
        switch (type) {
            case BACKTRACKING:
                runBacktrack(initState);
                break;
        }
    }

    private PSRState init() {
        HashMap<String, ArrayList<Integer>> domain = new HashMap<>();
        sudoku.byBlock(new Sudoku.DoEveryElementInterface() {

            @Override
            public void execute(int line, int col, int block,
                    int nInBlock, int n) {
                PSRVar v = new PSRVar(line, col, block, nInBlock);
                ArrayList<Integer> newDomain = new ArrayList<>();
                if (n != 0) {
                    newDomain.add(n);
                } else {
                    for (int i = 1; i < sudoku.getOrderSq() + 1; i++) {
                        newDomain.add(i);
                    }
                }
                var.add(v);
                domain.put(v.getHashString(), newDomain);
            }

        });
        return new PSRState(domain);
    }

    public void showSolution() {
        if (type == PSRType.BACKTRACKING) {
            for (int i = 0; i < sudoku.getOrderSq()*sudoku.getOrderSq(); i++) {
                if (i != 0 && i % sudoku.getOrderSq() == 0)
                    System.out.println();
                System.out.print(var.get(i).n +" ");
            }
        }
        System.out.println();
    }

    private void runBacktrack(PSRState initState) {
        curState = initState;
        ArrayList<PSRVar> solution = new ArrayList<>();
        PriorityQueue<PSRVar> variables = new PriorityQueue<>(new Comparator<PSRVar>() {

            @Override
            public int compare(PSRVar t, PSRVar t1) {
                int tSize = curState.getDomain(t.getHashString()).size();
                int t1Size = curState.getDomain(t1.getHashString()).size();
                return tSize - t1Size;
            }

        });

        variables.addAll(var);
        if (AC3(var) && PSRBacktrack(solution, variables, initState, null)) {
            logDebug("Solved");
        } else {
            logDebug("No solution found");
        }
        logDebug("Cycles: " + Cycles);
    }

    private boolean PSRBacktrack(ArrayList<PSRVar> solution,
            PriorityQueue<PSRVar> variables, PSRState state, PSRVar lastVar) {
        logDebug("Call PSRBacktrack()");
        Cycles++;
        if (variables.isEmpty()) {
            solved = true;
            return true;
        } else if (Cycles > MAX_CYCLES) {
            solved = false;
            return true;
        }
        PSRVar var = chooseOneFrom(variables);
        solution.add(var);

        ArrayList<Integer> thisDomain = state.getDomain(var.getHashString()),
                copyDomain = new ArrayList<>(thisDomain);
        for (int i : copyDomain) {
            var.setValue(i);
            thisDomain.clear();
            thisDomain.add(i);

            curState = new PSRState(state);
            ArrayList<PSRVar> list = new ArrayList<>();
            insertAllRelated(list, var);
            if (!AC3(list)) {
                continue;
            }

            if (PSRBacktrack(solution, variables, curState, lastVar)) {
                return true;
            }
            curState = state;

            if (lastVar != null && !var.isRelatedTo(lastVar)) {
                thisDomain.clear();
                thisDomain.addAll(copyDomain);
                variables.add(var);
                solution.remove(var);
                return false;
            }
        }
        thisDomain.clear();
        thisDomain.addAll(copyDomain);

        variables.add(var);
        solution.remove(var);
        lastVar = var;
        return false;
    }

    private PSRVar chooseOneFrom(PriorityQueue<PSRVar> variables) {
        return variables.poll();
    }

    private boolean AC3(ArrayList<PSRVar> variables) {
        logDebug("Call AC3");
        for (PSRVar localVar : variables) {
            ArrayList<PSRVar> tda = new ArrayList<>();
            tda.add(localVar);
            while (!tda.isEmpty()) {
                PSRVar tdaCandidate = tda.get(0);
                tda.remove(tdaCandidate);

                boolean addRelated = makeConsistent(tdaCandidate);
                if (curState.getDomain((tdaCandidate.getHashString())).isEmpty()) {
                    return false;
                }
                if (addRelated) {
                    insertAllRelated(tda, localVar);
                }

            }
        }
        return true;
    }

    private boolean makeConsistent(PSRVar var) {
        logDebug("Call makeConsistent()");
        boolean lineRest = makeStringListConsistent(var, sudoku.getLineString(var)),
                colRest = makeStringListConsistent(var, sudoku.getColumnString(var)),
                blkRest = makeStringListConsistent(var, sudoku.getBlockString(var));
        return lineRest && colRest && blkRest;
    }

    private boolean makeStringListConsistent(PSRVar var, ArrayList<String> list) {
        ArrayList<Integer> newDomain = new ArrayList<>();
        ArrayList<Integer> varDomain = curState.getDomain(var.getHashString());
        for (int x : varDomain) {
            boolean xIsValid = true;
            for (String s : list) {
                boolean xIsValidInS = false;
                for (int y : curState.getDomain(s)) {
                    if (x != y) {
                        xIsValidInS = true;
                        break;
                    }
                }
                xIsValid = xIsValidInS && xIsValid;
                if (!xIsValid) {
                    break;
                }
            }
            if (xIsValid) {
                newDomain.add(x);
            }
        }

        boolean isEqual = (varDomain.size() == newDomain.size());
        if (!isEqual) {
            varDomain.clear();
            varDomain.addAll(newDomain);
        }
        return !isEqual;
    }

    private void insertAllRelated(ArrayList<PSRVar> varList, PSRVar var) {
        logDebug("Call insertAllRelated()");
        int line = (var.line - 1) * sudoku.getOrderSq(), col = var.column - 1;
        int order = (int) Math.sqrt(sudoku.getOrderSq());
        int lblk = ((var.block / order) * order) * sudoku.getOrderSq(),
                cblk = (var.block % order) * order;

        for (int i = 0; i < sudoku.getOrderSq(); i++) {
            if (i != var.column - 1) {
                if (varList.indexOf(this.var.get(line + i)) == -1) {
                    varList.add(this.var.get(line + i));
                }
            }
            if (i != var.line - 1) {
                if (varList.indexOf(this.var.get((i * sudoku.getOrderSq()) + col)) == -1) {
                    varList.add(this.var.get((i * sudoku.getOrderSq()) + col));
                }
            }
            if (i != var.getNinBlock()) {
                if (varList.indexOf(this.var.get(lblk + cblk + (i % order) + ((i / order) * order))) == -1) {
                    varList.add(this.var.get(lblk + cblk + (i % order) + ((i / order) * sudoku.getOrderSq())));
                }
            }
        }
    }

    private String genDomainString(ArrayList<Integer> dom) {
        String s = "";

        for (int d : dom) {
            s += d + " ";
        }
        return s;
    }

    private String genVarString(ArrayList<PSRVar> var) {
        String s = "";

        for (PSRVar v : var) {
            s += v.getHashString() + " ";
        }
        return s;
    }

    public static void logDebug(Object o) {
        if (DEBUG) {
            String message;
            if (!(o instanceof String)) {
                message = String.valueOf(o);
            } else {
                message = (String) o;
            }
            System.out.println("log: " + message);
        }
    }
}
