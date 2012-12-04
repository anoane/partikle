TARG=\
	lpcug.pdf\

all: $(TARG)
%.pdf: %.tex
	pdflatex $<
	pdflatex $<
%.dvi: %.tex
	latex $<
	latex $<

distclean: clean
clean:
	rm -f $(TARG) 
	rm -f *.aux *.lo[gft] *.toc *.out

