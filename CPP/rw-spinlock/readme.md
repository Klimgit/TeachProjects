# Read-write spinlock

В этой задаче реализован spinlock, который поддерживает конкурентные чтения. При этом блокировка на запись работает как и с обычным spinlock, т.е. поддерживается эксклюзивное владение.


Идея алгоритма:

1. Создается атомарная переменная.
2. Внутри атомарной переменной первый младший бит выделяется для флага записи.
3. Остальные биты, начиная со второго, - это счетчик читателей.
4. Для `lockRead` должен соблюдаться инвариант: младший бит не должен быть равен 1.
5. Для `lockWrite` должен соблюдаться инвариант: младший бит должен быть равен 1 + счетчик читателей должен быть равен 0.
