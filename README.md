# restrander

A program written to restrand Fastq files.

Currently uses (very very naive) method of looking through each sequence for a PolyA tail of 12 consecutive As. If it finds one, the sequence is assumed to be forwards read, otherwise it is assumed to be reversed.

# how to use it

Simply run `restrander`, giving one input file and one output file:

```
./restrander input.fq output.fq
```
