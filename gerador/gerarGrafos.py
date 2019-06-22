import sys
import networkx as nx 
import matplotlib.pyplot as plt
from networkx.readwrite import json_graph 
import json

p = 0.1
if len(sys.argv)>=2:
    p = float(sys.argv[1])

print("Probabilidade de conexão: ",p)


def execute(n,p):
    G= nx.erdos_renyi_graph(n,p) 
    data = json_graph.cytoscape_data(G)
    num_vertex = len(data["elements"]["nodes"])
    num_edges = len(data["elements"]["edges"])
    print("Vertices: ",num_vertex, " Arestas: ",num_edges)
    json_ = {}
    json_["nome"]="GRAFO_ALEATORIO_"+str(num_vertex)
    vertices=[]
    for i in range(num_vertex):
        vertices.append(str(data["elements"]["nodes"][i]["data"]['value']))
    json_["vertices"]=vertices
    arestas=[]
    for i in range(num_edges):
        edge = []
        edge.append(str(data["elements"]["edges"][i]["data"]['source']))
        edge.append(str(data["elements"]["edges"][i]["data"]['target']))
        arestas.append(edge)
    json_["arestas"]=arestas
    with open('graph_'+str(num_vertex)+'.json', 'w') as json_file:  
        json.dump(json_, json_file)


execute(5,p)
execute(6,p)
execute(7,p)
execute(8,p)
execute(9,p)
execute(10,p)
execute(20,p)
execute(50,p)
execute(100,p)
execute(200,p)
execute(500,p)
execute(1000,p)


