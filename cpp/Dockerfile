# 使用基础镜像，包含 C++ 编译器
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    wget \
    software-properties-common \
    lsb-release \
    gnupg2 \
    curl \
    git \
    make \
    cmake \
    vim \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

RUN curl -fsSL https://apt.llvm.org/llvm.sh | bash -s -- 19

RUN apt-get update && apt-get install -y \
    clang-19 \
    clang++-19 \
    clangd-19 \
    lldb-19 \
    libc++-19-dev \
    libc++abi-19-dev \
    && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/clang clang /usr/bin/clang-19 100 \
    && update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-19 100 \
    && update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-19 100 \
    && update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-19 100

WORKDIR /workspace

CMD ["bash"]
