rm -f BAServ
g++ -v -o BAServ -g *.cpp *.h -pthread -I/usr/include/mysql/ -L/usr/lib/mysql -lmysqlclient
chmod +x BAServ
