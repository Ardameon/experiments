#Add this to end of your ~/.bashrc

#export PS1='\u@\h:\w`__git_ps1`\$ '
export PS1='(\[\033[01;36m\]\u@\h\[\033[00m\] \[\033[01;34m\]\W\[\033[00m\])\[\033[01;33m\]`__git_ps1`\[\033[00m\]\$ '

rm /home/pylesosik/.config/bcompare/registry.dat -f
alias current-tags="echo plan_month_$(date +%m) plan_week_$(date +%U)"

export LD_PRELOAD=
