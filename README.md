# IttSketch

## Introduction

In high-speed data streams, a small fraction of items that have specific characteristics are often the focus, such as frequent items, heavy changes, Super-Spreaders, or persistent items. We call them interesting items. Most existing algorithms are designed for only one specific characteristic/interest, and use different data structures for different interests. Existing algorithms can be divided into two kinds. The first kind records all information of the stream, and thus is not memory efficient. The second kind manages to only record the interesting items, but it is challenging to differentiate interesting items from others.
We generalize these characteristics into one aspect, which we call finding interesting items in data streams. To find interesting items, we propose a generic framework, InterestSketch. InterestSketch manages to record only interesting items. To address the challenge of differentiating interesting items from others, we propose a key technique called Probabilistic Replacement and then Increment (PRI). Our theoretical proofs show that when replacement is successful, with high probability, the new item has a higher interest than the current smallest interest. We conduct extensive experiments on three real datasets and one synthetic dataset, on four definitions of interesting items. Our experimental results show that compared with the state-of-the-art, for each definition of interest, our algorithm increases the insertion speed 2.2-7.7 times and decreases the error 74-3207 times.


## About this repo

This repo contains all the algorithms in our experiments, as shown in the following table.

| Task                      | Algorithms                               |
| ------------------------- | ---------------------------------------- |
| Finding frequent items | CM+heap,CM-CU+Heap,SS,SS+CF,unbiased-SS|
| Finding heavy changes   | FR,FR+CF |
| Finding Super-Spreaders   | OpenSketch,OLF,TLF  |
| Finding persistent items   |  Small-Space,PIE  |

This repo also contains a small demo to show how to use this algorithms with a small dataset.

- `IttSketch.pdf` can help you understand the paper better.
- `data`: traces for test, each 8 bytes in a trace is a item.
- `src`
  - `Frequency`: Finding frequent items (CM+heap,CM-CU+Heap,SS,SS+CF,unbiased-SS).
  - `HeavyChange`: Finding heavy changes (FR,FR+CF).
  - `Persistent`: Finding persistent items (Small-Space,PIE).
  - `SuperSpreader`: Finding Super-Spreaders (OpenSketch,OLF,TLF).
- `demo`: a small demo to show how to use these algorithms.

## Requirements
- cmake
- g++

We have tested this demo on MacOS 10.13.6.

## How to build
```
cd demo
sh demo.sh
```
