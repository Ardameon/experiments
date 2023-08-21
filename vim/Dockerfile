ARG CODENAME=jammy

FROM ubuntu:${CODENAME}

LABEL author="Mikhail Tyryshkin"
LABEL descrioption="VIM with appropriate pluggins to be good"

ENV DIR=/vim

WORKDIR $DIR

COPY . .

RUN echo "This message will appear only on image build"
RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get install apt-utils -y
RUN apt-get install vim -y

RUN cp .vimrc ~/

CMD \
    cat /etc/lsb-release && \
    echo "USER=${USER}" && \
    echo "HOME=${HOME}" && \
    ls -la /home && \
    ls -la $DIR && \
    pwd && \
    vim --version && \
    vim

#only one CMD is available for Dockerfile
#in multiple case only the las one will be aquired
#CMD [ "cat", "/etc/lsb-release" ]
#CMD [ "gcc", "--version" ]
#CMD [ "echo", "Something logging text!" ]
#CMD [ "ls", "-l" ]
#CMD [ "/bin/sh", "hello" ]
#CMD [ "gcc", "main.c" ]
#CMD [ "/app/a.out" ]
