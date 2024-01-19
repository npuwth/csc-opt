	instr 1: nop  
	instr 2: entrypc  
	instr 48: nop  

	instr 3: enter 64 
	instr 4: move 13 n$1
	instr 5: move 0 x$1
	instr 6: move 0 a$1
	instr 49: phi b$0 b$3 
	instr 76: move (49) b$1
	instr 56: phi f$0 f$2 
	instr 77: move (56) f$1
	instr 61: phi e$0 e$2 
	instr 78: move (61) e$1
	instr 65: phi d$0 d$2 
	instr 79: move (65) d$1
	instr 66: phi x$1 x$4 
	instr 80: move (66) x$2
	instr 72: phi a$1 a$3 
	instr 81: move (72) a$2
	instr 75: phi c$0 c$2 
	instr 82: move (75) c$1
	instr 7: cmplt a$2 n$1
	instr 8: blbc (7) [45]
	instr 9: move 0 b$2
	instr 10: move 0 x$3
	instr 50: phi b$2 b$4 
	instr 83: move (50) b$3
	instr 55: phi f$1 f$3 
	instr 84: move (55) f$2
	instr 60: phi e$1 e$3 
	instr 85: move (60) e$2
	instr 64: phi d$1 d$3 
	instr 86: move (64) d$2
	instr 71: phi x$3 x$5 
	instr 87: move (71) x$4
	instr 73: phi c$1 c$4 
	instr 88: move (73) c$2
	instr 11: cmplt b$3 n$1
	instr 12: blbc (11) [42]
	instr 13: move 0 c$3
	instr 54: phi f$2 f$4 
	instr 89: move (54) f$3
	instr 59: phi e$2 e$4 
	instr 90: move (59) e$3
	instr 62: phi d$2 d$5 
	instr 91: move (62) d$3
	instr 70: phi x$4 x$6 
	instr 92: move (70) x$5
	instr 74: phi c$3 c$5 
	instr 93: move (74) c$4
	instr 14: cmplt c$4 n$1
	instr 15: blbc (14) [39]
	instr 16: move 0 d$4
	instr 53: phi f$3 f$5 
	instr 94: move (53) f$4
	instr 57: phi e$3 e$6 
	instr 95: move (57) e$4
	instr 63: phi d$4 d$6 
	instr 96: move (63) d$5
	instr 69: phi x$5 x$7 
	instr 97: move (69) x$6
	instr 17: cmplt d$5 n$1
	instr 18: blbc (17) [36]
	instr 19: move 0 e$5
	instr 51: phi f$4 f$7 
	instr 98: move (51) f$5
	instr 58: phi e$5 e$7 
	instr 99: move (58) e$6
	instr 68: phi x$6 x$8 
	instr 100: move (68) x$7
	instr 20: cmplt e$6 n$1
	instr 21: blbc (20) [33]
	instr 22: move 0 f$6
	instr 52: phi f$6 f$8 
	instr 101: move (52) f$7
	instr 67: phi x$7 x$9 
	instr 102: move (67) x$8
	instr 23: cmplt f$7 n$1
	instr 24: blbc (23) [30]
	instr 25: add x$8 1
	instr 26: move (25) x$9
	instr 27: add f$7 1
	instr 28: move (27) f$8
	instr 29: br [52] 
	instr 30: add e$6 1
	instr 31: move (30) e$7
	instr 32: br [51] 
	instr 33: add d$5 1
	instr 34: move (33) d$6
	instr 35: br [53] 
	instr 36: add c$4 1
	instr 37: move (36) c$5
	instr 38: br [54] 
	instr 39: add b$3 1
	instr 40: move (39) b$4
	instr 41: br [50] 
	instr 42: add a$2 1
	instr 43: move (42) a$3
	instr 44: br [49] 
	instr 45: write x$2 
	instr 46: wrl  
	instr 47: ret 0 

