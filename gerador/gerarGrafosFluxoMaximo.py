import networkx as nx
import matplotlib.pyplot as plt
from random import randint
from networkx.readwrite import json_graph 
import json
import sys
from operator import itemgetter, attrgetter, methodcaller

#Execute script_name.py vertices arestas maximum_capacity
n = 9
a = 40
c = 20
if len(sys.argv)>=2:
    n = int(sys.argv[1])-1
if len(sys.argv)>=3:
    a = int(sys.argv[2])
if len(sys.argv)>=4:
    c = int(sys.argv[3])

def execute(n,a,cmax):
    G = nx.DiGraph()
    for i in range(a):
        G.add_edge(randint(0,n),randint(0,n), capacity=randint(0,cmax)) 
    data = json_graph.cytoscape_data(G)
    num_vertex = len(data["elements"]["nodes"])
    num_edges = len(data["elements"]["edges"])
    print("Vertices: ",num_vertex, " Arestas: ",num_edges)
    json_ = {}
    json_["nome"]="GRAFO_ALEATORIO_"+str(num_vertex)
    vertices=[]
    for i in range(num_vertex):
        vertices.append(str(data["elements"]["nodes"][i]["data"]['value']))
    ordered_vertices = sorted(vertices, key=lambda x:int(x))
    #print(vertices)
    #print(ordered_vertices)
    #ordered_edges = sorted(vertices, key=itemgetter(0))
    json_["vertices"]=ordered_vertices
    arestas=[]
    min_ = 1000000000
    max_ = -10000
    for i in range(num_edges):
        edge = []
        if data["elements"]["edges"][i]["data"]['source']<min_:
            min_ = data["elements"]["edges"][i]["data"]['source']
        if data["elements"]["edges"][i]["data"]['target']>max_:
            max_ = data["elements"]["edges"][i]["data"]['target']

        edge.append(str(data["elements"]["edges"][i]["data"]['source']))
        edge.append(str(data["elements"]["edges"][i]["data"]['target']))
        edge.append(str(data["elements"]["edges"][i]["data"]['capacity']))
        arestas.append(edge)
    ordered_arestas = sorted(arestas, key=lambda x:(int(x[0]),int(x[1])))
    json_["arestas"]=ordered_arestas
    print("min vertex:",min_)
    print("max vertex:", max_)
    flow_value, flow_dict = nx.maximum_flow(G, min_, max_)
    print("max_flow:",flow_value)
    with open('graph_'+str(num_vertex)+"_"+str(flow_value)+"_"+str(min_)+"_"+str(max_)+'.json', 'w') as json_file:  
        json.dump(json_, json_file)

execute(n,a,c)
