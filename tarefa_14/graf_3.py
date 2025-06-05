import matplotlib.pyplot as plt
import numpy as np
from data import sizes_bytes, tempos

sizes_mb = np.array(sizes_bytes) / (1024 * 1024)
tempos_np = np.array(tempos)
throughput = (sizes_mb * 10000) / tempos_npd

plt.figure(figsize=(10, 6))
plt.plot(sizes_mb, throughput, marker='o')
plt.title('Taxa de Transferência vs Tamanho da Mensagem')
plt.xlabel('Tamanho da Mensagem (MB)')
plt.ylabel('Throughput (MB/s)')
plt.grid(True)
plt.tight_layout()
plt.show()
