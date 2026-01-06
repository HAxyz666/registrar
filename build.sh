#!/bin/bash

# 构建C++模块化教务管理系统
echo "正在构建模块..."

# 清理之前的构建文件
rm -rf gcm.cache
rm -f registrar_system

# 使用CMake构建系统
if [ -f "CMakeLists.txt" ]; then
    echo "使用CMake构建..."
    mkdir -p build
    cd build
    cmake ..
    if command -v ninja &> /dev/null; then
        echo "使用Ninja构建..."
        ninja
    else
        echo "使用Make构建..."
        make
    fi
    cd ..
    cp build/REGISTRAR ./registrar_system
    echo "构建完成！"
    echo "运行程序: ./registrar_system"
else
    echo "直接编译所有文件..."
    # 尝试直接编译所有文件
    g++ -std=c++23 -fmodules-ts -I. *.cppm *.cpp -o registrar_system
    if [ $? -eq 0 ]; then
        echo "构建完成！"
        echo "运行程序: ./registrar_system"
    else
        echo "构建失败，尝试使用传统编译方式..."
        # 回退到传统编译方式（不使用模块）
        g++ -std=c++23 -I. *.cpp -o registrar_system
        if [ $? -eq 0 ]; then
            echo "传统编译方式成功！"
            echo "运行程序: ./registrar_system"
        else
            echo "编译失败！"
        fi
    fi
fi