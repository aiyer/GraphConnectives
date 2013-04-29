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
		 typename T
		,typename VertexPropertyMap1
		>
bool check_intersection( const std::vector<T>& vec1
						,const std::vector<T>& vec2
						,const VertexPropertyMap1& v_prop_map_g1
						)
{
	std::vector<T>::const_iterator it_vec1, it_vec2;
	
	for(it_vec1 = vec1.begin(); it_vec1 != vec1.end(); ++it_vec1){
		for(it_vec2 = vec2.begin(); it_vec2 != vec2.end(); ++it_vec2){
			if( get(v_prop_map_g1,*it_vec1) == get(v_prop_map_g1,*it_vec2) )
				return true;
		}
	}

	return false;
}

template <
		 typename Graph1
		,typename OutGraph
		,typename EdgePropertyMap1
		,typename VertexPropertyMapOg
		>
bool LineGraph(  const Graph1& g1
			   , OutGraph& og
			   , const EdgePropertyMap1& e_prop_map_g1
			   , VertexPropertyMapOg& v_prop_map_og
			 )
{
	//Creating vertex
	std::map<graph_traits<Graph1>::vertex_descriptor, std::string> v_names_g1;
	boost::associative_property_map<std::map<graph_traits<Graph1>::vertex_descriptor, std::string> > v_prop_map_g1(v_names_g1);
	for (unsigned int i = 0; i < num_vertices(g1); i++) {
		v_names_g1.insert(make_pair(i, "G1V" + boost::lexical_cast<std::string>(i)));
	}

	return LineGraph(g1, og, v_prop_map_g1, e_prop_map_g1, v_prop_map_og);
}

template <
		 typename Graph1,typename OutGraph
		,typename VertexPropertyMap1
		,typename EdgePropertyMap1
		,typename VertexPropertyMapOg
		>
bool LineGraph( const Graph1& g1, OutGraph& og,
				const VertexPropertyMap1& v_prop_map_g1,
				const EdgePropertyMap1& e_prop_map_g1,
				VertexPropertyMapOg& v_prop_map_og
			){

		//Graph requirements
		//TODO: do undirected check
		function_requires< VertexListGraphConcept<Graph1> >();
		function_requires< VertexListGraphConcept<OutGraph> >();

		typedef typename graph_traits<Graph1>::vertex_descriptor vertex1_t;
		typedef typename graph_traits<Graph1>::vertices_size_type size_type;
		function_requires< ReadablePropertyMapConcept<VertexPropertyMap1, vertex1_t> >();
		/*TODO: Add func_req for og too*/

		if(num_vertices(g1) == 0){
			return true;
		}

		typename graph_traits<Graph1>::vertex_iterator vi1_start,vi1_end, vj1_start, vj1_end;
		typedef typename boost::graph_traits<Graph1>::adjacency_iterator adjacency_iterator;
		std::map< graph_traits<Graph1>::edge_descriptor  
				, std::vector<graph_traits<Graph1>::vertex_descriptor> 
				> edge_end_points;

		std::map< graph_traits<Graph1>::edge_descriptor  
				, graph_traits<Graph1>::vertex_descriptor 
				> edge_vertex_mapping;

		typename graph_traits<Graph1>::edge_iterator ei_start,ei_end;
		int i=0;
		graph_traits<Graph1>::vertex_descriptor v;
		for (tie(ei_start, ei_end) = edges(g1); ei_start != ei_end ; ++ei_start,++i){	
					
			edge_end_points[*ei_start].push_back( source(*ei_start,g1) );
			edge_end_points[*ei_start].push_back( target(*ei_start,g1) );
			
			//add new vertex to og.
			v = add_vertex(og);
			
			//value_type should match.
 			v_prop_map_og[v] = get(e_prop_map_g1,*ei_start);
			edge_vertex_mapping[*ei_start] = v;
		}

		std::map< graph_traits<Graph1>::edge_descriptor  
				, std::vector<graph_traits<Graph1>::vertex_descriptor> 
				>::iterator it_map1;

		std::map< graph_traits<Graph1>::edge_descriptor  
				, std::vector<graph_traits<Graph1>::vertex_descriptor> 
				>::iterator it_map2;

		for (it_map1 = edge_end_points.begin(); it_map1 != edge_end_points.end();++it_map1){
			it_map2 = it_map1;
			++it_map2;
			for (; it_map2 != edge_end_points.end(); ++it_map2){
				
				//check if the edges have common vertex
				if( check_intersection( it_map2->second, it_map1->second, v_prop_map_g1) ){
		
					add_edge(edge_vertex_mapping[it_map1->first]
							,edge_vertex_mapping[it_map2->first]
							,og);
				
				//multiple edges could share same vertex..Hence v_prop_map cannot be used for naming edges in line_graph.
				/*e_prop_map_og[it_map1->first] = get(v_prop_map_g1,edge_vertex_mapping[it_map1->first]);
				  e_prop_map_og[it_map2->first] = get(v_prop_map_g1,edge_vertex_mapping[it_map2->first]);*/
				}
			}
		}
		
		return true;
	}

/**
* Overload in case the user passes only graphs
*/
template <
		  typename Graph1
		, typename OutGraph
		>
bool LineGraph( const Graph1& g1, OutGraph& og) {
	//Creating vertex property map
	std::map<graph_traits<Graph1>::vertex_descriptor, std::string> v_names_g1;
	boost::associative_property_map<std::map<graph_traits<Graph1>::vertex_descriptor, std::string> > v_prop_map_g1(v_names_g1);
	for (unsigned int i = 0; i < num_vertices(g1); i++) {
		v_names_g1.insert(make_pair(i, "G1V" + boost::lexical_cast<std::string>(i)));
	}
	
	std::map<graph_traits<Graph1>::edge_descriptor, std::string> e_names_g1;
	associative_property_map< std::map<graph_traits<Graph1>::edge_descriptor
							 ,std::string> 
							> e_prop_map_g1(e_names_g1);


	std::map<graph_traits<OutGraph>::vertex_descriptor, std::string> v_names_og;
	associative_property_map<std::map<graph_traits<OutGraph>::vertex_descriptor, 
						std::string> > v_prop_map_og(v_names_og);

	typename graph_traits<Graph1>::edge_iterator ei1_start,ei1_end;
	int i = 0;
	for (tie(ei1_start, ei1_end) = edges(g1) ; ei1_start != ei1_end ; ++ei1_start, i++){	
		e_names_g1.insert(make_pair(*ei1_start, "G1E" + boost::lexical_cast<std::string>(i)));
	}

	return LineGraph(g1, og, v_prop_map_g1, e_prop_map_g1, v_prop_map_og);
}
