ARG CODENAME=xenial
ARG VSN="3.18.1"

FROM ubuntu:${CODENAME}
LABEL author="Alexey Yershov"
LABEL version="${VSN}"
LABEL description="SMG2016 Toolchain for building deps"
LABEL maintainer="Eltex"

ENV \
    USER=eltex \
    DEBIAN_FRONTEND=noninteractive \
    CODENAME=${CODENAME} \
    TERM=xterm \
    BU_LOCAL=true \
    BU_GENERATION=201802050700 \
    TZ=Asia/Novosibirsk \
    AWS_ENDPOINT="https://s3.ngn.eltex.loc:9000" \
    APT_REPO="http://apt.ngn.eltex.loc" \
    APT_PROXY="http://apt.proxy.ngn.eltex.loc:8000" \
    AWS_SECRET_ACCESS_KEY=ssw4MountsSecret \
    AWS_ACCESS_KEY_ID=ssw4MountsAccess \
    AWS_CA_BUNDLE=/usr/local/share/ca-certificates/EltexRootCA.crt

ENV PATH="/toolchain/armv7-marvell-linux-gnueabi-softfp_i686/bin/:${PATH}"
ENV PATH=toolchain/marvell/bin/:${PATH}

ADD ${APT_REPO}/eltex.gpg /etc/apt/trusted.gpg.d/eltex.gpg
ADD http://intdocs.eltex.loc:8090/download/attachments/337631/EltexRootCA.crt?version=1&modificationDate=1574070724000&api=v2 \
    ${AWS_CA_BUNDLE}

# RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN \
    chmod +r ${AWS_CA_BUNDLE} &&\
    adduser --disabled-password --gecos '' ${USER} &&\
    mkdir -p /home/${USER}/.cache &&\
    chown -Rc ${USER}:${USER} "/home/${USER}/" &&\
    chmod 644 /etc/apt/trusted.gpg.d/eltex.gpg &&\
    sed -i -e "s/^deb-src/#deb-src/" /etc/apt/sources.list &&\
    echo "deb [arch=amd64] ${APT_REPO}/bionic/3.14 unstable external" > /etc/apt/sources.list.d/eltex-ecss10-unstable.list && \
    echo "Acquire::http::Proxy \"${APT_PROXY}\";" >> /etc/apt/apt.conf.d/00aptproxy

RUN \
    apt-get update &&\
    apt-get --quiet --yes upgrade &&\
    apt-get --quiet --yes --no-install-recommends install \
    apt-utils automake bash bash-completion build-essential chrpath curl \
    debconf debhelper devscripts dh-make dh-systemd dirmngr dpkg dput-ng \
    fakeroot gcc git git-buildpackage gnupg ca-certificates \
    libsystemd-dev lintian locales aptitude gawk \
    make python-paramiko ssh sudo wget texinfo awscli\
    g++ python2.7 libtool-bin autoconf pkg-config zlib1g-dev autotools-dev \
    flex bison rsync lib32z1 lib32ncurses5 gettext autopoint kmod cpio \
    libtool tftp-hpa libglib2.0-dev cmake patch cppcheck asn1c u-boot-tools mtd-utils && \
    echo "${USER} ALL=NOPASSWD: ALL" > /etc/sudoers.d/${USER} &&\
    update-ca-certificates &&\
    locale-gen en_US.UTF-8 ru_RU.UTF-8 &&\
    update-locale LANG=en_US.UTF-8 &&\
    apt-get clean && apt-get autoremove --purge && apt-get autoclean &&\
    ln -s /usr/bin/make /usr/bin/gmake &&\
    ln -s /usr/bin/aclocal-1.15 /usr/bin/aclocal-1.14 &&\
    ln -s /usr/bin/automake-1.15 /usr/bin/automake-1.14 &&\
    aws --endpoint-url ${AWS_ENDPOINT} s3 cp s3://files/ecss-toolchain-lib/ecss-toolchain-lib.tar.gz /ecss-toolchain-lib.tar.gz &&\
    tar -xvf /ecss-toolchain-lib.tar.gz && rm /ecss-toolchain-lib.tar.gz &&\
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

RUN \
    aws --endpoint-url ${AWS_ENDPOINT} s3 cp s3://files/smg-toolchain/smg2016_toolchain.tar.gz /smg2016_toolchain.tar.gz &&\
    mkdir -p /toolchain && tar -xf /smg2016_toolchain.tar.gz -C /toolchain && rm /smg2016_toolchain.tar.gz

#from PVS-studio
RUN \
    apt-get update -yq \
    && apt-get install -yq apt-transport-https \
    && apt-get install -yq pvs-studio strace \
    && apt-get clean -yq

WORKDIR /home/${USER}/project
VOLUME ["/home/${USER}/project"]
CMD ["/bin/run.sh /bin/bash"]
