
import networkx as nx
import numpy

import scipy.linalg
G=nx.read_adjlist("test_4nodes.gpfc",delimiter=" ",nodetype=int)
sc = nx.betweenness_centrality(G,None,False)
print "Betweenness Centrality"
print  sc
print "Shortes ptath"
sp = nx.shortest_path(G)
print sp

