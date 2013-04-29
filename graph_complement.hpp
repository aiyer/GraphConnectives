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
	typename Graph1,
	typename OutGraph,
	typename VertexPropertyMap1,
	typename VertexPropertyMapOg
		>
bool Complement( const Graph1& g, OutGraph& og,
				const VertexPropertyMap1& v_prop_map_g, VertexPropertyMapOg& v_prop_map_og
			)
	{
		//Graph requirements
		function_requires< VertexListGraphConcept<Graph1> >();
		function_requires< VertexListGraphConcept<OutGraph> >();

		typedef typename graph_traits<Graph1>::vertex_descriptor vertex1_t;
		typedef typename graph_traits<Graph1>::vertices_size_type size_type;
    
		function_requires< ReadablePropertyMapConcept<VertexPropertyMap1, vertex1_t> >();
		/*TODO: Add func_req for og too*/

		if(num_vertices(g) == 0){
			return true;
		}

		typename graph_traits<Graph1>::vertex_iterator vi_start, vi_end, vj_start, vj_end;
		typedef typename boost::graph_traits<Graph1>::adjacency_iterator adjacency_iterator;

		std::pair<adjacency_iterator, adjacency_iterator> p;
		for(tie(vi_start, vi_end) = boost::vertices(g); vi_start != vi_end; ++vi_start ){
			for(tie(vj_start,vj_end) = boost::vertices(g); vj_start != vj_end; ++vj_start ){
				if (v_prop_map_g[*vi_start] == v_prop_map_g[*vj_start]) 
					continue;
				
				p = adjacent_vertices(*vi_start, g);
				for (; p.first != p.second; p.first++) {
					if (v_prop_map_g[*(p.first)] == v_prop_map_g[*vj_start]) {						
						break;
					}
				}
				if (p.first == p.second) {
					add_edge (*vi_start, *vj_start, og);
					v_prop_map_og[*vi_start] = get(v_prop_map_g, *vi_start);
					v_prop_map_og[*vj_start] = get(v_prop_map_g, *vj_start);
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
	,typename OutGraph
		>
bool Complement( const Graph1& g, OutGraph& og) {
	//Creating Vertex Property Map
	std::map<graph_traits<Graph1>::vertex_descriptor, std::string> v_names_g;
	boost::associative_property_map<std::map<graph_traits<Graph1>::vertex_descriptor, std::string> > v_prop_map_g(v_names_g);
	for (unsigned int i = 0; i < num_vertices(g); i++) {
		v_names_g.insert(make_pair(i, "G1V" + boost::lexical_cast<std::string>(i)));
	}
	
	std::map<graph_traits<OutGraph>::vertex_descriptor, std::string> v_names_og;
	associative_property_map<std::map<graph_traits<OutGraph>::vertex_descriptor, 
						std::string> > v_prop_map_og(v_names_og);
	
	return Complement(g, og, v_prop_map_g, v_prop_map_og); 
}
