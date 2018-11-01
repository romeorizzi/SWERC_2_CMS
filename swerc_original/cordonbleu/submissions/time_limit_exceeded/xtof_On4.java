/* swerc - c.durr - 2017

  Cordon Bleu

  implementation in O(n^4)
*/

import java.util.*;

class KuhnMunkres {

    // ------------------------------------- le graphe

    // taille du graphe
    int n;

    // poids des aretes
    double[][] w;

    // ------------------------------------- le couplage

    //couplage : m0[i] = sommet j dans V auquel i est couple, m1=inverse
    int[] m0, m1;
    final static int LIBRE=-1;

    // cardinalite du couplage
    int m;

    // l'arbre alternant
    // racine est un sommet libre de S
    int racine;
    // path : T -> V definit avec m0 l'arbre alternant
    // path[v] = predecesseur de v dans l'arbre
    int[] path;

    // ------------------------------------- l'etiquetage, etc.

    //etiquetage
    double[] l0, l1;

    //ensembles S,T
    boolean[] S, T;

    // slack en anglais,
    // pour tout v pas dans T :
    // margeVal[v] = min l0[u]+l1[v]-w[u][v] sur u dans S
    // et marge0[v] est u qui minimise
    double[] margeVal;
    int[]    marge0;


    KuhnMunkres(int _n) {
        n = _n;
        w        = new double[n][n];
        m0       = new int[n];
        m1       = new int[n];
        path     = new int[n];
        l0       = new double[n];
        l1       = new double[n];
        S        = new boolean[n];
        T        = new boolean[n];
        margeVal = new double[n];
        marge0   = new int[n];
    }

    // ------------------------------------- parties de l'algorithme

   // chercher dans U un sommet non couple'
    int sommetLibre() {
        for(int u=0; u<n; u++)
            if (m0[u]==LIBRE)
                return u;
        throw new Error("recherche de sommet libre sur couplage parfait");
   }

    // creer un etiquetage trivial initial
    void initEtiquettes() {
        double sup;
        // etiquetage initial
        for (int u=0; u<n; u++) {
            sup = w[u][0];
            for (int v=1; v<n; v++)
                if (sup<w[u][v])
                    sup = w[u][v];
            l0[u] = sup;
        }
        Arrays.fill(l1, 0.0);
        // et couplage vide
        Arrays.fill(m0, LIBRE);
        Arrays.fill(m1, LIBRE);
        m = 0;
    }

    // chercher a augmenter le couplage
    void augmenterCouplage() {
        int y0=0, y1, next;
        double a=0.0, marge_uv;
        do {
            // chercher (y0,y1) dans S*!T avec la plus petite marge
            y1 = LIBRE;
            for (int v=0; v<n; v++)
                if (! T[v]) {
                    if (y1==LIBRE || a > margeVal[v]) {
                        y1 = v;
                        y0 = marge0[v];
                        a  = margeVal[v];
                    }
                }

            // si a>0, alors N_l(S)=T et
            // on peut ameliorer l par a = marge[y]
            if (a > 0) {
                for (int u=0; u<n; u++)
                    if (S[u])
                        l0[u]  -= a;
                for (int v=0; v<n; v++)
                    if (T[v])
                        l1[v]  += a;
        else
      margeVal[v] -= a;
                a = 0;
            }

            // desormais a==0 et N_l(S) =/= T, et y1 est de N_l(S)-T.
            // on augmente l'arbre
            if (m1[y1]!=LIBRE) {
                int u = m1[y1];
                // on ajoute un nouveau sommet dans S, il faut mettre a jour la marge
                S[u] = true;
                for (int v=0; v<n; v++)
                    if (! T[v]) {
                        marge_uv = l0[u] + l1[v] - w[u][v];
                        if (margeVal[v]> marge_uv) {
                            margeVal[v] = marge_uv;
                            marge0[v]   = u;
                        }
                    }
            }
            T[y1]     = true;
            path[y1]  = y0;
        } while (m1[y1]!=LIBRE);

        // y1 est libre, on peut augmenter le couplage
        // remonter le chemin de y1 vers la racine
        do {
            y0    = path[y1];
            next  = m0[y0];
            // changer le couplage
            m1[y1] = y0;
            m0[y0] = y1;
            // iterer plus loin sur le chemin vers la racine
            y1 = next;
        } while (y0!=racine);
        m++;
    }

    int solve() {
        initEtiquettes();
        // chercher a rendre le coupage parfait
        while (m<n) {
            // chercher sommet libre u et poser S={u}, T={}
            Arrays.fill(S, false);
            Arrays.fill(T, false);
            racine = sommetLibre();
            S[racine] = true;
            // intialiser la marge
            for(int v=0; v<n; v++) {
                margeVal[v] = l0[racine]+l1[v]-w[racine][v];
                marge0[v]   = racine;
            }
            augmenterCouplage();
        }
        int val = 0;
        for (int i=0; i < n; i++) {
          val += w[i][m0[i]];
        }
        return val;
    }
}



class xtof_On4 {

    public static int dist(int x1, int y1, int x2, int y2) {
      return Math.abs(x1 - x2) + Math.abs(y1 - y2);
    }

    public static void main(String args[]) {
        Scanner in = new Scanner(System.in);
        int nb_bottles = in.nextInt();
        int nb_couriers = in.nextInt();
        int bottle_x[] = new int[nb_bottles];
        int bottle_y[] = new int[nb_bottles];
        int courier_x[] = new int[nb_couriers];
        int courier_y[] = new int[nb_couriers];
        for (int i=0; i < nb_bottles; i++) {
          bottle_x[i] = in.nextInt();
          bottle_y[i] = in.nextInt();
        }
        for (int i=0; i < nb_couriers; i++) {
          courier_x[i] = in.nextInt();
          courier_y[i] = in.nextInt();
        }
        int lab_x = in.nextInt();
        int lab_y = in.nextInt();
        int n = nb_couriers + nb_bottles - 1;
        KuhnMunkres G = new KuhnMunkres(n);
        for (int j=0; j < nb_bottles; j++) {
          int d_bl = dist(bottle_x[j], bottle_y[j], lab_x, lab_y);
          for (int i=0; i<nb_couriers; i++) {
            int d_cb = dist(courier_x[i], courier_y[i], bottle_x[j], bottle_y[j]);
            G.w[i][j] = - d_cb - d_bl;
          }
          for (int i=nb_couriers; i < n; i++) {
            G.w[i][j] = -2 * d_bl;
          }
        }
        System.out.println( -G.solve() );
    }

}
