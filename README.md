# INF 102 — Funcionário com Base de Dados SQL (SQLite)

Trabalho da disciplina **INF 102 — Programação de Computadores II** (UNIVIÇOSA, 2026/I — prof. Anderson Lamas).

Reescrita do programa da classe `Funcionario` que gravava em arquivo texto (`funcionarios.txt`) para persistir os dados em uma **base de dados SQL (SQLite)**, mantendo a mesma funcionalidade do código original.

## Funcionalidade

Menu de console:

1. Cadastrar funcionário (`nome`, `salario`)
2. Imprimir funcionários
3. Buscar funcionário por nome
4. Sair

Os dados ficam na tabela `funcionario` do arquivo `funcionarios.db`, criado automaticamente na primeira execução.

## Arquivos

| Arquivo | Descrição |
|---------|-----------|
| `funcionarios_sqlite.cpp` | Código-fonte do programa |
| `sqlite3.c` / `sqlite3.h` | SQLite amalgamation 3.53.2 (biblioteca oficial — terceiros, [sqlite.org](https://www.sqlite.org)) |

> O executável (`*.exe`) e o banco gerado (`*.db`) não são versionados — ver `.gitignore`. Compile a partir do fonte.

## Arquitetura

Três camadas:

- `class Funcionario` — domínio (`nome`, `salario`).
- `class FuncionarioDAO` — persistência: toda a lógica SQL, com *prepared statements* (`sqlite3_prepare_v2` + `sqlite3_bind_*`).
- `main()` — menu de console.

Esquema da tabela:

```sql
CREATE TABLE IF NOT EXISTS funcionario (
    id      INTEGER PRIMARY KEY AUTOINCREMENT,
    nome    TEXT    NOT NULL,
    salario REAL    NOT NULL
);
```

## Como compilar

Com o `sqlite3.c` incluído (não precisa instalar lib externa, qualquer `g++`):

```bash
g++ funcionarios_sqlite.cpp sqlite3.c -o funcionarios.exe -O2
```

> No Windows, compilando a amalgamation com g++, compile `sqlite3.c` como C (`gcc -c sqlite3.c -o sqlite3.o`) e depois linke: `g++ funcionarios_sqlite.cpp sqlite3.o -o funcionarios.exe`.

Alternativa com a lib do sistema instalada (`-lsqlite3`):

```bash
g++ funcionarios_sqlite.cpp -o funcionarios.exe -lsqlite3
```

## Como rodar

```bash
./funcionarios.exe
```
