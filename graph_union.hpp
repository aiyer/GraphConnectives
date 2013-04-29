#include <boost/property_map/property_map.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_map/property_map_iterator.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/copy.hpp>
#include <boost/utility.hpp>
#include <boost/detail/algorithm.hpp>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

using namespace boost;

template <
	typename Graph1, typename Graph2,typename OutGraph,
	typename EdgePropertyMap1, typename EdgePropertyMap2,
	typename VertexPropertyMap1, typename VertexPropertyMap2,
	typename VertexPropertyMapOg, typename EdgePropertyMapOg
		>
bool Union( const Graph1& g1, const Graph2& g2, OutGraph& og, const EdgePropertyMap1& e_prop_map_g1, 
			const EdgePropertyMap2& e_prop_map_g2,const VertexPropertyMap1& v_prop_map_g1,
			const VertexPropertyMap2& v_prop_map_g2,VertexPropertyMapOg& v_prop_map_og, 
			EdgePropertyMapOg& e_prop_map_og
			)
	{
		//Graph requirements
		function_requires< VertexListGraphConcept<Graph1> >();
		function_requires< EdgeListGraphConcept<Graph1> >();
		function_requires< VertexListGraphConcept<Graph2> >();
		function_requires< EdgeListGraphConcept<Graph2> >();	
		function_requires< VertexListGraphConcept<OutGraph> >();
		function_requires< EdgeListGraphConcept<OutGraph> >();	

		typedef typename graph_traits<Graph1>::vertex_descriptor vertex1_t;
		typedef typename graph_traits<Graph2>::vertex_descriptor vertex2_t;
		typedef typename graph_traits<Graph1>::edge_descriptor edge1_t;
		typedef typename graph_traits<Graph2>::edge_descriptor edge2_t;
		typedef typename graph_traits<Graph1>::vertices_size_type size_type;
    
		function_requires< ReadablePropertyMapConcept<EdgePropertyMap1, edge1_t> >();		
		function_requires< ReadablePropertyMapConcept<EdgePropertyMap2, edge2_t> >();		
		function_requires< ReadablePropertyMapConcept<VertexPropertyMap1, vertex1_t> >();
		function_requires< ReadablePropertyMapConcept<VertexPropertyMap2, vertex2_t> >();
		/*TODO: Add func_req for og too*/

		if(num_vertices(g1) == 0){
			copy_graph(g2,og);
			return true;
		}else if(num_vertices(g2) == 0){
			copy_graph(g1,og);
			return true;
		}

		/*Check if both graphs have common vertex */
		typename graph_traits<Graph1>::vertex_iterator vi1_start,vi1_end;
		typename graph_traits<Graph2>::vertex_iterator vi2_start,vi2_end;
		for(tie(vi1_start,vi1_end)=boost::vertices(g1); vi1_start != vi1_end; ++vi1_start ){			
			for(tie(vi2_start,vi2_end)=boost::vertices(g2); vi2_start != vi2_end; ++vi2_start ){		
				if( get(v_prop_map_g1,*vi1_start) == get(v_prop_map_g2,*vi2_start) ){
					return false;
				}
			}
		}

		/*check if both graphs have common edge*/
		typename graph_traits<Graph1>::edge_iterator ei1_start,ei1_end;
		typename graph_traits<Graph2>::edge_iterator ei2_start,ei2_end;
		for(tie(ei1_start,ei1_end)=boost::edges(g1); ei1_start != ei1_end; ++ei1_start ){			
			for(tie(ei2_start,ei2_end)=boost::edges(g2); ei2_start != ei2_end; ++ei2_start ){		
				if( get(e_prop_map_g1,*ei1_start) == get(e_prop_map_g2,*ei2_start)){					
					return false;
				}
			}

		}
		
		/*vector to store the edge_names*/
		std::vector<typename property_traits<EdgePropertyMapOg>::value_type> edge_names_og;

		/*Copy edges from Graph 1 and Graph 2 to OutGraph*/
		for (tie(ei1_start, ei1_end) = edges(g1) ; ei1_start != ei1_end ; ++ei1_start){			
			add_edge(source(*ei1_start,g1), target(*ei1_start,g1), og);
			v_prop_map_og[source(*ei1_start,g1)] = get(v_prop_map_g1,source(*ei1_start,g1));
			v_prop_map_og[target(*ei1_start,g1)] = get(v_prop_map_g1,target(*ei1_start,g1));
			edge_names_og.push_back(get(e_prop_map_g2,*ei1_start));			
		}
		
		for (tie(ei2_start, ei2_end) = edges(g2) ; ei2_start != ei2_end ; ++ei2_start){			
			add_edge(source(*ei2_start,g2)+num_vertices(g1), target(*ei2_start,g2)+num_vertices(g2), og);
			v_prop_map_og[source(*ei2_start,g2) + num_vertices(g1)] = get(v_prop_map_g2,source(*ei2_start,g1));
			v_prop_map_og[target(*ei2_start,g2) + num_vertices(g2)] = get(v_prop_map_g2,target(*ei2_start,g1));
			edge_names_og.push_back(get(e_prop_map_g2,*ei2_start));
		}		
		/*Copy - End*/

		typename graph_traits<OutGraph>::edge_iterator ei_start,ei_end;
		int index=0;
		for (tie(ei_start, ei_end) = edges(og) ; ei_start != ei_end ; ++ei_start, ++index){			
			e_prop_map_og[*ei_start] =  edge_names_og[index];
		}
		return true;

	}

/**
* Overload in case the user passes only the graphs and vertex property maps
*/
template <
	typename Graph1, typename Graph2,typename OutGraph,	
	typename VertexPropertyMap1, typename VertexPropertyMap2,
	typename VertexPropertyMapOg
		>
typename boost::enable_if<
				  boost::is_same < typename graph_traits<Graph1>::vertex_descriptor
								  ,typename VertexPropertyMap1::key_type
								>
				, bool>::type
Union( const Graph1& g1, const Graph2& g2, OutGraph& og, const VertexPropertyMap1& v_prop_map_g1,
			const VertexPropertyMap2& v_prop_map_g2, VertexPropertyMapOg& v_prop_map_og
			) {
	std::map<graph_traits<Graph1>::edge_descriptor, std::string> e_names_g1;
	boost::associative_property_map<std::map<graph_traits<Graph1>::edge_descriptor, std::string> > e_prop_map_g1(e_names_g1);

	typename graph_traits<Graph1>::edge_iterator ei1_start,ei1_end;
	typename graph_traits<Graph2>::edge_iterator ei2_start,ei2_end;

	int i = 0;
	for (tie(ei1_start, ei1_end) = edges(g1) ; ei1_start != ei1_end ; ++ei1_start, i++){		
		e_names_g1.insert(make_pair(*ei1_start, "G1E" + boost::lexical_cast<std::string>(i)));
	}

	i = 0;
	std::map<graph_traits<Graph2>::edge_descriptor, std::string> e_names_g2;
	boost::associative_property_map<std::map<graph_traits<Graph2>::edge_descriptor, std::string> > e_prop_map_g2(e_names_g2);
	for (tie(ei2_start, ei2_end) = edges(g2) ; ei2_start != ei2_end ; ++ei2_start, i++){		
		e_names_g2.insert(make_pair(*ei2_start, "G2E" + boost::lexical_cast<std::string>(i)));
	}4

	std::map<graph_traits<OutGraph>::edge_descriptor, std::string> e_names_og;
	associative_property_map<std::map<graph_traits<OutGraph>::edge_descriptor, 
						std::string> > e_prop_map_og(e_names_og);

	return Union(g1, g2, og, e_prop_map_g1, e_prop_map_g2, v_prop_map_g1, v_prop_map_g2, v_prop_map_og, e_prop_map_og); 
}

/**
* Overload in case the user passes only the graphs and edge property maps
*/
template <
	typename Graph1, typename Graph2,typename OutGraph,
	typename EdgePropertyMap1, typename EdgePropertyMap2,	
	typename EdgePropertyMapOg
		>
typename boost::enable_if<
		  boost::is_same < typename graph_traits<Graph1>::edge_descriptor
						 , typename EdgePropertyMap1::key_type
						>
		, bool>::type
Union( const Graph1& g1, const Graph2& g2, OutGraph& og, const EdgePropertyMap1& e_prop_map_g1, 
			const EdgePropertyMap2& e_prop_map_g2, EdgePropertyMapOg& e_prop_map_og
			)
{
	std::map<graph_traits<Graph1>::vertex_descriptor, std::string> v_names_g1;
	boost::associative_property_map<std::map<graph_traits<Graph1>::vertex_descriptor, std::string> > v_prop_map_g1(v_names_g1);
	for (unsigned int i = 0; i < num_vertices(g1); i++) {
		v_names_g1.insert(make_pair(i, "G1V" + boost::lexical_cast<std::string>(i)));
	}

	std::map<graph_traits<Graph2>::vertex_descriptor, std::string> v_names_g2;
	boost::associative_property_map<std::map<graph_traits<Graph2>::vertex_descriptor, std::string> > v_prop_map_g2(v_names_g2);
	for (unsigned int i = 0; i < num_vertices(g2); i++) {
		v_names_g2.insert(make_pair(i, "G2V" + boost::lexical_cast<std::string>(i)));
	}

	std::map<graph_traits<OutGraph>::vertex_descriptor, std::string> v_names_og;
	associative_property_map<std::map<graph_traits<OutGraph>::vertex_descriptor, 
						std::string> > v_prop_map_og(v_names_og);

	return Union(g1, g2, og, e_prop_map_g1, e_prop_map_g2, v_prop_map_g1, v_prop_map_g2, v_prop_map_og, e_prop_map_og); 
}



/**
* Overload in case the user passes only graphs
*/
template <
	typename Graph1, typename Graph2,typename OutGraph
		>
bool Union( const Graph1& g1, const Graph2& g2, OutGraph& og) {
	//Creating vertex Property Map
	std::map<graph_traits<Graph1>::vertex_descriptor, std::string> v_names_g1;
	boost::associative_property_map<std::map<graph_traits<Graph1>::vertex_descriptor, std::string> > v_prop_map_g1(v_names_g1);
	for (unsigned int i = 0; i < num_vertices(g1); i++) {
		v_names_g1.insert(make_pair(i, "G1V" + boost::lexical_cast<std::string>(i)));
	}

	std::map<graph_traits<Graph2>::vertex_descriptor, std::string> v_names_g2;
	boost::associative_property_map<std::map<graph_traits<Graph2>::vertex_descriptor, std::string> > v_prop_map_g2(v_names_g2);
	for (unsigned int i = 0; i < num_vertices(g2); i++) {		
		v_names_g2.insert(make_pair(i, "G2V" + boost::lexical_cast<std::string>(i)));
	}
	
	//Creating edge Property Map
	std::map<graph_traits<Graph1>::edge_descriptor, std::string> e_names_g1;
	boost::associative_property_map<std::map<graph_traits<Graph1>::edge_descriptor, std::string> > e_prop_map_g1(e_names_g1);

	typename graph_traits<Graph1>::edge_iterator ei1_start,ei1_end;
	typename graph_traits<Graph2>::edge_iterator ei2_start,ei2_end;

	int i = 0;
	for (tie(ei1_start, ei1_end) = edges(g1) ; ei1_start != ei1_end ; ++ei1_start, i++){
		e_names_g1.insert(make_pair(*ei1_start, "G1E" + boost::lexical_cast<std::string>(i)));
	}

	i = 0;
	std::map<graph_traits<Graph2>::edge_descriptor, std::string> e_names_g2;
	boost::associative_property_map<std::map<graph_traits<Graph2>::edge_descriptor, std::string> > e_prop_map_g2(e_names_g2);
	for (tie(ei2_start, ei2_end) = edges(g2) ; ei2_start != ei2_end ; ++ei2_start, i++){		
		e_names_g2.insert(make_pair(*ei2_start, "G2E" + boost::lexical_cast<std::string>(i)));
	}

	std::map<graph_traits<OutGraph>::vertex_descriptor, std::string> v_names_og;
	associative_property_map<std::map<graph_traits<OutGraph>::vertex_descriptor, 
						std::string> > v_prop_map_og(v_names_og);


	std::map<graph_traits<OutGraph>::edge_descriptor, std::string> e_names_og;
	associative_property_map<std::map<graph_traits<OutGraph>::edge_descriptor, 
						std::string> > e_prop_map_og(e_names_og);


	return Union(g1, g2, og, e_prop_map_g1, e_prop_map_g2, v_prop_map_g1, v_prop_map_g2, v_prop_map_og, e_prop_map_og); 
}
