    instr 1: nop
    instr 2: entrypc
    instr 3: enter 288
    instr 4: move 0 i@-272
    instr 5: cmplt i@-272 4
    instr 6: blbc (5) [28]
    instr 7: move 0 j@-280
    instr 8: cmplt j@-280 3
    instr 9: blbc (8) [24]
    instr 10: mul i@-272 24
    instr 11: add m1_base#-96 FP
    instr 12: add (11) (10)
    instr 13: mul j@-280 8
    instr 14: add (12) (13)
    instr 15: mul j@-280 2
    instr 16: add i@-272 (15)
    instr 17: store (16) (14)
    instr 18: mul j@-280 2
    instr 19: add i@-272 (18)
    instr 20: write (19)
    instr 21: add j@-280 1
    instr 22: move (21) j@-280
    instr 23: br [8]
    instr 24: wrl
    instr 25: add i@-272 1
    instr 26: move (25) i@-272
    instr 27: br [5]
    instr 28: move 0 i@-272
    instr 29: cmplt i@-272 4
    instr 30: blbc (29) [52]
    instr 31: move 0 j@-280
    instr 32: cmplt j@-280 3
    instr 33: blbc (32) [49]
    instr 34: mul j@-280 32
    instr 35: add m2_base#-192 FP
    instr 36: add (35) (34)
    instr 37: mul i@-272 8
    instr 38: add (36) (37)
    instr 39: mul i@-272 24
    instr 40: add m1_base#-96 FP
    instr 41: add (40) (39)
    instr 42: mul j@-280 8
    instr 43: add (41) (42)
    instr 44: load (43)
    instr 45: store (44) (38)
    instr 46: add j@-280 1
    instr 47: move (46) j@-280
    instr 48: br [32]
    instr 49: add i@-272 1
    instr 50: move (49) i@-272
    instr 51: br [29]
    instr 52: wrl
    instr 53: move 0 i@-272
    instr 54: cmplt i@-272 3
    instr 55: blbc (54) [73]
    instr 56: move 0 j@-280
    instr 57: cmplt j@-280 4
    instr 58: blbc (57) [69]
    instr 59: mul i@-272 32
    instr 60: add m2_base#-192 FP
    instr 61: add (60) (59)
    instr 62: mul j@-280 8
    instr 63: add (61) (62)
    instr 64: load (63)
    instr 65: write (64)
    instr 66: add j@-280 1
    instr 67: move (66) j@-280
    instr 68: br [57]
    instr 69: wrl
    instr 70: add i@-272 1
    instr 71: move (70) i@-272
    instr 72: br [54]
    instr 73: move 0 i@-272
    instr 74: cmplt i@-272 3
    instr 75: blbc (74) [91]
    instr 76: move 0 j@-280
    instr 77: cmplt j@-280 3
    instr 78: blbc (77) [88]
    instr 79: mul i@-272 24
    instr 80: add m3_base#-264 FP
    instr 81: add (80) (79)
    instr 82: mul j@-280 8
    instr 83: add (81) (82)
    instr 84: store 0 (83)
    instr 85: add j@-280 1
    instr 86: move (85) j@-280
    instr 87: br [77]
    instr 88: add i@-272 1
    instr 89: move (88) i@-272
    instr 90: br [74]
    instr 91: wrl
    instr 92: move 0 i@-272
    instr 93: cmplt i@-272 3
    instr 94: blbc (93) [136]
    instr 95: move 0 j@-280
    instr 96: cmplt j@-280 3
    instr 97: blbc (96) [133]
    instr 98: move 0 k@-288
    instr 99: cmplt k@-288 4
    instr 100: blbc (99) [130]
    instr 101: mul i@-272 24
    instr 102: add m3_base#-264 FP
    instr 103: add (102) (101)
    instr 104: mul j@-280 8
    instr 105: add (103) (104)
    instr 106: mul i@-272 24
    instr 107: add m3_base#-264 FP
    instr 108: add (107) (106)
    instr 109: mul j@-280 8
    instr 110: add (108) (109)
    instr 111: mul k@-288 24
    instr 112: add m1_base#-96 FP
    instr 113: add (112) (111)
    instr 114: mul j@-280 8
    instr 115: add (113) (114)
    instr 116: mul i@-272 32
    instr 117: add m2_base#-192 FP
    instr 118: add (117) (116)
    instr 119: mul k@-288 8
    instr 120: add (118) (119)
    instr 121: load (115)
    instr 122: load (120)
    instr 123: mul (121) (122)
    instr 124: load (110)
    instr 125: add (124) (123)
    instr 126: store (125) (105)
    instr 127: add k@-288 1
    instr 128: move (127) k@-288
    instr 129: br [99]
    instr 130: add j@-280 1
    instr 131: move (130) j@-280
    instr 132: br [96]
    instr 133: add i@-272 1
    instr 134: move (133) i@-272
    instr 135: br [93]
    instr 136: move 0 i@-272
    instr 137: cmplt i@-272 3
    instr 138: blbc (137) [156]
    instr 139: move 0 j@-280
    instr 140: cmplt j@-280 3
    instr 141: blbc (140) [152]
    instr 142: mul i@-272 24
    instr 143: add m3_base#-264 FP
    instr 144: add (143) (142)
    instr 145: mul j@-280 8
    instr 146: add (144) (145)
    instr 147: load (146)
    instr 148: write (147)
    instr 149: add j@-280 1
    instr 150: move (149) j@-280
    instr 151: br [140]
    instr 152: wrl
    instr 153: add i@-272 1
    instr 154: move (153) i@-272
    instr 155: br [137]
    instr 156: ret 0
    instr 157: nop
