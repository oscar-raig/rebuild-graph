from networkx import *

#G=krackhardt_kite_graph()
G=wheel_graph(5)
print G
print(G.edges(data=True))


#write_adjlist(G,"krackhardt_kite_grap")
write_adjlist(G,"wheel5.txt")


