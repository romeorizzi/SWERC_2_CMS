import java.util.Scanner;
import java.util.Stack;
import java.util.ArrayList;

        
class solLouisAlphabet2 {

    static class Auto {
        int[][] trans;
        boolean [] is_final;
        int size;

        public Auto(int n) {
            size=n;
            trans = new int [size][alphabet.length()] ;
            is_final = new boolean[size];
        }
    
        public Auto(Auto a, Auto b) {
            this(a.size*b.size);
            for(int sa = 0 ; sa  < a.size ; sa++) {
                for(int sb = 0 ; sb  < b.size ; sb++) {
                    is_final[sa*b.size+sb] = a.is_final[sa] && b.is_final[sb] ;
                    for(int l = 0 ; l < alphabet.length() ; l++) {
                        trans[sa*b.size+sb][l] = a.trans[sa][l] * b.size + b.trans[sb][l] ;
                    }
                }
            }
        }

        public Auto simplify() {
            int remap[] = new int[size];
            boolean seen[] = new boolean[size];
            for(int i = 0 ; i < size ; i++)
                seen[i]=false;
            Stack<Integer> todo = new Stack<Integer>();
            todo.add(0);
            seen[0]=true;
            while(!todo.empty()) {
                int cur = todo.pop();
                for(int l = 0 ; l < alphabet.length() ; l++) {
                    if(!seen[trans[cur][l]]) {
                        seen[trans[cur][l]]=true;
                        todo.push(trans[cur][l]);
                    }
                }
            }
            int nb_seen = 0 ;
            for(int i = 0 ; i < size ; i++) {
                remap[i] = nb_seen ;
                if(seen[i])
                    nb_seen++;
            }
            Auto res = new Auto(nb_seen);
            for(int s = 0 ; s < size ; s++)
                if(seen[s]) {
                    for(int l = 0 ; l < alphabet.length() ; l++ )
                        res.trans[remap[s]][l] = remap[trans[s][l]] ;
                    res.is_final[remap[s]] = is_final[s] ;
                }
            return res;
        }
    
        public Auto(String s) {
            this(s.length()-1);
            int dec = 0, fail =0;
            for(int i = 0 ; i+dec < s.length() ; i++) {
                if (s.charAt(i) == '>') {
                    dec=1;
                    fail=i;
                }
                is_final[i] = (fail==0) ;
                for(int l = 0 ; l < alphabet.length() ; l++) {
                    if(s.charAt(i+dec) == alphabet.charAt(l)) {
                        trans[i][l] = (i+1)%size;
                    }
                    else {
                        trans[i][l] = fail;
                        trans[i][l] = trans[fail][l];
                    }
                    
                }
            }
        }

        void print() {
            System.out.println(size);
            for(int s= 0 ; s < size ; s++ )
                System.out.print(is_final[s]?"1":"0");
            System.out.println("");
            for(int s= 0 ; s < size ; s++ ){
                for(int l = 0 ; l < alphabet.length() ; l++)
                    System.out.print(trans[s][l]+" ");
                System.out.println("");
            }
        }
    
        int count(int steps) {
            int c[][] = new int [2][size] ;
            for(int i = 0 ; i < size ; i++)
                if(is_final[i])
                    c[0][i] = 1 ;
                else
                    c[0][i] = 0 ;
            for(int s = 0 ; s < steps ; s++ ) {
                for(int i = 0 ; i < size ; i++) {
                    int res = 0 ;
                    for(int l = 0 ; l < alphabet.length() ; l++) {
                        res += c[s&1][trans[i][l]] ;
                    }
                    c[(s+1)&1][i] = (res%M) ;
                } 
            }
            return c[steps&1][0] ;
        }
    }

    static final int M = 10000000;
    static String alphabet;
    
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int steps = in.nextInt();
        alphabet = in.nextLine().split(" ")[1];
        String[] rules = in.nextLine().split("\\|");
        Auto res = new Auto(rules[0]);
        for(int i = 1 ; i < rules.length ; i++)
            res = (new Auto(res,new Auto(rules[i]))).simplify();
        System.out.println(res.count(steps));
    }
}
