	instr 1: nop  
	instr 2: entrypc  
	instr 80: nop  

	instr 3: enter 104 
	instr 4: move 0 i@-8
	instr 5: cmplt i@-8 10
	instr 6: blbc (5) [16]
	instr 7: mul i@-8 8
	instr 8: add array_base#-104 FP
	instr 9: add (8) (7)
	instr 10: sub 10 i@-8
	instr 11: sub (10) 1
	instr 12: store (11) (9)
	instr 13: add i@-8 1
	instr 14: move (13) i@-8
	instr 15: br [5] 
	instr 16: move 0 i@-8
	instr 17: cmplt i@-8 10
	instr 18: blbc (17) [27]
	instr 19: mul i@-8 8
	instr 20: add array_base#-104 FP
	instr 21: add (20) (19)
	instr 22: load (21) 
	instr 23: write (22) 
	instr 24: add i@-8 1
	instr 25: move (24) i@-8
	instr 26: br [17] 
	instr 27: wrl  
	instr 28: move 0 i@-8
	instr 29: cmplt i@-8 10
	instr 30: blbc (29) [67]
	instr 31: move 0 j@-16
	instr 32: cmplt j@-16 i@-8
	instr 33: blbc (32) [64]
	instr 34: mul j@-16 8
	instr 35: add array_base#-104 FP
	instr 36: add (35) (34)
	instr 37: mul i@-8 8
	instr 38: add array_base#-104 FP
	instr 39: add (38) (37)
	instr 40: load (36) 
	instr 41: load (39) 
	instr 42: cmple (40) (41)
	instr 43: blbs (42) [61]
	instr 44: mul i@-8 8
	instr 45: add array_base#-104 FP
	instr 46: add (45) (44)
	instr 47: load (46) 
	instr 48: move (47) temp@-24
	instr 49: mul i@-8 8
	instr 50: add array_base#-104 FP
	instr 51: add (50) (49)
	instr 52: mul j@-16 8
	instr 53: add array_base#-104 FP
	instr 54: add (53) (52)
	instr 55: load (54) 
	instr 56: store (55) (51)
	instr 57: mul j@-16 8
	instr 58: add array_base#-104 FP
	instr 59: add (58) (57)
	instr 60: store temp@-24 (59)
	instr 61: add j@-16 1
	instr 62: move (61) j@-16
	instr 63: br [32] 
	instr 64: add i@-8 1
	instr 65: move (64) i@-8
	instr 66: br [29] 
	instr 67: move 0 i@-8
	instr 68: cmplt i@-8 10
	instr 69: blbc (68) [78]
	instr 70: mul i@-8 8
	instr 71: add array_base#-104 FP
	instr 72: add (71) (70)
	instr 73: load (72) 
	instr 74: write (73) 
	instr 75: add i@-8 1
	instr 76: move (75) i@-8
	instr 77: br [68] 
	instr 78: wrl  
	instr 79: ret 0 

