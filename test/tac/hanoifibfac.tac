    instr 1: nop
    instr 2: enter 0
    instr 3: cmpeq n@16 0
    instr 4: blbc (3) [8]
    instr 5: add res_base#32720 GP
    instr 6: store 1 (5)
    instr 7: br [16]
    instr 8: sub n@16 1
    instr 9: param (8)
    instr 10: call [2]
    instr 11: add res_base#32720 GP
    instr 12: load (11)
    instr 13: mul n@16 (12)
    instr 14: add res_base#32720 GP
    instr 15: store (13) (14)
    instr 16: ret 8
    instr 17: enter 16
    instr 18: cmple n@16 1
    instr 19: blbc (18) [23]
    instr 20: add res_base#32720 GP
    instr 21: store 1 (20)
    instr 22: br [38]
    instr 23: sub n@16 1
    instr 24: param (23)
    instr 25: call [17]
    instr 26: add res_base#32720 GP
    instr 27: load (26)
    instr 28: move (27) x@-8
    instr 29: sub n@16 2
    instr 30: param (29)
    instr 31: call [17]
    instr 32: add res_base#32720 GP
    instr 33: load (32)
    instr 34: move (33) y@-16
    instr 35: add x@-8 y@-16
    instr 36: add res_base#32720 GP
    instr 37: store (35) (36)
    instr 38: ret 8
    instr 39: enter 0
    instr 40: write from@24
    instr 41: write to@16
    instr 42: wrl
    instr 43: add count_base#32728 GP
    instr 44: load (43)
    instr 45: add (44) 1
    instr 46: add count_base#32728 GP
    instr 47: store (45) (46)
    instr 48: ret 16
    instr 49: enter 0
    instr 50: cmpeq height@16 1
    instr 51: blbc (50) [56]
    instr 52: param from@40
    instr 53: param to@24
    instr 54: call [39]
    instr 55: br [71]
    instr 56: param from@40
    instr 57: param to@24
    instr 58: param by@32
    instr 59: sub height@16 1
    instr 60: param (59)
    instr 61: call [49]
    instr 62: param from@40
    instr 63: param to@24
    instr 64: call [39]
    instr 65: param by@32
    instr 66: param from@40
    instr 67: param to@24
    instr 68: sub height@16 1
    instr 69: param (68)
    instr 70: call [49]
    instr 71: ret 32
    instr 72: enter 0
    instr 73: add count_base#32728 GP
    instr 74: store 0 (73)
    instr 75: param 1
    instr 76: param 2
    instr 77: param 3
    instr 78: param height@16
    instr 79: call [49]
    instr 80: wrl
    instr 81: add count_base#32728 GP
    instr 82: load (81)
    instr 83: write (82)
    instr 84: wrl
    instr 85: ret 8
    instr 86: entrypc
    instr 87: enter 0
    instr 88: add a_base#32760 GP
    instr 89: store 16807 (88)
    instr 90: add m_base#32752 GP
    instr 91: store 127 (90)
    instr 92: add m_base#32752 GP
    instr 93: load (92)
    instr 94: mul (93) 256
    instr 95: add (94) 255
    instr 96: add m_base#32752 GP
    instr 97: store (95) (96)
    instr 98: add m_base#32752 GP
    instr 99: load (98)
    instr 100: mul (99) 256
    instr 101: add (100) 255
    instr 102: add m_base#32752 GP
    instr 103: store (101) (102)
    instr 104: add m_base#32752 GP
    instr 105: load (104)
    instr 106: mul (105) 256
    instr 107: add (106) 255
    instr 108: add m_base#32752 GP
    instr 109: store (107) (108)
    instr 110: add m_base#32752 GP
    instr 111: load (110)
    instr 112: add a_base#32760 GP
    instr 113: load (112)
    instr 114: div (111) (113)
    instr 115: add q_base#32744 GP
    instr 116: store (114) (115)
    instr 117: add m_base#32752 GP
    instr 118: load (117)
    instr 119: add a_base#32760 GP
    instr 120: load (119)
    instr 121: mod (118) (120)
    instr 122: add r_base#32736 GP
    instr 123: store (121) (122)
    instr 124: param 7
    instr 125: call [2]
    instr 126: add res_base#32720 GP
    instr 127: load (126)
    instr 128: write (127)
    instr 129: wrl
    instr 130: wrl
    instr 131: param 11
    instr 132: call [17]
    instr 133: add res_base#32720 GP
    instr 134: load (133)
    instr 135: write (134)
    instr 136: wrl
    instr 137: wrl
    instr 138: param 3
    instr 139: call [72]
    instr 140: wrl
    instr 141: ret 0
    instr 142: nop
