import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Dados da tabela de eficiência
# Formato: cada linha representa uma quantidade de threads
data = [
    [1.00, 1.00, 1.00, 1.00, 1.00],     # 1 thread
    [0.98, 0.97, 0.97, 0.97, 0.98],     # 2 threads
    [0.96, 0.94, 0.94, 0.95, 0.94],     # 4 threads
    [0.92, 0.89, 0.90, 0.89, 0.90],     # 8 threads
    [0.79, 0.74, 0.74, 0.75, 0.75],     # 16 threads
    [0.63, 0.56, 0.50, 0.59, 0.62]      # 32 threads
]

# Tamanhos do problema e número de threads
problem_sizes = ['1x', '2x', '4x', '8x', '16x']
threads = [1, 2, 4, 8, 16, 32]

# Criar DataFrame
df = pd.DataFrame(data, index=threads, columns=problem_sizes)

# Plotar o heatmap
plt.figure(figsize=(8, 6))
sns.heatmap(df, annot=True, fmt=".2f", cmap="YlGnBu", cbar_kws={'label': 'Eficiência'})
plt.title("Eficiência paralela (Heatmap)")
plt.xlabel("Tamanho do Problema")
plt.ylabel("Número de Threads")
plt.tight_layout()
plt.show()
