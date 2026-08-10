OpenSSL BN Multiplication — Internal Architecture Overview
This repository documents the internal implementation of big‑number multiplication in OpenSSL’s crypto/bn subsystem.
The code provides multiple multiplication engines—classical, recursive, partial‑recursive, and low‑half—optimized for cryptographic workloads such as RSA, DH, ECC, and Montgomery arithmetic.

The goal is to make the BN multiplication pipeline auditable, understandable, and hackable for systems programmers, cryptography engineers, and reverse‑engineers.

📌 Components
Component	Purpose
bn_sub_part_words	Subtract arrays of unequal length (supports recursive algorithms)
bn_mul_recursive	Full Karatsuba multiplication for equal halves
bn_mul_part_recursive	Karatsuba for uneven halves
bn_mul_low_recursive	Compute only the lower half of a product
bn_mul_normal	Classical O(n²) multiplication
bn_mul_fixed_top	Dispatcher selecting the optimal algorithm
BN_mul	Public API wrapper


These functions cooperate to provide fast, constant‑time‑aware multiplication across all operand sizes.

🔢 Structural Foundations
OpenSSL stores big numbers as arrays of machine words (BN_ULONG).
The multiplication code operates directly on these arrays, respecting invariants such as:

no leading zero limbs

correct top and dmax

constant‑time behavior when required

From the file:

“All lengths are the number of BN_ULONGs…”

⚙️ Classical Multiplication (bn_mul_normal)
The fallback engine implements the standard schoolbook algorithm:

“r[i+j] += a[i] * b[j]”

Optimizations include:

4‑word loop unrolling

branch minimization

specialized primitives (bn_mul_words, bn_mul_add_words)

Used when operands are small or uneven.

⚡ Karatsuba Multiplication (bn_mul_recursive)
Implements the divide‑and‑conquer Karatsuba algorithm when operand halves are equal and large enough.

Key features:

recursive splitting

sign‑aware subtraction

comba fast‑paths for 4‑ and 8‑word operands

scratch‑buffer management

From the code:

“We calculate a[0]b[0], a[1]b[1], (a[0]-a[1])*(b[1]-b[0])”

🧩 Uneven Karatsuba (bn_mul_part_recursive)
Handles real‑world BIGNUMs where halves differ in size.
It performs:

partial subtraction

recursive splitting

fallback to classical multiplication when recursion becomes inefficient

This function is the glue that makes Karatsuba work on arbitrary operand sizes.

🔻 Low‑Half Multiplication (bn_mul_low_recursive)
Used by Montgomery reduction and other operations that only require the lower half of a product.

From the file:

“We multiply… and return the result… compute only the lower half”

It avoids computing the upper half entirely, saving cycles.

🧠 Algorithm Dispatcher (bn_mul_fixed_top)
Selects the optimal multiplication strategy based on operand size and shape:

comba4/comba8 for fixed small sizes

full Karatsuba

partial‑recursive Karatsuba

classical multiplication

Also handles:

temporary BIGNUM allocation

sign computation

fixed‑top normalization

From the file:

“Find out the power of two lower or equal to the longest of the two numbers”

📈 Performance Summary
Algorithm	Complexity	Best Use Case
Classical	O(n²)	Small or uneven operands
Karatsuba	O(n^1.585)	Large balanced operands
Comba	O(n²) but optimized	Fixed small sizes (4, 8 words)
Low‑half	O(n^1.585)	Montgomery reduction


OpenSSL dynamically chooses the fastest safe path.
