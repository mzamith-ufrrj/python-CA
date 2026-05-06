import pygame
import numpy as np
from random import seed
from random import randint
import math
import matplotlib.pyplot as plt



#condição de contorno circular
def funcao_transicao_circular(CA_matriz_0, CA_matriz_1, CA_Y, CA_X):
   '''
   for j in range(0, CA_Y):
        for i in range(0, CA_X):
            N = 0
            S = 0
            L = 0
            O = 0
            NO = 0
            NE = 0
            SO = 0
            SE = 0
            C = CA_matriz_0[j][i]
            
            im1 = i - 1
            iM1 = i + 1
            
            jm1 = j - 1
            jM1 = j + 1
            
            if im1 < 0:
                im1 = CA_X - 1

            if iM1 == CA_X:
                iM1 = 0

            if jm1 < 0:
                jm1 = CA_Y - 1

            if jM1 == CA_Y:
                jM1 = 0

            N = CA_matriz_0[jm1][i]
            NO = CA_matriz_0[jm1][im1]
            NE = CA_matriz_0[jm1][iM1]
            

            S = CA_matriz_0[jM1][i]
            SO = CA_matriz_0[jM1][im1]
            SE = CA_matriz_0[jM1][iM1]

            O = CA_matriz_0[j][im1]
            L = CA_matriz_0[j][iM1]
        
            
            soma = N + NO + NE + S + SO + SE + O + L                
            #regra
            if C == 0 and soma == 3:
                CA_matriz_1[j][i] = 1
            elif C == 1 and soma >= 2 and soma <= 3:
                CA_matriz_1[j][i] = 1
            else:
                CA_matriz_1[j][i] = 0

    '''

def condicao_inicial(CA_Y, CA_X, prob):
    CA_matriz_0 =  np.zeros((CA_Y, CA_X))
    CA_matriz_1 =  np.zeros((CA_Y, CA_X))
    rng = np.random.default_rng()
    for j in range(0, CA_Y):
        for i in range(0, CA_X):

            value = rng.random()
            #p = j * CA_X + i + 1
            #if p % 2 == 0:
            #    CA_matriz_0[j][i] = 1
            if value < prob:
                CA_matriz_0[j][i] = 0
            else:
                value = rng.random()
                if value < 0.5:
                    CA_matriz_0[j][i] = 1
                else:
                    CA_matriz_0[j][i] = 2
            
    return CA_matriz_0, CA_matriz_1
    
'''
Função principal - primeira a ser chamada
'''
if __name__ == '__main__':
    width  = 800
    height = 600
    FPS    = 60

    CA_X   = 8
    CA_Y   = 8
    max_time_step = 30
    prob = 0.30


    CA_matriz_0, CA_matriz_1 = condicao_inicial(CA_Y, CA_X, prob)

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

    while running:
        clock.tick(FPS)
        gameDisplay.fill((0, 0, 0))


        for j in range(0, CA_Y):
            for i in range(0, CA_X):
                s = CA_matriz_0[j][i]
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
            update = False
            funcao_transicao_circular(CA_matriz_0, CA_matriz_1, CA_Y, CA_X)

            aux = CA_matriz_1
            CA_matriz_1 = CA_matriz_0
            CA_matriz_0 = aux





                
        pygame.display.flip()


