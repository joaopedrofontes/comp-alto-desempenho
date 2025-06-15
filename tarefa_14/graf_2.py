import matplotlib.pyplot as plt


sizes_2 = [
    8, 16, 32, 64, 128, 256, 512, 1024, 2048,
    4096, 8192, 16384, 32768, 65536,
    131072, 262144, 524288, 1048576,
    2097152, 4194304, 8388608, 16777216
]


times_2 = [
    0.03, 0.03, 0.04, 0.04, 0.04, 0.05, 0.05, 0.06, 0.07,
    0.09, 0.10, 0.15, 0.21, 0.33,
    0.59, 0.56, 0.99, 1.84,
    3.54, 6.94, 13.76, 27.39
]

sizes_kb_labels_2 = [f"{s//1024 if s >= 1024 else s} {'KB' if s >= 1024 else 'B'}" for s in sizes_2]

plt.figure(figsize=(12, 6))
plt.plot(sizes_kb_labels_2, times_2, marker='o', linestyle='-', color='green')
plt.xticks(rotation=45)
plt.xlabel("Tamanho da Mensagem")
plt.ylabel("Tempo de Execução")
plt.title("Tempo de Execução vs Tamanho da Mensagem (MPI) - Segunda Medição")
plt.grid(True)
plt.tight_layout()

#plt.savefig("grafico_2.png", dpi=300)

plt.show()
