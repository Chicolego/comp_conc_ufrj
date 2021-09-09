public class Main {
    public final static int NTHREADS = 4;
    public final static int N = 3500000;


    public static void main(String[] args) {

        //Reserva de espaço
        Thread[] threads = new Thread[NTHREADS];
        int[] vetor = new int[N];

        //inicializa o vetor
        for(int i = 0; i < N; i++){
            vetor[i] = i;
        }

        //inicializa as threads
        for(int i = 0; i < NTHREADS; i++){
            threads[i] = new Thread(new ContaPares(i, vetor, NTHREADS));
        }

        //inicia as trheads
        for(int i = 0; i < NTHREADS; i++){
            threads[i].start();
        }

        //aguarda o término das threads
        for (Thread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                return;
            }
        }

        //Corretude
        for (int i : vetor) {
            if (i % 2 == 0) Corretude.Incrementa();
        }

        if (Corretude.get() != ContaPares.get()){
            System.out.println("Teste Incorreto!");
        }
        else{
            System.out.println("Verificação bem sucedida. Ambos resultaram em " + Corretude.get());
        }
    }
}
