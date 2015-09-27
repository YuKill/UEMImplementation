package prog;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

/**
 *
 * @author jao
 */
public class Grafo {

    private ArrayList<Vertice> vertices = new ArrayList();

    public ArrayList<Vertice> getVertices() {
        return vertices;
    }

    public void setVertices(ArrayList<Vertice> vertices) {
        this.vertices = vertices;
    }
    
    public Grafo(){
        vertices = new ArrayList<Vertice>();
    }
    
    public Grafo(String algoritmo, String nomeArq) {

        FileReader arquivo;

        try {

            arquivo = new FileReader(nomeArq);
            BufferedReader linha = new BufferedReader(arquivo);
            ArrayList<String> file = new ArrayList();

            while (linha.ready()) {

                file.add(linha.readLine());

            }

            arrayToGraph(file, algoritmo);

        } catch (IOException exc) {

            exc.printStackTrace();

        }

    }

    private void arrayToGraph(ArrayList<String> file, String algoritmo) {
        
        int i = 0, index = 0;
        String linha;
        String[] strings;
        Vertice vertice = null;

        while (i < file.size()) {

            linha = file.get(i);

            if (!(linha.isEmpty() || linha.equals("\n") || linha.equals("#"))) {

                strings = linha.split("\\s+");


                if (strings.length < 2) {

                    adicionaVertice(strings, index);
                    index++;

                } else {

                    adicionaAresta(strings, algoritmo);
                }

            }

            i++;
        }


    }

    private void adicionaVertice(String[] strings, int index) {

        Vertice vertice = new Vertice();
        vertice.setIndex(index);
        vertice.setNome(strings[0]);
        this.vertices.add(vertice);

    }

    private void adicionaAresta(String[] strings, String algoritmo) {

        Vertice verticeOrigem = null, verticeDestino = null;
        int peso = 0;

        for (Vertice v : this.vertices) {

            if (v.getNome().equals(strings[0])) {

                verticeOrigem = v;

            }

            if (v.getNome().equals(strings[1])) {

                verticeDestino = v;
            }

        }

        if (strings.length == 3) {

            peso = Integer.parseInt(strings[2]);

        }

        insereAresta(verticeOrigem, verticeDestino, peso);

        if (algoritmo.equals("mst") && !verticeDestino.equals(verticeOrigem)) {

            insereAresta(verticeDestino, verticeOrigem, peso);

        }

    }

    public void insereAresta(Vertice verticeOrigem, Vertice verticeDestino, int peso) {

        Aresta aresta = new Aresta();

        aresta.setVertice(verticeDestino);
        aresta.setPeso(peso);
        aresta.setProx(verticeOrigem.getVerticeAdj());
        
        verticeOrigem.setVerticeAdj(aresta);

    }

}
