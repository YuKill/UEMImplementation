package prog;

/**
 *
 * @author jao
 */
public class Vertice {

    private String nome;
    private int index;
    private int cor;
    private int tempoFin;
    private int tempoIni;
    private Integer distancia;
    private Vertice pai;
    private Aresta verticeAdj;

    public Vertice() {
        
        index = 0;
        cor = 0;
        pai = null;
        distancia = 0;
        tempoFin = 0;
        tempoIni = 0;
    }

    public int getIndex() {
        return index;
    }

    public void setIndex(int index) {
        this.index = index;
    }

    public String getNome() {
        return nome;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public int getCor() {
        return cor;
    }

    public void setCor(int cor) {
        this.cor = cor;
    }

    public Integer getDistancia() {
        return distancia;
    }

    public void setDistancia(Integer distancia) {
        this.distancia = distancia;
    }

    public Vertice getPai() {
        return pai;
    }

    public void setPai(Vertice pai) {
        this.pai = pai;
    }

    public Aresta getVerticeAdj() {
        return verticeAdj;
    }

    public void setVerticeAdj(Aresta verticeAdj) {
        this.verticeAdj = verticeAdj;
    }

    public int getTempoFin() {
        return tempoFin;
    }

    public void setTempoFin(int tempoFin) {
        this.tempoFin = tempoFin;
    }

    public int getTempoIni() {
        return tempoIni;
    }

    public void setTempoIni(int tempoIni) {
        this.tempoIni = tempoIni;
    }
    
    @Override
    public Vertice clone(){
        Vertice vertice = new Vertice();
        
        vertice.setIndex(index);
        vertice.setCor(this.getCor());
        vertice.setDistancia(this.getDistancia());
        vertice.setNome(this.getNome());
        
        return vertice;
    }

    @Override
    public String toString() {
        return this.nome;
    }

    
}
