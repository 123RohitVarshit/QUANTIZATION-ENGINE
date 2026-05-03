# llm-engine

A from-scratch LLM inference engine in C, built to explore adaptive quantization and layer streaming for memory-efficient inference on consumer hardware.

The core idea: existing systems choose either **quantization** (llama.cpp) or **layer streaming** (AirLLM) to solve the memory wall problem. This project implements both simultaneously, with a per-layer mixed-precision heuristic — demonstrating that large models can be explored on consumer hardware with significantly reduced peak memory.

---

## Motivation

| System | Approach | Memory | Speed | Quality |
|---|---|---|---|---|
| llama.cpp | Quantization | Medium | Fast | Slight loss |
| AirLLM | Layer streaming | Very low | Very slow | Full precision |
| **llm-engine** | **Both + mixed precision** | **Minimal** | **Better than AirLLM** | **Controlled loss** |

Nobody has cleanly combined layer-wise streaming with sensitivity-aware mixed-precision quantization as a unified design. This project explores that gap.

---

## Architecture

```
llm-engine/
├── src/
│   ├── gguf_parser.c     # Binary GGUF file parser
│   ├── gguf_parser.h
│   ├── quantize.c        # INT8 / INT4 dequantization engine
│   ├── quantize.h
│   ├── stream.c          # Layer-by-layer streaming loader
│   ├── stream.h
│   └── main.c
├── Makefile
└── README.md
```

---

## Progress

### Day 1 — GGUF Binary Parser ✅

Reads real LLM model files (Ollama/llama.cpp GGUF format) directly from disk.

- Validates magic number (`0x46554747`)
- Reads GGUF version and rejects unsupported old formats
- Extracts tensor count and metadata count from binary header

**Tested on:** `llama3.2:latest` (2GB, 255 tensors, 30 metadata pairs)

```
$ ./llm_engine.exe "path/to/llama3.2.gguf"
GGUF file is valid.
GGUF version: 3
Number of tensors: 255
Number of key-value pairs: 30
```

### Day 2 — Metadata Parser (in progress)
- Read all 30 key-value metadata pairs
- Extract architecture, layer count, embedding dimensions, attention heads

### Day 3 — Layer Streaming (planned)
- Load one transformer block at a time
- Measure peak memory at each step
- Never accumulate more than one layer in memory simultaneously

### Day 4 — Dequantization Engine (planned)
- Implement Q4_K and Q8_0 dequantization (real GGUF quantization formats)
- Measure per-tensor quantization error vs FP32 baseline

### Day 5 — Mixed Precision Heuristic (planned)
- Assign INT8 to sensitive layers (first and last 25%)
- Assign INT4 to middle layers
- Compare memory usage and error across assignments

### Day 6 — Benchmarking (planned)
- Peak memory per approach
- Dequantization time per layer
- Compression ratio vs error tradeoff

---

## Building

### Requirements

- GCC (via MSYS2 MinGW64 on Windows, or native on Linux/Mac)
- A GGUF model file (via [Ollama](https://ollama.com))

### Setup on Windows

```bash
# Install MSYS2 from https://www.msys2.org
# Then in MSYS2 MinGW64 terminal:
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make
```

### Compile

```bash
make
```

Or manually:

```bash
gcc -O2 -Wall -std=c11 -o llm_engine.exe src/main.c src/gguf_parser.c -lm
```

### Run

```bash
# Find your Ollama model path (PowerShell)
$manifest = Get-Content "$env:USERPROFILE\.ollama\models\manifests\registry.ollama.ai\library\llama3.2\latest" | ConvertFrom-Json
$digest = ($manifest.layers | Where-Object { $_.mediaType -eq "application/vnd.ollama.image.model" }).digest
$blobPath = "$env:USERPROFILE\.ollama\models\blobs\" + $digest.Replace(":", "-")
echo $blobPath

# Run the engine
./llm_engine.exe "C:/Users/YOU/.ollama/models/blobs/sha256-..."
```

---

## What is GGUF?

GGUF is the binary model format used by llama.cpp and Ollama. Every model you pull with `ollama pull` is stored as a GGUF file on disk — a raw binary file containing model weights in quantized format (Q4_K, Q8_0, etc.) alongside metadata describing the architecture.

This project parses that format directly without any external libraries.

---

## Background Reading

- [GGUF Format Specification](https://github.com/ggerganov/ggml/blob/master/docs/gguf.md)
- [AirLLM — Layer Streaming Inference](https://github.com/lyogavin/airllm)
- [llama.cpp](https://github.com/ggerganov/llama.cpp)
- [PRIMA.CPP — Distributed Layer Streaming (April 2025)](https://arxiv.org/abs/2504.08791)

---

*Built from scratch in C as a learning project. Every line written by hand — no copy-paste.*
