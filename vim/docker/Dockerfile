ARG CODENAME=jammy

FROM ubuntu:${CODENAME}

LABEL author="Mikhail Tyryshkin"
LABEL description="VIM with appropriate pluggins to be good"

ENV DIR=/vim
ENV TERM=xterm
ENV SRC_DIR=/src_dir
ENV LOCAL_VIMRC=.vimrc

WORKDIR $DIR

COPY . .

RUN echo "This message will appear only on image build"
RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get install dialog -y
RUN apt-get install apt-utils -y
RUN apt-get install git -y
RUN apt-get install vim -y
RUN apt-get install curl -y
RUN apt-get install fonts-powerline -y
RUN apt-get install fzf -y
RUN apt-get install ripgrep -y
RUN apt-get install locales -y

RUN apt-get install bat -y && \
    mkdir -p ~/.local/bin && \
    ln -s /usr/bin/batcat ~/.local/bin/bat

RUN apt-get install language-pack-ru -y
RUN locale-gen en_US.UTF-8 ru_RU.UTF-8
#RUN update-locale LANG=en_US.UTF-8
RUN update-locale LANG=ru_RU.UTF-8

RUN apt-get clean && apt-get autoremove --purge && apt-get autoclean

RUN cp docker_vimrc ~/.vimrc
RUN cp -r ./syntax ~/.vim
#RUN curl -fLo ~/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
RUN cp -r ./autoload ~/.vim

VOLUME [$SRC_DIR]

WORKDIR $SRC_DIR

CMD \
    cat /etc/lsb-release && \
    echo "USER=${USER}" && \
    echo "HOME=${HOME}" && \
    ls -la /home && \
    ls -la $DIR && \
    pwd && \
    vim --version && \
    vim +'PlugUpdate --sync' +qa && \
    vim /src_dir

#only one CMD is available for Dockerfile
#in multiple case only the las one will be aquired
#CMD [ "cat", "/etc/lsb-release" ]
#CMD [ "gcc", "--version" ]
#CMD [ "echo", "Something logging text!" ]
#CMD [ "ls", "-l" ]
#CMD [ "/bin/sh", "hello" ]
#CMD [ "gcc", "main.c" ]
#CMD [ "/app/a.out" ]
