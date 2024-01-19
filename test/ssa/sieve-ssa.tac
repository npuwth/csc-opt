	instr 1: nop  
	instr 2: entrypc  
	instr 61: nop  

	instr 3: enter 8016 
	instr 4: mul 0 8
	instr 5: add is_prime_base#-8016 FP
	instr 6: add (5) (4)
	instr 7: store 0 (6)
	instr 8: mul 1 8
	instr 9: add is_prime_base#-8016 FP
	instr 10: add (9) (8)
	instr 11: store 0 (10)
	instr 12: move 2 i$1
	instr 62: phi i$1 i$3 
	instr 68: move (62) i$2
	instr 13: cmplt i$2 1000
	instr 14: blbc (13) [22]
	instr 15: mul i$2 8
	instr 16: add is_prime_base#-8016 FP
	instr 17: add (16) (15)
	instr 18: store 1 (17)
	instr 19: add i$2 1
	instr 20: move (19) i$3
	instr 21: br [62] 
	instr 22: move 2 i$4
	instr 63: phi i$4 i$6 
	instr 69: move (63) i$5
	instr 67: phi j$0 j$5 
	instr 70: move (67) j$1
	instr 23: cmplt i$5 1000
	instr 24: blbc (23) [46]
	instr 25: mul i$5 8
	instr 26: add is_prime_base#-8016 FP
	instr 27: add (26) (25)
	instr 28: load (27) 
	instr 29: cmpeq (28) 0
	instr 30: blbs (29) [65]
	instr 31: move 2 j$2
	instr 66: phi j$2 j$4 
	instr 71: move (66) j$3
	instr 32: mul i$5 j$3
	instr 33: cmplt (32) 1000
	instr 34: blbc (33) [65]
	instr 35: mul i$5 j$3
	instr 36: mul (35) 8
	instr 37: add is_prime_base#-8016 FP
	instr 38: add (37) (36)
	instr 39: store 0 (38)
	instr 40: add j$3 1
	instr 41: move (40) j$4
	instr 42: br [66] 
	instr 65: phi j$1 j$3 
	instr 72: move (65) j$5
	instr 43: add i$5 1
	instr 44: move (43) i$6
	instr 45: br [63] 
	instr 46: move 2 i$7
	instr 64: phi i$7 i$9 
	instr 73: move (64) i$8
	instr 47: cmplt i$8 1000
	instr 48: blbc (47) [59]
	instr 49: mul i$8 8
	instr 50: add is_prime_base#-8016 FP
	instr 51: add (50) (49)
	instr 52: load (51) 
	instr 53: cmpeq (52) 0
	instr 54: blbs (53) [56]
	instr 55: write i$8 
	instr 56: add i$8 1
	instr 57: move (56) i$9
	instr 58: br [64] 
	instr 59: wrl  
	instr 60: ret 0 

