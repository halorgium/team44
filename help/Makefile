all: help

help: programmers users

programmers: program_help.tex
	latex2html program_help.tex

users: usr_help.tex
	latex2html usr_help.tex

clean: 
	rm -f *.aux *.pdf *.ps *.log *.dvi *.toc
