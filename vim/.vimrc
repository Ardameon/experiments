"------------------------------------------------
"                               .vimrc by Helius aka Eugene Samoylov
"                                       ghelius@gmail.com
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



set wrap                                                "перенос длинных строк
set lbr                                                 "переносить целые слова
set number                                      "показывать номера строк ...
set ruler                                               "показывать позицию курсора
set numberwidth=5               " ... в 4 символа минимум
set ts=4                                                "задать размер табуляции в четыре пробела
set shiftwidth=4               "Размер табуляции по умолчанию
set softtabstop=4
set tabstop=4
set scrolljump=7        "Теперь нет необходимости передвигать курсор к краю экрана, чтобы подняться в режиме редактирования
set scrolloff=7         "Теперь нет необходимости передвигать курсор к краю экрана, чтобы опуститься в режиме редактирования
set winheight=12
set winwidth=12
set winminheight=12 "set from 12 to 0 according dread idea
set winminwidth=7

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
map <C-K> :s/^/\/\//<CR>:nohl<CR>
map <C-U> :s/^\/\///<CR>:nohl<CR>
" создание табов
nmap <S-T> :tabnew<CR>
" удаление буфера
nmap <C-D> :Kwbd<CR>
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
map <F2> :A<CR>
" покахать/скрыть tag list
map <F3> :TlistToggle<CR>
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
call plug#end()

"set encoding=UTF-8


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
"TABS are SpecialKey
highlight SpecialKey ctermfg=240

nnoremap <S-k> 5k
nnoremap <S-j> 5j
vnoremap <S-k> 5k
vnoremap <S-j> 5j

nnoremap <C-h> :tabprev<CR>
nnoremap <C-l> :tabnext<CR>
inoremap <C-h> <Esc>:tabprev<CR>
inoremap <C-l> <Esc>:tabnext<CR>

set expandtab "always use spaces instead of tabs
set nowrap

"sudo apt-get install fonts-powerline
let g:airline_powerline_fonts=1
"let g:airline_left_sep = '▶'
"let g:airline_right_sep = '◀'

"IndentLine settings
let g:indentLine_showFirstIndentLevel=1
let g:indentLine_leadingSpaceEnabled=1
let g:indentLine_enabled=1
"let g:indentLine_fileType = ['c', 'cpp']
set list listchars=tab:\|\ "Display tabs as SpecialKey
