/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package prog;

import java.util.ArrayList;
import java.util.LinkedList;

/**
 *
 * @author yukill
 */
public class Algoritmo {

    public static void executaAlgoritmo(String algoritmo, Grafo grafo) {

        switch (algoritmo.charAt(0)) {

            case 'b':
                bfs(grafo);
                break;

            case 't':
                topologicalSort(grafo);
                break;

            case 's':
                scc(grafo);
                break;

            case 'm':
                mst(grafo);
                break;

        }

    }

    private static void bfs(Grafo grafo) {

        LinkedList<Vertice> queue = new LinkedList<Vertice>();
        Vertice u;
        Aresta v;

        inicializarBfs(grafo);
        

        queue.add(grafo.getVertices().get(0));
        while (!queue.isEmpty()) {

            u = queue.removeFirst();
            v = u.getVerticeAdj();

            while (v != null) {

                if (v.getVertice().getCor() == 0) {

                    v.getVertice().setCor(1);
                    v.getVertice().setDistancia(u.getDistancia() + 1);
                    v.getVertice().setPai(u);
                    queue.add(v.getVertice());

                }

                v = v.getProx();
            }

            u.setCor(2);
        }

        imprimeBfs(grafo);

    }

    private static void inicializarBfs(Grafo grafo) {

        for (Vertice v : grafo.getVertices()) {

            v.setCor(0);
            v.setDistancia(-1);
            v.setPai(null);

        }

        grafo.getVertices().get(0).setDistancia(0);
        grafo.getVertices().get(0).setCor(1);

    }

    private static void imprimeBfs(Grafo grafo) {

        for (int i = 0; i < grafo.getVertices().size(); i++) {

            if (grafo.getVertices().get(i).getDistancia() != -1) {

                System.out.println(grafo.getVertices().get(0).toString() + " " + grafo.getVertices().get(i).toString() + " " + grafo.getVertices().get(i).getDistancia());
            }
        }

    }

    private static void topologicalSort(Grafo grafo) {

        ArrayList<Vertice> ordenacao = new ArrayList();
        dfs(grafo, ordenacao, grafo.getVertices(), 0);
        
        int i = ordenacao.size() - 1;
        while (i >= 0) {

            System.out.println(ordenacao.get(i));
            i--;
        }
    }

    private static void dfs(Grafo grafo, ArrayList ord, ArrayList<Vertice> vertices, int mod) {

        Integer tempo = 0;
        for (Vertice v : grafo.getVertices()) {

            v.setCor(0);
            v.setPai(null);
        }

        for (Vertice v : vertices) {

            if (v.getCor() == 0) {

                dfsVisit(v, tempo, ord, mod);

                if (mod == 1) {

                    System.out.println();
                }
            }
        }
    }

    private static void dfsVisit(Vertice v, Integer tempo, ArrayList ord, int mod) {

        v.setCor(1);
        tempo += 1;
        v.setTempoIni(tempo);
        Aresta adj = v.getVerticeAdj();
        while (adj != null) {

            if (adj.getVertice().getCor() == 0) {

                adj.getVertice().setPai(v);
                dfsVisit(adj.getVertice(), tempo, ord, mod);
                if (mod == 1) {

                    System.out.print(" ");
                }
            }

            adj = adj.getProx();
        }

        v.setCor(2);

        if (mod == 1) {

            System.out.print(v.toString());
        }

        ord.add(v);
        tempo += 1;
        v.setTempoFin(tempo);
    }

    private static void scc(Grafo grafo) {

        ArrayList<Vertice> result = new ArrayList<Vertice>(), reverse = new ArrayList<Vertice>();
        Grafo gTrans;

        gTrans = createTransposta(grafo);

        dfs(grafo, result, grafo.getVertices(), 0);

        int i = result.size() - 1;
        while (i >= 0) {

            reverse.add(gTrans.getVertices().get(result.get(i).getIndex()));
            i--;
        }

        dfs(gTrans, result, reverse, 1);
    }

    private static Grafo createTransposta(Grafo grafo) {

        Vertice vert, destino;
        Grafo gTrans = new Grafo();

        for (Vertice v : grafo.getVertices()) {

            gTrans.getVertices().add(v.clone());

        }

        for (Vertice v : grafo.getVertices()) {
            Aresta a = new Aresta();

            a = v.getVerticeAdj();

            while (a != null) {

                destino = gTrans.getVertices().get(v.getIndex());
                vert = gTrans.getVertices().get(a.getVertice().getIndex());

                gTrans.insereAresta(vert, destino, a.getPeso());

                a = a.getProx();
            }
        }

        return gTrans;
    }

    private static void mst(Grafo grafo) {

        ArrayList<Vertice> heap = new ArrayList<Vertice>();
        Vertice[] Q = grafo.getVertices().toArray(new Vertice[grafo.getVertices().size()]);
        int size = Q.length;

        for (Vertice u : grafo.getVertices()) {

            u.setDistancia(null);
            u.setPai(null);
        }

        heap.add(grafo.getVertices().get(0));
        heap.get(0).setDistancia(0);
        Vertice u;
        Aresta a;

        while (size != 0) {

            u = extractMin(heap);

            if (Q[u.getIndex()] != null) {

                if (u.getPai() != null) {

                    System.out.println(u.getPai().getNome() + " " + u.getNome());
                }

                a = u.getVerticeAdj();
                Q[u.getIndex()] = null;
                size--;

                while (a != null) {

                    if ((Q[a.getVertice().getIndex()] != null) && ((a.getVertice().getDistancia() == null) || (a.getPeso() < a.getVertice().getDistancia()))) {

                        if (a.getVertice().getDistancia() == null) {

                            heap.add(0, a.getVertice());
                        }

                        a.getVertice().setPai(u);
                        a.getVertice().setDistancia(a.getPeso());
                        minHeapfy(heap, 0);
                    }

                    a = a.getProx();
                }
            }
        }

    }

    private static Vertice extractMin(ArrayList<Vertice> heap) {

        Vertice min;

        troca(heap, 0, heap.size() - 1);
        min = heap.remove(heap.size() - 1);
        minHeapfy(heap, 0);

        return min;
    }

    private static void troca(ArrayList<Vertice> heap, int source, int destino) {

        Vertice v;
        v = heap.get(source);
        heap.set(source, heap.get(destino));
        heap.set(destino, v);
    }

    private static void minHeapfy(ArrayList<Vertice> heap, int source) {

        int l, r, menor;
        l = (source * 2) + 1;
        r = (source * 2) + 2;

        if ((heap.size() - 1 >= l) && (heap.get(l).getDistancia() < heap.get(source).getDistancia())) {

            menor = l;
        } else {

            menor = source;
        }

        if ((heap.size() - 1 >= r) && (heap.get(r).getDistancia() < heap.get(menor).getDistancia())) {

            menor = r;
        }

        if (menor != source) {

            troca(heap, source, menor);
            minHeapfy(heap, menor);
        }


    }
}
