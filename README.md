# restrander

A program for restranding Fastq files.

Parses an input `.fq` file, classifying each record as either forward `+`, reverse `-` or ambiguous `?`, outputting to a new `.fq` with the classification of each read included in a `strand` tag. For reverse reads, the reverse-complement of the original sequence is recorded, and quality scores are also reversed. Also works on gzipped `.gz` files.

# Usage instructions

Simply run `restrander`, giving one input file and one output file:

```
./restrander input.fq output.fq
```

Optionally, you can provide a specific configuration file:

```
./restrander input.fq output.fq config.json
```

# How it works

Each forward read is assumed to have a particular form:

``` handle - barcode - TSO - mRNA - polyA - RT_reverse_complement - barcode_reverse_complement - handle_reverse complement ```

While a reverse read takes the form:

``` handle - barcode - RT - polyT - mRNA_reverse_complement - TSO_reverse_complement - barcode_reverse_complement - handle_reverse complement ```

Because of these differences, there are a few methods for classifying read direction:

## PolyA/PolyT Classification

A naive method of looking through each sequence for a PolyA tail of consecutive As near the start, and a PolyT tail of consecutive Ts near the end.

| PolyA tail present  | PolyT tail present  | Classification  |
| ------------------- | ------------------- | --------------- |
| Yes                 | No                  | Forward         |
| No                  | Yes                 | Reverse         |
| Yes                 | Yes                 | Ambiguous       |
| No                  | No                  | Ambiguous       |

The drawback of this method is that often sequences include native PolyA/PolyT tails, leading to ambiguous reads.

## TSO/RT Classification

A method of searching for the TSO and RT near the start of the read to classify it. Often the TSO and RT are not perfectly present in the read, allowing for some edit distance.

| TSO found in sequence | RT found in sequence | Classification  |
| --------------------- | --------------------- | --------------- |
| Yes                   | No                    | Forward         |
| No                    | Yes                   | Reverse         |
| Yes                   | Yes                   | Ambiguous       |
| No                    | No                    | Ambiguous       |

This method takes longer than PolyA/PolyT classification. Additionally, sometimes the TSO and RT are so wrong in the sequence that we cannot identify them, leading to ambiguous reads.

# Configurations

## PCB109

The default configuration. First applies PolyA/PolyT classification, then looks for the standard TSO/SSP and RT used in PCB109 chemistry.

## Trimmed

A configuration for trimmed reads. Trimmed reads do not have primers, they only have PolyA/PolyT tails. Hence, the trimmed pipeline only performs PolyA/PolyT classification.