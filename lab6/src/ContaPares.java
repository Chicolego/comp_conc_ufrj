public class ContaPares implements Runnable{
    private int id;
    private int[] vetor;
    private int nthreads;

    private static int total = 0;

    //chamada na main, faz a passagem do id da thread, do tamanho do vetor, e da quantidade de threads usadas
    public ContaPares(int id, int[] vetor, int nthreads){
        this.id = id;
        this.vetor = vetor;
        this.nthreads = nthreads;
    }

    public synchronized static int get() {
        return total;
    }

    public synchronized static void incrementa() {
        total++;
    }

    public void run(){
        for(int i = id; i < vetor.length; i+=nthreads){
            if(vetor[i] % 2 == 0) incrementa();
        }
    }

}
