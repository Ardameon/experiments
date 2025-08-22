"------------------------------------------------
"                               .vimrc by Mikhail Tyryshkin
"                                       helldiver@yandex.ru
"------------------------------------------------
" INSTALL all you need:
"
"------------------------------------------------
" VIMPLUG
"------------------------------------------------
"
" Install for vim
" curl -fLo ~/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
"
" Install for nvim
" sh -c 'curl -fLo "${XDG_DATA_HOME:-$HOME/.local/share}"/nvim/site/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim'
"
" Then :PlugUpdate
"
"------------------------------------------------
" Fuzzyfinder
"------------------------------------------------
"
" For good fzf need actual vim, you can install it manually from ppa
"
" $ sudo add-apt-repository ppa:jonathonf/vim
" $ sudo apt install vim
"
" $ sudo apt install fzf
" $ sudo apt install ripgrep
"
" For syntax highlighting we need to install bat
"
" $ sudo apt install bat
" $ mkdir -p ~/.local/bin
" $ ln -s /usr/bin/batcat ~/.local/bin/bat
"
" help fzf-vim - for more info
"
"------------------------------------------------
" SYNTAX extanding
"------------------------------------------------
"
" Find 'c.vim' file nearby
" Copy this file to ~/.vim/syntax
"
"------------------------------------------------
" TAGS
"------------------------------------------------
"
" To create tags of system files and custom files
" $ sudo apt install universal-ctags
" $ ctags --c-kinds=+p -R /usr/include/
" $ ctags --c-kinds=+p -R *
"
"------------------------------------------------
" Language servers
"------------------------------------------------
"------------------------------------------------
" Install ccls C/C++ language server
"------------------------------------------------
"
" $ sudo apt install ccls
"
" WARNING! If you have an issue with detecting standard c++ library by ccls, do next:
"
" $ ll /usr/lib/gcc/x86_64-linux-gnu/
"
" Choose max version (e.g. 12) and then install libstdc++ of appropriate
" version:
"
" $ sudo apt install libstdc++-12-dev
"
" If issue still exists, try to check if ccls clang version is matched to
" system clang:
"
" $ ccls --version
" $ clang --version
"
"------------------------------------------------
" Install python language server
"------------------------------------------------
"
" $ sudo apt install python3-pylsp
"
"------------------------------------------------
" Install cmake language server
"------------------------------------------------
"
" $ pip3 install cmake-language-server
"
"------------------------------------------------
" Install bash language server
"------------------------------------------------
"
" Install node v16 min via NVM
"
" curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/master/install.sh | bash
" nvm install v16
" npm i -g bash-language-server
"
" Test langugae-server
"
" bash-language-server --help
"
"------------------------------------------------

set magic
" фикс для русских клавиш
set langmap=йq,цw,уe,кr,еt,нy,гu,шi,щo,зp,х[,ъ],фa,ыs,вd,аf,пg,рh,оj,лk,дl,ж\\;,э',яz,чx,сc,мv,иb,тn,ьm,б\\,,ю.,ё`,ЙQ,ЦW,УE,КR,ЕT,НY,ГU,ШI,ЩO,ЗP,Х{,Ъ},ФA,ЫS,ВD,АF,ПG,РH,ОJ,ЛK,ДL,Ж:,Э\\",ЯZ,ЧX,СC,МV,ИB,ТN,ЬM,Б<,Ю>,Ё~

" ================== РЕДАКТИРОВАНИЕ =================
set autoindent          "включить автоотступы
set smartindent         "Умные" отступы (например, автоотступ после {)
set showmatch                   "показывать первую парную скобку после ввода второй ...
set cindent
syn on

"включаем автодополнения
autocmd FileType c set omnifunc=ccomplete#Complete
set omnifunc=syntaxcomplete#Complete

" Автозавершение слов по tab =)
function! InsertTabWrapper()
        let col = col('.') - 1
        if !col || getline('.')[col - 1] !~ '\k'
                return "\<tab>"
        else
                " если назад по строке встречается точка '.' или стрелка '->' и до точки или стрелки есть слово
                " найти его тип, и показать все члены этого типа из tags
                return "\<c-p>"
        endif
endfunction
imap <tab> <c-r>=InsertTabWrapper()<cr>

"function! SuperCleverTab()
"    if strpart(getline('.'), 0, col('.') - 1)
"        return "\<Tab>"
"    else
"        if &omnifunc != ''
"            return "\<C-X>\<C-O>"
"        elseif &dictionary != ''
"            return "\<C-K>"
"        else
"            return "\<C-N>"
"        endif
"    endif
"endfunction
"inoremap <Tab> <C-R>=SuperCleverTab()<cr>



set complete="" "Слова откуда будем завершать
set complete+=. "Из текущего буфера
set complete+=k "Из словаря
set complete+=b "Из других открытых буферов
set complete+=t "из тегов
set backspace=2 "разрешить backspace в режиме вставки
" включаем проверку синтаксиса
let g:syntastic_enable_signs=1

" ========== ОПЦИИ ОТОБРАЖЕНИЯ ТЕКСТА  =============
set t_Co=256                    "включить дополнительные цвета в терминале (полезно для команды TOhtml (t_Co=256), иначе цвета html кода нереально кислотные)

" цветовая схема
syntax on
colo default

set background=dark
highlight Statement ctermfg=yellow
highlight Comment ctermfg=blue
highlight PreProc ctermfg=green
highlight String ctermfg=green
highlight Conditional ctermfg=yellow
highlight LineNr ctermfg=240
highlight Number ctermfg=red
highlight Constant ctermfg=magenta
highlight cFormat ctermfg=magenta
highlight cSpecial ctermfg=magenta
highlight cType ctermfg=yellow
highlight Structure ctermfg=darkgreen
highlight Function ctermfg=lightblue
highlight Macro ctermfg=green



set lbr                                                 "переносить целые слова
set number                                      "показывать номера строк ...
set ruler                                               "показывать позицию курсора
set numberwidth=5               " ... в 4 символа минимум
set ts=4                                                "задать размер табуляции в четыре пробела
set shiftwidth=4               "Размер табуляции по умолчанию
set softtabstop=4
set tabstop=4
set winheight=12
set winwidth=12
set winminheight=12 "set from 12 to 0 according dread idea
set winminwidth=7
set scrolloff=7

" установка режима сварачивания блоков тескста
set foldmethod=manual
set hidden

" ================== ВНЕШНИЙ ВИД VIM =================
"set showtabline=2              "показывать строку вкладок всегда
set laststatus=2                "показывать строку статуса всегда
set showcmd                                     "показывать буфер вводимой команды

" ================== ПАРАМЕТРЫ ПОИСКА =================
set hlsearch                            "при поиске помечать все найденные строки
set incsearch                           "при поиске перескакивать на найденный текст в процессе набора строки
set ignorecase                  "игнорировать прописные/строчные при поиске

" ================== КОДИРОВКИ ФАЙЛОВ =================
set ffs=unix,dos,mac                                                                            "формат файла по умолчанию (влияет на окончания строк) - будет перебираться в указанном порядке
set fencs=utf-8,cp1251,koi8-r,cp866                     "варианты кодировки файла по умолчанию (все файлы по умолчанию сохраняются в этой кодировке)

"================== Сохранение сессии =================
" настройки файлов с сессиями
set sessionoptions=buffers,curdir,folds,tabpages,winsize,winpos,resize,globals
autocmd! bufwritepost .vimrc source $MYVIMRC "применять .vimrc налету!!! :D

"=================== Действия для разных типов файлов =

"autocmd FileType c call CCodeEdit()

" ================= ПРИВЯЗКА КНОПОК ===================
" < & > - делаем отступы для блоков
vmap < <gv
vmap > >gv
 " Выключаем ненавистный режим замены

" ресайз оконцев
map <S-Right>   :vertical resize +15<CR>
map <S-Left>    :vertical resize -15<CR>
map <s-down>    :resize +20<CR>
map <s-up>      :resize -20<CR>
" коментирование - ракоментирования текста
"map <C-K> :s/^/\/\//<CR>:nohl<CR>
"map <C-U> :s/^\/\///<CR>:nohl<CR>
" создание табов
"nmap <S-T> :tabnew<CR>
" удаление буфера
"nmap <C-D> :Kwbd<CR>
nmap <S-Q> <c-W>c
"<c-W>c
" переключение буферов и табов
nmap < :bp<CR>
nmap > :bn<CR>
" переключение между открытыми буферами по Tab - Shift Tab
"nmap <Tab> <c-W>W - с этой опцией не работает кобинация <c-i>
"nmap <S-Tab> <c-W>W
        " окно ниже и развернуть
        "imap <Tab> <Esc><C-W>j<C-W>_a
"       nmap <Tab> <C-W>j<C-W>_
        " окно выше и развернуть
        "imap <S-Tab> <Esc><C-W>k<C-W>_a
"       nmap <S-Tab> <C-W>k<C-W>_
" запускаем shell
map <F1> :shell<CR>
" переключиться между хидером и файлом
"map <F2> :A<CR>
" покахать/скрыть tag list
"map <F3> :TlistToggle<CR>
"открыть новую вкладку
imap <F4> <Esc>:browse tabnew<CR>
map <F4> <Esc>:browse tabnew<CR>

" открыть проводник vim
"map <F4> :call VimCommanderToggle()<CR>
"Перемещение между вкладками справа налево
imap <F5> <Esc> :tabprev <CR>
map <F5> :tabprev <CR>

"Перемещение между вкладками слева направо
imap <F6> <Esc> :tabnext <CR>i
map <F6> :tabnext <CR>

" открыть файл history
map <F10> :e history.txt<CR>
" диалог открытия файла
"nmap <silent> <S-o> :CommandT<CR>


" ================== Menu =============================
" по F8 вызывается меню кодировок
set encoding=utf-8
set termencoding=utf-8
set fileencoding=utf-8

set wildmenu
set wcm=<Tab>
menu Encoding.CP1251   :e ++enc=cp1251<CR>
menu Encoding.CP866    :e ++enc=cp866<CR>
menu Encoding.KOI8-U   :e ++enc=koi8-u<CR>
menu Encoding.UTF-8    :e ++enc=utf-8<CR>
map <F8> :emenu Encoding.<TAB>

" ------------------ Меню вставки разного -------------
"set wildmenu
"set wcm=<Tab>
"menu insert.//*80*          i//*****************************************************************************
"menu insert.Helius                                      i//@@@@@@ Changed by Helius @@@@@@<ESC>
"menu insert.date-time       :r !date +"\%d.\%m.\%Y \%k:\%M"<CR>
"map <F5> :emenu insert.<TAB>

" ---------- Меню редактирования конфигов -------------
set wildmenu
set wcm=<Tab>
menu config.main        :e $MYVIMRC<CR>
menu config.local       :e project.vim<CR>
map <F12> :emenu config.<TAB>

" ---------- Меню сохранения сессии -------------
set wildmenu
set wcm=<Tab>
menu vim.session_save        :silent mksession! vim.session
menu vim.session_load        :silent source vim.session<Tab>
map <F11> :emenu vim.<TAB>

" ------------------ Меню SVN -------------
"set wildmenu
"set wcm=<Tab>
"menu svn.vimdiff                                :set background=dark<CR>:let df = expand('%')<CR>:vs svn-<c-r>=df<CR><CR>:read !svn cat <c-r>=df<CR><CR>:windo diffthis<CR>:set diffopt=filler,iwhite<CR>gg:set wrap<CR><C-W>w:set wrap<CR>
"menu svn.diff_this                      :let df = expand('%')<CR>:new <CR>:set syntax=diff buftype=nofile<CR>:read !svn diff -x -b <c-r>=df <CR><CR>gg
"menu svn.commit_this            :!svn ci % --editor-cmd vim <CR><CR>:!<CR><CR>
"menu svn.status                                 :new<CR>:read !svn status<CR>:set buftype=nofile<CR>:/^[!ACDIMRX] .*<CR>
"menu svn.commit                                 :!svn ci --editor-cmd vim <CR><CR>:!<CR><CR>
"menu svn.update                                 :!svn up --editor-cmd vim <CR>
"menu svn.log                                            :new<CR>:read !svn log <CR>:set buftype=nofile<CR>gg:/^r[0-9]*<CR>
"menu svn.diff                                           :new<CR>:read !svn diff -x -b<CR>:set syntax=diff buftype=nofile<CR>gg
"menu svn.browser                                :!svnb<CR>
"map <F6> :emenu svn.<TAB>
" ----------------- Menu make --------------
set wildmenu
set wcm=<Tab>
menu make.make_all                                      :silent make! <CR><CR>:!<CR>:cw<CR>
menu make.make_run                                      :!make run<CR>
menu make.make_clean                            :silent make clean<CR><CR>:!<CR><CR>
menu make.make_tags                                     :!ctags -R <CR><CR>:!<CR><CR>

map <F7> :emenu make.<TAB>


"-------- Подключаем файл настроек проекта -------
if filereadable("project.vim")
        source project.vim
endif

"VIMPLUG
"Install for vim
"curl -fLo ~/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
"Install for nvim
"sh -c 'curl -fLo "${XDG_DATA_HOME:-$HOME/.local/share}"/nvim/site/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim'


call plug#begin()
Plug 'airblade/vim-gitgutter'
Plug 'vim-airline/vim-airline' "apt install fonts-powerline
Plug 'vim-airline/vim-airline-themes'
Plug 'scrooloose/nerdtree'
"Plug 'cdelledonne/vim-cmake'
"Plug 'ryanoasis/nerd-fonts'
"Plug 'ryanoasis/vim-devicons'
"Plug 'octol/vim-cpp-enhanced-highlight'
Plug 'bfrg/vim-cpp-modern'
Plug 'flazz/vim-colorschemes'
Plug 'tpope/vim-fugitive'
Plug 'ap/vim-css-color'
"Plug 'powerline/fonts'
Plug 'pboettch/vim-cmake-syntax'
Plug 'Yggdroot/indentLine'
Plug 'junegunn/fzf'
Plug 'junegunn/fzf.vim'
Plug 'tpope/vim-surround'
" Plugins for Language server support
Plug 'prabirshrestha/vim-lsp'
Plug 'mattn/vim-lsp-settings'
Plug 'prabirshrestha/asyncomplete.vim'
Plug 'prabirshrestha/asyncomplete-lsp.vim'
Plug 'jeetsukumaran/vim-markology'
Plug 'm-pilia/vim-ccls'
call plug#end()

let mapleader = " "

"set encoding=UTF-8

"vim-surround commands:
"
"csbB csrb csrB csBr cs'< cs'(
"ds' dsb dsB dsr ds<pre> ds{
"yssb yssB yssr yss'
"ySSb ySS{ yss<pre>
"vS' vSb vSB

nnoremap <C-n> :NERDTreeToggle<CR>

"set guifont=BitstreamVeraSansMono_NF:h13

"octol/vim-cpp-enhanced-highlight
let g:cpp_class_scope_highlight = 1
let g:cpp_member_variable_highlight = 1
let g:cpp_class_decl_highlight = 1
let g:cpp_posix_standard = 1
let g:cpp_experimental_simple_template_highlight = 1
let g:cpp_concepts_highlight = 1

"bfrg/vim-cpp-modern'
let g:cpp_function_highlight = 1
let g:cpp_member_highlight = 1
let g:cpp_simple_highlight = 1

highlight cStructMember ctermfg=cyan
highlight cFormat ctermfg=green
highlight cSpecial ctermfg=green
highlight cLabel ctermfg=red
highlight cUserLabel ctermfg=red
highlight cCharacter ctermfg=green
highlight cCppParen ctermfg=yellow
highlight cCppBracket ctermfg=yellow
highlight cBlock ctermfg=yellow
"highlight cBitField ctermfg=red
highlight cParen ctermfg=yellow
highlight cBracket ctermfg=yellow
highlight cComment ctermfg=grey
highlight Function ctermfg=lightblue cterm=bold term=bold
highlight link cMemberAccess Statement
"TABS are SpecialKey
highlight SpecialKey ctermfg=240
highlight Conceal ctermfg=240 ctermbg=NONE
highlight CursorLineNr ctermfg=grey cterm=bold ctermbg=234
highlight CursorLine cterm=NONE ctermbg=234
highlight diffAdded ctermfg=10 cterm=bold
highlight diffRemoved ctermfg=9 cterm=bold
highlight diffFile ctermfg=177 cterm=bold
highlight diffSubname ctermfg=lightblue
"highlight link diffSubname Identifier
highlight DiffAdd ctermfg=15 cterm=bold term=bold ctermbg=33
highlight DiffChange ctermfg=15 cterm=bold term=bold ctermbg=8
highlight DiffDelete ctermfg=15 cterm=bold term=bold ctermbg=9
highlight DiffText ctermfg=15 cterm=bold term=bold ctermbg=124

"nnoremap <S-k> 5k
"nnoremap <S-j> 5j
"vnoremap <S-k> 5k
"vnoremap <S-j> 5j

"moving selected lines
"vnoremap J :m '>+1<CR>gv=gv
"vnoremap K :m '<-2<CR>gv=gv

"always center cursor line after moving
nnoremap <C-d> <C-d>zz
nnoremap <C-u> <C-u>zz
nnoremap <C-i> <C-i>zz
nnoremap <C-o> <C-o>zz
"center cursor line on search movings
nnoremap n nzzzv
nnoremap N Nzzzv

nnoremap <C-h> :tabprev<CR>
nnoremap <C-l> :tabnext<CR>
inoremap <C-h> <Esc>:tabprev<CR>
inoremap <C-l> <Esc>:tabnext<CR>

"copy to clipboard
vnoremap <C-y> "+y

set expandtab "always use spaces instead of tabs
set nowrap

"don't use .swp files
"set noswapfile

"sudo apt-get install fonts-powerline
let g:airline_powerline_fonts=1
"let g:airline_left_sep = '▶'
"let g:airline_right_sep = '◀'

"IndentLine settings
let g:indentLine_showFirstIndentLevel=1
let g:indentLine_leadingSpaceEnabled=1
"let g:indentLine_leadingSpaceChar = '.'
let g:indentLine_setColors=0
let g:indentLine_color_term=240
let g:indentLine_color_tty_dark=0
let g:indentLine_bufTypeExclude = ['help', 'terminal']
let g:indentLine_bufNameExclude = ['_.*', 'NERD_tree.*']
"let g:indentLine_enabled=1
"let g:indentLine_fileType = ['c', 'cpp']
set list listchars=tab:\|\ "Display tabs as SpecialKey

"alt + f alias
execute "set <A-f>=\ef"
"for good fzf need actual vim, you can install it manually from ppa
"   sudo add-apt-repository ppa:jonathonf/vim
"apt install fzf
"apt install rg
"
"For syntax highlighting we need to install bat
"
"$ sudo apt install bat
"$ mkdir -p ~/.local/bin
"$ ln -s /usr/bin/batcat ~/.local/bin/bat
"
"help fzf-vim - for more info

command! -bang -nargs=* CustomBLines
    \ call fzf#vim#grep(
    \   'rg --with-filename --column --line-number --no-heading --smart-case . '.fnameescape(expand('%')), 1,
    \   fzf#vim#with_preview({'options': '--keep-right --delimiter : --nth 4.. --preview "bat -p --color always {}"'}, 'up:60%' ))

"Marks command with preview
function! s:fzf_preview_p(bang, ...) abort
    let preview_args = get(g:, 'fzf_preview_window', ['up:50%', 'ctrl-/'])
    if empty(preview_args)
        return { 'options': ['--preview-window', 'hidden'] }
    endif

    " For backward-compatiblity
    if type(preview_args) == type('')
        let preview_args = [preview_args]
    endif
    return call('fzf#vim#with_preview', extend(copy(a:000), preview_args))
endfunction

command! -bar -bang MP
            \ call fzf#vim#marks(
            \     s:fzf_preview_p(<bang>0, {'placeholder': '$([ -r $(echo {4} | sed "s#^~#$HOME#") ]  && [ -n {4} ] || echo ' . fzf#shellescape(expand('%')) . '):{2}',
            \               'options': '--preview-window +{2}-/2'}),
            \     <bang>0)

"global content search
nnoremap <A-f> :Rg<CR>
nnoremap <M-g> :Rg<CR>
"global content search using word under cursor"
nnoremap <Leader>f :Rg <C-R><C-W><CR>
"file content search
nnoremap <C-f> :CustomBLines<CR>
"global file search
nnoremap <C-p> :FZF<CR>
"global tags search
"nnoremap <S-t> :Tags<CR>
"global tags search using word under cursor
"nnoremap <leader>t :Tags <C-R><C-W><CR>
nnoremap <leader>t :Tags<CR>
"file lines search
nnoremap <S-l> :Lines<CR>
"buffers search
nnoremap <leader>b :Buffers<CR>
"marks of file
nnoremap <leader>m :MP<CR>
"git diffs show
nnoremap <leader>G :GFiles?<CR>
"git show commits
nnoremap <leader>c :Commits<CR>
"remove search result highlighting
nnoremap <leader>h :noh<CR>
"manage QuickFix window
nnoremap <leader>q :cclose<CR>
nnoremap <C-j> :cnext<CR>
nnoremap <C-k> :cprev<CR>


let g:fzf_layout = { 'window': { 'width': 0.9, 'height': 0.9 } }
let g:fzf_tags_command = 'ctags -R --c-kinds=+pxl'
let g:fzf_preview_window = ['up,60%', 'ctrl-/']

set cursorline
set relativenumber
set nobackup
set writebackup

" Let's save undo info!
if !isdirectory($HOME."/.vim")
    call mkdir($HOME."/.vim", "", 0770)
endif
if !isdirectory($HOME."/.vim/undo-dir")
    call mkdir($HOME."/.vim/undo-dir", "", 0700)
endif
set undodir=~/.vim/undo-dir
set undofile
"Save marks in vim info
set viminfo='1000,f1

"search for tags in current file dirrectory, then search for tags in all upper
"dirrectories till $HOME
set tags=./tags,tags;$HOME

"Center screen after going to tag
nnoremap <C-]> <C-]>zz
"Go to next tag (should be used afre 'CTRL-]')
nnoremap <F2> :tn<CR>zz
"Show tags list to select (should be used afre 'CTRL-]')
nnoremap <F3> :ts<CR>

"'CTRL-]' - search and go to tag under cursor
"'CTRL-T' - pop all tag-stack

"To create tags of system files and custom files
"$ sudo apt install universal-ctags
"$ ctags --c-kinds=+p -R /usr/include/
"$ ctags --c-kinds=+p -R *

"autoremove trailing whitespaces aftr saving file
autocmd BufWritePre * :%s/\s\+$//e

" Determines whether to use spaces or tabs on the current buffer.
function TabsOrSpaces()
    if getfsize(bufname("%")) > 1024000
        " File is very large, just use the default.
        return
    endif

    let numTabs=len(filter(getbufline(bufname("%"), 1, 250), 'v:val =~ "^\\t"'))
    let numSpaces=len(filter(getbufline(bufname("%"), 1, 250), 'v:val =~ "^ "'))

    if numTabs > numSpaces
        setlocal noexpandtab
    endif
endfunction

" Call the function after opening a buffer
autocmd BufReadPost * call TabsOrSpaces()

"Go to start of block
nnoremap <leader>r yi{
"For pointers in C/C++
inoremap <C-o> ->

"Paste directly from copy register
nnoremap <leader>p "0p
vnoremap <leader>p "0p
nnoremap <leader>P "0P
vnoremap <leader>P "0P

" Register ccls C++ lanuage server.
if executable('ccls')
   au User lsp_setup call lsp#register_server({
      \ 'name': 'ccls',
      \ 'cmd': {server_info->['ccls']},
      \ 'root_uri': {server_info->lsp#utils#path_to_uri(lsp#utils#find_nearest_parent_file_directory(lsp#utils#get_buffer_path(), 'compile_commands.json'))},
      \ 'initialization_options': {'cache': {'directory': expand('~/.cache/ccls') }},
      \ 'allowlist': ['c', 'cpp', 'objc', 'objcpp', 'cc'],
      \ })
endif

" Key bindings for vim-lsp.
"nn <F2> :LspDefinition<cr>zz
nn <leader>d :LspDefinition<cr>zz
nn <leader>R :LspRename<cr>

"nn <F3> :tn<CR>zz
"nn <silent> <M-r> :LspReferences<cr>
"nn <f2> :LspRename<cr>
"nn <silent> <M-a> :LspWorkspaceSymbol<cr>
"nn <silent> <M-l> :LspDocumentSymbol<cr>
nn <leader>l :call lsp#enable()<cr>
nn <leader>L :call lsp#disable()<cr>:LspStopServer<cr>

" Key bindings for asyncomplete
inoremap <expr> <Tab>   pumvisible() ? "\<C-n>" : "\<Tab>"
inoremap <expr> <S-Tab> pumvisible() ? "\<C-p>" : "\<S-Tab>"
inoremap <expr> <cr>    pumvisible() ? asyncomplete#close_popup() : "\<cr>"
imap <c-@> <Plug>(asyncomplete_force_refresh)

inoremap ( ()<Left>
inoremap [ []<Left>
inoremap { {}<Left>
inoremap " ""<Left>
inoremap ' ''<Left>

let g:lsp_auto_enable = 1
"let g:lsp_completion_documentation_delay = 520
let g:lsp_diagnostics_enabled = 1
let g:lsp_diagnostics_echo_cursor = 1
let g:lsp_diagnostics_float_cursor = 1
"let g:lsp_preview_float = 1
"let g:lsp_preview_max_height = 6
"let g:lsp_hover_ui = 'preview'

let g:asyncomplete_popup_delay = 200

highlight Pmenu ctermfg=0 ctermbg=177 guibg=NONE
highlight SignColumn ctermfg=red ctermbg=234 guibg=NONE

" Install python pylsp server (apt install python3-pylsp)
if executable('pylsp')
    " pip install python-lsp-server
    au User lsp_setup call lsp#register_server({
        \ 'name': 'pylsp',
        \ 'cmd': {server_info->['pylsp']},
        \ 'allowlist': ['python'],
        \ })
endif

" Install cmake language server (pip3 install cmake-language-server)
if executable('cmake-language-server')
  au User lsp_setup call lsp#register_server({
  \ 'name': 'cmake',
  \ 'cmd': {server_info->['cmake-language-server']},
  \ 'root_uri': {server_info->lsp#utils#path_to_uri(lsp#utils#find_nearest_parent_file_directory(lsp#utils#get_buffer_path(), 'build/'))},
  \ 'whitelist': ['cmake'],
  \ 'initialization_options': {
  \   'buildDirectory': 'build',
  \ }
  \})
endif

"Install node v16 min via NVM
"
"curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/master/install.sh | bash
"nvm install v16
"npm i -g bash-language-server
"
"Test langugae-server
"
"bash-language-server --help
if executable('bash-language-server')
  au User lsp_setup call lsp#register_server({
        \ 'name': 'bash-language-server',
        \ 'cmd': {server_info->[&shell, &shellcmdflag, 'bash-language-server start']},
        \ 'allowlist': ['sh', 'bash'],
        \ })
endif


"GitGutter plagin settings
let g:gitgutter_max_sign = -1
let g:gitgutter_sign_added              = '++'
let g:gitgutter_sign_modified           = '~~'
let g:gitgutter_sign_removed            = '__'
let g:gitgutter_sign_removed_first_line = '‾‾'
let g:gitgutter_sign_removed_above_and_below = '_¯'
let g:gitgutter_sign_modified_removed   = '~_'

"highlight GitGutterAdd    guifg=#009900 ctermfg=2
"highlight GitGutterChange guifg=#bbbb00 ctermfg=3
"highlight GitGutterDelete guifg=#ff2222 ctermfg=1
"highlight SignColumn      guibg=#009900 ctermbg=7

let g:gitgutter_map_keys = 0
nnoremap <leader>gg :GitGutterToggle<CR>
nnoremap <leader>gG :GitGutterLineHighlightsToggle<CR>
nnoremap <leader>gd :GitGutterPreviewHunk<CR>
nnoremap <leader>gD :GitGutterDiffOrig<CR><C-W><left>
nnoremap <leader>gh :execute "GitGutterQuickFixCurrentFile \| copen"<CR>
nnoremap <leader>gH :execute "GitGutterQuickFix \| copen"<CR>
nnoremap <leader>gu :GitGutterUndoHunk<CR>
nnoremap <leader>gf :GitGutterFold<CR>
nmap <leader>gn :GitGutterNextHunk<CR>zz
nmap <leader>gN :GitGutterPrevHunk<CR>zz
nnoremap <leader>gs :GitGutterStageHunk<CR>

"Markology settings"
let g:markology_include="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
highlight MarkologyHLu cterm=bold ctermfg=173 ctermbg=234 guifg=green guibg=blue
highlight MarkologyHLl cterm=bold ctermfg=173 ctermbg=234 guifg=green guibg=blue
highlight MarkologyHLo cterm=bold ctermfg=173 ctermbg=234 guifg=green guibg=blue

nmap <silent> ;1 <Plug>MarkologyEnable
nmap <silent> ;0 <Plug>MarkologyDisable
nmap <silent> ;! <Plug>MarkologyToggle
nmap <silent> ;, <Plug>MarkologyPlaceMarkToggle
nmap <silent> ;+ <Plug>MarkologyPlaceMark
nmap <silent> ;- <Plug>MarkologyClearMark
nmap <silent> ;_ <Plug>MarkologyClearAll
nmap <silent> ;] <Plug>MarkologyNextLocalMarkPos
nmap <silent> ;[ <Plug>MarkologyPrevLocalMarkPos
nmap <silent> ;} <Plug>MarkologyNextLocalMarkByAlpha
nmap <silent> ;{ <Plug>MarkologyPrevLocalMarkByAlpha
nmap <silent> ;? <Plug>MarkologyLocationList
nmap <silent> ;~ <Plug>MarkologyQuickFix


"doxygen
"
" for function declarations and others (external) - detiled after <. >
iabbrev ddde /** .*/
" for methods, enum elements and data structs (internal) - detiled after <. >
iabbrev dddi /**< .*/

"general brief documentation.
iabbrev dddb /** @brief DESCRIOTION */
"name section documentation.
iabbrev dddn /**@name SECTION_NAME@{*/
"function documentation.
iabbrev dddfunc /**@copybrief FUNC_NAME@param[in,out]  NAME  DESC@return ret_desc*/
"file header documentation.
iabbrev dddfh /**@file@brief HEADER_DESC@defgroup GROUP_NAME GROUP_DESC@ingroup PARENT_GROUP_NAME@{*/
"file source documentation.
iabbrev dddfs /**@file@brief SOURCE_DESC@addtogroup GROUP_NAME@{*/
"file source documentation.
iabbrev dddc /** @} */

" Group markers
""iabbrev dddgd /** @defgroup <uniq_key> Label Description.@{*//** @}*/
""iabbrev dddga /** @addtogroup <uniq_key> @{*//** @}*/
""iabbrev dddgw /** @weakgroup <uniq_key> @{*//** @}*/

"Termdebug
nnoremap <Down> :Over<CR>
nnoremap <leader><Down> :Step<CR>

"vim-ccls settings"

nnoremap <leader>o :CclsCallers<CR>
nnoremap <leader>O :CclsCallees<CR>
nnoremap <leader>F :CclsCallHierarchy<CR>

let g:ccls_size = 20
let g:ccls_position = 'botright'
let g:ccls_orientation = 'horizontal'
