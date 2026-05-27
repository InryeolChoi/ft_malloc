augroup ft_malloc_42_style
	autocmd!
	autocmd FileType c,h setlocal noexpandtab
	autocmd FileType c,h setlocal tabstop=4
	autocmd FileType c,h setlocal shiftwidth=4
	autocmd FileType c,h setlocal textwidth=80
	autocmd FileType c,h setlocal colorcolumn=81
	autocmd BufWritePre *.c,*.h %s/\s\+$//e
augroup END
