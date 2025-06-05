import matplotlib.pyplot as plt
from data import sizes_bytes, tempos

plt.figure(figsize=(10, 6))
plt.loglog(sizes_bytes, tempos, marker='o')
plt.title('Tempo vs Tamanho (Escala Log-Log)')
plt.xlabel('Tamanho da Mensagem (Bytes)')
plt.ylabel('Tempo (s)')
plt.grid(True, which="both", ls="--")
plt.tight_layout()
plt.show()
