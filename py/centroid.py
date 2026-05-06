import numpy as np
import matplotlib.pyplot as plt

def calcular_centroide(grid):
    """
    Calcula o centroide das células ativas (valor > 0) em uma malha 2D.
    """
    y_coords, x_coords = np.where(grid > 0)

    if len(x_coords) == 0:
        return None

    centroide_x = np.mean(x_coords)
    centroide_y = np.mean(y_coords)

    return (centroide_x, centroide_y), (x_coords, y_coords)

# Configuração da Malha e do Glider
malha = np.zeros((10, 10))
glider_coords = [(1, 2), (2, 3), (3, 1), (3, 2), (3, 3)]
for r, c in glider_coords:
    malha[r, c] = 1

# Processamento
resultado = calcular_centroide(malha)

if resultado:
    pos, (x_pts, y_pts) = resultado

    # Plotagem
    plt.figure(figsize=(6, 6))

    # Plota as células vivas (pontos azuis)
    plt.scatter(x_pts, y_pts, color='royalblue', s=100, label='Células Ativas')

    # Plota o centroide (X vermelho)
    plt.scatter(pos[0], pos[1], color='red', marker='x', s=200, linewidths=3, label='Centroide')

    # Configurações do gráfico
    plt.title("Localização do Centroide - Glider", fontsize=14)
    plt.xlabel("Coluna (x)")
    plt.ylabel("Linha (y)")
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.legend()

    # Ajusta os limites e inverte o eixo Y para parecer uma matriz
    plt.xlim(-0.5, 9.5)
    plt.ylim(-0.5, 9.5)
    plt.gca().invert_yaxis()

    plt.show()
    print(f"O centroide está em: X={pos[0]:.2f}, Y={pos[1]:.2f}")
else:
    print("Nenhuma célula ativa para plotar.")
