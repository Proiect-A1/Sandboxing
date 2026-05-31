Se dă o permutare $A$ a numerelor de la $1$ la $N$. Operatorul $\oplus$ din limbajul C/C++ realizează operația XOR (disjuncție exclusivă pe biți).

| $\oplus$ | 0 | 1 |
|:---:|:---:|:---:|
| 0 | 0 | 1 |
| 1 | 1 | 0 |

# Cerință

Scrieți un program care să rezolve următoarele două cerințe:

1. Construiți o altă permutare $B$ astfel încât expresia $E = (A_1 + B_1) \oplus (A_2 + B_2) \oplus \ldots \oplus (A_N + B_N)$ să aibă valoare **minimă**.
2. Construiți o altă permutare $B$ astfel încât expresia $E = (A_1 + B_1) \oplus (A_2 + B_2) \oplus \ldots \oplus (A_N + B_N)$ să aibă valoare **maximă**.

# Date de intrare

Prima linie conține numărul $C$ care poate avea valoarea $1$ sau $2$ în funcție de cerința ce trebuie rezolvată. Pe a doua linie se va afla numărul $N$, iar pe a treia linie se vor afla $N$ numere distincte cuprinse între $1$ și $N$, reprezentând permutarea $A$.

# Date de ieșire

Prima linie va conține valoarea expresiei $E$ în funcție de cerință (pentru cerința 1 se va afișa minimul, iar pentru cerința 2 se va afișa maximul). Pe a doua linie se vor afișa $N$ numere distincte cuprinse între $1$ și $N$, separate prin câte un spațiu, reprezentând o permutare $B$ cu ajutorul căreia se obține valoarea $E$.

# Restricții și precizări

- $1 \leq N \leq 1\,000\,000$
- $1 \leq A_i \leq N$, $A_i \neq A_j\ \forall\ i \neq j$
- Dacă există mai multe șiruri $B$ care satisfac condițiile, puteți afișa oricare dintre ele.

| # | Punctaj | Restricții |
|:---:|:---:|:---|
| 1 | 16 | $C = 1,\ 1 \leq N \leq 10$ |
| 2 | 16 | $C = 2,\ 1 \leq N \leq 10$ |
| 3 | 23 | $C = 1$ |
| 4 | 45 | $C = 2$ |

# Exemplu

`stdin`
```
1
2
2 1
```

`stdout`
```
0
1 2
```

**Explicație:** $E = (2+1) \oplus (1+2) = 3 \oplus 3 = 0$. Aceasta este valoarea minimă ce se poate obține.

---

`stdin`
```
2
5
4 3 1 5 2
```

`stdout`
```
14
5 4 3 2 1
```

**Explicație:** $E = (4+5) \oplus (3+4) \oplus (1+3) \oplus (5+2) \oplus (2+1) = 9 \oplus 7 \oplus 4 \oplus 7 \oplus 3 = 14$. Se poate demonstra că nu se poate obține o valoare mai mare în acest caz.
