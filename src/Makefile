cc = gcc
cflag = -c -g -O0
cinclude = -I/usr/include/mysql -I/usr/include/mysql++
clib = -lmysqlpp -lmysqlclient -lfl
srcs = alg.c Animation.c base64.c md5lib.c CmdPackage.c ConsoleColor.c cpuinfo.c dmi_info.c ds_DBLList.c FileReadLine.c hdinfo.c HWInfo.c HWCheck.c OsstartChecker.c lib_StringAdd.c nicinfo.c usbinfo.c AgentConnector.cpp Agent.cpp CmdProcessor.cpp CMSG.cpp HWCollection.cpp MainClient.cpp MainEvent.cpp MainLogger.cpp MainServer.cpp NTCFConnector.cpp NTCFExecutor.cpp NTCFPackage.cpp SQLConnector.cpp SubClient.cpp SubEvent.cpp MulticastServer.cpp MulticastAgent.cpp ClientPolicy.cpp ServerPolicy.cpp User.cpp lex.yy.c y.tab.c PolicyScriptInterpreter.cpp LogicTreeNode.cpp LogicTree.cpp LogicTreeParser.cpp LogicTreeAnalyzer.cpp TokenTaker.cpp ExString.cpp PolicyOperator.cpp LogicStrPolicyConvertor.cpp SoftInfo.cpp PolicyUpdater.cpp NTCSDBConfig.cpp OsstartPolicyOperator.cpp t_ntcs_hwpolicy.cpp t_ntcs_hwpolicy_item.cpp

objects = *.o
ar = libtpmcmn.a

main:
	lex plcyScript.lex
	yacc -d plcyScript.y
	$(cc) $(cflag) $(cinclude) $(srcs)

install:
	ar crv $(ar)  $(objects)
	mv $(ar) /usr/lib/

clean:
	rm $(objects) 
