
	.text
	.align 1

song_140_track_0:
	.byte 0xDA
	.byte 0x00
	.byte 0x70
	.byte 0xE5
	.byte 0x77
	.byte 0xDB
	.byte 0x03
	.byte 0xE1
	.byte 0x14
	.byte 0x33
	.byte 0xD8
	.byte 0x0C
	.byte 0xB4
	.byte 0xD5
	.byte 0x20
	.byte 0x30
	.byte 0x40
	.byte 0x50
	.byte 0x6B
	.byte 0x0F
song_140_track_0_goto_0:
	.byte 0xD5
	.byte 0xC3
	.byte 0xA3
	.byte 0x91
	.byte 0x71
	.byte 0x51
	.byte 0x41
	.byte 0x73
	.byte 0x55
	.byte 0x61
	.byte 0x73
	.byte 0x87
	.byte 0xD4
	.byte 0x37
	.byte 0x5F
	.byte 0xFD
	.byte 0x00
	.int song_140_track_0_goto_0

song_140_track_1:
	.byte 0xDB
	.byte 0x03
	.byte 0xE1
	.byte 0x18
	.byte 0x22
	.byte 0xD8
	.byte 0x0C
	.byte 0xC4
	.byte 0xD4
	.byte 0x50
	.byte 0x40
	.byte 0x30
	.byte 0x20
	.byte 0xD5
	.byte 0xCB
	.byte 0x01
	.byte 0x50
	.byte 0x02
	.byte 0x50
	.byte 0x08
song_140_track_1_goto_0:
	.byte 0xD8
	.byte 0x0C
	.byte 0xC4
	.byte 0xD4
	.byte 0x55
	.byte 0x45
	.byte 0x23
	.byte 0xD5
	.byte 0xC1
	.byte 0xA1
	.byte 0x91
	.byte 0x71
	.byte 0x91
	.byte 0xA1
	.byte 0xC1
	.byte 0xD4
	.byte 0x21
	.byte 0xD8
	.byte 0x0C
	.byte 0xC7
	.byte 0x6F
	.byte 0xD8
	.byte 0x0C
	.byte 0xC5
	.byte 0xD5
	.byte 0x77
	.byte 0xD4
	.byte 0x27
	.byte 0xFD
	.byte 0x00
	.int song_140_track_1_goto_0

song_140_track_2:
	.byte 0xD8
	.byte 0x0C
	.byte 0x10
	.byte 0x05
	.byte 0xD4
	.byte 0xC0
	.byte 0x00
	.byte 0x50
	.byte 0x00
	.byte 0xC0
	.byte 0x02
	.byte 0xC0
	.byte 0x00
	.byte 0x50
	.byte 0x00
	.byte 0xC0
	.byte 0x02
	.byte 0xC0
	.byte 0x02
	.byte 0x50
	.byte 0x00
	.byte 0x60
	.byte 0x00
	.byte 0x70
	.byte 0x00
song_140_track_2_goto_0:
	.byte 0x50
	.byte 0x00
	.byte 0xC0
	.byte 0x00
	.byte 0x50
	.byte 0x00
	.byte 0xC0
	.byte 0x00
	.byte 0x50
	.byte 0x00
	.byte 0xC0
	.byte 0x00
	.byte 0x50
	.byte 0x00
	.byte 0xC0
	.byte 0x00
	.byte 0x70
	.byte 0x00
	.byte 0xD3
	.byte 0x20
	.byte 0x00
	.byte 0xD4
	.byte 0x70
	.byte 0x00
	.byte 0xD3
	.byte 0x20
	.byte 0x00
	.byte 0xD4
	.byte 0x70
	.byte 0x00
	.byte 0xD3
	.byte 0x20
	.byte 0x00
	.byte 0xD4
	.byte 0x70
	.byte 0x00
	.byte 0xD3
	.byte 0x20
	.byte 0x00
	.byte 0xD4
	.byte 0x80
	.byte 0x00
	.byte 0xD3
	.byte 0x30
	.byte 0x00
	.byte 0xD4
	.byte 0x80
	.byte 0x00
	.byte 0xD3
	.byte 0x30
	.byte 0x00
	.byte 0xD4
	.byte 0x80
	.byte 0x00
	.byte 0xD3
	.byte 0x30
	.byte 0x00
	.byte 0xD4
	.byte 0x80
	.byte 0x00
	.byte 0xD3
	.byte 0x30
	.byte 0x00
	.byte 0xD4
	.byte 0x70
	.byte 0x00
	.byte 0xD3
	.byte 0x20
	.byte 0x00
	.byte 0xD4
	.byte 0x70
	.byte 0x00
	.byte 0xD3
	.byte 0x20
	.byte 0x00
	.byte 0xD4
	.byte 0x70
	.byte 0x00
	.byte 0xD3
	.byte 0x20
	.byte 0x00
	.byte 0xD4
	.byte 0x70
	.byte 0x00
	.byte 0x40
	.byte 0x00
	.byte 0xFD
	.byte 0x00
	.int song_140_track_2_goto_0



	.align 4
	.global song_140_Header
song_140_Header:
	.byte 0x38
	.byte 0x03
	.short 0x0000
	.int song_140_track_0
	.int 0x00000001
	.int song_140_track_1
	.int 0x00000002
	.int song_140_track_2
