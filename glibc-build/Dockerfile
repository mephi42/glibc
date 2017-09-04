FROM fedora:26
MAINTAINER mephi42 <mephi42@gmail.com>
RUN dnf install -y @development-tools dnf-plugins-core && \
    dnf builddep -y glibc
