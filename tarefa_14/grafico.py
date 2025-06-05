import matplotlib.pyplot as plt
import numpy as np
from data import sizes_bytes, tempos

sizes_kb = np.array(sizes_bytes) / 1024

plt.figure(figsize=(10, 6))
plt.plot(sizes_kb, tempos, marker='o')
plt.title('Tempo de Troca de 10000 Mensagens vs Tamanho da Mensagem (Escala Linear)')
plt.xlabel('Tamanho da Mensagem (KB)')
plt.ylabel('Tempo (segundos)')
plt.grid(True)
plt.tight_layout()
plt.show()
