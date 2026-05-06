import pygame
import numpy as np
from random import seed
from random import randint
import math
import matplotlib.pyplot as plt
import pandas as pd


#condição de contorno circular
def estatistica(tempo:int, CA_matriz:np.ndarray, CA_Y:int, CA_X:int)-> np.array([]):
    centroid_soma =  np.zeros(4)
    tipo0 = 0
    tipo1 = 0
    tipo2 = 0

    for j in range(0, CA_Y):
        for i in range(0, CA_X):
            C = CA_matriz[j][i]
            if C == 0:
                tipo0 = tipo0 + 1
            if C == 1:
                tipo1 = tipo1 + 1
            if C == 2:
                tipo2 = tipo2 + 1
    print("\t Estatistica - quantos de cada estado:")
    print(f"\t\t Vazios:{tipo0} / Tipo 1:{tipo1} / Tipo 2:{tipo2}")
    #Grau de tolerancia ou intolerancia

    insati_1 = 0.0
    insati_2 = 0.0
    for j in range(0, CA_Y):
        for i in range(0, CA_X):
            QuemSouEu = CA_matriz[j][i]

            type1 = 0
            type2 = 0
            if QuemSouEu > 0:
                px = (int(QuemSouEu)-1) * 2 + 0
                py = (int(QuemSouEu)-1) * 2 + 1
                centroid_soma[px] = centroid_soma[px] + i
                centroid_soma[py] = centroid_soma[py] + j

                for jj in range(-1, 2):
                    for ii in range(-1,2):
                        jjj = j + jj
                        iii = i + ii
                        if not (jj == ii and ii == 0):

                            if jjj >= CA_Y:
                                jjj = jjj % CA_Y

                            if jjj < 0:
                                jjj = CA_Y + jjj

                            if iii >= CA_X:
                                iii = iii % CA_X

                            if iii < 0:
                                iii = CA_X + iii

                            C = CA_matriz[jjj][iii]
                            if C == 1:
                                type1 = type1 + 1

                            if C == 2:
                                type2 = type2 + 1

                if QuemSouEu == 1 and type2 >= 4:
                    insati_1 = insati_1 + (type2 / 8.0)


                elif QuemSouEu == 2 and type1 >= 4:
                    insati_2 = insati_2 + (type1 / 8.0)

    if tipo1 > 0:
        insati_1 = insati_1 / tipo1
    else:
        insati_1 = 0.0
    if tipo2 > 0:
        insati_2 = insati_2 / tipo2
    else:
        insati_2 = 0.0

    centroid_soma[0] = centroid_soma[0] / tipo1
    centroid_soma[1] = centroid_soma[1] / tipo1

    centroid_soma[2] = centroid_soma[2] / tipo2
    centroid_soma[3] = centroid_soma[3] / tipo2
    print(f"\t Grau de insatisfação:")
    print(f"\t\t Tipo 1:{insati_1:.4f} em média / centroid: ({centroid_soma[0]:.4f}. {centroid_soma[1]:.4f})")
    print(f"\t\t Tipo 2:{insati_2:.4f} em média  / centroid: ({centroid_soma[2]:.4f}. {centroid_soma[3]:.4f})")

    vetor = np.array([tempo, insati_1, insati_2])
    return vetor

def encontrar_espacos_vagos(CA_matriz:np.ndarray, CA_Y:int, CA_X:int) -> list:
    vazios = []
    for j in range(0, CA_Y):
        for i in range(0, CA_X):
            C = CA_matriz[j][i]
            if C == 0:
                points = {"i": i, "j": j}
                vazios.append(points)
    return vazios

def funcao_transicao_circular(CA_matriz:np.ndarray, CA_Y:int, CA_X:int) -> list:

    vazios = encontrar_espacos_vagos(CA_matriz, CA_Y, CA_X)
    rng = np.random.default_rng()
    proxima_posicao = []


    for j in range(0, CA_Y):
        for i in range(0, CA_X):
            type1 = 0
            type2 = 0
            QuemSouEu = CA_matriz[j][i]
            if QuemSouEu > 0:
                for jj in range(-1, 2):
                    for ii in range(-1,2):
                        jjj = j + jj
                        iii = i + ii
                        if not (jj == ii and ii == 0):

                            if jjj >= CA_Y:
                                jjj = jjj % CA_Y

                            if jjj < 0:
                                jjj = CA_Y + jjj

                            if iii >= CA_X:
                                iii = iii % CA_X

                            if iii < 0:
                                iii = CA_X + iii

                            C = CA_matriz[jjj][iii]
                            if C == 1:
                                type1 = type1 + 1

                            if C == 2:
                                type2 = type2 + 1

                if QuemSouEu == 1 and type2 >= 4:

                    n = len(vazios)
                    if n > 0:
                        p = rng.integers(0, n)
                        p_de = {"i": i, "j": j}
                        p_para = vazios[p]
                        p_movimento = {"QuemSouEu": QuemSouEu, "de": p_de, "para": p_para}
                        proxima_posicao.append(p_movimento)
                        del vazios[p]
                    else:

                        p_de = {"i": i, "j": j}
                        p_para = {"i": i, "j": j}
                        p_movimento = {"QuemSouEu": QuemSouEu, "de": p_de, "para": p_para}
                        proxima_posicao.append(p_movimento)

                elif QuemSouEu == 2 and type1 >= 4:

                    n = len(vazios)
                    if n > 0:
                        p = rng.integers(0, n)
                        p_de = {"i": i, "j": j}
                        p_para = vazios[p]
                        p_movimento = {"QuemSouEu": QuemSouEu, "de": p_de, "para": p_para}
                        proxima_posicao.append(p_movimento)
                        del vazios[p]
                    else:

                        p_de = {"i": i, "j": j}
                        p_para = {"i": i, "j": j}
                        p_movimento = {"QuemSouEu": QuemSouEu, "de": p_de, "para": p_para}
                        proxima_posicao.append(p_movimento)

                else:

                    p_de = {"i": i, "j": j}
                    p_para = {"i": i, "j": j}
                    p_movimento = {"QuemSouEu": QuemSouEu, "de": p_de, "para": p_para}
                    proxima_posicao.append(p_movimento)

    return proxima_posicao

def atualizacao(CA_matriz:np.ndarray, CA_Y:int, CA_X:int, proxima_posicao:list):

    CA_matriz.fill(0)
    for p_movimento in proxima_posicao:
        QuemSouEu = p_movimento["QuemSouEu"]
        p_de = p_movimento["de"]
        p_para = p_movimento["para"]
        i = p_para["i"]
        j = p_para["j"]
        CA_matriz[j][i] = QuemSouEu

def condicao_inicial(CA_Y:int, CA_X:int, prob:float)-> np.ndarray:
    CA_matriz =  np.zeros((CA_Y, CA_X))

    rng = np.random.default_rng()
    for j in range(0, CA_Y):
        for i in range(0, CA_X):

            value = rng.random()
            #p = j * CA_X + i + 1
            #if p % 2 == 0:
            #    CA_matriz_0[j][i] = 1
            if value < prob:
                CA_matriz[j][i] = 0
            else:
                value = rng.random()
                if value < 0.1:
                    CA_matriz[j][i] = 1
                else:
                    CA_matriz[j][i] = 2
            
    return CA_matriz
    
'''
Função principal - primeira a ser chamada
'''
if __name__ == '__main__':
    width  = 800
    height = 600
    FPS    = 60

    CA_X   = 100
    CA_Y   = 100
    max_time_step = 30
    prob = 0.10


    CA_matriz = condicao_inicial(CA_Y, CA_X, prob)

    pygame.init()
    gameDisplay = pygame.display.set_mode((width,height))
    pygame.display.set_caption('Autômato Celular')
    gameDisplay.fill((0, 0, 0))

    btn    = pygame.Rect(0, 0, 100, 30)
    rect1  = pygame.Rect(0, 30, 100, 100)
    clock  = pygame.time.Clock()

    running = True
    update  = False


    deltaX =  (width / CA_X)
    deltaY =  (height / CA_Y)
    tempo = 0

    convergencia = []
    while running:
        clock.tick(FPS)
        gameDisplay.fill((0, 0, 0))


        for j in range(0, CA_Y):
            for i in range(0, CA_X):
                s = CA_matriz[j][i]
                if s > 0:
                    x = i  * deltaX
                    y = j  * deltaY
                    if s == 1:
                        pygame.draw.rect(gameDisplay, (255, 0, 0), (x + 1, y + 1, deltaX - 2, deltaY - 2), 0)
                    if s == 2:
                        pygame.draw.rect(gameDisplay, (0, 255, 0), (x + 1, y + 1, deltaX - 2, deltaY - 2), 0)



        for e in pygame.event.get():
            if e.type == pygame.QUIT:
                running = False
            if e.type == pygame.KEYDOWN:
                if e.key == pygame.K_ESCAPE:
                    running = False

                if e.key == pygame.K_u:
                    update = not update
        if update:
            print("Update")
            tempo = tempo + 1
            proxima_posicao = funcao_transicao_circular(CA_matriz, CA_Y, CA_X)
            atualizacao(CA_matriz, CA_Y, CA_X, proxima_posicao)
            vetor = estatistica(tempo, CA_matriz, CA_Y, CA_X)
            convergencia.append(vetor)
            acc = math.fabs(vetor[1]) + math.fabs(vetor[2])
            print(acc)
            if acc < 1E-10:
                running = False
            #update = False
            #f tempo == 1000:
            #    update = False

                
        pygame.display.flip()
    pygame.quit()
    df = pd.DataFrame(convergencia, columns=['timestep', 'classe 1', 'classe 2'])

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

    ax1.scatter(df['timestep'], df['classe 1'], color='blue', marker='o')
    ax1.set_title(f'Modelo de Schelling - Condição inicial - probabilidade de espaços vazios {prob} - Insatisfação média')
    ax1.set_xlabel('Instante de tempo')
    ax1.set_ylabel('Classe 1')
    ax1.set_ylim(0, 1)
    ax1.grid(True)

    ax2.scatter(df['timestep'], df['classe 2'], color='green', marker='o') # Mudei a cor para diferenciar
    ax2.set_title(f'Modelo de Schelling - Condição inicial - probabilidade de espaços vazios {prob} - Insatisfação média')
    ax2.set_xlabel('Instante de tempo')
    ax2.set_ylabel('Classe 2')
    ax2.set_ylim(0, 1)
    ax2.grid(True)

    plt.tight_layout()
    #plt.savefig(plot_file_name, dpi=300)
    #print(f"\t - Arquivo salvo: {plot_file_name}")
    plt.show()
    print("End the game")

