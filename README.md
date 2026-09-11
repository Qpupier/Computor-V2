# computor-v2

`computor-v2` is an interactive C++ command-line mathematics interpreter. It evaluates expressions, retains variables and user-defined functions for the current session, uses exact rational arithmetic, and solves polynomial equations through degree two.

Developed as an academic project at 42 Lyon.

## Features

- Exact rational arithmetic, followed by a decimal approximation when useful.
- Complex numbers, vectors, matrices, polynomials, and single-argument functions.
- Type-inferred variables that can be reassigned to another type.
- Equation solving for constant, linear and quadratic polynomials, including complex roots.
- Built-in constants and functions, session history, deletion commands, and ASCII polynomial plots.
- Automated tests, each also checked by Valgrind.

## Requirements

- A C++11-or-newer compiler (`clang++` is the Makefile default).
- GNU Make.
- GNU Readline development files (`libreadline-dev` on Debian/Ubuntu).
- Valgrind, for `make test` and `make valgrind`.

On Debian/Ubuntu:

```sh
sudo apt-get install clang make libreadline-dev valgrind
```

The supplied `Dockerfile` provides Ubuntu 24.04 with the toolchain and dependencies:

```sh
docker build -t computor-v2 .
docker run --rm -it -v "$PWD":/workspace -w /workspace computor-v2 bash
make
```

## Build and run

```sh
make run
```

or

```sh
make
./computor-v2
```

The executable accepts no arguments. In a terminal it uses Readline and displays `> `. It also accepts piped or redirected input:

```sh
printf '2 + 2\nquit\n' | ./computor-v2
./computor-v2 < commands.txt
```

## Quick start

```text
> 2 + 4 * 2
10
> z = 3 + 2i
z = 3 + 2i
> A = [[1, 2]; [3, 4]]
A = [ 1 , 2 ]
[ 3 , 4 ]
> f(x) = x^2 - 1
f(χ) = χ^2 - 1
> f(3)
8
> x^2 - 5x + 6 = 0 ?
x^2 - 5x + 6
Δ = 1
Two solutions in ℝ:
S = {2, 3} ∈ ℝ
x1 = 2
x2 = 3
```

Actual output contains terminal colours. Polynomial values and stored functions also produce an ASCII plot; it is omitted above for readability.

## Expression language

Whitespace is accepted around operators. Adjacent valid operands can imply multiplication: `2x`, `2(3 + 4)`, `2i`, and two adjacent matrices. Do not add whitespace inside an implicit product: use `2i`, not `2 i`.

### Values

| Kind | Syntax | Example |
| --- | --- | --- |
| Rational number | integer or decimal literal | `42`, `-4.25`, `2 / 3` |
| Complex number | imaginary unit `i` | `3 - 2i`, `(1 + i)^2` |
| Vector | comma-separated values | `[1, 2, 3]` |
| Matrix | rows separated by `;` | `[[1, 2]; [3, 4]]` |
| Polynomial | symbolic identifier | `x^2 - 3x + 2` |

All matrix rows must have equal length. Matrix and vector elements may be expressions. Results are simplified exactly where possible; non-terminating rational values additionally show an approximation.

### Operators

| Operator | Meaning | Example |
| --- | --- | --- |
| `+`, `-` | addition and subtraction | `a - b` |
| `*`, `/`, `%` | multiplication, division, modulo | `7 % 3` |
| `^` | exponentiation with a natural-number exponent | `2^10` |
| `~` | inverse of a square, non-singular matrix | `~[[1, 2]; [3, 4]]` |
| `!` | factorial of a natural number | `5!` |
| `**` | matrix product | `A ** B` |

For matrices, `*` is element-wise multiplication, whereas `**` is matrix multiplication. Writing two matrices next to each other also denotes matrix multiplication. Parentheses control precedence; `|x|` denotes an absolute value and `||x||` a norm.

### Built-ins

`pi` and `e` are predefined constants.

| Function | Meaning | Alternative notation |
| --- | --- | --- |
| `sqrt(x)` | square root | — |
| `abs(x)` | absolute value | `|x|` |
| `norm(x)` | norm | `||x||` |
| `cos(x)`, `sin(x)`, `tan(x)` | trigonometric functions in radians | — |
| `exp(x)` | exponential | — |
| `rad(x)` | degrees to radians | — |
| `deg(x)` | radians to degrees | — |
| `fact(x)` | factorial | `x!` |

Functions validate their input type: factorial requires a natural number, for example, and a matrix inverse requires a square matrix with a non-zero determinant.

## Variables, functions, and equations

Names are case-insensitive when resolved. A name is either one alphabetic character (except `i`) or at least two alphabetic characters. Built-in names cannot be redefined.

### Assignment and reassignment

```text
> total = 2 + 4 * 2
total = 10
> total = [1, 2, 3]
total = [1, 2, 3]
> copy = total
copy = [1, 2, 3]
```

The type is inferred from the assigned expression. `last` holds a copy of the most recently successful expression, assignment, or equation result.

### User-defined functions

Define a one-variable polynomial function, then call it with an expression:

```text
> f(x) = 2*x^2 - 3*x + 1
f(χ) = 2 * χ^2 - 3 * χ + 1
> f(4)
21
```

The right-hand side must reduce to a polynomial with supported coefficients, and its parameter must match the parameter declared on the left. Defining a function displays an ASCII graph of the polynomial.

### Equations

`=` compares two expressions. Both sides are reduced to a polynomial, then the interpreter handles:

- Constant equations (`True` or `False`).
- Linear equations (the solution is assigned to the unknown).
- Quadratic equations, with real or complex roots as appropriate.

Appending `?` explicitly requests solving or evaluation:

```text
> 2*x - 7 = 0 ?
x = 7/2
x = 3.5
> x^2 + 1 = 0 ?
Two solutions in ℂ:
S = {-i, i} ∈ ℂ
```

## Session commands

| Command | Action |
| --- | --- |
| `?` | list built-ins, variables, and user-defined functions |
| `history` | show the complete command history and results |
| `delete <name>` | remove a user-defined variable or function |
| `delete all` | remove every user-defined variable and function |
| `quit` | exit cleanly |

`Ctrl-D` also exits in an interactive terminal. Readline provides standard editing and history navigation.

## Tests and development commands

| Command | Purpose |
| --- | --- |
| `make` | compile `computor-v2` |
| `make run` | build if needed, then start the interpreter |
| `make test` | run all test groups; each case is checked with Valgrind |
| `make test_debug` | run the suite with diagnostic output on failure |
| `make valgrind` | run the interpreter under detailed Valgrind checking |
| `make clean` | remove object and dependency files |
| `make fclean` | remove objects, dependencies, and the executable |
| `make re` | rebuild from scratch |
| `make size` | report file and line counts after cleaning |

The shell-based tests in `unit_tests/` cover syntax, errors, number and complex arithmetic, matrices and vectors, polynomials, built-ins, and end-to-end command sequences.

## Continuous integration with GitHub Actions

GitHub Actions is configured in [`.github/workflows/CI.yml`](.github/workflows/CI.yml). The workflow runs on every push to `master` and has three stages:

| Stage | What it does |
| --- | --- |
| `build-image` | Builds the Docker image from the repository `Dockerfile` and pushes it to `ghcr.io/qpupier/computor-v2/env:latest`. |
| `compile` | Runs in that image, builds with `make -j`, then uploads the compiled binary as a one-day artifact. |
| `test` | Downloads the artifact and runs the `format`, `logic_error`, `numbers`, `complexes`, `matrices`, `polynomials`, `subject`, and `bonus` test groups in parallel. |

The test matrix uses the scripts’ `debug` mode: each case is compared with its expected output and is checked with Valgrind. To reproduce the complete validation locally:

```sh
make test
```

The workflow publishes and consumes its container through GitHub Container Registry, authenticating with the repository `GITHUB_TOKEN`.

## Project layout

```text
.
├── inc/                                 Public headers and mathematical types
├── src/
│   ├── AST/                             Abstract-syntax-tree construction and reduction
│   ├── parser/                          Tokenisation, input handling, and command parsing
│   ├── quadratic_equation_resolution/
│   │   └── print_solutions/             Exact and rounded solution formatting
│   ├── types/                           Numeric, matrix, vector, polynomial, and function types
│   └── main.cpp                         Program entry point
├── unit_tests/                          Shell tests and expected-result fixtures
├── Dockerfile                           Reproducible Ubuntu development environment
├── Makefile                             Build, run, test, and cleanup targets
└── valgrind.supp                        Valgrind suppression rules (for Readline errors)
```

## Notes and limitations

- The session state is not persisted after exit.
- Equations above degree two are reported as unsupported.
- Exponents are restricted to the integer domain supported by the interpreter.
- Exact rational forms are retained before an approximate decimal representation is displayed.

## Author

Quentin PUPIER — [qpupier@student.42lyon.fr](mailto:qpupier@student.42lyon.fr)
