all: help

help: programmers assumptions users

programmers: program_help.tex
	pdflatex program_help.tex
	pdflatex program_help.tex

assumptions: assumptions.tex
	pdflatex assumptions.tex
	pdflatex assumptions.tex

users: usr_help.tex
	latex2html usr_help.tex

clean: 
	rm -f *.aux *.pdf *.ps *.log *.dvi *.toc 
	rm -fr usr_help
