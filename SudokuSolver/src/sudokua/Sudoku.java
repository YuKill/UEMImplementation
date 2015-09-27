/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sudokua;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.PriorityQueue;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author shaolin
 */
public class Sudoku {

    private int[][] tabuleiro;
    private boolean[][] fixos;
    private int[][] custos;
    private int ordem;
    private int custoTotal;

    public Sudoku best;

    public Sudoku(int ord) {
        int max = ord * ord;
        this.custos = new int[max][max];
        this.fixos = new boolean[max][max];
        this.tabuleiro = new int[max][max];
        this.ordem = ord;
        this.custoTotal = 0;
    }

    public Sudoku() {
    }

    public int[][] getTabuleiro() {
        return tabuleiro;
    }

    public void setTabuleiro(int[][] tabuleiro) {
        this.tabuleiro = tabuleiro;
    }

    public boolean[][] getFixos() {
        return fixos;
    }

    public void setFixos(boolean[][] fixos) {
        this.fixos = fixos;
    }

    public int[][] getCustos() {
        return custos;
    }

    public void setCustos(int[][] custos) {
        this.custos = custos;
    }

    public int getOrdem() {
        return ordem;
    }

    public void setOrdem(int ordem) {
        this.ordem = ordem;
    }

    public int getCustoTotal() {
        return custoTotal;
    }

    public void setCustoTotal(int custoTotal) {
        this.custoTotal = custoTotal;
    }

    public String tabToString(int[][] tab, int max) {
        String palavra = null;
        for (int i = 0; i < max; i++) {
            for (int j = 0; j < max; j++) {
                palavra = palavra + Integer.toString(tab[i][j]);
            }
        }
        return palavra;
    }

    public void printSudoku(Sudoku sk, int max) {
        for (int i = 0; i < max; i++) {
            for (int j = 0; j < max; j++) {
                System.out.print(sk.getTabuleiro()[i][j] + " ");
            }
            System.out.println("");
        }
//        System.out.println("");
//        for (int i=0; i<max; i++){
//            for (int j=0; j<max; j++){
//                System.out.print(sk.getFixos()[i][j]+" ");
//            }
//            System.out.println("");
//        }
//        System.out.println("Tabela de custos");
//        for (int i=0; i<max; i++){
//            for (int j=0; j<max; j++){
//                System.out.print(sk.getCustos()[i][j]+" ");
//            }
//            System.out.println("");
//        }
    }

    public Sudoku makeTabuleiro(String path) {
        FileReader fr;
        BufferedReader br;
        String linha;
        int i, j, zeros;
        int ord;
        Sudoku sk = null;

        try {
            fr = new FileReader(path);
            br = new BufferedReader(fr);
            i = 0;
            zeros = 0;
            linha = br.readLine();
            ord = Integer.parseInt(linha);
            sk = new Sudoku(ord);
            int max = ord * ord;
            int[][] valores = new int[max][max];
            boolean[][] fixos = new boolean[max][max];
            int[][] custos = new int[max][max];
            while (br.ready()) {
                linha = br.readLine();
                String[] vet = linha.split(" ");
                for (j = 0; j < vet.length; j++) {
                    if (vet[j].equals("0")) {
                        valores[i][j] = 0;
                        fixos[i][j] = false;
                    } else {
                        valores[i][j] = Integer.parseInt(vet[j]);
                        fixos[i][j] = true;
                    }
                    custos[i][j] = 0;
                }
                i++;
            }
            sk.setCustos(custos);
            sk.setFixos(fixos);
            sk.setTabuleiro(valores);
            return sk;

        } catch (FileNotFoundException ex) {
            Logger.getLogger(Sudoku.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(Sudoku.class.getName()).log(Level.SEVERE, null, ex);
        }
        return sk;
    }

    public ArrayList<Integer> todosPossiveis(int max) {
        ArrayList<Integer> lista = new ArrayList<>();
        for (int i = 1; i <= max; i++) {
            lista.add(i);
        }
        return lista;
    }

    public void randTab(Sudoku sk, int max) {
        for (int i = 0; i < max; i++) {
            ArrayList<Integer> todasPossi = todosPossiveis(max);
            ArrayList<Integer> contem = new ArrayList<>();
            for (int j = 0; j < max; j++) {
                int valor = sk.getTabuleiro()[i][j];
                if (valor != 0) {
                    contem.add(valor);
                }
            }
            ArrayList<Integer> falta = new ArrayList<>();
            for (Integer k : todasPossi) {
                if (!contem.contains(k)) {
                    falta.add(k);
                }
            }
            int it = 0;
            for (int j = 0; j < max; j++) {
                if (!sk.getFixos()[i][j]) {
                    sk.getTabuleiro()[i][j] = falta.get(it);
                    it++;
                }
            }
        }
    }

    public void calcCUsto(Sudoku sk, int max) {
        int valor;
        for (int lin = 0; lin < max; lin++) {
            for (int col = 0; col < max; col++) {
                int custo = 0;
//                if (!tab.getTabuleiro()[lin][col].isFixo()){
                valor = sk.getTabuleiro()[lin][col];
                for (int i = 0; i < max; i++) {
                    if (lin != i) {    //verifica coluna
                        if (valor == sk.getTabuleiro()[i][col]) {
                            custo++;
                        }
                    }
//                        if (col!=i){    //verifica coluna
//                            if (valor == sk.getTabuleiro()[lin][col]){
//                                custo++;
//                            }
//                        }                       
                }
//                }
                sk.getCustos()[lin][col] = custo;
//                System.out.println("["+lin+","+col+"]-> "+custo);
            }
        }

        //elimina possiveis repetidos quadrante
        int iniL, fimL, iniC, fimC, ord, lin, col;
        iniL = 0;
        iniC = 0;
        ord = sk.getOrdem();
        int custoTotal = 0;
        for (int quad = 0; quad < max; quad++) {     //quadrante
            fimL = iniL + ord;
//            System.out.println("quad> "+quad);
            for (lin = iniL; lin < fimL; lin++) {    //linha elemento
                fimC = iniC + ord;
                for (col = iniC; col < fimC; col++) {    //coluna elemento
//                    System.out.println("    lin> "+lin+" col> "+col);
                    int custo = sk.getCustos()[lin][col];
//                    if (tab.getTabuleiro()[lin][col].isFixo()){
                    valor = sk.getTabuleiro()[lin][col];
                    for (int i = iniL; i < fimL; i++) {  //percorre quadrante
                        for (int j = iniC; j < fimC; j++) {
//                                System.out.println("        ["+i+","+j+"]");
                            if ((lin != i) || (col != j)) {
                                if (valor == sk.getTabuleiro()[i][j]) {
                                    custo++;
                                }
                            }
                        }
                    }
//                    }
                    sk.getCustos()[lin][col] = custo;
//                    System.out.println("["+lin+","+col+"] custo: "+custo);
                    custoTotal = custoTotal + custo;
                }
            }
            if (((quad + 1) % ord) == 0) {
                iniL = iniL + ord;
                iniC = 0;
            } else {
                iniC = iniC + ord;
            }
        }
        sk.setCustoTotal(custoTotal);
    }

    public boolean ehSolucao(Sudoku sk, int max) {
        return (verificaLinhaColuna(sk, max) && verificaQuadrante(sk, max));
    }

    public boolean verificaLinhaColuna(Sudoku sk, int max) {
        boolean resp;
        int lin, col, aux;

        resp = true;
        lin = 0;
        col = 0;
        while (resp && lin < max) {
            col = 0;
            while (resp && col < max) {
                if (sk.getTabuleiro()[lin][col] != 0) {
                    aux = 0;
                    while (resp && aux < max) {
                        if (aux != col) {
                            if (sk.getTabuleiro()[lin][col] == sk.getTabuleiro()[lin][aux]) {
                                resp = false;
                            }
                        }
                        if (aux != lin) {
                            if (sk.getTabuleiro()[lin][col] == sk.getTabuleiro()[aux][col]) {
                                resp = false;
                            }
                        }
                        aux++;
                    }
                } else {
                    resp = false;
                }
                col++;
            }
            lin++;
        }
//        System.out.println("erro> lin"+lin+" "+" col"+col);
        return resp;
    }

    public boolean verificaQuadrante(Sudoku sk, int max) {
        int iniL, iniC, fimL, fimC, quad, q, lin, col;
        int ord = sk.getOrdem();
        boolean resp = true;
        iniL = 0;
        fimL = iniL + ord;
        q = 0;
        col = 0;
        iniC = 0;
        lin = iniL;
        while ((resp) && (q < max)) {
            lin = iniL;
            fimL = iniL + ord;
            ArrayList<Integer> lista = new ArrayList<>();
            while ((resp) && (lin < fimL)) {
                col = iniC;
                fimC = iniC + ord;
                while ((resp) && (col < fimC)) {
                    //System.out.println("["+lin+","+col+"]");
                    int valor = sk.getTabuleiro()[lin][col];
                    if ((valor == 0) || (lista.contains(valor))) {
                        resp = false;
                    } else {
                        lista.add(valor);
                    }
                    col++;
                }
                lin++;
            }
            q++;
            if ((q % ord) == 0) {
                iniL = iniL + ord;
                iniC = 0;
            } else {
                iniC = iniC + ord;
            }
        }
//        System.out.println("erro> lin"+lin+" "+" col"+col+" quad"+q);
        return resp;
    }

    public void troca(Sudoku sk, HashMap<String, Sudoku> filhos, int max) {
        for (int lin = 0; lin < max; lin++) {
            for (int col = 0; col < max; col++) {
                if (!sk.getFixos()[lin][col]) {
                    for (int i = col + 1; i < max; i++) {
                        if (!sk.getFixos()[lin][i]) {
                            Sudoku novo = clone(sk, max);
                            boolean auxFixos = sk.getFixos()[lin][col];
                            int auxCustos = sk.getCustos()[lin][col];
                            int auxValor = sk.getTabuleiro()[lin][col];

                            sk.getFixos()[lin][col] = novo.getFixos()[lin][i];
                            sk.getCustos()[lin][col] = novo.getCustos()[lin][i];
                            sk.getTabuleiro()[lin][col] = novo.getTabuleiro()[lin][i];

                            sk.getFixos()[lin][i] = auxFixos;
                            sk.getCustos()[lin][i] = auxCustos;
                            sk.getTabuleiro()[lin][i] = auxValor;

                            calcCUsto(novo, max);

                            if (!filhos.containsKey(novo.getTabuleiro().toString())) {
                                filhos.put(novo.getTabuleiro().toString(), novo);
                            }

                        }
                    }
                }
            }
        }
    }

    public void fazFilhos(Sudoku sk, HashMap<String, Sudoku> filhos, HashMap<String, Sudoku> finalizados, PriorityQueue<Sudoku> priori, int max) {
        for (int i = 0; i < 81; i++) {
            for (int j = i + 1; j < 81; j++) {
                int l1 = i / 9;
                int c1 = i % 9;
                int l2 = j / 9;
                int c2 = j % 9;
                if (l1 == l2) {
                    if (!sk.getFixos()[l1][c1] && !sk.getFixos()[l2][c2]) {
                        Sudoku novo = clone(sk, max);
                        //                    System.out.println("novooo");
                        //                    printSudoku(sk, max);
                        boolean auxFixos = novo.getFixos()[l1][c1];
                        int auxCustos = novo.getCustos()[l1][c1];
                        int auxValor = novo.getTabuleiro()[l1][c1];

                        novo.getFixos()[l1][c1] = novo.getFixos()[l2][c2];
                        novo.getCustos()[l1][c1] = novo.getCustos()[l2][c2];
                        novo.getTabuleiro()[l1][c1] = novo.getTabuleiro()[l2][c2];

                        novo.getFixos()[l2][c2] = auxFixos;
                        novo.getCustos()[l2][c2] = auxCustos;
                        novo.getTabuleiro()[l2][c2] = auxValor;

                        calcCUsto(novo, max);

                        String chave = tabToString(novo.getTabuleiro(), max);
                        if (!filhos.containsKey(chave) && !finalizados.containsKey(chave)) {
                            filhos.put(chave, novo);
                            priori.add(novo);
                        } else {
//                            System.out.println("repetiu");
                        }
                    }
                }
            }
        }
    }

    public Sudoku clone(Sudoku sk, int max) {
        Sudoku novo = new Sudoku(sk.getOrdem());
        for (int lin = 0; lin < max; lin++) {
            for (int col = 0; col < max; col++) {
                novo.getCustos()[lin][col] = sk.getCustos()[lin][col];
                novo.getFixos()[lin][col] = sk.getFixos()[lin][col];
                novo.getTabuleiro()[lin][col] = sk.getTabuleiro()[lin][col];
            }
        }
        novo.setCustoTotal(sk.getCustoTotal());
//        for (int lin=0; lin<max; lin++){
//            for (int col=0; col<max; col++){
//                c[lin][col] = sk.getCustos()[lin][col];
//                f[lin][col] = sk.getFixos()[lin][col];
//                v[lin][col] = sk.getTabuleiro()[lin][col];
//            }
//        }
//        novo.setCustos(c);
//        novo.setFixos(f);
//        novo.setTabuleiro(v);
//        novo.setCustoTotal(sk.getCustoTotal());
        return novo;
    }

    public Sudoku getMelhor(HashMap<String, Sudoku> filhos, HashMap<String, Sudoku> finalizados, PriorityQueue<Sudoku> priori, int max) {
//        int melhorCusto = (int) Float.POSITIVE_INFINITY;
//        String sk=null;
//        for (String k: filhos.keySet()){
//            if (filhos.get(k).getCustoTotal() < melhorCusto){
//                if (!finalizados.containsKey(k)){
//                    melhorCusto = filhos.get(k).getCustoTotal();
//                    sk = k;                   
//                }
//            }
//        }
//        
//        Sudoku melhor = clone(filhos.get(sk), max);
////        System.out.println("melhor   qqq");
////        printSudoku(melhor, max);
////        System.out.println("melhor  custo "+melhor.getCustoTotal());
//        filhos.remove(sk);

        Sudoku sk = priori.poll();
        String chave = tabToString(sk.getTabuleiro(), max);

        return sk;
    }

    public void clearPQ(PriorityQueue<Sudoku> priori, int maxPQ, int nSave) {
        if (priori.size() > maxPQ) {
            ArrayList<Sudoku> aux = new ArrayList<>();
            for (int i = 0; i < nSave; i++) {
                aux.add(priori.poll());
            }
            priori.clear();
            priori.addAll(aux);
        }
    }

    public int resolve(Sudoku sk, int max, int maxCiclos, int maxPQ, int nSave) {
        HashMap<String, Sudoku> filhos = new HashMap<>();
        HashMap<String, Sudoku> finalizados = new HashMap<>();
        PriorityQueue<Sudoku> pqFilhos = new PriorityQueue<>(new Comparator<Sudoku>() {

            @Override
            public int compare(Sudoku o1, Sudoku o2) {

                if (o1.getCustoTotal() < o2.getCustoTotal()) {
                    return -1;
                }
                if (o1.getCustoTotal() > o2.getCustoTotal()) {
                    return 1;
                }
                return 0;
            }
        });

        int ciclos = 0;
//        System.out.println("Tabuleiro inicial:");
//        printSudoku(sk, max);
        if (!ehSolucao(sk, max)) {
            randTab(sk, max);
//            System.out.println("Tabuleiro randomizado");
//            Sudoku.printSudoku(sk, max);
            calcCUsto(sk, max);
            filhos.put(sk.getTabuleiro().toString(), sk);
            pqFilhos.add(sk);
            while (sk.getCustoTotal() != 0 && ciclos < maxCiclos) {
//                System.out.println("inicio fazendo filhos");
                fazFilhos(sk, filhos, finalizados, pqFilhos, max);
//                System.out.println("filhos feitos");
//                System.out.println("inicio escolha");
                sk = getMelhor(filhos, finalizados, pqFilhos, max);
                clearPQ(pqFilhos, maxPQ, nSave);
//                System.out.println("            "+sk.getCustoTotal());
//                System.out.println("fim escolha");
                String chave = tabToString(sk.getTabuleiro(), max);
                finalizados.put(chave, sk);
                filhos.remove(chave);
                ciclos++;
//                System.out.println("                tamanho"+pqFilhos.size());
            }
//            System.out.println("SOLUCAÇÂO>" + ehSolucao(sk, max));
//            printSudoku(sk, max);
        }
        best = sk;
        return ciclos;
    }
}
