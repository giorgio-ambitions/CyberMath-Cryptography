# WolfSSL `ssl.c` — Low‑Level Technical Review

## Overview
This document provides a low‑level, mechanical analysis of WolfSSL’s `ssl.c` file.  
The goal is to understand its architectural role, lifecycle behavior, memory‑flow, and security‑critical hotspots **without relying on copyrighted source code**.

This review is suitable for:
- security engineering portfolios  
- open‑source contribution records  
- protocol analysis documentation  
- embedded/automotive cybersecurity work  

---

## 1. Architectural Summary
`ssl.c` is not the core TLS protocol engine.  
It acts as a **lifecycle and orchestration layer**, wiring together:

- TLS 1.2 logic (`tls.c`)
- TLS 1.3 logic (`tls13.c`)
- certificate and ASN.1 handling (`ssl_asn1.c`, `ssl_certman.c`)
- cryptographic operations (`ssl_crypto.c`)
- session management (`ssl_sess.c`)
- OpenSSL‑compatibility layers
- global/per‑context RNG initialization
- WriteDup support for split read/write threads

### Responsibilities
- CTX/SSL object creation  
- global library initialization  
- RNG setup  
- compatibility glue  
- teardown and cleanup  

### Assessment
The architecture is modular but the file is extremely large and heavily conditional.  
Splitting it into smaller components would improve auditability.

---

## 2. TLS State‑Machine Involvement
The TLS handshake state‑machine **does not live in `ssl.c`**.  
It is implemented in:

- `tls.c` (TLS 1.2)
- `tls13.c` (TLS 1.3)

`ssl.c` only creates objects, configures options, and delegates handshake execution.

### Correct State Flow (implemented elsewhere)
