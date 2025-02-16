Реализация функции unsigned long long ackermann(unsigned long long m, unsigned long long n) вычисляющая значение функции Аккермана в точке m, n.

Функция определена следующим образом:

1. n + 1, если m = 0
2. ackermann(m - 1, 1), если m > 0, n = 0
3. ackermann(m - 1, ackermann(m, n - 1)), если m > 0, n > 0