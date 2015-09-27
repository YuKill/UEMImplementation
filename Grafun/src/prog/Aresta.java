package prog;

/**
 *
 * @author jao
 */
public class Aresta {

    private int peso;
    private Vertice vertice;
    private Aresta prox;

    public Aresta() {

        peso = 0;
        prox = null;
        
    }

    public int getPeso() {
        return peso;
    }

    public void setPeso(int peso) {
        this.peso = peso;
    }

    public Aresta getProx() {
        return prox;
    }

    public void setProx(Aresta prox) {
        this.prox = prox;
    }

    public Vertice getVertice() {
        return vertice;
    }

    public void setVertice(Vertice vertice) {
        this.vertice = vertice;
    }
    
}
