import networkx as nx
number_vertex=10
pet = nx.wheel_graph(number_vertex)

diam= nx.diameter(pet)
print diam

info = nx.info(pet)
print info, "\n diam: ",nx.diameter(pet)
nx.write_adjlist(pet,"wheel"+ str(number_vertex) + ".txt")
