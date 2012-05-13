#include <mysql.h>
#include <stdio.h>

/*
int InitSql(MYSQL* mysql){
	mysql_init(mysql);

	if(!mysql_real_connect(mysql, "localhost", "root", "neostage", 
"br", 0, (char*)NULL, 0)){
		printf("sql connect error");
		return 1;
	}

	return 0;
}
*/
/*
int MySendQuery(char* query, MYSQL_RES** res, int* fields){
	extern MYSQL mysql;
	mysql_real_query(&mysql, query, strlen(query));
	*res= mysql_store_result(&mysql);
	*fields= mysql_field_count(&mysql);
	return 0;
}
*/
