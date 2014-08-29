
import networkx as nx
import numpy

import scipy.linalg

def _rescale(betweenness,n,normalized,directed=False,k=None):
    if normalized is True:
        if n <=2:
            scale=None  # no normalization b=0 for all nodes
        else:
            scale=1.0/((n-1)*(n-2))
    else: # rescale by 2 for undirected graphs
        if not directed:
            scale=1.0/2.0
        else:
            scale=None
    if scale is not None:
        if k is not None:
            scale=scale*n/k
        for v in betweenness:
            betweenness[v] *= scale
    return betweenness


def _accumulate_basic(betweenness,S,P,sigma,s):
    delta=dict.fromkeys(S,0)
    while S:
        w=S.pop()
        coeff=(1.0+delta[w])/sigma[w]
        for v in P[w]:
            delta[v] += sigma[v]*coeff
        if w != s:
            betweenness[w]+=delta[w]
    return betweenness



def _single_source_shortest_path_basic(G,s):
    S=[]
    P={}
    for v in G:
        P[v]=[]
    sigma=dict.fromkeys(G,0.0)    # sigma[v]=0 for v in G
    D={}
    sigma[s]=1.0
    D[s]=0
    Q=[s]
    while Q:   # use BFS to find shortest paths
        v=Q.pop(0)
        S.append(v)
        Dv=D[v]
        sigmav=sigma[v]
        for w in G[v]:
            if w not in D:
                Q.append(w)
                D[w]=Dv+1
            if D[w]==Dv+1:   # this is a shortest path, count paths
                sigma[w] += sigmav
                P[w].append(v) # predecessors 
    return S,P,sigma






G=nx.read_adjlist("test_4nodes.gpfc",delimiter=" ",nodetype=int)
#G=nx.read_adjlist("wheel14.txt",delimiter=" ",nodetype=int)
sc= nx.betweenness_centrality(G,None,True)
print "Betweenness Centrality"
print  sc
exit(1)
k=None
normalized=True
weight=None
endpoints=False 
seed=None

betweenness=dict.fromkeys(G,0.0) # b[v]=0 for v in G
if k is None:
       nodes = G
else:
        random.seed(seed)
        nodes = random.sample(G.nodes(), k)
for s in nodes:
        # single source shortest paths
        if weight is None:  # use BFS
            S,P,sigma=_single_source_shortest_path_basic(G,s)
        else:  # use Dijkstra's algorithm
            S,P,sigma=_single_source_dijkstra_path_basic(G,s,weight)

	print "S: ",S
	print "P: ", P
	print "sigma : " , sigma
        # accumulation
        if endpoints:
            betweenness=_accumulate_endpoints(betweenness,S,P,sigma,s)
        else:
            betweenness=_accumulate_basic(betweenness,S,P,sigma,s)
    # rescaling

betweenness=_rescale(betweenness, len(G),
                         normalized=normalized,
                         directed=G.is_directed(),
                         k=k)

print betweenness

