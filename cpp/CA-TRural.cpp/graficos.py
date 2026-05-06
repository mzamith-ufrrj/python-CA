#!/opt/anaconda/bin/python
import numpy as np
import matplotlib.pyplot as plt
import sys
import pandas as pd
def velocity_density(dt, lanes):
    F = []
    X = []
    Y = []
    for i in range(0, lanes):
        F.append('Faixa {}'.format(i))
        x = df['density_{}'.format(i)] * 133
        y = df['velocity_{}'.format(i)] * 1.5 * 3.6
        X.append(x)
        Y.append(y)
    
    fig, ax =  plt.subplots(figsize=(12, 8))

    #ax.plot(xcub1, ycub1, '-', label=r'$ax^3+bx^2+cx$', c="red")
    #ax.plot(xcub2, ycub2, '-', label=r'$ax^3+bx^2+cx+d$', c="green")
    for i in range(0, lanes):
        ax.plot(X[i], Y[i], '.', label=F[i])
    

    #label = r'$\frac{1}{{{{}}}x^2 + {{{}}}$'.format(XAdust1[0], XAdust1[1] )

    plt.legend()
    #print(plabel)
    plt.title('Velocidade x Densidade')

    ax.set_xlabel('Densidade (veículos/km)')
    ax.set_ylabel('Velocidade (km/h)')
    #plt.ylim([-0.1, 1.2])
    #plt.xlim([min(Xi)-10,  max(Xi)+10])
    plt.grid()
    #filename = 'inter-Runge-{:03}.eps'.format(points+1)
    #plt.savefig(filename)
    #print('Arquivo: ', filename, " salvo")
    plt.show()
def flow_velocity(dt, lanes):
    F = []
    X = []
    Y = []
    for i in range(0, lanes):
        F.append('Faixa {}'.format(i))
        x = df['flow_{}'.format(i)] / 5 * 3600
        y = df['velocity_{}'.format(i)] * 1.5 * 3.6
        X.append(x)
        Y.append(y)
    
    fig, ax =  plt.subplots(figsize=(12, 8))

    #ax.plot(xcub1, ycub1, '-', label=r'$ax^3+bx^2+cx$', c="red")
    #ax.plot(xcub2, ycub2, '-', label=r'$ax^3+bx^2+cx+d$', c="green")
    for i in range(0, lanes):
        ax.plot(X[i], Y[i], '.', label=F[i])
    

    #label = r'$\frac{1}{{{{}}}x^2 + {{{}}}$'.format(XAdust1[0], XAdust1[1] )

    plt.legend()
    #print(plabel)
    plt.title('Velocidade x Fluxo ')

    ax.set_xlabel('Fluxo (veículos/h)')
    ax.set_ylabel('Velocidade (km/h)')
    #plt.ylim([-0.1, 1.2])
    #plt.xlim([min(Xi)-10,  max(Xi)+10])
    plt.grid()
    #filename = 'inter-Runge-{:03}.eps'.format(points+1)
    #plt.savefig(filename)
    #print('Arquivo: ', filename, " salvo")
    plt.show()
def flow_density(dt, lanes):
    F = []
    X = []
    Y = []
    for i in range(0, lanes):
        F.append('Faixa {}'.format(i))
        x = df['density_{}'.format(i)] * 133
        y = df['flow_{}'.format(i)] / 5 * 3600
        X.append(x)
        Y.append(y)
    
    fig, ax =  plt.subplots(figsize=(12, 8))

    #ax.plot(xcub1, ycub1, '-', label=r'$ax^3+bx^2+cx$', c="red")
    #ax.plot(xcub2, ycub2, '-', label=r'$ax^3+bx^2+cx+d$', c="green")
    #ax.plot(X, Y, '.', label='Faixa 0', c="blue")
    for i in range(0, lanes):
        ax.plot(X[i], Y[i], '.', label=F[i])
    
    ax.axhline(y=2160, color='r', linestyle='-')
    #label = r'$\frac{1}{{{{}}}x^2 + {{{}}}$'.format(XAdust1[0], XAdust1[1] )

    plt.legend()
    #print(plabel)
    plt.title('Fluxo x Densidade')

    ax.set_xlabel('Densidade (veículos/km)')
    ax.set_ylabel('Fluxo (veículo/h)')
    #plt.ylim([-0.1, 1.2])
    #plt.xlim([min(Xi)-10,  max(Xi)+10])
    plt.grid()
    #filename = 'inter-Runge-{:03}.eps'.format(points+1)
    #plt.savefig(filename)
    #print('Arquivo: ', filename, " salvo")
    plt.show()
   
def help():
    
    print("\nUsage:")
    print("\t python graficos.py <tipo> <arquivo> <número de faixas>")
    print("\t\t tipo: 0 = fluxo x densidade") 
    print("\t\t       1 = fluxo x velocidade")
    print("\t\t       2 = densidade x velocidade")
    print("\t\t       3 = cluster de influência")
    print("\t\t arquivo: arquivo de log <ponto de observação / foto da pista / cluster")
    print("\t\t número de faixas: Número de faixas da simulação")
    
if __name__ == "__main__":
    # sys.argv[1]
    if (len(sys.argv) != 4):
        #print(len(sys.argv))
        help()
        sys.exit(0)
    type_g = int(sys.argv[1]) # 0 = flow x density / 1 = flow x velocity / 2 = density x velocity / 3 = cluster
    filename = sys.argv[2]
    lanes = int(sys.argv[3])
    columns = []
    columns.append('timestep')
    columns.append('samples')
    
    for i in range(0, lanes):
        columns.append('flow_{}'.format(i))
        columns.append('density_{}'.format(i))
        columns.append('velocity_{}'.format(i))
    columns.append('Az')
    
    
    
    df = pd.read_csv(filename, names=columns, delimiter=';', header=None)
    if type_g == 0:
        flow_density(df, lanes)
    elif type_g == 1:
        flow_velocity(df, lanes)
    elif type_g == 2:
        velocity_density(df, lanes)
    #else if type_g == 3:
    #    cluster(df, lanes)
    
    

'''
    X_p = df_p['density'] * 133
    Y_p = df_p['flow'] / 5 * 3600
    X_f = df_f['density'] * 133
    Y_f = df_f['flow'] / 5 * 3600
    fig, ax =  plt.subplots(figsize=(12, 8))

    #ax.plot(xcub1, ycub1, '-', label=r'$ax^3+bx^2+cx$', c="red")
    #ax.plot(xcub2, ycub2, '-', label=r'$ax^3+bx^2+cx+d$', c="green")
    ax.plot(X_p, Y_p, '.', label='Photo', c="green")
    ax.plot(X_f, Y_f, '.', label='Photo', c="red")
    ax.axhline(y=2160, color='r', linestyle='-')
    #label = r'$\frac{1}{{{{}}}x^2 + {{{}}}$'.format(XAdust1[0], XAdust1[1] )

    plt.legend()
    #print(plabel)
    plt.title('Densidade por fluxo')
    #plt.ylim([-0.1, 1.2])
    #plt.xlim([min(Xi)-10,  max(Xi)+10])
    plt.grid()
    #filename = 'inter-Runge-{:03}.eps'.format(points+1)
    #plt.savefig(filename)
    #print('Arquivo: ', filename, " salvo")
    plt.show()
'''
