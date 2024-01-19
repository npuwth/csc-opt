	instr 1: nop  
	instr 2: entrypc  
	instr 59: nop  

	instr 3: enter 24 
	instr 4: mul 0 8
	instr 5: add prime_base#29568 GP
	instr 6: add (5) (4)
	instr 7: store 2 (6)
	instr 8: mul 0 8
	instr 9: add prime_base#29568 GP
	instr 10: add (9) (8)
	instr 11: load (10) 
	instr 12: write (11) 
	instr 13: move 1 i$1
	instr 14: move 3 v$1
	instr 61: phi i$1 i$4 
	instr 66: move (61) i$2
	instr 62: phi j$0 j$3 
	instr 67: move (62) j$1
	instr 65: phi v$1 v$3 
	instr 68: move (65) v$2
	instr 15: cmplt i$2 400
	instr 16: blbc (15) [57]
	instr 17: move 0 j$2
	instr 64: phi j$2 j$6 
	instr 69: move (64) j$3
	instr 18: cmplt j$3 i$2
	instr 19: blbc (18) [45]
	instr 20: mul j$3 8
	instr 21: add prime_base#29568 GP
	instr 22: add (21) (20)
	instr 23: mul j$3 8
	instr 24: add prime_base#29568 GP
	instr 25: add (24) (23)
	instr 26: load (22) 
	instr 27: load (25) 
	instr 28: mul (26) (27)
	instr 29: cmple (28) v$2
	instr 30: blbs (29) [34]
	instr 31: sub i$2 1
	instr 32: move (31) j$4
	instr 33: br [63] 
	instr 34: mul j$3 8
	instr 35: add prime_base#29568 GP
	instr 36: add (35) (34)
	instr 37: load (36) 
	instr 38: mod v$2 (37)
	instr 39: cmpeq (38) 0
	instr 40: blbc (39) [63]
	instr 41: move i$2 j$7
	instr 63: phi j$4 j$3 j$7 
	instr 70: move (63) j$5
	instr 42: add j$5 1
	instr 43: move (42) j$6
	instr 44: br [64] 
	instr 45: cmpeq j$3 i$2
	instr 46: blbc (45) [60]
	instr 47: mul i$2 8
	instr 48: add prime_base#29568 GP
	instr 49: add (48) (47)
	instr 50: store v$2 (49)
	instr 51: write v$2 
	instr 52: add i$2 1
	instr 53: move (52) i$3
	instr 60: phi i$2 i$3 
	instr 71: move (60) i$4
	instr 54: add v$2 2
	instr 55: move (54) v$3
	instr 56: br [61] 
	instr 57: wrl  
	instr 58: ret 0 

