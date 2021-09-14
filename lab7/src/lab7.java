class Leitora extends Thread{
    LE le;
    int id;

    public Leitora(LE le, int id) {
        this.le = le;
        this.id = id;
    }

    public synchronized boolean Primo (int n){

        if(n == 0 || n == 1){return false;}

        for(int i = 2; i < n; i++){
            if(n % i == 0) {return false;}
        }
        return true;
    }

    public synchronized void run(){
        le.EntraLeitor(this.id);

        if(Primo(lab7.n)){
            System.out.println(lab7.n + " É primo!");
        }
        else{
            System.out.println(lab7.n + " NÃO é primo.");
        }

        le.SaiLeitor(this.id);
    }
}

class Escritora extends Thread{
    LE le;
    int id;

    public Escritora(LE le, int id) {
        this.le = le;
        this.id = id;
    }

    public synchronized void Transforma(){
        le.EntraEscritor(this.id);

        lab7.n = this.id;

        le.SaiEscritor(this.id);
    }

    public void run(){
        Transforma();
    }
}

class LeitoraeEscritora extends Thread{
    LE le;
    int id;

    public LeitoraeEscritora(LE le, int id) {
        this.le = le;
        this.id = id;
    }

    public synchronized boolean Par (int n){
        return n % 2 == 0;
    }

    public synchronized void Transforma(){
        le.EntraEscritor(this.id);

        lab7.n *= 2;

        le.SaiEscritor(this.id);
    }

    public synchronized void run() {
        le.EntraLeitor(this.id);

        if(Par(lab7.n)){
            System.out.println(lab7.n + " é par!");
        }else {
            System.out.println(lab7.n + " é ímpar!");
        }

        le.SaiLeitor(this.id);
        Transforma();
    }
}

public class lab7 {

    public static int n = 5721;
    static final int L = 3;
    static final int E = 4;
    static final int LE = 2;

    public static void main(String[] args) {

        LE le = new LE();

        //Reserva de espaço
        Leitora[] leitora = new Leitora[L];
        Escritora[] escritora = new Escritora[E];
        LeitoraeEscritora[] leitoraeEscritora = new LeitoraeEscritora[LE];

        //inicializa as threads leitoras
        for(int i = 0; i < L; i++){
            leitora[i] = new Leitora(le,i);
            leitora[i].start();
        }

        //inicializa as threads escritoras
        for(int i = 0; i < E; i++){
            escritora[i] = new Escritora(le,i);
            escritora[i].start();
        }

        //inicializa as threads leitoras e escritoras
        for(int i = 0; i < LE; i++){
            leitoraeEscritora[i] = new LeitoraeEscritora(le,i);
            leitoraeEscritora[i].start();
        }


    }
}
