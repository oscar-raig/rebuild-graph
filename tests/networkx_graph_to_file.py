from networkx import *

G=krackhardt_kite_graph()
print G
print(G.edges(data=True))


write_adjlist(G,"krackhardt_kite_grap")



