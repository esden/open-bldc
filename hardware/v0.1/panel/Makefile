GERBMERGE = gerbmerge
GERBV = gerbv

CFG=layout.cfg
DEF=layout.def

_panel: 
	cd ..; make gerber
	$(GERBMERGE) $(CFG) $(DEF)


_view:
	$(GERBV) merged.*.ger



clean:
	rm -f merged.* *~
