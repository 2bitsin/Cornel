FROM  ikkepop/ia16-tools:latest
RUN apt update && apt upgrade -y 

# make /bin/sh symlink to bash instead of dash:
RUN echo "dash dash/sh boolean false" | debconf-set-selections
RUN DEBIAN_FRONTEND=noninteractive dpkg-reconfigure dash

# set ENV to execute startup scripts
ENV ENV ~/.profile

ENV WATCOM=/usr/bin/watcom
ENV PATH=/usr/bin/ia16-elf-gcc/bin:$WATCOM/binl64:$PATH

SHELL ["/bin/bash", "-c", "-l"] 