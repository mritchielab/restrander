# restrander

A simple program written to restrand Fastq files.

Currently uses a (very naive) method of looking through each sequence for a PolyA tail of ~12 consecutive As, and a PolyT tail of ~12 consecutive Ts. 
- If it finds a PolyA tail, and it doesn't find a PolyT tail, the sequence is assumed to be a forward read. 
- If there's a PolyT tail but no PolyA tail, it's assumed to be a reverse read. 
- If both tails are present, the read is ambiguous.

Each read is printed to the output file. Reads receive a `direction` tag, which is either `forward`, `reverse` or `ambiguous`.
For reverse reads, the reverse-complement of their original sequence is recorded. Their quality scores are also reversed.

# how to use it

Simply run `restrander`, giving one input file and one output file:

```
./restrander input.fq output.fq
```
