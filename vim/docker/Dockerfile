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
ENV DEFAULT_UID=1000
ENV DEFAULT_GID=1000
ENV UID=${DEFAULT_UID}
ENV GID=${DEFAULT_GID}

WORKDIR $VIM_WORK_DIR

COPY . .

RUN adduser --disabled-password --gecos '' ${VIM_USER}
RUN mkdir -p ${VIM_USER_HOME}/.cache

RUN echo "This message will appear only on image build"
RUN apt-get update -yq
RUN apt-get upgrade -yq
RUN apt-get install dialog -yq
RUN apt-get install apt-utils -yq
RUN apt-get install bash -yq
RUN apt-get install git -yq
RUN apt-get install vim -yq
RUN apt-get install curl -yq
RUN apt-get install fonts-powerline -yq
RUN apt-get install fzf -yq
RUN apt-get install ripgrep -yq
RUN apt-get install locales -yq

RUN apt-get install bat -yq && \
    mkdir -p ${VIM_USER_HOME}/.local/bin && \
    ln -s /usr/bin/batcat ${VIM_USER_HOME}/.local/bin/bat

RUN apt-get install language-pack-ru -yq
RUN locale-gen en_US.UTF-8 ru_RU.UTF-8
#RUN update-locale LANG=en_US.UTF-8
RUN update-locale LANG=ru_RU.UTF-8

RUN apt-get install universal-ctags -yq

RUN apt-get install python3 python3-pip -yq
RUN apt-get install python3-pylsp -yq
RUN apt-get install ccls -yq
RUN pip3 install cmake-language-server

# Need to install nodejs v16 minimum, maybe use image dependency?
#RUN curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/master/install.sh | bash && \
#    nvm install v16 && \
#    npm i -g bash-language-server

RUN apt-get clean && apt-get autoremove --purge && apt-get autoclean

RUN cp ${LOCAL_VIMRC} ${VIM_USER_HOME}/.vimrc
RUN mkdir -p ${VIM_USER_HOME}/.vim/syntax && cp -r ./syntax ${VIM_USER_HOME}/.vim
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
