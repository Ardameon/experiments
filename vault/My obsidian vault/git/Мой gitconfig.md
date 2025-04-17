
> [!note] Поместить в `~` в виде файла `.gitconfig`

```git
[user]
	email = tyryshkinmv@gmail.com
	name = Mikhail Tyryshkin

[diff]
    Btool = meld
[difftool]
    prompt = no

[color]
  diff   = true
  grep   = true
  status = true
  ui     = true

  [color "branch"]
    current = yellow reverse
    local   = yellow
    remote  = green

  [color "diff"]
    meta       = normal
    frag       = magenta bold
    old        = red bold
    new        = blue bold
    whitespace = red reverse

  [color "status"]
    added      = yellow
    changed    = green
    untracked  = cyan

[alias]
  co = checkout
  ci = commit
  st = status
  br = branch
  hi = log --pretty=format:\"%Cblue%h%Creset %Cgreen%ad%Creset | %s %Cred%d%Creset [%Cgreen%an%Creset]\" --graph --date=iso
  h10 = log --pretty=format:\"%Cblue%h%Creset %Cgreen%ad%Creset | %s %Cred%d%Creset [%Cgreen%an%Creset]\" --graph --date=iso --max-count=10
  type = cat-file -t
  dump = cat-file -p
  #odd = "!git -c diff.noprefix=false diff -U0 -w --no-color -- \"$@\" | git apply --cached --ignore-whitespace --unidiff-zero #"
  odd = "!f() { git diff -U0 --ignore-space-at-eol --no-color -- \"$@\" | git apply --cached --ignore-whitespace --unidiff-zero; }; f"

  poff = config --global core.pager \"\"
  pon = config --global core.pager \"less\"
  lgp = !git pon && git log --all --graph --decorate '--pretty=tformat:%Cblue%h%Creset %s %Cred%d%Creset' && git poff
  lgt = !git pon && git log --all --graph --simplify-by-decoration '--pretty=tformat:%Cblue%h%Creset %s %Cred%d%Creset' && git poff
  fp = format-patch -k -1 HEAD --stdout
  fpr = format-patch -k --stdout
  fpa = format-patch -k -o patches --root
  df = !git pon && git diff


  lg1 = log --graph --all --format=format:'%C(bold blue)%h%C(reset) — %C(bold green)(%ar)%C(reset) %C(white)%s%C(reset) %C(bold white)— %an%C(reset)%C(bold yellow)%d%C(reset)' --abbrev-commit --date=relative
  lg2 = log --graph --all --format=format:'%C(bold blue)%h%C(reset) — %C(bold cyan)%aD%C(reset) %C(bold green)(%ar)%C(reset)%C(bold yellow)%d%C(reset)%n'' %C(white)%s%C(reset) %C(bold white)— %an%C(reset)' --abbrev-commit
  lg = !"git lg1"
  hist = log --pretty=format:\"%h %ad | %s%d [%an]\" --graph --date=short


[core]
	pager = less
	editor = vim

[log]
  decorate = full
[merge]
	tool = bc3

```