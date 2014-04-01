import networkx as nx
import numpy

import scipy.linalg

G = nx.Graph([(0,1),(1,2),(1,5),(5,4),(2,4),(2,3),(4,3),(3,6)])
nodelist = G.nodes() # ordering of nodes in matrix

A = nx.to_numpy_matrix(G,nodelist)

# convert to 0-1 matrix

A[A!=0.0] = 1

w,vec = numpy.linalg.eigh(A)

print w
print

expw = numpy.exp(w)

print "Exponential"
print expw
print

mapping = dict(zip(nodelist,range(len(nodelist))))

print "len node list"
print len(nodelist)

print
print "range len node list"
print range(len(nodelist))
print
print "nodelist"
print
print zip(nodelist,range(len(nodelist)))
print
print "dict"
print mapping


print "vect"
print vec
print


sc={}

# computing communicabilities
for u in G:

	sc[u]={}

	for v in G:

	    s = 0

	    p = mapping[u]

	    q = mapping[v]

	    for j in range(len(nodelist)):

		s += vec[:,j][p,0]*vec[:,j][q,0]*expw[j]

	    sc[u][v] = float(s)

print sc
