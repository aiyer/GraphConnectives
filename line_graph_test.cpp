#include "stdafx.h"
#include "line_graph.hpp"
//
//int main()
//{	
	//typedef property<edge_name_t, std::string> EdgeNameProperty;
	//typedef property<vertex_name_t, std::string> VertexNameProperty;
	//typedef adjacency_list<vecS, vecS, undirectedS, VertexNameProperty, EdgeNameProperty> MyGraphType;
//
//	typedef std::pair<int,int> Pair;
//	Pair edge_array[] = { Pair(0,1), Pair(1,2), Pair(0,3), 
//						  Pair(2,3), Pair(1,3)						  
//						   };
//	int num_elements = sizeof(edge_array)/sizeof(edge_array[0]);
//
//	/*Graph G1*/
	/*MyGraphType G;*/
//	for (int i = 0; i < num_elements; ++i){
//		add_edge(edge_array[i].first, edge_array[i].second,EdgeNameProperty("G1E"+boost::lexical_cast<std::string>(i)),G);		
//	}
//
	/*property_map<MyGraphType, vertex_name_t>::type pm_vertex_names_g = get(vertex_name_t(), G);    */
//	boost::put(pm_vertex_names_g, 0, "A");
//	boost::put(pm_vertex_names_g, 1, "B");
//	boost::put(pm_vertex_names_g, 2, "C");
//	boost::put(pm_vertex_names_g, 3, "D");
//	property_map<MyGraphType, edge_name_t>::type pm_edge_names_g = get(edge_name_t(), G);
//
//	/*Output Graph*/
//	MyGraphType og;
//	property_map<MyGraphType, vertex_name_t>::type pm_vertex_names_og = get(vertex_name_t(), og);
//	
//	//LineGraph(G, og,pm_vertex_names_g,pm_edge_names_g,pm_vertex_names_og); 
//	//LineGraph(G, og); 
//	LineGraph(G, og,pm_edge_names_g,pm_vertex_names_og); 
//	
//	// Print Graph og
//	std::cout << "Line Graph: " << std::endl;
//	graph_traits<MyGraphType>::edge_iterator ei1_start,ei1_end;	
//	int i = 0;
//	for(tie(ei1_start,ei1_end)=boost::edges(og); ei1_start != ei1_end; ++ei1_start, i++ ){		
//
//		std::cout<<get(pm_vertex_names_og,source(*ei1_start,og))<<"->"<<get(pm_vertex_names_og,target(*ei1_start,og))<<std::endl;
//		std::cout << source(*ei1_start, og) << "->" << target(*ei1_start, og) << std::endl;
//	}
//
//	std::cout << "\n\n Total Edges: " << i << std::endl;
//
//	return 0;
//}
//
