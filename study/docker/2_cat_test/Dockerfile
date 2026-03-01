ARG UBUNTU_VERSION=22.04

FROM ubuntu:${UBUNTU_VERSION}

ENV APP_DIR=/app
ENV APP_VOLUME=/data

RUN echo "Image is using Ubuntu ${APP_DIR}"
# Create user for container
RUN useradd -m -u 1000 appuser \
      && cat /etc/group | grep appuser

WORKDIR ${APP_DIR}
# Exposed port just for example
EXPOSE 5060/udp
# Volume to mount from host
VOLUME ${APP_VOLUME}

# Use created user
USER appuser:appuser

# Copy with changing ownership
COPY --chown=appuser:appuser README .

# Command to be executed (can not be overwritten)
ENTRYPOINT ["cat"]
# Default parameter (can be overwritten by docker run)
CMD ["README"]
