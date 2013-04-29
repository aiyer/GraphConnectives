#include "stdafx.h"
#include "line_graph.hpp"
#include "graph_union.hpp"
#include <boost/graph/adjacency_list.hpp>  
#include <boost/graph/random.hpp> 
#include <boost/random/linear_congruential.hpp>  

#include <iostream>  
#include <ctime>  

using namespace std;


  int main()  
  {   
	  typedef adjacency_list<setS,vecS,undirectedS> graph_t;     
	  unsigned int iteration_max =10;    
	  unsigned int N =1000;   
	  unsigned int M =1000;     
	  clock_t start, finish;       
	 // for( int it =1; it <=iteration_max; ++it){        
	
	  for(int i =0;i<3;i++){
		  graph_t g1;
		  graph_t g2;		  
		  graph_t g3;
		  std::cout<<N<<" "<<M<<"\n";
		  boost::minstd_rand gen(1);          
		 // start =clock();  	
		  boost::generate_random_graph( g1, N, M, gen, false ,false);

		  boost::generate_random_graph( g2, N, M, gen, false ,false);
		  //finish =clock();      
		  //std::cout <<it <<" "   <<(double)(finish - start)/(double)CLOCKS_PER_SEC <<" "; 
		  start =clock();          
		  LineGraph(g1,g3);
		  //Union(g1,g2,g3);
		  finish =clock();            
		  std::cout <<(double)(finish - start)/(double)CLOCKS_PER_SEC <<std::endl;        
		  N += 1000;
		  M = N;

	  }
	 // }     
	  return 0;  
  }  
