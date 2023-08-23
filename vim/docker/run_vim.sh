#!/bin/bash -e

vim +'PlugUpdate --sync' +qa && \
vim ${SRC_DIR}
