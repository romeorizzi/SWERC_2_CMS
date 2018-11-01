/* Ioana, drafty code */

import java.io.InputStreamReader;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;
import java.io.FileInputStream;
import java.io.BufferedReader;

public class Ingredients_Draft
{
    int budget, recipes_count, dish_count;
    public class Neighbour {
    	int index,  pluscost, plusprestige;
    	public Neighbour(int index, int pluscost, int plusprestige) {
    		this.index = index; this.pluscost = pluscost; this.plusprestige = plusprestige;
    	}
    }
    public class Dish {
    	public String name;
    	public int cost, prest;
    	public ArrayList<Neighbour> outneighb;
    	public Dish(String name) {
    		this.cost = -1; this.prest = 0;this.name = name; outneighb = new ArrayList<Neighbour>();
    	}
    	public void addOutNeighb(int index, int pluscost, int plusprestige){
    		Neighbour newneighb = new Neighbour(index, pluscost, plusprestige);
    		outneighb.add(newneighb);
    	}
    }
    
    Dish[] dishes;
    HashMap<String,Integer> indices;
    int[] indegree;
    boolean[] seen;
    
    private int getindex(String name) {
    	Integer idx = indices.get(name); 
    	if (null!=idx) return idx;
    	Dish newdish = new Dish(name);
    	dishes[dish_count] = newdish;
    	indices.put(name, dish_count);
    	dish_count++;
    	return dish_count-1;
    }
     
    public void readDataAndComputeCP()  throws Exception{
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        budget = Integer.valueOf(reader.readLine());
        recipes_count = Integer.valueOf(reader.readLine());
        dishes = new Dish[recipes_count*2];
	    indices = new HashMap<String,Integer>();
	    dish_count = 0;
	    for (int i = 0; i < recipes_count; ++i) {
	    	String[] parts = reader.readLine().split(" ");	
	    	int idx1 = getindex(parts[0]); 
	    	int idx2 = getindex(parts[1]);
	    	dishes[idx2].addOutNeighb(idx1,Integer.valueOf(parts[3]).intValue(),Integer.valueOf(parts[4]).intValue());	   
	    }
	    
	    indegree = new int[dish_count]; seen = new boolean[dish_count];
	    for (int i = 0; i<dish_count; ++i) {
	        indegree[i] = 0;
	        seen[i] = false;
	    }
	    for (int i = 0; i<dish_count; ++i)
	        for (Neighbour n:dishes[i].outneighb)
	            indegree[n.index]++;
	   
	    int last = 0;
	    int[] queue = new int[dish_count];
	    for (int i = 0; i<dish_count; ++i) 
	        if (indegree[i] == 0) {
	            dishes[i].cost = 0;
	            queue[last] =i;
	            seen[i] = true;
	            last++;
	        }
	    int first = 0;
	    while (first <last){
	       Dish dishfirst = dishes[queue[first]];
	       for (Neighbour n:dishfirst.outneighb)
	            if (!seen[n.index]) {
	                Dish crtdish = dishes[n.index];
	                int c = dishfirst.cost + n.pluscost;
    		        int p = dishfirst.prest+n.plusprestige;
    		        if (crtdish.cost  == -1 || crtdish.cost > c || ((crtdish.cost == c) && (crtdish.prest<p))) {
    			        crtdish.cost = c; crtdish.prest = p;
    		        }
    		        indegree[n.index]--;
    		        if (indegree[n.index] == 0) {
    		            queue[last] = n.index;
    		            seen[n.index] = true;
    		            last++;
    		        }
    		    }
    	    first++;
       }
       
    }
    
    public void solve() {
    	int[][] highestprest = new int[budget+1][dish_count];
    	for (int i = 0; i<=budget; ++i) {    	
    		for (int j = 0; j<dish_count; ++j) {
    			highestprest[i][j] = -1;
    			if (j > 0) 
    				highestprest[i][j] = highestprest[i][j-1];
    			else if (i == 0)
    				highestprest[i][j] = 0;   			
    			if (dishes[j].cost <= i) {
    				int lastbudget = i-dishes[j].cost;
    				int lastprestige = -1;
    				if (j == 0) {
    					if (lastbudget == 0) lastprestige = 0;
    				} else 
    					lastprestige = highestprest[lastbudget][j-1];
    				
    				if (lastprestige != -1 && lastprestige + dishes[j].prest > highestprest[i][j])
    					highestprest[i][j] = lastprestige + dishes[j].prest;
    			 }
    		}
    	}
    	
    	if (dish_count == 0) {System.out.println("0\n0");return;}
    	int bestbudget = 0;
    	for (int i = 1; i <= budget; ++i)
    		if (highestprest[i][dish_count-1] > highestprest[bestbudget][dish_count-1])
    			bestbudget = i;
    	System.out.println(highestprest[bestbudget][dish_count-1]);
    	System.out.println(bestbudget);
    		
    }
    
    public static void main (String[] args) throws Exception {
    	Ingredients_Draft ing = new Ingredients_Draft();
    	ing.readDataAndComputeCP();
    	ing.solve();
    }
}
        
