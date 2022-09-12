FROM gcc

ENV DIR=/app

WORKDIR $DIR

COPY . .

RUN echo "This message will appear only on image build"

#only one CMD is available for Dockerfile
#in multiple case only the las one will be aquired
CMD [ "gcc", "main.c", "-o", "hello" ]
#CMD [ "gcc", "--version" ]
#CMD [ "echo", "Something logging text!" ]
#CMD [ "ls", "-l" ]
#CMD [ "/bin/sh", "hello" ]
#CMD [ "gcc", "main.c" ]
#CMD [ "/app/a.out" ]
