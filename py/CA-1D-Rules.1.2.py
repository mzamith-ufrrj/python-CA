import matplotlib.pyplot as plt
from matplotlib.colors import LinearSegmentedColormap
import numpy as np
import sys
ONE_D_LEN = 400
TIMESTEPS = 400
TRUE_CELL_POS = 200
BOUNDARIES_CONDITION = "periodic" #reflexive, constante, periodic
IS_RAND = False
PROB_P = 0.50
CONSTANT_VALUE = 0    


def apply_rule(matrix, rule):
   
    for t in range(1, TIMESTEPS):
        for x in range(0, ONE_D_LEN):
            x_l = -1
            x_r = -1
            x_c = matrix[t-1][x]
            if x > 0:
                x_l = matrix[t-1][x - 1]
            else:
                if BOUNDARIES_CONDITION == "reflexive":
                     x_l = matrix[t-1][x + 1]
                elif BOUNDARIES_CONDITION == 'constante':
                    x_l = CONSTANT_VALUE
                elif  BOUNDARIES_CONDITION == 'periodic':
                    x_l = matrix[t-1][ONE_D_LEN - 1]
                else:
                    print('Error: BoundaryCondition unknown in x == 0')
                    sys.exit(-1)

            if x < ONE_D_LEN - 1:
                x_r = matrix[t-1][x + 1]
            else:
                if BOUNDARIES_CONDITION == 'reflexive':
                    x_r = matrix[t-1][x - 1]
                elif BOUNDARIES_CONDITION == 'constante':
                    x_r = CONSTANT_VALUE
                elif  BOUNDARIES_CONDITION == 'periodic':
                    x_r = matrix[t-1][0]
                else:
                    print('Error: BoundaryCondition unknown in x == X')
                    sys.exit(-1)

            if x_l == -1 or x_c == -1 or x_r == -1:
                    txt = 'Error: Values are not defined  ({}, {}) --> ({}, {}, {}) '.format(t, x, x_l, x_c, x_r)
                    print(txt)
                    sys.exit(-1)
            matrix[t][x] =  rule(x_l, x_c, x_r)
            

def rule90(x_l, x_c, x_r):
    return (x_l and (not x_r)) or  ((not x_l) and x_r)

def plot_ca(matrix,  filename="", color_active=(0.0, 0.0, 1.0, 1.0)):
    """
    matrix: a matriz do AC
    color_active: Tupla RGBA para a cor da célula (ex: Verde Neon)
    """
    # Definimos o fundo como totalmente transparente (0.0 no Alpha)
    # E a célula ativa com a cor escolhida
    color_background = (0.0, 0.0, 0.0, 0.0) 
    
    cmap = LinearSegmentedColormap.from_list(
        'ca_alpha', [color_background, color_active], N=2
    )

    plt.figure(figsize=(10, 15))
    
    # O parâmetro 'interpolation=none' evita que o matplotlib tente 
    # suavizar os pixels, mantendo o aspecto de "pixel art" nítido
    plt.imshow(matrix, cmap=cmap, interpolation='none')
    
    plt.axis('off')

    # transparent=True garante que o fundo da figura também seja alpha
    if filename != "":
        plt.savefig(filename, transparent=True, bbox_inches='tight',
pad_inches=0)
    plt.show()

# No seu __main__, substitua o bloco de plotagem por:
# plot_ca(matrix, color_on='#00FF41') # Verde Matrix clássico
# plot_ca(matrix, color_on='#FF4500') # Laranja vibrante
if __name__ == "__main__":
    print("\n\t*** Autômato Celular elementar***")
    print(f"\t   - Domínio({ONE_D_LEN}, {TIMESTEPS})")
    matrix =  np.zeros((TIMESTEPS, ONE_D_LEN), dtype=np.int32)
    matrix[0][TRUE_CELL_POS] = 1

    apply_rule(matrix=matrix, rule=rule90 )
    plot_ca(matrix = matrix)
