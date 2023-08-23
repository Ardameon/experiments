ARG CODENAME=jammy

FROM ubuntu:${CODENAME}

LABEL author="Mikhail Tyryshkin"
LABEL description="VIM with appropriate pluggins to be good"

ENV VIM_WORK_DIR=/vim
ENV TERM=xterm
ENV SRC_DIR=/src_dir
ENV LOCAL_VIMRC=docker_vimrc
ENV VIM_USER=vimusr
ENV VIM_USER_HOME=/home/${VIM_USER}

WORKDIR $VIM_WORK_DIR

COPY . .

RUN adduser --disabled-password --gecos '' ${VIM_USER}
RUN mkdir -p ${VIM_USER_HOME}/.cache

RUN echo "This message will appear only on image build"
RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get install dialog -y
RUN apt-get install apt-utils -y
RUN apt-get install bash -y
RUN apt-get install git -y
RUN apt-get install vim -y
RUN apt-get install curl -y
RUN apt-get install fonts-powerline -y
RUN apt-get install fzf -y
RUN apt-get install ripgrep -y
RUN apt-get install locales -y

RUN apt-get install bat -y && \
    mkdir -p ${VIM_USER_HOME}/.local/bin && \
    ln -s /usr/bin/batcat ${VIM_USER_HOME}/.local/bin/bat

RUN apt-get install language-pack-ru -y
RUN locale-gen en_US.UTF-8 ru_RU.UTF-8
#RUN update-locale LANG=en_US.UTF-8
RUN update-locale LANG=ru_RU.UTF-8

RUN apt-get clean && apt-get autoremove --purge && apt-get autoclean

RUN cp ${LOCAL_VIMRC} ${VIM_USER_HOME}/.vimrc
RUN cp -r ./syntax ${VIM_USER_HOME}/.vim
#RUN curl -fLo ${VIM_USER_HOME}/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
RUN cp -r ./autoload ${VIM_USER_HOME}/.vim

VOLUME [$SRC_DIR]

WORKDIR $SRC_DIR
#VIM_USER $VIM_USER:$VIM_USER

CMD \
    cat /etc/lsb-release && \
    echo "VIM_USER=${VIM_USER}" && \
    echo "VIM_USER_HOME=${VIM_USER_HOME}" && \
    ls -la /home && \
    ls -la $VIM_WORK_DIR && \
    pwd && \
    vim --version && \
    /bin/bash $VIM_WORK_DIR/run.sh $VIM_WORK_DIR/run_vim.sh
