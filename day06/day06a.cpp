#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>

static const char* kInputs[] = {
  "D5Q)KRQ",
  "SDF)J68",
  "Y1M)LWN",
  "7GL)Z6T",
  "HBX)3YJ",
  "66W)HJV",
  "P5J)M38",
  "19D)3WK",
  "Y29)FDW",
  "JRJ)84R",
  "ZNR)Y2H",
  "GXD)68L",
  "SKY)SMP",
  "6Q7)D77",
  "C5C)ZK6",
  "VX2)C32",
  "WV2)DHZ",
  "155)WPJ",
  "XS9)9VG",
  "GV3)LKB",
  "5TP)JGW",
  "JNJ)TJJ",
  "FLF)RY7",
  "R93)MMX",
  "W56)1BV",
  "4G7)L11",
  "RRQ)K5V",
  "NBV)NDZ",
  "JJN)BK5",
  "LLZ)H22",
  "937)NPB",
  "8YH)3JM",
  "WM3)JBB",
  "RHK)V8L",
  "CDC)LQP",
  "V1X)6P6",
  "Y91)CVZ",
  "DRM)GSZ",
  "RHW)DRM",
  "R4N)JTL",
  "8SK)ZY6",
  "GSY)MZK",
  "NQT)LG5",
  "FGL)NQV",
  "WVX)NSP",
  "CV3)BW1",
  "WCX)3CM",
  "R6S)QRF",
  "C4Q)KY8",
  "8F8)6YH",
  "FS3)66T",
  "CT1)LRH",
  "2NN)VXW",
  "9RL)BPB",
  "TVN)5YN",
  "7JL)NSG",
  "N53)6Q7",
  "WYJ)L25",
  "FYV)CB9",
  "NPB)TXF",
  "5HF)4R8",
  "KTF)JCB",
  "TCZ)TS2",
  "MJS)1NB",
  "TS8)DYY",
  "CVZ)QDD",
  "Q8M)SG3",
  "8FX)Z6J",
  "23S)3TG",
  "PJB)MD6",
  "3XZ)1QJ",
  "BXF)B1W",
  "FVK)PPP",
  "LR6)FQ3",
  "ZW5)TKG",
  "5FG)CWX",
  "YMB)51J",
  "CTY)185",
  "N24)7ZR",
  "CWZ)1ZN",
  "TPR)VTB",
  "D8K)26M",
  "1NT)9B4",
  "Y29)ZRF",
  "7FP)RBL",
  "ZT5)KBL",
  "LBG)Q3F",
  "Q82)YTR",
  "LDF)TDR",
  "H21)KQF",
  "2R7)646",
  "ZRS)29Y",
  "2S5)KJ8",
  "K6J)PWF",
  "FH3)W8P",
  "C6J)K3W",
  "X42)FGX",
  "7YK)3T4",
  "F3C)FSV",
  "6G7)25M",
  "XK2)TVN",
  "61W)T6X",
  "9VG)PRQ",
  "KRY)39T",
  "M8B)S6C",
  "1BT)BBX",
  "8VT)J53",
  "M1G)S8W",
  "BHL)ZW5",
  "9TL)JKQ",
  "BN2)CNM",
  "P2D)R7B",
  "7KW)HNJ",
  "KJ8)YJY",
  "FG6)F2Y",
  "98R)X99",
  "QXM)SPY",
  "JCD)Y9M",
  "VQG)7ZW",
  "G5Y)TRG",
  "DYZ)D5Q",
  "R7T)LR6",
  "RNZ)M4C",
  "H4S)SG6",
  "ZK6)QPP",
  "9Y4)WD7",
  "FVN)MMM",
  "FVM)HYR",
  "PN4)R72",
  "FT3)MCX",
  "NCR)YPF",
  "NQS)TVJ",
  "5HP)H7N",
  "51J)BHZ",
  "9S5)RDX",
  "NKC)KX3",
  "N79)8VW",
  "3K8)TJ9",
  "2YN)3WP",
  "S8W)M6M",
  "Z1C)9BS",
  "DYY)3XM",
  "C86)Y1M",
  "HST)QWM",
  "JTL)1F8",
  "YTZ)L63",
  "7D9)MQK",
  "XPC)TM4",
  "5FG)YL5",
  "9XB)YOU",
  "S4Q)FYV",
  "6F3)RZF",
  "1JH)RSF",
  "LBT)PHC",
  "GYD)R4R",
  "KX3)Z44",
  "W39)JPW",
  "398)7FQ",
  "NGK)MJS",
  "H7F)P9T",
  "XSK)4T9",
  "QK5)LMZ",
  "YZD)6S2",
  "VMT)J75",
  "5BG)PTG",
  "LBC)454",
  "6RH)QFJ",
  "JY2)ZBX",
  "R25)RLN",
  "PRH)398",
  "SZB)T9Z",
  "7JD)G1L",
  "J7T)57M",
  "M38)B3J",
  "R67)WNZ",
  "YCV)CHR",
  "5HV)Q8J",
  "2RD)PDK",
  "DQR)PQB",
  "FJK)83V",
  "LRW)88Z",
  "PQB)QT5",
  "WZH)JVN",
  "JPW)6S4",
  "4PR)ZXH",
  "8FJ)P4W",
  "B4M)PV4",
  "TKG)22H",
  "LV9)FCY",
  "KKV)BVV",
  "8QZ)4RL",
  "P9T)MVZ",
  "9GP)B2D",
  "XJJ)FTX",
  "ZLX)RRQ",
  "84R)6L3",
  "9F9)1NT",
  "KQF)KKS",
  "LXX)QK5",
  "1DW)5Z5",
  "SB1)R6W",
  "8XV)QXM",
  "DBC)T7D",
  "L6N)6H4",
  "T5Z)BZ3",
  "MH9)MBS",
  "77R)PVZ",
  "PVT)DMF",
  "FHZ)JNV",
  "MMX)8J2",
  "H5S)PPK",
  "HJS)61M",
  "CDC)TVH",
  "GD2)P66",
  "WPJ)2J8",
  "4BS)6RY",
  "7S1)59M",
  "F8C)TGL",
  "GXY)6NL",
  "42B)WYJ",
  "59M)L12",
  "QFJ)28H",
  "H7T)KTF",
  "MD6)Y42",
  "SM4)3VC",
  "5Z4)DYZ",
  "W62)YZD",
  "RVZ)LLW",
  "PNS)S4Q",
  "FFM)X2S",
  "174)R69",
  "5PW)GJD",
  "P66)ZJB",
  "X99)QV4",
  "2QR)M8L",
  "MMB)WRB",
  "B2L)SAN",
  "42B)FPK",
  "GNP)NNJ",
  "JXR)6NB",
  "PXX)NCR",
  "FDX)9F9",
  "BN1)61B",
  "YH8)BNB",
  "FGN)G9P",
  "TBB)YNR",
  "6NL)9ZJ",
  "BKD)BN1",
  "1SS)9K9",
  "N2F)KV3",
  "LRH)TT3",
  "99M)MNJ",
  "GJB)CCQ",
  "RY7)PWH",
  "RKL)Y1X",
  "K13)P73",
  "H7D)RKH",
  "TNN)YTZ",
  "FRH)Y5K",
  "YST)5QS",
  "L7W)7CG",
  "WN1)SJC",
  "NDZ)8D2",
  "XSK)G14",
  "NS7)1XK",
  "6JV)5SD",
  "G9P)XMJ",
  "W7Y)5HM",
  "9F9)1XR",
  "RCM)2G5",
  "F8F)4JJ",
  "YTW)L91",
  "RBM)GJB",
  "6CD)WJ5",
  "X7N)8TQ",
  "TFB)XCK",
  "QT5)4T6",
  "J31)F85",
  "3C6)ZTS",
  "P1D)XPY",
  "Y9M)ZRT",
  "J4N)YWS",
  "G6R)KZ5",
  "HWY)ZJ3",
  "VJK)QKJ",
  "NSG)4BS",
  "L8G)XS9",
  "TDR)42B",
  "D89)9ZK",
  "T54)DRG",
  "Z4W)XNL",
  "9J3)YVP",
  "K6S)S2C",
  "SGQ)9FN",
  "FMD)34X",
  "YJY)8FJ",
  "8TQ)JNT",
  "R82)M1G",
  "M9Y)FGL",
  "7MQ)1FZ",
  "6XQ)R88",
  "JDY)174",
  "84H)KKV",
  "8XZ)WZH",
  "TLK)SDF",
  "Z52)TB9",
  "WRJ)N2F",
  "FPL)8XZ",
  "Y5K)595",
  "4KF)6Y2",
  "J2N)YMB",
  "G5Y)MXG",
  "85D)LRW",
  "XXF)KJR",
  "NS5)4SL",
  "X6X)RJK",
  "J53)6CG",
  "Q82)LR9",
  "L91)TGV",
  "H7B)PKX",
  "4BM)XJJ",
  "2DG)D99",
  "2G5)PBF",
  "J2T)FGR",
  "WVL)7YP",
  "W2S)H1Q",
  "MD2)7GL",
  "JLX)GSB",
  "VNB)662",
  "PNY)39H",
  "B5S)TXR",
  "QR4)4XP",
  "31L)R93",
  "C32)SN8",
  "9ZJ)F9P",
  "MPM)TFB",
  "MBN)LBT",
  "FGX)6CB",
  "DXH)D8K",
  "46T)VQ6",
  "G2J)YBJ",
  "ZHV)N8D",
  "39T)LGY",
  "R6W)FXJ",
  "WD7)HKR",
  "SG3)HKK",
  "5BL)JGS",
  "H4D)DGS",
  "G1Y)H5S",
  "VCY)M9Y",
  "9BS)FT1",
  "YLW)6QN",
  "ZSG)5QX",
  "27Q)KPT",
  "RDK)267",
  "VQG)RZH",
  "6CB)GZZ",
  "COM)FPL",
  "6KC)BYY",
  "Y2H)BLN",
  "W45)L6N",
  "RJK)H7D",
  "M1G)XK2",
  "TYY)LDF",
  "C86)3GY",
  "FXJ)WJ2",
  "NGK)5TP",
  "MD6)FR2",
  "ZZQ)3LZ",
  "S4D)SZ4",
  "SPY)W2S",
  "83R)82B",
  "CCQ)K34",
  "Y3S)YHD",
  "S47)ZJX",
  "HJM)HTF",
  "LW8)9GP",
  "KWC)FDX",
  "H4D)4K7",
  "5CH)F47",
  "PKX)XXF",
  "JCB)G2J",
  "TCC)M6T",
  "SN8)GFG",
  "J8P)7GY",
  "SJC)3BL",
  "7JB)K7L",
  "45Q)F3C",
  "2R1)JSC",
  "ZN9)4PR",
  "HJM)8SK",
  "R88)DCN",
  "FS3)D89",
  "R12)DZB",
  "TJJ)MD2",
  "HRD)6RH",
  "HWM)BHB",
  "Z23)NJN",
  "DW3)R9X",
  "3LZ)LJ4",
  "MXG)QDY",
  "RV1)XH5",
  "SGD)R2G",
  "PH8)GD2",
  "MSN)17P",
  "7RC)DV8",
  "H8Q)BHL",
  "BLN)BYL",
  "ZCW)GNP",
  "FDW)5JY",
  "H14)R82",
  "LG5)B5S",
  "LYX)P6Q",
  "BXF)JJN",
  "89Y)T54",
  "XM2)7JB",
  "L11)3W2",
  "PWH)LW8",
  "QXM)C5C",
  "G1L)6BT",
  "2H4)SRJ",
  "V58)J2Q",
  "TM4)CXW",
  "HL6)ZSG",
  "2TK)NLN",
  "M1Z)F3D",
  "S16)NK9",
  "4BW)TYY",
  "JRZ)V14",
  "5VN)ZWD",
  "ZY6)2YC",
  "5JY)2D9",
  "DLM)JCK",
  "SBN)83K",
  "D48)ZZY",
  "Q8J)BKD",
  "3RD)H7T",
  "WX6)NGX",
  "3YJ)CLC",
  "CWX)G6R",
  "83K)S36",
  "SMT)6F3",
  "6NF)ZNR",
  "MBS)DY8",
  "26S)89C",
  "595)S15",
  "RPR)2CG",
  "2TM)8PF",
  "LL3)XLW",
  "CDQ)S9Y",
  "9JL)KZT",
  "FGR)8LH",
  "HJV)RHW",
  "T27)VZ9",
  "21S)XVM",
  "FSK)YQT",
  "PM3)37N",
  "H3C)9VW",
  "PHC)5HP",
  "N34)3XZ",
  "5PW)6NP",
  "JRP)M8B",
  "CNM)S3N",
  "P4W)MN8",
  "G6P)N79",
  "YHD)4MG",
  "ZN6)TR2",
  "DKH)WXS",
  "Y52)FZ4",
  "J1Y)MMB",
  "3CY)R67",
  "9B4)NBQ",
  "QKJ)GWX",
  "WNZ)W39",
  "NLQ)JSG",
  "JKQ)TTD",
  "6SY)98R",
  "1VF)WN1",
  "N2F)3JS",
  "QGY)LB7",
  "WQ7)QYH",
  "4T9)CWZ",
  "D8Y)HRD",
  "CK7)94C",
  "17C)VK9",
  "1YX)N24",
  "JGS)TCT",
  "WM9)2R1",
  "3BL)HD9",
  "P61)L21",
  "SRJ)2NN",
  "DV8)SVZ",
  "SG5)J1Y",
  "KJR)YH8",
  "9VW)H7F",
  "ZN6)7YK",
  "M1S)NQT",
  "R4R)3HQ",
  "R6S)KM7",
  "KV3)V5Q",
  "281)K31",
  "L44)MDD",
  "ZJ3)FKW",
  "6T5)K2Q",
  "JNT)WZ4",
  "5BX)27Q",
  "ZGY)W62",
  "6DT)G3P",
  "KPT)SYL",
  "BW1)9RX",
  "KT4)M92",
  "M6M)DDY",
  "QFK)R8G",
  "1Z2)115",
  "8D2)R4N",
  "V4B)6KC",
  "1W6)JSL",
  "185)T27",
  "HYR)G5Y",
  "CXW)45Q",
  "7CG)9RR",
  "P94)Y6B",
  "MDD)2F8",
  "TXR)CTY",
  "61B)8XV",
  "6JG)CXY",
  "6H4)PW9",
  "3WP)VMT",
  "9K2)715",
  "CL9)RDK",
  "Z1C)NS7",
  "2JB)JRP",
  "YSS)M48",
  "ZQN)92R",
  "4R8)76L",
  "3W2)P61",
  "JSG)VQG",
  "MLP)RCB",
  "4RL)4G7",
  "MP8)SB1",
  "4T6)VRC",
  "DLZ)G1Y",
  "G9C)5HF",
  "NGX)BLQ",
  "FTX)N34",
  "H21)D1F",
  "YZS)7FP",
  "479)7KT",
  "GJD)3C6",
  "333)7DX",
  "J66)X6X",
  "MZK)1P8",
  "1P8)W7Y",
  "2YC)F62",
  "TTD)4XH",
  "NS4)DQ9",
  "X6Q)MK6",
  "PWK)D48",
  "59M)W45",
  "FX8)6N9",
  "XLW)8LR",
  "ZGY)YLC",
  "2S5)HWK",
  "K53)MBF",
  "DQ9)5ZK",
  "MF2)Q8M",
  "DZB)479",
  "G6M)9SR",
  "ZBX)464",
  "2PF)9B6",
  "LZP)2QR",
  "VJK)L7S",
  "TR7)5XP",
  "FTK)TFG",
  "TP3)PCL",
  "J9L)6ZL",
  "F3D)GP4",
  "J68)DBC",
  "BJ9)61W",
  "94C)MPM",
  "CXW)ZT5",
  "L63)K64",
  "RBL)MRY",
  "5YN)ZGY",
  "V2F)6QC",
  "GS2)WSY",
  "FK5)LL3",
  "4K7)K6S",
  "K3Y)H4D",
  "7ZW)Y8B",
  "FT1)Y7N",
  "TT3)8WV",
  "JNV)775",
  "2MF)NQS",
  "LBG)HDV",
  "YTW)J9F",
  "BKX)TNN",
  "JZW)WGJ",
  "S36)Z4W",
  "5BP)MLP",
  "267)B3P",
  "YQY)1L4",
  "ZWD)4Y4",
  "GP4)2RD",
  "JP9)GSS",
  "5XP)9TL",
  "8BT)C8X",
  "8HR)3G5",
  "4R9)4BW",
  "3VC)HBX",
  "D1F)5BX",
  "WXS)L7W",
  "YCR)YY3",
  "SG3)7KV",
  "8NQ)K53",
  "RBL)1P4",
  "3GY)VX4",
  "XZ4)D2S",
  "2MF)ZKW",
  "4BX)B3B",
  "GX1)5VN",
  "HTF)8F8",
  "LQP)HW5",
  "KP4)9K2",
  "MRJ)YTW",
  "RSW)J1K",
  "715)C86",
  "VXG)28F",
  "S3N)5BG",
  "YTR)TCC",
  "84C)ZRH",
  "WJ2)7WL",
  "94Q)F7N",
  "QDY)XSK",
  "CHL)47S",
  "H2D)PM3",
  "HNJ)MH9",
  "4XH)HXG",
  "YPF)JY2",
  "2F8)VC9",
  "78M)Y91",
  "Y1F)Z1C",
  "V75)RCM",
  "GMQ)2MF",
  "ZJB)XJ4",
  "9DR)M1S",
  "82B)VX2",
  "DFL)CL9",
  "3F9)747",
  "8M7)RLH",
  "R2G)Y6D",
  "ZY6)YNJ",
  "GFG)1K7",
  "D2S)J41",
  "9K9)X9L",
  "ZYZ)1Z2",
  "BHF)S47",
  "HKR)NY7",
  "WGJ)SBN",
  "QYH)FCJ",
  "H37)SYZ",
  "42T)155",
  "NLN)BXF",
  "56C)1NZ",
  "1XR)X2H",
  "6CB)HST",
  "MP8)G6P",
  "P62)FTV",
  "XH5)WVL",
  "F2Y)XM2",
  "7ZW)WBZ",
  "MN8)NBV",
  "C3S)XLF",
  "X9L)CB5",
  "8FC)GR5",
  "3F7)SG5",
  "3W2)NH8",
  "Z44)NNY",
  "P73)MBN",
  "LMZ)2FC",
  "7VB)TPR",
  "3TG)96K",
  "Z8S)6ZG",
  "MBN)2NC",
  "Y42)8X9",
  "K7L)M3T",
  "68L)X6L",
  "JV3)2M5",
  "XNM)C4Q",
  "L6X)3F9",
  "JY2)5QL",
  "9GP)H8Q",
  "M92)5Z4",
  "1HL)9DR",
  "7FQ)FK5",
  "T9T)J8P",
  "M4C)GS2",
  "PVH)83R",
  "T9Z)4BM",
  "ZTS)ZMJ",
  "F4V)8QZ",
  "JDY)6MF",
  "S15)WX6",
  "KJR)YKV",
  "K52)6VX",
  "6NB)MP8",
  "9FN)CK6",
  "FWB)8YH",
  "R69)2X8",
  "M6T)281",
  "464)Y52",
  "2R7)94Q",
  "3G5)GX1",
  "JGW)FSK",
  "26M)TLK",
  "QMC)KWC",
  "F47)V58",
  "T6X)ZCW",
  "PDK)F4V",
  "TYY)NWR",
  "Y41)CDC",
  "HLY)Y3S",
  "77R)YD3",
  "TBG)H2D",
  "NSP)833",
  "K64)F44",
  "VW6)H14",
  "SX6)X11",
  "5QS)HLY",
  "S2C)VRG",
  "2J6)RKL",
  "1BV)V79",
  "6BT)YTP",
  "NJN)FWN",
  "NTC)S12",
  "X2S)R5C",
  "1X2)1LT",
  "5MS)SZW",
  "F44)G6W",
  "P2D)YST",
  "BHB)CK7",
  "GRL)BN2",
  "X6L)GBZ",
  "3JM)L44",
  "29Y)J2T",
  "FGX)GP5",
  "7KV)FJZ",
  "5ZK)KVW",
  "T6M)KRD",
  "RZH)P8J",
  "ZJX)KKR",
  "3G5)6NF",
  "BYY)L8G",
  "TR2)93G",
  "NHX)7D9",
  "SZW)F3B",
  "Y1X)8V9",
  "VJZ)Y3X",
  "N2Q)BLB",
  "775)Y29",
  "R72)3P7",
  "B3P)ZC2",
  "KZ5)GMM",
  "GNH)66W",
  "9SR)21S",
  "RPR)6JG",
  "PTG)9FR",
  "YHD)KT4",
  "31F)TG2",
  "M48)FMD",
  "S9Y)3RD",
  "L4F)51M",
  "TFG)FX8",
  "BQ6)MQN",
  "Y7G)XWP",
  "F85)P2G",
  "WRB)RCW",
  "GRX)HL6",
  "FZ4)CT1",
  "DMF)S4D",
  "4Y4)JV3",
  "83V)CLY",
  "NQV)KXC",
  "C8Y)1DW",
  "RSF)F8C",
  "4JJ)QH6",
  "W9T)GNV",
  "8Y7)FNK",
  "8J2)Q3M",
  "8QZ)89Y",
  "DHZ)4R9",
  "MNJ)9J3",
  "SG6)Z52",
  "RSC)1YX",
  "2D9)26S",
  "92R)NP6",
  "VZ9)VW6",
  "LPP)9JL",
  "ZMJ)DZH",
  "F7N)YSS",
  "8FX)17C",
  "KRQ)2JB",
  "MNP)1JH",
  "DHZ)B4M",
  "BH4)CQ3",
  "F5Z)7F4",
  "MBF)RB7",
  "Z6T)W1N",
  "6ZL)3Z4",
  "RWP)K52",
  "84Z)C38",
  "3Z4)HJM",
  "FR2)GXY",
  "VRC)F5Z",
  "NBM)6XQ",
  "4XP)FVN",
  "PVB)5BL",
  "99B)DXN",
  "1P4)TW1",
  "Y7N)T2K",
  "FPK)LLZ",
  "JVN)BBB",
  "GWX)GMQ",
  "GMM)5MS",
  "FQ1)R7T",
  "4R8)N53",
  "2FC)8YG",
  "L7S)R25",
  "NWR)QFK",
  "JP9)J9L",
  "DXN)P2D",
  "JCK)822",
  "4HM)L4F",
  "ZC2)ZRJ",
  "MCX)J7T",
  "93G)QMC",
  "QPP)H54",
  "W1N)K13",
  "K2Q)5ZC",
  "TXF)6CD",
  "HDV)56C",
  "753)CPP",
  "822)VNB",
  "BZ3)YCV",
  "RCB)GBQ",
  "YL5)C6J",
  "R5C)R6S",
  "MM8)H37",
  "JBB)RRR",
  "DSL)SMT",
  "RKH)VXG",
  "3T4)MM6",
  "PJB)9GT",
  "RDX)TP3",
  "6XT)MMN",
  "3DH)NS5",
  "YNJ)S7L",
  "D77)GV3",
  "7GY)VJK",
  "TRG)PXX",
  "PW9)9GQ",
  "DCN)G7J",
  "PRQ)B2L",
  "Y6D)8NQ",
  "8PF)P62",
  "PVZ)ZQN",
  "ZYZ)JNJ",
  "TCT)2YN",
  "D8M)9TJ",
  "4MG)4HM",
  "6VX)K6J",
  "QV3)F9W",
  "8DB)W9T",
  "TB9)PNY",
  "NH6)4BX",
  "5ZC)SM4",
  "6QC)MRJ",
  "MM6)GN7",
  "HWK)M1Z",
  "RHW)LH3",
  "H5S)XZ4",
  "57M)PJW",
  "9RX)MRR",
  "R7B)3TZ",
  "N9L)CP4",
  "TGD)YLW",
  "66T)K7M",
  "HV9)WM9",
  "76X)DW3",
  "XVM)P1D",
  "RLN)J7M",
  "NY7)DLM",
  "HKK)N2Q",
  "SZ4)XS1",
  "LWN)WRJ",
  "1ZD)LSS",
  "9GT)7KW",
  "K13)NS4",
  "RF5)S16",
  "F9P)P94",
  "323)XNM",
  "G14)93M",
  "3VC)WV2",
  "LB7)8Y7",
  "2X8)HV9",
  "GR5)7MQ",
  "NBQ)QV3",
  "PV4)FJK",
  "7F4)23S",
  "C44)K3Y",
  "1QJ)6GG",
  "KVW)2HC",
  "54J)25F",
  "16R)PN4",
  "K7M)JXR",
  "XMM)1HL",
  "WBZ)HJS",
  "GSS)CPD",
  "F4F)ZN9",
  "V9S)GRL",
  "MRR)DVF",
  "9HK)DSP",
  "MQN)JDY",
  "BVV)Q5Z",
  "NQS)SKY",
  "FTV)RDV",
  "KRD)DQR",
  "XLF)RVZ",
  "MQM)3DH",
  "NNM)ZZQ",
  "WYN)9XB",
  "28H)H3C",
  "CV9)V75",
  "7ZR)PWK",
  "52B)NBM",
  "4SL)NQF",
  "PPP)6G3",
  "GZZ)QP4",
  "RWT)R5S",
  "MBM)6T5",
  "VX4)GXD",
  "GZP)L6X",
  "2HC)NGK",
  "Z6J)SGD",
  "5TP)HWM",
  "FJZ)FQ1",
  "1LR)Z1P",
  "ZKW)X7N",
  "61M)1W6",
  "YLC)Q82",
  "3XM)FHZ",
  "VK9)TR7",
  "Z88)ZBV",
  "BBB)TS8",
  "GSZ)3CY",
  "WD6)RWT",
  "TFG)LXX",
  "LH3)N9L",
  "WSY)45M",
  "Y4P)FVK",
  "NNY)JCD",
  "J9F)Q14",
  "C5C)VZJ",
  "LJ4)2PF",
  "KKR)SKM",
  "S7L)5FG",
  "DY8)MK7",
  "51M)5CH",
  "CLY)4QQ",
  "KZT)46T",
  "KMG)9HK",
  "93G)DSL",
  "NH8)MF2",
  "45M)WM3",
  "H54)42T",
  "RLH)H7B",
  "17P)8SW",
  "V8L)WQ7",
  "F3B)2H5",
  "5Z5)PVH",
  "1FZ)54J",
  "6N9)BKX",
  "Z6H)2BX",
  "34X)WJF",
  "RZF)RBM",
  "PLH)FW4",
  "L21)SX6",
  "W7Y)6G7",
  "GP5)W56",
  "P6Q)7VB",
  "J75)BR5",
  "MCF)84Z",
  "K31)4SC",
  "MM8)NLQ",
  "47S)FH3",
  "MDD)DM4",
  "7S5)FY1",
  "GBQ)MCF",
  "YNR)8M7",
  "9FR)RSW",
  "ZSS)LBG",
  "KZT)PVB",
  "JRZ)RPR",
  "39H)TP9",
  "7QR)1BT",
  "6CG)2TM",
  "VRG)8DB",
  "TG2)TCZ",
  "6RY)XPG",
  "HW5)C44",
  "89C)WDT",
  "XNL)52B",
  "R5S)C3S",
  "8SW)8FC",
  "H1Q)MSN",
  "DGS)PZ7",
  "DDY)SVB",
  "J68)RNZ",
  "7DX)2J6",
  "XS1)4TN",
  "646)8VT",
  "HXG)HWY",
  "LVH)QXR",
  "XWP)7JL",
  "GSZ)71W",
  "4WM)WVX",
  "ZXH)ZN6",
  "NNJ)ZYZ",
  "B2D)DXH",
  "BYL)DJG",
  "ZRH)ZHV",
  "M8L)WCX",
  "GXG)DLZ",
  "N8D)VJZ",
  "833)MBM",
  "ZX6)X6Q",
  "SZB)NYT",
  "X2H)G6M",
  "NQF)C8Y",
  "VXW)3QV",
  "BP1)CV3",
  "5HM)8BT",
  "1F8)FFM",
  "K3W)GSY",
  "GXG)FVM",
  "747)6K4",
  "2J6)753",
  "2JV)Q1H",
  "YHK)19D",
  "R8G)KT5",
  "JGS)T5Z",
  "S12)DBK",
  "ZRJ)RF5",
  "J1K)W3D",
  "GBZ)GZP",
  "25F)C7B",
  "SVB)8FX",
  "VQ6)Z8S",
  "C44)KRY",
  "115)SGQ",
  "YVP)4WM",
  "J8P)323",
  "T2K)85D",
  "1ZN)LZP",
  "BVT)FS3",
  "8V9)7RC",
  "D99)NHX",
  "2J8)Z8M",
  "5QL)1LR",
  "TJ9)P5J",
  "4K7)KP4",
  "PPK)9RL",
  "3WK)KMG",
  "37N)1YC",
  "KBL)J2N",
  "GNV)MZZ",
  "CB5)RV1",
  "Q14)H9P",
  "6NP)6HF",
  "LVH)LPP",
  "KKR)2H4",
  "2NC)RSC",
  "9GQ)HVW",
  "5SD)FT3",
  "GN7)WLF",
  "Q3F)1X2",
  "F9W)J4N",
  "H9P)BVT",
  "2CG)1ZD",
  "3JS)CHL",
  "H7B)NTC",
  "QP4)C2K",
  "KKS)1ZX",
  "Q1H)1VF",
  "9TJ)RWP",
  "LSS)CH2",
  "MVZ)7QR",
  "VZJ)8HR",
  "ZRT)5LC",
  "YTP)GYD",
  "1LT)NKC",
  "TS2)YZS",
  "DXH)V2F",
  "G6R)X42",
  "LR9)F2W",
  "SMP)31L",
  "25M)HCL",
  "G7J)2S5",
  "C8X)ZLX",
  "Z42)V4B",
  "MMM)6JV",
  "G3P)JRZ",
  "J7M)CV9",
  "ZBV)GRX",
  "Q3M)9Y4",
  "9B6)C4H",
  "D8D)PVT",
  "ZRF)TBG",
  "WDT)3F7",
  "22H)YCR",
  "TW1)TBB",
  "DSP)ZX7",
  "R9X)GS5",
  "NYT)WD6",
  "WJF)LYX",
  "4PR)FWB",
  "B5V)5PW",
  "6HF)V9S",
  "V5Q)V1X",
  "HCL)333",
  "CXY)7BN",
  "P8J)Y7G",
  "V14)JP9",
  "PWF)WYN",
  "MK6)FTK",
  "YQT)76X",
  "398)PJB",
  "MRY)DNY",
  "1XK)65K",
  "6P6)ZSS",
  "CHR)LV9",
  "CPP)J31",
  "6Y2)NR6",
  "3Z3)QR4",
  "17P)7JD",
  "CQ3)PNS",
  "FKW)PH8",
  "K34)76M",
  "TGV)KYG",
  "T7D)H21",
  "9SR)VCY",
  "W1N)G46",
  "2BX)MM8",
  "BK5)1SS",
  "4SC)M88",
  "6MF)Y41",
  "28F)84H",
  "6S2)BP1",
  "XJ4)77R",
  "P2G)6FW",
  "C2K)R9Q",
  "KM7)GNH",
  "6S4)5WF",
  "DNY)QGY",
  "8YG)937",
  "9RR)HQQ",
  "6L3)F4F",
  "N79)FRH",
  "6QN)T83",
  "MQK)LVH",
  "TVH)9S5",
  "YWS)FQJ",
  "L12)CDQ",
  "454)X4T",
  "2D9)FG6",
  "BHZ)LBC",
  "7BN)BQ6",
  "TGL)78M",
  "BLQ)84C",
  "4T6)DFL",
  "PCL)7S5",
  "QWM)J66",
  "CDQ)6SY",
  "TB9)BHF",
  "5QX)6XT",
  "71W)H4S",
  "PZ7)F8F",
  "7YP)RHK",
  "FWN)FLF",
  "LFN)BJ9",
  "RRR)SZB",
  "1YC)2DG",
  "QPP)TGD",
  "ZZY)YHK",
  "1L4)R12",
  "1ZX)4KF",
  "CPD)BH4",
  "YD3)99M",
  "F62)6RS",
  "WQQ)Z88",
  "QFJ)XMM",
  "QV4)JRJ",
  "DRG)5BP",
  "H1Q)NH6",
  "7FP)ZRS",
  "HQQ)MQM",
  "4TN)3K8",
  "D77)MNP",
  "HL6)Y1F",
  "37Z)NNM",
  "KT5)6DT",
  "BPB)JLX",
  "QXR)B5V",
  "FK5)T9T",
  "1K7)JZW",
  "GS5)ZX6",
  "DJG)D8Y",
  "LLW)T6M",
  "YNR)D8M",
  "ZZQ)16R",
  "G46)DKH",
  "H7N)Z6H",
  "CLC)Z23",
  "QH6)CFS",
  "FQJ)WQQ",
  "WVL)G9C",
  "DM4)2JV",
  "BNB)Y4P",
  "ZX7)5HV",
  "R9Q)99B",
  "KY8)23H",
  "L25)GXG",
  "H7F)K4G",
  "W3D)LFN",
  "YKV)PLH",
  "W45)2R7",
  "B3J)XPC",
  "C38)D8D",
  "FSV)7S1",
  "M92)892",
  "NP6)BXP",
  "8X9)FGN",
  "NK9)PRH",
  "MMN)Z42",
  "8LR)31F",
  "Y8B)YQY",
  "QV4)37Z",
  "S7L)2TK",
  "K5V)Y3T",
  "HLY)3Z3",
};


int main(int argc, char** argv)
{
  std::map<std::string, size_t> ids;
  std::vector<size_t> orbits;

  const size_t numOrbits = sizeof(kInputs) / sizeof(kInputs[0]);
  orbits.reserve(numOrbits);

  ids["COM"] = 0;
  orbits.push_back(0);
  for (size_t i = 0; i < numOrbits; i++) {
    const char* input = kInputs[i];
    if (input[3] != ')') {
      fprintf(stderr, "Orbit labels aren't all length 3: %s\n", input);
      return EXIT_FAILURE;
    }

    std::string parentName(input, 3);
    std::string childName(input + 4, 3);

    size_t parent;
    if (ids.count(parentName) == 0) {
      parent = orbits.size();
      ids[parentName] = parent;
      orbits.push_back(0);
    }
    else {
      parent = ids[parentName];
    }

    size_t child;
    if (ids.count(childName) == 0) {
      child = orbits.size();
      ids[childName] = child;
      orbits.push_back(parent);
    }
    else {
      child = ids[childName];
      orbits[child] = parent;
    }
  }

  uint64_t total = 0;
  for (size_t i = 0; i < orbits.size(); i++) {
    size_t j = i;
    while (orbits[j] != j) {
      j = orbits[j];
      ++total;
    }
  }

  printf("Answer is: %llu\n", total);
  return EXIT_SUCCESS;
}
