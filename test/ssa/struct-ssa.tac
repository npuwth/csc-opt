	instr 1: nop  
	instr 2: entrypc  
	instr 120: nop  

	instr 3: enter 72 
	instr 4: add c_base#-48 FP
	instr 5: add (4) z_offset#16
	instr 6: add (5) r_offset#8
	instr 7: store 987654321 (6)
	instr 8: add a_base#32728 GP
	instr 9: add (8) x_offset#0
	instr 10: store 1 (9)
	instr 11: add a_base#32728 GP
	instr 12: add (11) y_offset#8
	instr 13: store 2 (12)
	instr 14: add c_base#-48 FP
	instr 15: add (14) x_offset#0
	instr 16: store 9 (15)
	instr 17: add c_base#-48 FP
	instr 18: add (17) y_offset#8
	instr 19: store 0 (18)
	instr 20: mul 0 40
	instr 21: add b_base#32608 GP
	instr 22: add (21) (20)
	instr 23: add (22) x_offset#0
	instr 24: store 3 (23)
	instr 25: mul 0 40
	instr 26: add b_base#32608 GP
	instr 27: add (26) (25)
	instr 28: add (27) y_offset#8
	instr 29: store 4 (28)
	instr 30: add a_base#32728 GP
	instr 31: add (30) x_offset#0
	instr 32: load (31) 
	instr 33: mul (32) 40
	instr 34: add b_base#32608 GP
	instr 35: add (34) (33)
	instr 36: add (35) x_offset#0
	instr 37: store 5 (36)
	instr 38: add a_base#32728 GP
	instr 39: add (38) x_offset#0
	instr 40: load (39) 
	instr 41: mul (40) 40
	instr 42: add b_base#32608 GP
	instr 43: add (42) (41)
	instr 44: add (43) y_offset#8
	instr 45: store 6 (44)
	instr 46: add a_base#32728 GP
	instr 47: add (46) x_offset#0
	instr 48: load (47) 
	instr 49: sub (48) 1
	instr 50: mul (49) 40
	instr 51: add b_base#32608 GP
	instr 52: add (51) (50)
	instr 53: add (52) x_offset#0
	instr 54: load (53) 
	instr 55: sub (54) 1
	instr 56: mul (55) 40
	instr 57: add b_base#32608 GP
	instr 58: add (57) (56)
	instr 59: add (58) x_offset#0
	instr 60: store 7 (59)
	instr 61: add a_base#32728 GP
	instr 62: add (61) y_offset#8
	instr 63: load (62) 
	instr 64: sub (63) 2
	instr 65: mul (64) 40
	instr 66: add b_base#32608 GP
	instr 67: add (66) (65)
	instr 68: add (67) x_offset#0
	instr 69: load (68) 
	instr 70: sub (69) 1
	instr 71: mul (70) 40
	instr 72: add b_base#32608 GP
	instr 73: add (72) (71)
	instr 74: add (73) y_offset#8
	instr 75: store 8 (74)
	instr 76: add a_base#32728 GP
	instr 77: add (76) x_offset#0
	instr 78: load (77) 
	instr 79: write (78) 
	instr 80: add a_base#32728 GP
	instr 81: add (80) y_offset#8
	instr 82: load (81) 
	instr 83: write (82) 
	instr 84: wrl  
	instr 85: move 0 i$1
	instr 121: phi i$1 i$3 
	instr 122: move (121) i$2
	instr 86: cmplt i$2 3
	instr 87: blbc (86) [104]
	instr 88: mul i$2 40
	instr 89: add b_base#32608 GP
	instr 90: add (89) (88)
	instr 91: add (90) x_offset#0
	instr 92: load (91) 
	instr 93: write (92) 
	instr 94: mul i$2 40
	instr 95: add b_base#32608 GP
	instr 96: add (95) (94)
	instr 97: add (96) y_offset#8
	instr 98: load (97) 
	instr 99: write (98) 
	instr 100: wrl  
	instr 101: add i$2 1
	instr 102: move (101) i$3
	instr 103: br [121] 
	instr 104: add c_base#-48 FP
	instr 105: add (104) x_offset#0
	instr 106: load (105) 
	instr 107: write (106) 
	instr 108: add c_base#-48 FP
	instr 109: add (108) y_offset#8
	instr 110: load (109) 
	instr 111: write (110) 
	instr 112: wrl  
	instr 113: add c_base#-48 FP
	instr 114: add (113) z_offset#16
	instr 115: add (114) r_offset#8
	instr 116: load (115) 
	instr 117: write (116) 
	instr 118: wrl  
	instr 119: ret 0 

