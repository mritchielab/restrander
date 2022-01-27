# restrander

A simple program written to restrand Fastq files.

Currently uses a (very naive) method of looking through each sequence for a PolyA tail of 12 consecutive As. If it finds one, the sequence is assumed to be a forward read, otherwise it is assumed to be a reverse read.

Each read is printed to the output file. Forward reads are not modified. Reverse reads receive a `_rev` tag on their identifier line, and the reverse-complement of their sequence is recorded. Their quality scores have also been reversed.

# how to use it

Simply run `restrander`, giving one input file and one output file:

```
./restrander input.fq output.fq
```
