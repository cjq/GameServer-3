# 指定编译器
CC = gcc

# CFLAG包括头文件目录
CFLAGS =  -g -Wall -std=gnu99

# 头文件查找路径
INC = 	
	

# 静态链接库
LIBS = -lpthread
# 目标
TARGET = node
# 源文件
SRC = main.cpp PluginManager.cpp\

# 源文件编译为目标文件
OBJS = $(SRC:.cpp=.o)

.PHONY: clean

# 链接为可执行文件
$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LIBS)

# 清楚可执行文件和目标文件
clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

# 编译规则 加入头文件 $@代表目标文件 $< 代表第一个依赖文件
%.o:%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<