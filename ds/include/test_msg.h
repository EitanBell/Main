#ifndef __TEST_MSG_H__
#define __TEST_MSG_H__

void TestMsgTestTitle(char *tests_name);

void TestMsgTestStat(int stat, char *tests_name);

void TestMsgUnitTitle(char *func_name);

void TestMsgUnitStat(int stat, char *func_name);

void TestMsgCheckTitle(char *tests_name, size_t test_num);

void TestMsgCheckStat(int stat);

#endif /* __TEST_MSG_H__ */

