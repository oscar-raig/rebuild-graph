
import networkx as nx
import numpy
import sys
import scipy.linalg


def _rescale(sc,normalized):
    # helper to rescale betweenness centrality
    if normalized is True:
        order=len(sc)
        if order <=2:
            scale=None
        else:
            scale=1.0/((order-1.0)**2-(order-1.0))
    if scale is not None:
        for v in sc:
            sc[v] *= scale
    return sc


G=nx.read_adjlist("test_4nodes.gpfc",delimiter=" ",nodetype=int)
sc = nx.communicability_betweenness_centrality(G)
print "Communicability Betweenness Centrality"
print sc
sys.exit()

normalized = True
nodelist = G.nodes() # ordering of nodes in matrix
n = len(nodelist)
A = nx.to_numpy_matrix(G,nodelist)
# convert to 0-1 matrix
A[A!=0.0] = 1
print A
expA = scipy.linalg.expm(A)
print expA
mapping = dict(zip(nodelist,range(n)))
print mapping
sc = {}
for v in G:
# remove row and col of node v
        i = mapping[v]
        row = A[i,:].copy()
        col = A[:,i].copy()
	print "A",i
	print A
        A[i,:] = 0
        A[:,i] = 0
	print "A less",i
	print A
        B = (expA - scipy.linalg.expm(A)) / expA
   
	print B
	# sum with row/col of node v and diag set to zero
        B[i,:] = 0
        B[:,i] = 0
        B -= scipy.diag(scipy.diag(B))
	print "B sense diagonal"
	print B
        sc[v] = float(B.sum())
	print "sc"
        print sc
	# put row and col back
        A[i,:] = row
        A[:,i] = col
    # rescaling
sc = _rescale(sc,normalized=normalized)


print sc
