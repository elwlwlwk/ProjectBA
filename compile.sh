rm -f BAServ
g++ -v -o BAServ *.cpp *.h -pthread -I/usr/include/mysql/ -L/usr/lib/mysql -lmysqlclient
