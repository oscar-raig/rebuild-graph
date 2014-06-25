import networkx as nx
import sys
number_vertex=int(sys.argv[1])
print "Number of vertex of wheel graph" +  str(number_vertex)
pet = nx.wheel_graph(number_vertex)

diam= nx.diameter(pet)
print diam

info = nx.info(pet)
print info, "\n diam: ",nx.diameter(pet)
nx.write_adjlist(pet,"wheel"+ str(number_vertex) + ".txt")
