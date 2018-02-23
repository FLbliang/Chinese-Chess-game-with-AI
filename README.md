﻿# Description
经典的中国象棋游戏，玩法是AI与玩家对战
PS: 整个项目的代码需要在配合C++ Qt框架来编译使用
##核心技术
###界面UI
UI采用的是QT框架的界面来显示
###搜索算法
代码里实现了三种AI搜索算法，分别是alpha beta剪枝搜索，MTD算法和PVS算法，经过本人测试比较，PVS算法的搜索速度最快，所以默认采用的是PVS搜索
同时利用哈希算法配合搜索算法极大优化了搜索算法性能。
###核心判断函数
判断函数是重中之重，需要考虑棋子本身的权重，特定棋子在特定位置的权重，以及棋子在不同局势中它的权重等，
需要综合判断各种影响来确定棋子的权重，从而提供不同分数的局面形成剪枝树让搜索算法去判断剪枝。
###AI的水平
经过多次测试，AI在level为6以上超过了腾讯游戏象棋中级水平，有一定几率战胜高级，但是level为6的搜索速度较慢
而level为5的水平与腾讯游戏象棋中的中级水平相当，搜索速度也比较可观

