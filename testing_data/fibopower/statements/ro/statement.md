Se consideră șirul numerelor Fibonacci $1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, \ldots$, definit prin $fib[1] = 1$, $fib[2] = 1$, $fib[i] = fib[i-1] + fib[i-2]$.

Spunem că un număr natural $x$ este **fibopower** dacă acesta se poate descompune în produs de trei numere Fibonacci distincte. De exemplu, numărul $48$ este fibopower deoarece $48 = 2 \cdot 3 \cdot 8$.

Se consideră șirul $A = (A_1, A_2, \ldots, A_n)$ cu $n$ elemente numere naturale nenule, respectiv un număr natural $k$ cuprins între $1$ și $n$. O **secvență** a șirului $A$ este formată din valori situate pe poziții consecutive: $A_i, A_{i+1}, \ldots, A_j$, unde $1 \le i \le j \le n$.

Pe șirul $A$ se fac $q$ interogări de tipul $x \ y$, cu semnificația: să se determine numărul secvențelor $A_i, A_{i+1}, \ldots, A_j$ cu $x \le i \le j \le y$ care conțin exact $k$ numere fibopower.

# Cerință

Fiind cunoscute $n$, $k$, $q$ și cele $n$ elemente ale șirului $A$, să se determine răspunsul pentru cele $q$ interogări date.

# Date de intrare

De la `stdin` se citesc:
- Pe prima linie: numerele naturale $n$, $k$ și $q$.
- Pe a doua linie: $n$ numere naturale nenule reprezentând elementele șirului $A$.
- Pe următoarele $q$ linii: câte o interogare sub forma a două numere naturale $x \ y$.

# Date de ieșire

La `stdout` se vor afișa $q$ linii; pe linia $i$ ($1 \le i \le q$) se scrie răspunsul la cea de-a $i$-a interogare.

# Restricții și precizări

- $1 \le n \le 10^5$
- $1 \le q \le 10^5$
- $1 \le k \le n$
- $1 \le A_i \le 10^9$, pentru $1 \le i \le n$
- $1 \le x \le y \le n$ pentru orice interogare

| Subtask | Punctaj | Restricții suplimentare |
|---------|---------|-------------------------|
| 1 | 26 | $1 \le n \le 1000$, $q = 1$, $x = 1$, $y = n$ |
| 2 | 27 | $1000 < n \le 10^5$, $q = 1$, $x = 1$, $y = n$ |
| 3 | 8 | $1 < n \le 100$, $1 < q \le 100$ |
| 4 | 9 | $100 < n, q \le 3500$ |
| 5 | 8 | $n = 10^5$, $k = 1$ |
| 6 | 22 | Fără restricții suplimentare |

# Exemplu

`stdin`
```
6 2 2
5 6 21 48 6 9
1 6
2 5
```

`stdout`
```
6
3
```

**Explicație:** $n = 6$, $k = 2$. În șir există 3 numere fibopower: $A_2 = 6 = 1 \cdot 2 \cdot 3$, $A_4 = 48 = 2 \cdot 3 \cdot 8$, $A_5 = 6 = 1 \cdot 2 \cdot 3$.

Pentru prima interogare $[1, 6]$, cele 6 secvențe cu exact 2 numere fibopower sunt: $[5,6,21,48]$, $[6,21,48]$, $[21,48,6]$, $[21,48,6,9]$, $[48,6]$, $[48,6,9]$.

Pentru a doua interogare $[2, 5]$, cele 3 secvențe cu exact 2 numere fibopower sunt: $[6,21,48]$, $[21,48,6]$, $[48,6]$.
