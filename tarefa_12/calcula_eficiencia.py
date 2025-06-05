def calcular_eficiencia(input_path, output_path):
    with open(input_path, 'r') as f:
        linhas = f.readlines()

    tempos = []
    for linha in linhas:
        partes = linha.strip().split('&')
        tempos.append([float(x.strip()) for x in partes])


    n_linhas = len(tempos)
    n_colunas = len(tempos[0]) - 1


    tempos_1_thread = tempos[0][1:]

    with open(output_path, 'w') as f:
        for linha in tempos:
            p = int(linha[0])
            linha_saida = f"{p}"
            for j in range(n_colunas):
                T1 = tempos_1_thread[j]
                Tp = linha[j + 1]
                eficiencia = T1 / (p * Tp)
                linha_saida += f" & {eficiencia:.2f}"
            linha_saida += " \\\\"
            f.write(linha_saida + "\n")

    print(f"Arquivo gerado: {output_path}")


if __name__ == "__main__":
    input_arquivo = "tempos.txt"
    output_arquivo = "eficiencia.txt"
    calcular_eficiencia(input_arquivo, output_arquivo)
