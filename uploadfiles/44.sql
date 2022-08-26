CREATE table t1(
	id int(11) NOT NULL auto_increment,
    col1 VARCHAR(20) DEFAULT NULL,
	col2 VARCHAR(20) DEFAULT NULL,
	col3 VARCHAR(20) DEFAULT NULL,
	other_column CHAR(20) DEFAULT NULL,
	PRIMARY KEY (id),
	KEY idx_t1 (other_column),
	KEY col1_col2_col3_idx (col1,col2,col3)
);




mysql> explain  select col1 from t1 where col1 > 'a' ;#直接在辅助索引树上命中
+----+-------------+-------+------------+-------+--------------------+--------------------+---------+------+------+----------+--------------------------+
| id | select_type | table | partitions | type  | possible_keys      | key                | key_len | ref  | rows | filtered | Extra                    |
+----+-------------+-------+------------+-------+--------------------+--------------------+---------+------+------+----------+--------------------------+
|  1 | SIMPLE      | t1    | NULL       | index | col1_col2_col3_idx | col1_col2_col3_idx | 69      | NULL |    1 |   100.00 | Using where; Using index |
+----+-------------+-------+------------+-------+--------------------+--------------------+---------+------+------+----------+--------------------------+
1 row in set, 1 warning (0.00 sec)



mysql> explain  select * from t1 where col1 > 'a' ;#col1-----叶子节点主键 ---- 回表  #
+----+-------------+-------+------------+-------+--------------------+--------------------+---------+------+------+----------+-----------------------+
| id | select_type | table | partitions | type  | possible_keys      | key                | key_len | ref  | rows | filtered | Extra                 |
+----+-------------+-------+------------+-------+--------------------+--------------------+---------+------+------+----------+-----------------------+
|  1 | SIMPLE      | t1    | NULL       | range | col1_col2_col3_idx | col1_col2_col3_idx | 23      | NULL |    1 |   100.00 | Using index condition |
+----+-------------+-------+------------+-------+--------------------+--------------------+---------+------+------+----------+-----------------------+
1 row in set, 1 warning (0.00 sec)

mysql> 
