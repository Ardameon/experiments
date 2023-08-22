ARG CODENAME=jammy

FROM ubuntu:${CODENAME}

LABEL author="Mikhail Tyryshkin"
LABEL description="VIM with appropriate pluggins to be good"

ENV DIR=/vim
ENV TERM=xterm
ENV SRC_DIR=/src_dir
ENV LOCAL_VIMRC=docker_vimrc
ENV USER=vimusr
ENV USER_HOME=/home/${USER}

WORKDIR $DIR

COPY . .

RUN adduser --disabled-password --gecos '' ${USER}
RUN mkdir -p ${USER_HOME}/.cache

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
    mkdir -p ${USER_HOME}/.local/bin && \
    ln -s /usr/bin/batcat ${USER_HOME}/.local/bin/bat

RUN apt-get install language-pack-ru -y
RUN locale-gen en_US.UTF-8 ru_RU.UTF-8
#RUN update-locale LANG=en_US.UTF-8
RUN update-locale LANG=ru_RU.UTF-8

RUN apt-get clean && apt-get autoremove --purge && apt-get autoclean

RUN cp ${LOCAL_VIMRC} ${USER_HOME}/.vimrc
RUN cp -r ./syntax ${USER_HOME}/.vim
#RUN curl -fLo ${USER_HOME}/.vim/autoload/plug.vim --create-dirs https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
RUN cp -r ./autoload ${USER_HOME}/.vim
RUN chown -vR ${USER}:${USER} "${USER_HOME}"
RUN ls -la ${USER_HOME}

VOLUME [$SRC_DIR]

WORKDIR $SRC_DIR
USER $USER:$USER

CMD \
    cat /etc/lsb-release && \
    echo "USER=${USER}" && \
    echo "HOME=${HOME}" && \
    ls -la /home && \
    ls -la $DIR && \
    pwd && \
    vim --version && \
    vim +'PlugUpdate --sync' +qa && \
    vim ${SRC_DIR}
