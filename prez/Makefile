FORMAT=revealjs
THEME=moon

all:
	pandoc -V theme=${THEME} --variable transition="linear" \
						--template template.revealjs \
						-V highlight-style="github" \
						-t ${FORMAT} -s README.md \
						-o index.html
