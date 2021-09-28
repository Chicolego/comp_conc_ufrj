class Questao4 {
    private int leitores, escritor, escritorEmEspera;

    // Construtor
    Questao4() {
        this.escritorEmEspera = 0;
        this.leitores = 0; // Leitores lendo
        this.escritor = 0; // Escritor escrevendo
    }

    // Entrada de um leitor
    public synchronized void EntraLeitor (int id) {
        try {
            while (this.escritor > 0 || this.escritorEmEspera > 0) {
                System.out.println ("Bloqueado o leitor " + id + ".");
                wait();  // Aguarda sinal para continuar
            }
            this.leitores++;
            System.out.println ("O leitor " + id + " está lendo.");
        } catch (InterruptedException e) { }
    }

    // Saída de um leitor
    public synchronized void SaiLeitor (int id) {

        this.leitores--;
        System.out.println ("Retirado o leitor " + id + ".");

        if (this.leitores == 0)
            this.notify(); // Notifica possível escritor bloqueado de que não há mais leitores
    }

    // Entrada de um escritor
    public synchronized void EntraEscritor (int id) {
        try {
            while ((this.leitores > 0) || (this.escritor > 0)) {
                System.out.println ("Bloqueado o escritor " + id + ".");
                this.escritorEmEspera ++;
                wait();  // Aguarda sinal para continuar
            }
            this.escritor++;
            System.out.println ("O escritor " + id + " está escrevendo.");
        } catch (InterruptedException e) { }
    }

    // Saída de um escritor
    public synchronized void SaiEscritor (int id) {
        this.escritor--;
        this.escritorEmEspera--; // Saída da espera para realizar a tarefa
        notifyAll(); // Sinaliza para leitores ou escritores bloqueados
        System.out.println ("Retirado o escritor " + id + ".");
    }
}
