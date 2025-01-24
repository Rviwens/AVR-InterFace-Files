

 const static  uint8_t ASCII[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00,   // space 00  0DEC 20 hexASCII 
	0x00, 0x00, 0x2f, 0x00, 0x00,   // !     01  5  21
	0x00, 0x07, 0x00, 0x07, 0x00,   // "     02  10 22
	0x14, 0x7f, 0x14, 0x7f, 0x14,   // #     03  15 23
	0x24, 0x2a, 0x7f, 0x2a, 0x12,   // $     04  20 24
	0x23, 0x13, 0x08, 0x64, 0x62,   // %     05  25 25
	0x36, 0x49, 0x55, 0x22, 0x50,   // &     06  30 26
	0x00, 0x05, 0x03, 0x00, 0x00,   // '     07  35 27
	0x00, 0x1c, 0x22, 0x41, 0x00,   // (     08  40 28
	0x00, 0x41, 0x22, 0x1c, 0x00,   // )     09  45 29
	0x14, 0x08, 0x3E, 0x08, 0x14,   // *     10  50 2a
	0x08, 0x08, 0x3E, 0x08, 0x08,   // +     11   2b
	0x00, 0x00, 0xA0, 0x60, 0x00,   // ,     12   2c
	0x08, 0x08, 0x08, 0x08, 0x08,   // -     13   2d
	0x00, 0x60, 0x60, 0x00, 0x00,   // .     14   2e
	0x20, 0x10, 0x08, 0x04, 0x02,   // /     15   2f
	0x3E, 0x51, 0x49, 0x45, 0x3E,   // 0     16   30
	0x00, 0x42, 0x7F, 0x40, 0x00,   // 1     17   31
	0x42, 0x61, 0x51, 0x49, 0x46,   // 2     18   32
	0x21, 0x41, 0x45, 0x4B, 0x31,   // 3     19   33
	0x18, 0x14, 0x12, 0x7F, 0x10,   // 4     20   34
	0x27, 0x45, 0x45, 0x45, 0x39,   // 5     21   35
	0x3C, 0x4A, 0x49, 0x49, 0x30,   // 6     22   36
	0x01, 0x71, 0x09, 0x05, 0x03,   // 7     23   37
	0x36, 0x49, 0x49, 0x49, 0x36,   // 8     24   38
	0x06, 0x49, 0x49, 0x29, 0x1E,   // 9     25   39
	0x00, 0x36, 0x36, 0x00, 0x00,   // :     26   3a
	0x00, 0x56, 0x36, 0x00, 0x00,   // ;     27   3b
	0x08, 0x14, 0x22, 0x41, 0x00,   // <     28   3c
	0x14, 0x14, 0x14, 0x14, 0x14,   // =     29   3d
	0x00, 0x41, 0x22, 0x14, 0x08,   // >     30   3e
	0x02, 0x01, 0x51, 0x09, 0x06,   // ?     31   3f
	0x32, 0x49, 0x59, 0x51, 0x3E,   // @     32   40
	0x7C, 0x12, 0x11, 0x12, 0x7C,   // A     33   41
	0x7F, 0x49, 0x49, 0x49, 0x36,   // B     34   42
	0x3E, 0x41, 0x41, 0x41, 0x22,   // C     35   43
	0x7F, 0x41, 0x41, 0x22, 0x1C,   // D     36   44
	0x7F, 0x49, 0x49, 0x49, 0x41,   // E     37   45
	0x7F, 0x09, 0x09, 0x09, 0x01,   // F     38   46
	0x3E, 0x41, 0x49, 0x49, 0x7A,   // G     39   47
	0x7F, 0x08, 0x08, 0x08, 0x7F,   // H     40   48
	0x00, 0x41, 0x7F, 0x41, 0x00,   // I     41   49
	0x20, 0x40, 0x41, 0x3F, 0x01,   // J     42   4a
	0x7F, 0x08, 0x14, 0x22, 0x41,   // K     43   4b
	0x7F, 0x40, 0x40, 0x40, 0x40,   // L     44   4c
	0x7F, 0x02, 0x0C, 0x02, 0x7F,   // M     45   4d
	0x7F, 0x04, 0x08, 0x10, 0x7F,   // N     46   4e
	0x3E, 0x41, 0x41, 0x41, 0x3E,   // O     47   4f
	0x7F, 0x09, 0x09, 0x09, 0x06,   // P     48   50
	0x3E, 0x41, 0x51, 0x21, 0x5E,   // Q     49   51
	0x7F, 0x09, 0x19, 0x29, 0x46,   // R     50   52
	0x46, 0x49, 0x49, 0x49, 0x31,   // S     51   53
	0x01, 0x01, 0x7F, 0x01, 0x01,   // T     52   54
	0x3F, 0x40, 0x40, 0x40, 0x3F,   // U     53   55
	0x1F, 0x20, 0x40, 0x20, 0x1F,   // V     54   56
	0x3F, 0x40, 0x38, 0x40, 0x3F,   // W     55   57
	0x63, 0x14, 0x08, 0x14, 0x63,   // X     56   58
	0x07, 0x08, 0x70, 0x08, 0x07,   // Y     57   59
	0x61, 0x51, 0x49, 0x45, 0x43,   // Z     58   5a
	0x00, 0x7F, 0x41, 0x41, 0x00,   // [     59   5b
	0x55, 0xAA, 0x55, 0xAA, 0x55,   //       60   5c Backslash (Checker pattern)
	0x00, 0x41, 0x41, 0x7F, 0x00,   // ]     61   5d
	0x04, 0x02, 0x01, 0x02, 0x04,   // ^     62   5e
	0x40, 0x40, 0x40, 0x40, 0x40,   // _     63   5f underscore
	0x00, 0x03, 0x05, 0x00, 0x00,   // `     64   60 apostrophe
	0x20, 0x54, 0x54, 0x54, 0x78,   // a     65   61
	0x7F, 0x48, 0x44, 0x44, 0x38,   // b     66   62
	0x38, 0x44, 0x44, 0x44, 0x20,   // c     67   63
	0x38, 0x44, 0x44, 0x48, 0x7F,   // d     68   64
	0x38, 0x54, 0x54, 0x54, 0x18,   // e     69   65
	0x08, 0x7E, 0x09, 0x01, 0x02,   // f     70   66
	0x18, 0xA4, 0xA4, 0xA4, 0x7C,   // g     71   67
	0x7F, 0x08, 0x04, 0x04, 0x78,   // h     72   68
	0x00, 0x44, 0x7D, 0x40, 0x00,   // i     73   69
	0x40, 0x80, 0x84, 0x7D, 0x00,   // j     74   6a
	0x7F, 0x10, 0x28, 0x44, 0x00,   // k     75   6b
	0x00, 0x41, 0x7F, 0x40, 0x00,   // l     76   6c
	0x7C, 0x04, 0x18, 0x04, 0x78,   // m     77   6d
	0x7C, 0x08, 0x04, 0x04, 0x78,   // n     78   6e
	0x38, 0x44, 0x44, 0x44, 0x38,   // o     79   6f
	0xFC, 0x24, 0x24, 0x24, 0x18,   // p     80   70
	0x18, 0x24, 0x24, 0x18, 0xFC,   // q     81   71
	0x7C, 0x08, 0x04, 0x04, 0x08,   // r     82   72
	0x48, 0x54, 0x54, 0x54, 0x20,   // s     83   73
	0x04, 0x3F, 0x44, 0x40, 0x20,   // t     84   74
	0x3C, 0x40, 0x40, 0x20, 0x7C,   // u     85   75
	0x1C, 0x20, 0x40, 0x20, 0x1C,   // v     86   76
	0x3C, 0x40, 0x30, 0x40, 0x3C,   // w     87   77
	0x44, 0x28, 0x10, 0x28, 0x44,   // x     88   78
	0x1C, 0xA0, 0xA0, 0xA0, 0x7C,   // y     89   79
	0x44, 0x64, 0x54, 0x4C, 0x44,   // z     90   7a
	0x00, 0x10, 0x7C, 0x82, 0x00,   // {     91   7b
	0x00, 0x00, 0xFF, 0x00, 0x00,   // |     92   7c
	0x00, 0x82, 0x7C, 0x10, 0x00,   // }     93   7d
    0x00, 0x06, 0x09, 0x09, 0x06
	};
	
	
    



















