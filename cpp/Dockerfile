# 使用基础镜像，包含 C++ 编译器
FROM ubuntu:22.04

# 设置非交互模式以避免安装过程中出现提示
ENV DEBIAN_FRONTEND=noninteractive

# 更新包列表并安装必要的软件包
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    gdb \
    vim \
    && rm -rf /var/lib/apt/lists/*

# 设置工作目录
WORKDIR /app

# 安装 VSCode C++ 扩展依赖（如需要）
RUN apt-get update && apt-get install -y \
    libgl1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

# 默认命令启动 bash
CMD ["bash"]
