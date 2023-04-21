<p align="center">
    <img src="figures/logo.png" title="Restrander" alt="Restrander" width="300">
</p>

A fast, accurate program for orienting and quality-checking cDNA sequencing reads.

## Introduction

In transcriptomic analyses, it is helpful to keep track of the strand of the RNA molecules. However, the Oxford Nanopore long-read cDNA sequencing protocols generate reads that correspond to either the first or second-strand cDNA, therefore the strandedness of the initial transcript has to be inferred bioinformatically.

Restrander parses an input fastq, infers the orientation of each read, and prints to an output fastq. The strand of each read is recorded with the `strand` tag, either `+` or `-`. Each read from the reverse strand is replaced with its reverse-complement, ensuring all reads in the output have the same orientation as the original transcripts. 

Only well-formed reads are included in the main output file; reads whose strand cannot be inferred are marked with `strand=?`, and optionally filtered out into an “unknown” fastq to be handled separately by the user. If Restrander is configured to detect artefacts, these artefactual reads will also be placed in the “unknown” fastq.

In a typical cDNA-seq analysis pipeline, Restrander would be applied after basecalling, and before mapping. 

## Installation

```bash
git clone https://github.com/jakob-schuster/restrander.git
make
```

## Usage

Run `restrander` with one input file, one output file and one configuration file. The input and/or output can optionally be gzipped. The configuration provides the TSO and RTP sequences, and different configurations are used for different protocols.

```bash
# for standard ONT PCB109 data:
./restrander input.fq.gz output.fq.gz config/PCB109.json
```
