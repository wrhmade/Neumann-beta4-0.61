TOOLPATH = ../z_tools/
TOOLPATH_WIN = ..\z_tools
INCPATH  = ../z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
ULTRAISO   = $(TOOLPATH_WIN)\ultraiso\ultraiso.exe
COPY     = copy
DEL      = del
CMD	 = cmd /c

# デフォルト動作

default :
	$(MAKE) bootfd.img

# ファイル生成規則

bootfd.img : krnlia32/ipl20.bin krnlia32/krnlia32.bin Makefile \
		a/a.prg hello3/hello3.prg hello4/hello4.prg hello5/hello5.prg \
		winhelo/winhelo.prg winhelo2/winhelo2.prg winhelo3/winhelo3.prg \
		star1/star1.prg stars/stars.prg stars2/stars2.prg \
		lines/lines.prg walk/walk.prg noodle/noodle.prg \
		beepdown/beepdown.prg color/color.prg color2/color2.prg \
		sosu/sosu.prg sosu2/sosu2.prg sosu3/sosu3.prg \
		typeipl/typeipl.prg type/type.prg iroha/iroha.prg \
		chklang/chklang.prg
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:krnlia32/ipl20.bin len:512 from:0 to:0 \
		copy from:krnlia32/krnlia32.bin to:@: \
		copy from:krnlia32/ipl20.nas to:@: \
		copy from:pictdata/desktop1.jpg to:@: \
		copy from:make.bat to:@: \
		copy from:lines/lines.prg to:@: \
		copy from:type/type.prg to:@: \
		copy from:iroha/iroha.prg to:@: \
		copy from:chklang/chklang.prg to:@: \
		copy from:euc.txt to:@: \
		copy from:nihongo/HZK16.hzk to:@: \
		copy from:tview/tview.prg to:@: \
		copy from:calc/calc.prg to:@: \
		copy from:bball/bball.prg to:@: \
		copy from:invader/invader.prg to:@: \
		copy from:fcount/fcount.prg to:@: \
		copy from:gview/gview.prg to:@: \
		copy from:pictdata/qz.jpg to:@: \
		copy from:pictdata/night.bmp to:@: \
		copy from:pictdata/fujisan.jpg to:@: \
		copy from:pictdata/test.jpg to:@: \
		copy from:mmlplay/mmlplay.prg to:@: \
		copy from:mmldata/daigo.mml to:@: \
		copy from:mmldata/daiku.mml to:@: \
		copy from:mmldata/fujisan.mml to:@: \
		copy from:mmldata/kirakira.mml to:@: \
		copy from:mstest/mstest.prg to:@: \
		copy from:lnks/intro.lnk to:@: \
		copy from:sysintro.txt to:@: \
		copy from:pictdata/tam.jpg to:@: \
		copy from:pictdata/zz.jpg to:@: \
		imgout:bootfd.img
# コマンド

run :
	$(MAKE) bootfd.img
	$(COPY) bootfd.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install :
	$(MAKE) bootfd.img
	$(IMGTOL) w a: bootfd.img

full :
	$(MAKE) -C krnlia32
	$(MAKE) -C apilib
	$(MAKE) -C a
	$(MAKE) -C hello3
	$(MAKE) -C hello4
	$(MAKE) -C hello5
	$(MAKE) -C winhelo
	$(MAKE) -C winhelo2
	$(MAKE) -C winhelo3
	$(MAKE) -C star1
	$(MAKE) -C stars
	$(MAKE) -C stars2
	$(MAKE) -C lines
	$(MAKE) -C walk
	$(MAKE) -C noodle
	$(MAKE) -C beepdown
	$(MAKE) -C color
	$(MAKE) -C color2
	$(MAKE) -C sosu
	$(MAKE) -C sosu2
	$(MAKE) -C sosu3
	$(MAKE) -C typeipl
	$(MAKE) -C type
	$(MAKE) -C iroha
	$(MAKE) -C chklang
	$(MAKE) -C asciichr
	$(MAKE) -C argstest
	$(MAKE) -C tview
	$(MAKE) -C calc
	$(MAKE) -C bball
	$(MAKE) -C invader
	$(MAKE) -C fcount
	$(MAKE) -C gview
	$(MAKE) -C bainian
	$(MAKE) -C counter
	$(MAKE) -C crach1
	$(MAKE) -C mmlplay
	$(MAKE) -C mstest
	$(MAKE) -C mstest2
	$(MAKE) -C input
	$(MAKE) bootfd.img

run_full :
	$(MAKE) full
	$(COPY) bootfd.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install_full :
	$(MAKE) full
	$(IMGTOL) w a: bootfd.img

run_os :
	$(MAKE) -C krnlia32
	$(MAKE) run

clean :
# 何もしない

src_only :
	$(MAKE) clean
	-$(DEL) bootfd.img

clean_full :
	$(MAKE) -C krnlia32		clean
	$(MAKE) -C apilib		clean
	$(MAKE) -C a			clean
	$(MAKE) -C hello3		clean
	$(MAKE) -C hello4		clean
	$(MAKE) -C hello5		clean
	$(MAKE) -C winhelo		clean
	$(MAKE) -C winhelo2		clean
	$(MAKE) -C winhelo3		clean
	$(MAKE) -C star1		clean
	$(MAKE) -C stars		clean
	$(MAKE) -C stars2		clean
	$(MAKE) -C lines		clean
	$(MAKE) -C walk			clean
	$(MAKE) -C noodle		clean
	$(MAKE) -C beepdown		clean
	$(MAKE) -C color		clean
	$(MAKE) -C color2		clean
	$(MAKE) -C sosu			clean
	$(MAKE) -C sosu2		clean
	$(MAKE) -C sosu3		clean
	$(MAKE) -C typeipl		clean
	$(MAKE) -C type			clean
	$(MAKE) -C iroha		clean
	$(MAKE) -C chklang		clean
	$(MAKE) -C asciichr		clean
	$(MAKE) -C argstest		clean
	$(MAKE) -C tview		clean
	$(MAKE) -C calc			clean
	$(MAKE) -C bball		clean
	$(MAKE) -C invader		clean
	$(MAKE) -C fcount		clean
	$(MAKE) -C gview		clean
	$(MAKE) -C bainian		clean
	$(MAKE) -C counter		clean
	$(MAKE) -C crach1		clean
	$(MAKE) -C mmlplay		clean
	$(MAKE) -C mstest		clean
	$(MAKE) -C mstest2		clean
	$(MAKE) -C input 		clean

src_only_full :
	$(MAKE) -C krnlia32		src_only
	$(MAKE) -C apilib		src_only
	$(MAKE) -C a			src_only
	$(MAKE) -C hello3		src_only
	$(MAKE) -C hello4		src_only
	$(MAKE) -C hello5		src_only
	$(MAKE) -C winhelo		src_only
	$(MAKE) -C winhelo2		src_only
	$(MAKE) -C winhelo3		src_only
	$(MAKE) -C star1		src_only
	$(MAKE) -C stars		src_only
	$(MAKE) -C stars2		src_only
	$(MAKE) -C lines		src_only
	$(MAKE) -C walk			src_only
	$(MAKE) -C noodle		src_only
	$(MAKE) -C beepdown		src_only
	$(MAKE) -C color		src_only
	$(MAKE) -C color2		src_only
	$(MAKE) -C sosu			src_only
	$(MAKE) -C sosu2		src_only
	$(MAKE) -C sosu3		src_only
	$(MAKE) -C typeipl		src_only
	$(MAKE) -C type			src_only
	$(MAKE) -C iroha		src_only
	$(MAKE) -C chklang		src_only
	$(MAKE) -C asciichr		src_only
	$(MAKE) -C argstest		src_only
	$(MAKE) -C tview		src_only
	$(MAKE) -C calc			src_only
	$(MAKE) -C bball		src_only
	$(MAKE) -C invader		src_only
	$(MAKE) -C fcount		src_only
	$(MAKE) -C gview		src_only
	$(MAKE) -C bainian		src_only
	$(MAKE) -C counter		src_only
	$(MAKE) -C crach1		src_only
	$(MAKE) -C mmlplay		src_only
	$(MAKE) -C mstest		src_only
	$(MAKE) -C mstest2		src_only
	$(MAKE) -C input		src_only
	-$(DEL) bootfd.img

refresh :
	$(MAKE) full
	$(MAKE) clean_full
	-$(DEL) bootfd.img

rebuild :
	$(MAKE) refresh
	$(MAKE) full

chs :
	$(COPY) .\nihongo\font\hzk16 .\nihongo\HZK16.hzk
	-$(DEL) bootfd.img
	$(MAKE)

cht :
	$(COPY) .\nihongo\font\hzk16f .\nihongo\HZK16.hzk 
	-$(DEL) bootfd.img
	$(MAKE)

vga:
	$(COPY) .\krnlia32\displays\asmhead-640.nas .\krnlia32\asmhead.nas
	$(MAKE) krnl
	-$(DEL) bootfd.img
	$(MAKE)
	

svga:
	$(COPY) .\krnlia32\displays\asmhead-800.nas .\krnlia32\asmhead.nas
	$(MAKE) krnl
	-$(DEL) bootfd.img
	$(MAKE)
 
xga:
	$(COPY) .\krnlia32\displays\asmhead-1024.nas .\krnlia32\asmhead.nas
	$(MAKE) krnl
	-$(DEL) bootfd.img
	$(MAKE)

sxga:
	$(COPY) .\krnlia32\displays\asmhead-1280.nas .\krnlia32\asmhead.nas
	$(MAKE) krnl
	-$(DEL) bootfd.img
	$(MAKE)
	

all_display:
	-$(DEL) imageout\bootfd-vga.img
	-$(DEL) imageout\bootfd-svga.img
	-$(DEL) imageout\bootfd-xga.img
	-$(DEL) imageout\bootfd-sxga.img
	-$(DEL) bootfd.img
	$(MAKE) vga
	$(MAKE) clean_full	
	$(MAKE) full
	$(COPY) bootfd.img imageout\bootfd-vga.img
	-$(DEL) bootfd.img
	$(MAKE) svga
	$(MAKE) clean_full	
	$(MAKE) full
	$(COPY) bootfd.img imageout\bootfd-svga.img
	-$(DEL) bootfd.img
	$(MAKE) xga
	$(MAKE) clean_full	
	$(MAKE) full
	$(COPY) bootfd.img imageout\bootfd-xga.img
	-$(DEL) bootfd.img
	$(MAKE) sxga
	$(MAKE) clean_full	
	$(MAKE) full
	$(COPY) bootfd.img imageout\bootfd-sxga.img
	-$(DEL) bootfd.img
	$(MAKE) svga
	$(MAKE) clean_full

makeiso_full:
	-$(DEL) imageout\iso\sources\vga.img
	-$(DEL) imageout\iso\sources\svga.img
	-$(DEL) imageout\iso\sources\xga.img
	-$(DEL) imageout\iso\sources\sxga.img
	-$(DEL) imageout\iso\sources\grldr
	-$(DEL) imageout\iso\sources\menu.lst
	-$(DEL) imageout\iso\sources\grub.exe
	-$(DEL) imageout\iso\sources\grldr_cd.bin
	$(MAKE) all_display
	$(COPY) imageout\bootfd-vga.img imageout\iso\sources\vga.img
	$(COPY) imageout\bootfd-svga.img imageout\iso\sources\svga.img
	$(COPY) imageout\bootfd-xga.img imageout\iso\sources\xga.img
	$(COPY) imageout\bootfd-sxga.img imageout\iso\sources\sxga.img
	$(COPY) grub\grldr imageout\iso\sources\grldr
	$(COPY) grub\menu.lst imageout\iso\sources\menu.lst
	$(COPY) grub\grub.exe imageout\iso\sources\grub.exe
	$(COPY) grub\grldr_cd.bin imageout\iso\sources\grldr_cd.bin
	$(MAKE) makeiso

makeiso:
	$(ULTRAISO) -volume "WBXOSCD" -file "imageout\iso\sources\*.*" -output "imageout\iso\output\bootcd.iso" -joliet -lowercase -bootfile "imageout\iso\sources\grldr_cd.bin"
	
krnl:
	$(MAKE) -C krnlia32 clean
	$(MAKE) -C krnlia32
