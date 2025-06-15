import matplotlib.pyplot as plt

sizes = [
    8, 16, 32, 64, 128, 256, 512, 1024, 2048,
    4096, 8192, 16384, 32768, 65536,
    131072, 262144, 524288, 1048576,
    2097152, 4194304, 8388608, 16777216
]

times = [
    0.34, 0.28, 0.29, 0.30, 0.31, 0.33, 0.38, 0.49, 1.19,
    1.99, 2.22, 2.62, 3.45, 1.54,
    3.82, 4.89, 7.03, 8.82,
    15.35, 26.81, 47.82, 96.01
]

sizes_kb_labels = [f"{s//1024 if s >= 1024 else s} {'KB' if s >= 1024 else 'B'}" for s in sizes]


plt.figure(figsize=(12, 6))
plt.plot(sizes_kb_labels, times, marker='o', linestyle='-', color='blue')
plt.xticks(rotation=45)
plt.xlabel("Tamanho da Mensagem")
plt.ylabel("Tempo de Execução")
plt.title("Tempo de Execução vs Tamanho da Mensagem (MPI)")
plt.grid(True)
plt.tight_layout()

#plt.savefig("grafico_1.png", dpi=300)

plt.show()
