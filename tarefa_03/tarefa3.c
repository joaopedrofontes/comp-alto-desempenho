/*
Tarefa 3:
Implemente um programa em C que
calcule uma aproximação de π usando
uma série matemática, variando o
número de iterações e medindo o tempo
de execução. Compare os valores
obtidos com o valor real de π e analise
como a acurácia melhora com mais
processamento. Reflita sobre como esse
comportamento se repete em aplicações
reais que demandam resultados cada vez
mais precisos, como simulações físicas e
inteligência artificial.
*/



#include <stdio.h>
#include <quadmath.h>
#include <math.h>
#include <time.h>


__float128 calcula_pi_leibniz(long int n) {
    __float128 pi = 0;
    int sinal = 1;

    for(long int i = 0; i < n; i++) {
        pi += sinal * (4.0Q / (2 * i + 1));
        sinal = -sinal;
    }

    return pi;
}


__float128 calcula_pi_gauss_legendre(int n) {
    __float128 a = 1.0Q;
    __float128 b = 1.0Q / sqrtq(2.0Q);
    __float128 t = 0.25Q;
    __float128 p = 1.0Q;
    for (int i = 0; i < n; i++) {
        __float128 a_n = (a + b) / 2.0Q;
        __float128 b_n = sqrtq(a * b);
        __float128 t_n = t - p * powq(a - a_n, 2);
        a = a_n;
        b = b_n;
        t = t_n;
        p *= 2.0Q;
    }
    __float128 pi = powq(a + b, 2) / (4.0Q * t);
    return pi;
}

int conta_digitos_corretos(__float128 pi_calculado) {
    char str_calculado[50], str_math_pi[50];
    
    quadmath_snprintf(str_calculado, sizeof(str_calculado), "%.34Qg", pi_calculado);
    quadmath_snprintf(str_math_pi, sizeof(str_math_pi), "%.34Qg", M_PIq);  
    
    int digitos_corretos = 0;
    
    for (int i = 2; str_math_pi[i] != '\0'; i++) {
        if (str_math_pi[i] == str_calculado[i]) {
            digitos_corretos++;
        } else {
            break;
        }
    }

    return digitos_corretos;
}


int main() {
    long int n_leibniz = 1000000000;
    int n_gauss_legendre = 10;

    //calcular tempo
    struct timespec inicio, fim;
    double tempo_decorrido;

    printf("==========Leibniz========\n");
    
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    __float128 pi_leibniz = calcula_pi_leibniz(n_leibniz);
    clock_gettime(CLOCK_MONOTONIC, &fim);

    char buffer[50];
    quadmath_snprintf(buffer, sizeof(buffer), "%.34Qg", pi_leibniz);
    printf("%s (pi leibniz)\n", buffer);

    char buffer_pi[50];
    quadmath_snprintf(buffer_pi, sizeof(buffer_pi), "%.34Qg", M_PIq);
    printf("%s (math.h pi)\n", buffer_pi);

    
    int corretos = conta_digitos_corretos(pi_leibniz);
    
    printf("Dígitos corretos: %d\n", corretos);
    printf("Quantidade de iterações: %ld\n", n_leibniz);

    tempo_decorrido = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Tempo de execução: %.9f segundos\n", tempo_decorrido);


    printf("\n=========Gauss-Legendre=========\n");

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    __float128 pi_gauss_legendre = calcula_pi_gauss_legendre(n_gauss_legendre);
    clock_gettime(CLOCK_MONOTONIC, &fim);

    quadmath_snprintf(buffer, sizeof(buffer), "%.34Qg", pi_gauss_legendre);
    printf("%s (pi gauss_legendre)\n", buffer);

    printf("%s (math.h pi)\n", buffer_pi);

    int corretos_gauss_legendre = conta_digitos_corretos(pi_gauss_legendre);
    
    printf("Digitos corretos: %d\n", corretos_gauss_legendre);
    printf("Quantidade de iterações: %d\n", n_gauss_legendre);

    tempo_decorrido = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Tempo de execução: %.9f segundos\n", tempo_decorrido);

    return 0;
}
