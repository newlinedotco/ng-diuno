FORMAT=revealjs
THEME=moon
STYLE=default

all:
	pandoc -V theme=${THEME} --variable transition="linear" \
						--template template.revealjs \
						-V highlight-style="${STYLE}" \
						-t ${FORMAT} -s README.md \
						-o index.html

pdf:
	pandoc --latex-engine=xelatex -t beamer README.md -o index.pdf