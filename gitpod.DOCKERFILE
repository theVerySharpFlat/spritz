FROM gitpod/workspace-full-vnc
RUN sudo apt-get update
RUN sudo apt install -y xorg-dev libgl1-mesa-dev cmake ninja-build
