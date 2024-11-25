"Copy this file to ~/.vim/syntax
"For manual see :help pattern
"also see $VIMRUNTIME/syntax/c.vim or ~/.vim/plugged/vim-cpp-modern/after/syntax for examples

"MACRO_NAME
syn match cMyMacro "\<\u[A-Z0-9_]\+\>"
hi def link cMyMacro cString

"eENUM_NAME E_ENUM_NAME kCONST_NAME
syn match cMyEnum "\<e\u[A-Z0-9_]\+\>\|\<k\u[A-Z0-9_]\+\>\|\<E_\u[A-Z0-9_]\+\>"
hi def link cMyEnum  cConstant

"my_type_t my_type_e my_type_s
syn match cMyType "\<\D\w\+_t\>\|\<\D\w\+_e\>\|\<\D\w\+_s\>\|\<\D\w\+_i\>\|\<\D\w\+_func\>"
hi def cMyType ctermfg=32 cterm=bold

"use cMyParenAndBrackets only with redefining hi for parens and brackets errors
syn match cMyParenAndBrackets "[\(\)\<\>\[\]\{\}]"
hi def link cMyParenAndBrackets Statement
hi def link cErrInBracket cMyParenAndBrackets
hi def link cErrInParen cMyParenAndBrackets
hi def link cParenError cMyParenAndBrackets

syn match cMySpecSym "[\:\.\;\,\*\!\?\=\+\-\%\&\~\|]"
hi def link cMySpecSym Statement

"Copy that to see highlight group of item under cursor in status bar
"set statusline+=%{synIDattr(synID(line('.'),col('.'),1),'name')}
