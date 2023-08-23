#!/bin/bash -e
DEFAULT_UID=1000
DEFAULT_GID=1000
DEFAULT_USER=defuser
CONTAINER_USER=$VIM_USER
CONTAINER_USER_HOME=$VIM_USER_HOME
CONTAINER_WORK_DIR=$VIM_WORK_DIR

if [ -z ${UID+x} ]; then
    echo "UID is unset, use ${DEFAULT_UID}"
    UID=${DEFAULT_UID}
fi
if [ -z ${GID+x} ]; then
    echo "GID is unset, use ${DEFAULT_GID}"
    GID=${DEFAULT_GID}
fi

# Изменяем пользователя eltex под uid/gid процесса запустившего контейнер
echo "user: ${CONTAINER_USER}(${UID}/${GID}), home: ${HOME}, execute: $*"
sed -i -e "s/${CONTAINER_USER}:x:[[:digit:]]*:[[:digit:]]*:/${CONTAINER_USER}:x:$UID:$GID:/g" /etc/passwd
sed -i -e "s/${CONTAINER_USER}:x:[[:digit:]]*:/${CONTAINER_USER}:x:$GID:root/g" /etc/group
chown -Rv "${CONTAINER_USER}":"${CONTAINER_USER}" ${CONTAINER_USER_HOME}
chown -Rv "${CONTAINER_USER}":"${CONTAINER_USER}" ${CONTAINER_USER_HOME}/.cache ${CONTAINER_USER_HOME}/.profile \
    ${CONTAINER_USER_HOME}/.bash* ${CONTAINER_USER_HOME}/.erlang* &> /dev/null || true
chown -Rv "${CONTAINER_USER}":"${CONTAINER_USER}" ${CONTAINER_WORK_DIR}
exec runuser -u ${CONTAINER_USER} "$@"
#/usr/bin/sudo -E -H -u "${CONTAINER_USER}" PATH=${PATH} /bin/bash -c "$*"
