#include "stdafx.h"
#include "graph_join.hpp"
//
//
//int main()
//{	
//	typedef property<edge_name_t, std::string> EdgeNameProperty;
//	typedef property<vertex_name_t, std::string> VertexNameProperty;
//	typedef adjacency_list<vecS, vecS, directedS, VertexNameProperty, EdgeNameProperty> MyGraphType;
//
//	typedef std::pair<int,int> Pair;
//	Pair edge_array[] = { Pair(0,1), Pair(0,2), Pair(0,3), 
//						  Pair(0,4), Pair(2,0), Pair(3,0), 
//						  Pair(2,4), Pair(3,1), Pair(3,4), 
//						  Pair(4,0), Pair(4,1) };
//
//	int num_elements = sizeof(edge_array)/sizeof(edge_array[0]);
//	MyGraphType G1, G2;
//
//	property_map<MyGraphType, vertex_name_t>::type pm_vertex_names_g1 = get(vertex_name_t(), G1);    
//	
//	for (int i = 0; i < num_elements; ++i){
//		add_edge(edge_array[i].first, edge_array[i].second,EdgeNameProperty("G1E"+boost::lexical_cast<std::string>(i)),G1);		
//	}
//	
//	boost::put(pm_vertex_names_g1, 0, "A");
//	boost::put(pm_vertex_names_g1, 1, "B");
//	boost::put(pm_vertex_names_g1, 2, "C");
//	boost::put(pm_vertex_names_g1, 3, "D");
//	boost::put(pm_vertex_names_g1, 4, "E");
//	property_map<MyGraphType, edge_name_t>::type pm_edge_names_g1 = get(edge_name_t(), G1);
//
//	property_map<MyGraphType, vertex_name_t>::type pm_vertex_names_g2 = get(vertex_name_t(), G2);    	
//	for (int i = 0; i < num_elements; ++i){
//		add_edge(edge_array[i].first, edge_array[i].second, EdgeNameProperty("G2E"+boost::lexical_cast<std::string>(10 - i)), G2);
//	}	
//	boost::put(pm_vertex_names_g2, 0, "I");
//	boost::put(pm_vertex_names_g2, 1, "J");
//	boost::put(pm_vertex_names_g2, 2, "F");
//	boost::put(pm_vertex_names_g2, 3, "G");
//	boost::put(pm_vertex_names_g2, 4, "H");
//	property_map<MyGraphType, edge_name_t>::type pm_edge_names_g2 = get(edge_name_t(), G2);   
//
//	MyGraphType og;
//	property_map<MyGraphType, vertex_name_t>::type pm_vertex_names_og = get(vertex_name_t(), og);
//	property_map<MyGraphType, edge_name_t>::type pm_edges_names_og = get(edge_name_t(), og);
//
//	//Join(G1, G2, og, pm_edge_names_g1, pm_edge_names_g2, pm_vertex_names_g1, pm_vertex_names_g2, pm_vertex_names_og, pm_edges_names_og); 
//	//Join(G1, G2, og, pm_edge_names_g1, pm_edge_names_g2, pm_edges_names_og); 
//	Join(G1, G2, og, pm_vertex_names_g1, pm_vertex_names_g2, pm_vertex_names_og); 
//	//Join(G1, G2, og); 
//
//	// Print Graph og
//	std::cout << "Graph Join : " << std::endl;
//	graph_traits<MyGraphType>::edge_iterator ei1_start,ei1_end;	
//	int i = 0;
//	for(tie(ei1_start,ei1_end)=boost::edges(og); ei1_start != ei1_end; ++ei1_start, i++ ){		
//		
//		std::cout << "Edge Name : " << get(pm_edges_names_og,*ei1_start) << " ";
//		std::cout << "Edge : " << get(pm_vertex_names_og, source(*ei1_start,og))
//				<< "->" << get(pm_vertex_names_og, target(*ei1_start,og)) << std::endl;
//		//std::cout << source(*ei1_start, og) << "->" << target(*ei1_start, og) << std::endl;
//	}
//	std::cout << "\n\n Total Edges : " << i << std::endl;
//	return 0;
//}
//
