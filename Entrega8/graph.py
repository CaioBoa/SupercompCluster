import matplotlib.pyplot as plt
import numpy as np

# Dados fornecidos
MochilaAleatoria = [(1, 399, 377, 0.004249), (2, 404, 239, 0.001077), (3, 522, 598, 0.001213)]
MochilaSub = [(1, 723, 380, 0.001468), (2, 1065, 239, 0.001198), (3, 918, 590, 0.0009067)]
MochilaHillClimb = [(1, 365, 373, 0.000096), (2, 302, 236, 0.001732), (3, 584, 598, 0.001506)]

# Função para gerar o gráfico de barras
def graficar_barras(MochilaAleatoria, MochilaSub, MochilaHillClimb):
    iterations = [1, 2, 3]
    
    # Extraindo os tempos de execução
    tiemposAleatoria = [MochilaAleatoria[i][1] for i in range(3)]
    tiemposSub = [MochilaSub[i][1] for i in range(3)]
    tiemposHillClimb = [MochilaHillClimb[i][1] for i in range(3)]

    # Configurando o gráfico de barras
    barWidth = 0.25
    r1 = np.arange(len(iterations))
    r2 = [x + barWidth for x in r1]
    r3 = [x + barWidth for x in r2]

    # Plotando as barras
    plt.bar(r1, tiemposAleatoria, color='b', width=barWidth, edgecolor='grey', label='Mochila Aleatoria')
    plt.bar(r2, tiemposSub, color='g', width=barWidth, edgecolor='grey', label='Mochila Sub')
    plt.bar(r3, tiemposHillClimb, color='r', width=barWidth, edgecolor='grey', label='Mochila Hill Climb')

    # Adicionando labels e título
    plt.xlabel('Entrada', fontweight='bold')
    plt.ylabel('Valor', fontweight='bold')
    plt.title('Comparação de Valor por Algoritmo')
    
    # Adicionando os ticks nos eixos
    plt.xticks([r + barWidth for r in range(len(iterations))], iterations)

    # Mostrando a legenda
    plt.legend()

    # Exibindo o gráfico
    plt.show()

# Chamando a função para exibir o gráfico de barras
graficar_barras(MochilaAleatoria, MochilaSub, MochilaHillClimb)
