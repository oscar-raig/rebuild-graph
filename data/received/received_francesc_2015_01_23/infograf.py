# -*- coding: utf-8 -*-
# 
# Francesc Comellas 
#

import networkx as nx

print 'comencem llegint el graf'
print

##G=nx.read_edgelist("newman2-adj.txt",nodetype=int,delimiter=',')


G=nx.read_adjlist("swws-inipy.txt",nodetype=int)


##G=nx.barabasi_albert_graph(1000,3)
print 'Comprovem que si es dirigit o no: is_directed(G) '
print  nx.is_directed(G)

# calculem algunes propietats elementals

print 'calculem algunes propietats generals'
print
print nx.info(G)
print
print 'Comprovem es connex amb: is_connected(G) '
print  nx.is_connected(G)
print 'distribucio de grauss: '
print  
print  nx.degree_histogram(G)
print
print 'grau maxim: '
print
print len(nx.degree_histogram(G))-1
print
print 'grau mig calculat programant-ho'
print 'l ordre del graf es : '
print  G.order()
print
print 'la mida (nombre de branques): '
print  G.size()
print
print 'el diametre del graf es: '
print  nx.diameter(G)
print
print 'distribucio de grauss: '
print nx.degree_histogram(G)
## print ' els graus per node '
## print nx.degree(G)
## print ' els valors dels graus '
## print nx.degree(G).values()
print
print
print '============================='
print 'la  betweenness centrality: '
print  nx.betweenness_centrality(G)
print
print '============================='

print 'la communicability  centrality: '
print  nx.communicability_centrality(G)
print
print '============================='
print 'la communicability betweenness centrality: '
print  nx.communicability_betweenness_centrality(G)
print
print '============================='
print 'Calculem ara el grau mig'
print

degs=nx.degree(G).values()

avgdegG=0
for i in range(0,G.order()):
     avgdegG = avgdegG+degs[i]


print 'el grau mig: '
print  (avgdegG*1.0)/G.order()

print
print 'La distancia mitjana cal programar-la a partir de funcions'
print 'existents a NetowrkX. En concret la shorstest path length '
print 'que dona la llargada de camins curts entre parelles de vertexs'
print '( triga  uns   en un PC a 2.4 GHz )'

# ########################################
# Calcul   AVERAGE DISTANCE

sumavgdst=0
maxavgdst=0
minavgdst=9999
Gnodes=G.nodes()
for i in range(0,G.order()-1):
    for j in range(i+1,G.order()): 
        distij=nx.shortest_path_length(G,Gnodes[i],Gnodes[j])
        if distij>maxavgdst:
            maxavgdst=distij
        if distij<minavgdst:
            minavgdst=distij
        sumavgdst=sumavgdst+distij
print
print 'Avg Dist = %-6.4f' % (2*sumavgdst*1.0/(G.order()*(G.order()-1)))
print
print '(%d' % minavgdst,'- %d'% maxavgdst,')'
print '---------------------------------'
print

print 'calculem algunes propietats generals'
print
print nx.info(G)



print 'El clustering coeficient promig esta implementat '
print 'pero tambe el podem calcular com es veu aqui'


# ##################################################################################################
#  CLUSTERING COEFFICIENT 

ClustC=nx.clustering(G).values()

sumClust=0
maxCC=0
minCC=9

for i in range(1,G.order()):
      sumClust = sumClust + ClustC[i]
      if  ClustC[i]>maxCC:
            maxCC= ClustC[i]
      if  ClustC[i]<minCC:
            minCC=ClustC[i]

print
print 'Avg Clust Coeff. NetworkX =  %-6.4f ' % (nx.average_clustering(G))
print
print 'Avg Clust Coeff. Calculat =  %-6.4f ' %  (sumClust/G.order())
print
print '(minClust, maxClust)= (%-6.4f' % minCC,'-  %-6.6f'% maxCC,')'
print
print '**********************************'
print
print

# ##################################################################################################
#  CORRELATION COEFFICIENT 
import math


def mean(alist):
    mean = sum(alist)*1.0/len(alist)
    return mean

def standardDev(alist):
    theMean = mean(alist)
    
    sum = 0
    for item in alist:
        difference = item - theMean
        diffsq = difference ** 2
        sum = sum + diffsq
        
    sdev = math.sqrt(sum/(len(alist)-1))
    return sdev

def correlation(xlist, ylist):
    xbar = mean(xlist)
    ybar = mean(ylist)
    xstd = standardDev(xlist)
    ystd = standardDev(ylist)
    num = 0.0
    for i in range(len(xlist)):
        num = num + (xlist[i]-xbar) * (ylist[i]-ybar)
    corr = num / ((len(xlist)-1) * xstd * ystd) 
    return corr

print
print 'el fitxer ha d acabar amb retorn de carro'
print 'sino < perd > un vertex '
print

Gedges=G.edges()
xlist=[]
ylist=[]
for i in range(0,G.size()):
      xlist.append(G.degree(Gedges[i][0]))
      xlist.append(G.degree(Gedges[i][1]))
      ylist.append(G.degree(Gedges[i][1]))
      ylist.append(G.degree(Gedges[i][0]))
#      print G.degree(Gedges[i][0]),'<->',G.degree(Gedges[i][1])

print
print 'corr real      M3 = %-6.4f' % correlation(xlist,ylist)


print ' ============================================================= '
print ' ============================================================= '
print
print ' I finalment dibuix !! - '
try:
    import matplotlib.pyplot as plt
    import matplotlib
except:
    raise

sequ=nx.degree(G).values()

deg_seq=sorted(sequ,reverse=True) # degree/BC/AvgDst sequence




##plt.loglog(deg_seq,'b-',marker='o')
##plt.title("Grau - Rank")
##plt.ylabel("grau")
##plt.xlabel("rank")


##plt.axes([0.45,0.45,0.45,0.45])


pos=nx.spring_layout(G,scale=200,iterations=100)
plt.axis('off')
nx.draw_networkx_nodes(G,pos,node_size=200)
nx.draw_networkx_labels(G, pos)
nx.draw_networkx_edges(G,pos,alpha=0.8)


plt.savefig("graus.png")
plt.show()





