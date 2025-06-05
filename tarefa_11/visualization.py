import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Lê os dados do arquivo binário
with open('velocidade.bin', 'rb') as f:
    NX = np.fromfile(f, dtype=np.int32, count=1)[0]
    data = np.fromfile(f, dtype=np.float64)
    
# Reformata os dados (cada frame tem NX pontos)
num_frames = len(data) // NX
velocidade = data.reshape((num_frames, NX))

# Cria a malha espacial
x = np.linspace(0, (NX-1)*0.1, NX)

# Configura a figura
fig, ax = plt.subplots(figsize=(10, 6))
line, = ax.plot(x, velocidade[0])
ax.set_xlim(0, x[-1])
ax.set_ylim(0.9, 1.2)
ax.set_xlabel('Posição')
ax.set_ylabel('Velocidade')
ax.set_title('Evolução Temporal da Velocidade do Fluido')
ax.grid(True)

# Função de atualização para a animação
def update(frame):
    line.set_ydata(velocidade[frame])
    ax.set_title(f'Evolução Temporal - Passo {frame*10}')
    return line,

# Cria a animação
ani = animation.FuncAnimation(
    fig, update, frames=num_frames, interval=50, blit=True)

plt.tight_layout()
plt.show()

# Para salvar a animação (opcional)
# ani.save('fluido_viscoso.mp4', writer='ffmpeg', fps=20)