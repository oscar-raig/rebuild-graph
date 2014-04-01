import networkx as nx
import sys
#print ( "Creating Graph" )
#G=nx.read_gml("test.gml")
G=nx.read_gml(sys.argv[1])


for j in G:
	print j,
	# Grau del node
        kj=G.degree(j)
        # Conjunt de de veins
        v=G.neighbors(j)
        sumkm=0
        for m in v:
		if m > j:
			print m,
	print
