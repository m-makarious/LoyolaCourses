#!/usr/bin/python3
s = open('rosalind_dna.txt','r').read()
for n in ["A", "C", "T", "G"]:
	print(s.count(n), end " ")
