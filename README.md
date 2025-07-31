# 🧠 Vectorization Benchmark: Modularity vs Efficiency

This project benchmarks the performance of **modular arithmetic operations** written in C++, with the goal of understanding:

* Whether high-level C++ code gets **automatically optimized** by the compiler (e.g., using SIMD).
* How **code modularity** (e.g., using helper functions like `add()`, `multiply()`) affects the compiler's ability to optimize.
* Why sometimes the **expected low-level optimizations** (like use of `xmm`/`ymm` registers) don’t appear — even with `-O3`.

---

## 🔧 Build Instructions (CMake)

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

If using GCC or Clang, make sure the following flags are applied **per target** to enable SIMD-aware optimizations:

```cmake
target_compile_options(vector_math PRIVATE
    -O3
    -march=native
    -ffast-math
    -funroll-loops
    -ftree-vectorize
    -fopt-info-vec
    -fopt-info-vec-missed
)
```

---

## 🚀 What It Does

The program runs two versions of a basic arithmetic pipeline:

### 1. Modular approach

```cpp
std::vector<int> summed = add(numbers1, numbers2);
std::vector<int> squared = multiply(summed, summed);
```

### 2. Fused approach

```cpp
fusedMultAdd(numbers1, numbers2);
```

Both perform:

```
(numbers1[i] + numbers2[i])²
```

But in different ways.

---

## 📉 Performance Results

Despite using `-O3`, the modular version is **significantly slower** than the fused version.

Why? Because the compiler:

* **Fails to vectorize** the modular code (no `xmm/ymm` used).
* Scalar instructions like `movl`, `addl` are used instead.
* Memory allocations and push\_back inside loops **prevent optimization**.

Use `objdump -d ./vector_math | grep xmm` to confirm: **no SIMD instructions are emitted**.

---

## ⚖️ Modularity vs Efficiency

| Modularity (Clean Code)         | Efficiency (Fast Code)   |
| ------------------------------- | ------------------------ |
| Separated logic (add, multiply) | Fused into one loop      |
| Push-back for abstraction       | Manual allocation + `[]` |
| Easy to test and maintain       | Compiler-friendly layout |
| Hurts optimization chances      | Enables vectorization    |

> ✅ The more modular and abstract your code is, the harder it is for the compiler to apply low-level optimizations like loop fusion, unrolling, and SIMD.

---

## 🧠 Why Optimizations Are Missing

Despite high-level optimization flags (`-O3`, etc.), the compiler does **not vectorize** modular code due to:

### 🚫 Push-Backs

Using `result.push_back(...)` makes it hard for the compiler to reason about memory. It can’t safely vectorize when the size grows dynamically.

### 🚫 Unknown Memory Alignment

The `std::vector<int>` data isn’t guaranteed to be 16- or 32-byte aligned. Vector instructions (like `vmovdqa`) require aligned data.

### 🚫 Function Boundaries

Separating `add()` and `multiply()` into different functions breaks optimization scopes. Even with inlining, alias analysis becomes harder.


---