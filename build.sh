#!/bin/bash

git submodule update --init --recursive #初始化子模块

git submodule foreach git checkout master #切换子模块到主分支

git submodule foreach git pull #更新子模块

xcodebuild -jobs 1 -configuration Debug #编译Debug版本
xcodebuild -jobs 1 -configuration Release #编译发布版本
