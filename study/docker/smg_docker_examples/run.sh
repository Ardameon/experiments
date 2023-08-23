#!/bin/bash -e
DEFAULT_UID=1000
DEFAULT_GID=1000
DEFAULT_USER=eltex

if [ -z ${UID+x} ]; then
    echo "UID is unset, use ${DEFAULT_UID}"
    UID=${DEFAULT_UID}
fi
if [ -z ${GID+x} ]; then
    echo "GID is unset, use ${DEFAULT_GID}"
    GID=${DEFAULT_GID}
fi

# Изменяем пользователя eltex под uid/gid процесса запустившего контейнер
echo "user: ${USER}(${UID}/${GID}), home: ${HOME}, execute: $*"
sed -i -e "s/${DEFAULT_USER}:x:[[:digit:]]*:[[:digit:]]*:/${DEFAULT_USER}:x:$UID:$GID:/g" /etc/passwd
sed -i -e "s/${DEFAULT_USER}:x:[[:digit:]]*:/${DEFAULT_USER}:x:$GID:root/g" /etc/group
chown -Rv "${USER}":"${USER}" /home/${DEFAULT_USER}
chown -Rv "${USER}":"${USER}" /home/${DEFAULT_USER}/.cache /home/${DEFAULT_USER}/.profile \
    /home/${DEFAULT_USER}/.bash* /home/${DEFAULT_USER}/.erlang* &> /dev/null || true
exec runuser -u ${USER} "$@"
/usr/bin/sudo -E -H -u "${USER}" PATH=${PATH} /bin/bash -c "$*"
