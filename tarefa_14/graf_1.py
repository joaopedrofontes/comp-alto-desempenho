import matplotlib.pyplot as plt
from data import sizes_bytes, tempos


plt.figure(figsize=(10, 6))
plt.semilogx(sizes_bytes, tempos, marker='o')
plt.title('Tempo vs Tamanho (Eixo X Logarítmico)')
plt.xlabel('Tamanho da Mensagem (Bytes, escala log)')
plt.ylabel('Tempo (s)')
plt.grid(True, which="both", ls="--")
plt.tight_layout()
plt.show()
