1. BIGNUM structure management
The file maintains the internal invariants of a BIGNUM:

limbs stored in d

top = used limbs

dmax = allocated limbs

neg = sign

flags = secure/const‑time/static metadata

It ensures top ≤ dmax, no leading zero limbs, and correct sign handling.

2. Memory lifecycle
Core functions:

BN_new, BN_secure_new — allocate BN objects (normal or secure memory)

bn_expand2 — safely expand limb storage

BN_free, BN_clear_free — free memory, optionally wiping secrets

This layer guarantees safe, secure memory handling for cryptographic integers.

3. Bit and word operations
Basic structural operations:

BN_num_bits — bit length (normal or constant‑time)

BN_set_bit, BN_clear_bit — bit manipulation

bn_correct_top — restore canonical form after changes

These maintain the internal consistency of the number.

4. Binary encoding/decoding
Centralized conversion between BIGNUM and byte arrays:

big‑endian / little‑endian

signed / unsigned

padded / unpadded

Handled through two engines:

bin2bn (decode)

bn2binpad (encode)

This isolates representation logic from arithmetic code.

5. Comparisons
Implements signed and unsigned comparisons:

BN_ucmp — magnitude

BN_cmp — signed

constant‑time variants when flags require it

Used by modular arithmetic, prime generation, and protocol validation.

6. Constant‑time structural helpers
Side‑channel‑safe primitives:

BN_consttime_swap — swap two BIGNUMs without branches

bn_correct_top_consttime — constant‑time top correction

Essential for constant‑time exponentiation and key‑handling routines.

7. Flags, predicates, and security bits
Utility functions:

flag manipulation (BN_set_flags, BN_with_flags)

predicates (BN_is_zero, BN_is_one, etc.)

BN_security_bits — SP800‑57 security estimation

Final summary
bn_lib.c is the structural backbone of OpenSSL’s BIGNUM system.
It defines how big numbers are stored, resized, represented, compared, and handled securely, enabling all arithmetic and cryptographic operations built on top of it.
