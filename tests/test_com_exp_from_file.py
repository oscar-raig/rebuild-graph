
import networkx as nx
import numpy

import scipy.linalg
G=nx.read_adjlist("test.gpfc",delimiter=" ",nodetype=int)
sc = nx.communicability_centrality_exp(G)
# alternative implementation that calculates the matrix exponential
    
nodelist = G.nodes() # ordering of nodes in matrix
A = nx.to_numpy_matrix(G,nodelist)
# convert to 0-1 matrix
A[A!=0.0] = 1

print "A matrix"
print A
print

expA = scipy.linalg.expm(A)

print expA
print 

# convert diagonal to dictionary keyed by node
sc = dict(zip(nodelist,map(float,expA.diagonal())))
print  sc
