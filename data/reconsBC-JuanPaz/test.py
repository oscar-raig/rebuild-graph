import networkx as nx

pet = nx.petersen_graph()

diam= nx.diameter(pet)
print diam

info = nx.info(pet)
print info
nx.write_adjlist(pet,"pet.txt")
