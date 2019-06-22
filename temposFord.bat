@echo off
echo MATRIZ
echo FordFulkerson
matriz.exe json/graph_6_capacity.json FordFulk 0 0 5
matriz.exe json/graph_8_capacity.json FordFulk 0 0 7
matriz.exe json/graph_30_flow_7_0_29.json FordFulk 0 0 29
matriz.exe json/graph_50_78_0_49.json FordFulk 0 0 49
matriz.exe json/graph_100_43_0_99.json FordFulk 0 0 99
matriz.exe json/graph_200_89_0_198.json FordFulk 0 0 198
matriz.exe json/graph_500_27_0_499.json FordFulk 0 0 499
matriz.exe json/graph_1000_90_0_999.json FordFulk 0 0 999

echo LISTA
echo FordFulkerson
lista.exe json/graph_6_capacity.json FordFulk 0 0 5
lista.exe json/graph_8_capacity.json FordFulk 0 0 7
lista.exe json/graph_30_flow_7_0_29.json FordFulk 0 0 29
lista.exe json/graph_50_78_0_49.json FordFulk 0 0 49
lista.exe json/graph_100_43_0_99.json FordFulk 0 0 99
lista.exe json/graph_200_89_0_198.json FordFulk 0 0 198
lista.exe json/graph_500_27_0_499.json FordFulk 0 0 499
lista.exe json/graph_1000_90_0_999.json FordFulk 0 0 999