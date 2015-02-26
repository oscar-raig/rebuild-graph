from networkx import *
import networkx as nx


G=krackhardt_kite_graph()
#G=wheel_graph(5)
print G
print(G.edges(data=True))


#write_adjlist(G,"krackhardt_kite_grap")
#write_adjlist(G,"wheel5.txt")

bc = nx.betweenness_centrality(G);

print bc

normalized= False
betweenness=dict.fromkeys(G,0.0) # b[v]=0 for v in G
for s in G:
    print "Working with s:",s,"----------"
    S=[]
    P={}
    for v in G:
        P[v]=[]
    sigma=dict.fromkeys(G,0)    # sigma[v]=0 for v in G
    D={}
    sigma[s]=1
    D[s]=0
    Q=[s]
    while Q:   # use BFS to find shortest paths
        v=Q.pop(0)
        S.append(v)
        for w in G[v]:
            if w not in D:
#                print "w not in D",w
                Q.append(w)
                D[w]=D[v]+1
            if D[w]==D[v]+1:   # this is a shortest path, count paths
#                print "We have found shortest path w Dw v Dv",w,D[w],v,D[v]
                sigma[w]=sigma[w]+sigma[v]
		print "sigma w w", w , " " , sigma[w]
                P[w].append(v) # predecessors

	
    delta=dict.fromkeys(G,0) 
    while S:
        w=S.pop()
        for v in P[w]:
#	    print "v ", v, "Is connected with w ",w 
            delta[v]=delta[v]+\
                      (float(sigma[v])/float(sigma[w]))*(1.0+delta[w])
	    print "New delta " , delta[v], " For node " ,v
        if w != s:
#	    print "Delta ", delta[w];
            betweenness[w]=betweenness[w]+delta[w]
	    print "Actualitzatn betweeness ", betweenness[w]," ", w                  
# normalize
if normalized:
        order=len(betweenness)
        if order <=2:
            print "no normalization b=0 for all nodes"
        scale=1.0/((order-1)*(order-2))
        for v in betweenness:
            betweenness[v] *= scale

print betweenness
