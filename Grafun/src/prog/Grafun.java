/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package prog;

/**
 *
 * @author jao
 */
public class Grafun {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {/*
        System.out.print("parametros: ");
        for (String s : args) {
            System.out.print(s + " ");
        }
        System.out.println();
        System.out.println(System.getProperty("user.dir"));*/
        Grafo g = new Grafo("mst", "src/1.g");
        //g.imprimeGrafo();
        Algoritmo.executaAlgoritmo("mst", g);

    }
}
