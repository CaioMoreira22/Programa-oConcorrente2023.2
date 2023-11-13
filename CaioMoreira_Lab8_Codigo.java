/* Disciplina: Programacao Concorrente */
/* Nome: Caio Moraes Santos Moreira  */
/* Laboratório: 8 */
/* Codigo */

import java.util.LinkedList;

//-------------------------------------------------------------------------------
//Classe que define um pool de threads 
//!!! Documentar essa classe !!!
class FilaTarefas {
    private final int nThreads;
    private final MyPoolThreads[] threads;
    private final LinkedList<Runnable> queue;
    private boolean shutdown;

    public FilaTarefas(int nThreads) {
        this.shutdown = false;
        this.nThreads = nThreads;
        queue = new LinkedList<Runnable>();
        threads = new MyPoolThreads[nThreads];
        for (int i=0; i<nThreads; i++) {
            threads[i] = new MyPoolThreads();
            threads[i].start();
        } 
    }

    public void execute(Runnable r) {
        synchronized(queue) {
            if (this.shutdown) return;
            queue.addLast(r);
            queue.notify();
        }
    }
    
    public void shutdown() {
        synchronized(queue) {
            this.shutdown=true;
            queue.notifyAll();
        }
        for (int i=0; i<nThreads; i++) {
          try { threads[i].join(); } catch (InterruptedException e) { return; }
        }
    }

    private class MyPoolThreads extends Thread {
       public void run() {
         Runnable r;
         while (true) {
           synchronized(queue) {
             while (queue.isEmpty() && (!shutdown)) {
               try { queue.wait(); }
               catch (InterruptedException ignored){}
             }
             if (queue.isEmpty() && shutdown) return;  
             r = (Runnable) queue.removeFirst();
           }
           try { r.run(); }
           catch (RuntimeException e) {}
         } 
       } 
    } 
}
//-------------------------------------------------------------------------------

//--PASSO 1: cria uma classe que implementa a interface Runnable 
class VerificadorPrimos implements Runnable {
    private long number;
    private static int contadorPrimos = 0; // Contador compartilhado entre as threads
    private static int totalPrimosEncontrados = 0;

    public VerificadorPrimos(long number) {
        this.number = number;
    }
    
    private static synchronized int incrementarContador() {
        totalPrimosEncontrados++;
        return ++contadorPrimos;
    }

    public static int getTotalPrimosEncontrados() {
        return totalPrimosEncontrados;
    }

    private static boolean ehPrimo(long n) {
        if (n <= 1) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        for (int i = 3; i <= Math.sqrt(n) + 1; i += 2) {
           if (n % i == 0) return false;
        }
        return true;
    }

    @Override
    public void run(){
        if(ehPrimo(number)) {
            incrementarContador();
        }
    }
}

//Classe da aplicação (método main)
class main {
    private static final int NTHREADS = 10;
    private static int n = 10000000; //Valor até onde vai o intervalo desejado

    public static void main(String[] args) {
      FilaTarefas pool = new FilaTarefas(NTHREADS); 
      
      for (int i = 0; i <= n; i++) {
        
        Runnable r = new VerificadorPrimos(i);
        pool.execute(r);
      }

      
      pool.shutdown();
      System.out.println("Total de primos encontrados: " + VerificadorPrimos.getTotalPrimosEncontrados());
   }
}
