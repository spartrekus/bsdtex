	
all:
	  bsdtex test.mrk
	  bsdbib mybib.mrk
	  pdflatex test
	  bibtex test
	  pdflatex test
	  pdflatex test
	  screen -d -m mupdf test.pdf 
 
