FROM gitpod/workspace-full-vnc
RUN sudo apt-get update && \
    sudo rm -rf /var/lib/apt/lists/*
RUN sudo apt install xorg-dev