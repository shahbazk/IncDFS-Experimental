# ******************************************************************************
# * Makefile for the project.
# *
# * Part of Project: Incremental-DFS-Experimental: 
# *                  https://github.com/shahbazk/IncDFS-Experimental
# *
# * Copyright (C) 2017 Shahbaz Khan <shahbazk@cse.iitk.ac.in>
# *
# * All rights reserved. Published under the BSD-2 license in the LICENSE file.
# ******************************************************************************/


OBJS = Tree.h Tree.o Graph.o
CC = g++ -std=c++0x  #g++ -std=c++11
DEBUG = -g
CFLAGS = -Wall -c -O3 $(DEBUG)
LFLAGS = -Wall -O3 $(DEBUG)
PTH = .
SRC = $(PTH)/src
INC = $(PTH)/include
BIN = $(PTH)/bin
TST = $(PTH)/tests

.PHONY: clean all test exp 

all: $(BIN)/Logger.o $(BIN)/Tree.o $(BIN)/Graph.o $(BIN)/LevelAnc.o $(BIN)/VerifyDFS.o\
     $(BIN)/StaticDFS.o $(BIN)/IncSDFS.o $(BIN)/DS_WDFS.o $(BIN)/FTDFS.o $(BIN)/IncWDFS.o \
     $(BIN)/IncFDFS.o $(BIN)/IncADFS.o

$(BIN)/Logger.o : $(SRC)/Logger.cpp $(INC)/Logger.h
	$(CC) $(CFLAGS) $(SRC)/Logger.cpp -o $(BIN)/Logger.o
	$(CC) $(CFLAGS) -D FAST $(SRC)/Logger.cpp -o $(BIN)/LoggerF.o

$(BIN)/Tree.o : $(SRC)/Tree.cpp $(INC)/Tree.h
	$(CC) $(CFLAGS) $(SRC)/Tree.cpp -o $(BIN)/Tree.o
	$(CC) $(CFLAGS) -D FAST $(SRC)/Tree.cpp -o $(BIN)/TreeF.o

$(BIN)/Graph.o : $(SRC)/Graph.cpp $(INC)/Graph.h
	$(CC) $(CFLAGS) $(SRC)/Graph.cpp -o $(BIN)/Graph.o
	$(CC) $(CFLAGS) -D FAST $(SRC)/Graph.cpp -o $(BIN)/GraphF.o

$(BIN)/StaticDFS.o : $(SRC)/StaticDFS.cpp $(INC)/StaticDFS.h $(INC)/Tree.h $(INC)/Graph.h \
			$(INC)/Logger.h
	$(CC) $(CFLAGS) $(SRC)/StaticDFS.cpp -o $(BIN)/StaticDFS.o 

$(BIN)/StaticDFS_INT.o : $(SRC)/StaticDFS.cpp $(INC)/StaticDFS.h $(INC)/Tree.h $(INC)/Graph.h \
			$(INC)/Logger.h
	$(CC) $(CFLAGS) -D DFS_INTERRUPT $(SRC)/StaticDFS.cpp -o $(BIN)/StaticDFS_INT.o 

$(BIN)/LevelAnc.o : $(SRC)/LevelAnc.cpp $(INC)/LevelAnc.h $(INC)/Tree.h $(INC)/Graph.h
	$(CC) $(CFLAGS) $(SRC)/LevelAnc.cpp -o $(BIN)/LevelAnc.o
	$(CC) $(CFLAGS) -D FAST $(SRC)/LevelAnc.cpp -o $(BIN)/LevelAncF.o


$(BIN)/IncSDFS.o : $(SRC)/IncSDFS.cpp $(INC)/IncSDFS.h $(INC)/Tree.h $(INC)/Graph.h \
		     $(INC)/StaticDFS.h $(INC)/LevelAnc.h $(INC)/Logger.h
	$(CC) $(CFLAGS) $(SRC)/IncSDFS.cpp -o $(BIN)/IncSDFS.o 

$(BIN)/IncFDFS.o : $(SRC)/IncFDFS.cpp $(INC)/IncFDFS.h $(INC)/Tree.h $(INC)/Graph.h \
		     $(INC)/StaticDFS.h $(INC)/LevelAnc.h $(INC)/Logger.h
	$(CC) $(CFLAGS) $(SRC)/IncFDFS.cpp -o $(BIN)/IncFDFS.o 

$(BIN)/IncADFS.o : $(SRC)/IncADFS.cpp $(INC)/IncADFS.h $(INC)/Tree.h $(INC)/Graph.h \
		     $(INC)/StaticDFS.h $(INC)/LevelAnc.h $(INC)/Logger.h
	$(CC) $(CFLAGS) $(SRC)/IncADFS.cpp -o $(BIN)/IncADFS.o 


$(BIN)/VerifyDFS.o : $(SRC)/VerifyDFS.cpp $(INC)/VerifyDFS.h $(INC)/Tree.h $(INC)/Graph.h \
			$(INC)/LevelAnc.h
	$(CC) $(CFLAGS) $(SRC)/VerifyDFS.cpp -o $(BIN)/VerifyDFS.o 


$(BIN)/DS_WDFS.o : $(INC)/DS_WDFS.h $(SRC)/DS_WDFS.cpp $(SRC)/DS_WDFS.Init.cpp \
		   $(INC)/StaticDFS.h $(INC)/Tree.h $(INC)/Graph.h $(INC)/Logger.h
	$(CC) $(CFLAGS) $(SRC)/DS_WDFS.cpp -o $(BIN)/DS_WDFS.o 


$(BIN)/FTDFS.o : $(SRC)/FTDFS.cpp $(INC)/FTDFS.h $(INC)/DS_WDFS.h $(INC)/StaticDFS.h \
		 $(INC)/Tree.h $(INC)/Graph.h	$(INC)/Logger.h
	$(CC) $(CFLAGS) $(SRC)/FTDFS.cpp -o $(BIN)/FTDFS.o 
       

$(BIN)/IncWDFS.o : $(SRC)/IncWDFS.cpp $(INC)/IncWDFS.h $(INC)/FTDFS.h $(INC)/DS_WDFS.h \
		 $(INC)/StaticDFS.h $(INC)/Tree.h $(INC)/Graph.h $(INC)/Logger.h
	$(CC) $(CFLAGS) $(SRC)/IncWDFS.cpp -o $(BIN)/IncWDFS.o 



#########
# TESTS #
#########

$(BIN)/testTG : $(TST)/testTreeGraph.cpp $(BIN)/Tree.o $(BIN)/Graph.o \
		$(BIN)/VerifyDFS.o $(BIN)/LevelAnc.o $(BIN)/Logger.o	 
	$(CC) $(LFLAGS) $(TST)/testTreeGraph.cpp -o $(BIN)/testTG $(BIN)/Tree.o $(BIN)/Graph.o \
			$(BIN)/VerifyDFS.o $(BIN)/LevelAnc.o	$(BIN)/Logger.o

$(BIN)/testSDFS :  $(TST)/testSDFS.cpp	$(BIN)/StaticDFS.o $(BIN)/Tree.o $(BIN)/Graph.o \
			$(BIN)/VerifyDFS.o $(BIN)/LevelAnc.o $(BIN)/Logger.o	 
	$(CC) $(LFLAGS) $(TST)/testSDFS.cpp -o $(BIN)/testSDFS \
		$(BIN)/StaticDFS.o $(BIN)/Tree.o $(BIN)/Graph.o \
		$(BIN)/VerifyDFS.o $(BIN)/LevelAnc.o $(BIN)/Logger.o	

$(BIN)/testIncSDFS : $(TST)/testIncSDFS.cpp $(BIN)/IncSDFS.o $(BIN)/StaticDFS.o \
	  $(BIN)/Tree.o $(BIN)/Graph.o $(BIN)/VerifyDFS.o $(BIN)/LevelAnc.o\
	$(BIN)/Logger.o		
	$(CC) $(LFLAGS) $(TST)/testIncSDFS.cpp -o $(BIN)/testIncSDFS $(BIN)/IncSDFS.o \
	 	$(BIN)/StaticDFS.o $(BIN)/Tree.o $(BIN)/Graph.o \
	 	$(BIN)/VerifyDFS.o $(BIN)/LevelAnc.o $(BIN)/Logger.o	
	
$(BIN)/testIncFDFS : $(TST)/testIncFDFS.cpp $(BIN)/IncFDFS.o $(BIN)/StaticDFS.o \
	  $(BIN)/Tree.o $(BIN)/Graph.o $(BIN)/VerifyDFS.o $(BIN)/LevelAnc.o\
	$(BIN)/Logger.o		
	$(CC) $(LFLAGS) $(TST)/testIncFDFS.cpp -o $(BIN)/testIncFDFS $(BIN)/IncFDFS.o \
	 	$(BIN)/StaticDFS.o $(BIN)/Tree.o $(BIN)/Graph.o \
	 	$(BIN)/VerifyDFS.o $(BIN)/LevelAnc.o $(BIN)/Logger.o	
	
$(BIN)/testIncADFS : $(TST)/testIncADFS.cpp $(BIN)/IncADFS.o $(BIN)/StaticDFS.o \
	  $(BIN)/Tree.o $(BIN)/Graph.o $(BIN)/VerifyDFS.o $(BIN)/LevelAnc.o\
	$(BIN)/Logger.o		
	$(CC) $(LFLAGS) $(TST)/testIncADFS.cpp -o $(BIN)/testIncADFS $(BIN)/IncADFS.o \
	 	$(BIN)/StaticDFS.o $(BIN)/Tree.o $(BIN)/Graph.o \
	 	$(BIN)/VerifyDFS.o $(BIN)/LevelAnc.o $(BIN)/Logger.o	
	
$(BIN)/testDS_WDFS : $(TST)/testDS_WDFS.cpp  $(BIN)/DS_WDFS.o $(BIN)/StaticDFS.o \
	 $(BIN)/Tree.o $(BIN)/Graph.o 	$(BIN)/VerifyDFS.o $(BIN)/LevelAnc.o $(BIN)/Logger.o	
	$(CC) $(LFLAGS) $(TST)/testDS_WDFS.cpp -o $(BIN)/testDS_WDFS \
		 $(BIN)/DS_WDFS.o $(BIN)/StaticDFS.o $(BIN)/Tree.o $(BIN)/Graph.o $(BIN)/Logger.o 

$(BIN)/testFTDFS : $(TST)/testFTDFS.cpp $(BIN)/FTDFS.o $(BIN)/DS_WDFS.o $(BIN)/StaticDFS.o \
	$(BIN)/Tree.o $(BIN)/Graph.o $(BIN)/VerifyDFS.o $(BIN)/LevelAnc.o $(BIN)/Logger.o	
	$(CC) $(LFLAGS) $(TST)/testFTDFS.cpp -o $(BIN)/testFTDFS $(BIN)/FTDFS.o \
		$(BIN)/DS_WDFS.o $(BIN)/StaticDFS.o $(BIN)/Tree.o $(BIN)/Graph.o\
		$(BIN)/VerifyDFS.o $(BIN)/LevelAnc.o $(BIN)/Logger.o 

$(BIN)/testIncWDFS : $(TST)/testIncWDFS.cpp $(BIN)/IncWDFS.o $(BIN)/FTDFS.o $(BIN)/StaticDFS.o \
	  $(BIN)/DS_WDFS.o $(BIN)/Tree.o $(BIN)/Graph.o $(BIN)/VerifyDFS.o $(BIN)/LevelAnc.o\
	$(BIN)/Logger.o		
	$(CC) $(LFLAGS) $(TST)/testIncWDFS.cpp -o $(BIN)/testIncWDFS $(BIN)/IncWDFS.o \
	 $(BIN)/FTDFS.o	$(BIN)/StaticDFS.o $(BIN)/Tree.o $(BIN)/Graph.o \
	 $(BIN)/DS_WDFS.o  $(BIN)/VerifyDFS.o $(BIN)/LevelAnc.o $(BIN)/Logger.o	
	


###############
# Experiments #
###############

TLIB= $(BIN)/Tree.o $(BIN)/Graph.o $(BIN)/LevelAnc.o $(BIN)/Logger.o $(BIN)/StaticDFS.o
TLIBF= $(TLIB) $(BIN)/IncFDFS.o
TLIBS= $(TLIB) $(BIN)/IncSDFS.o
TLIBW= $(TLIB) $(BIN)/FTDFS.o $(BIN)/DS_WDFS.o

$(BIN)/IncFDFS_VarN :  $(TST)/IncDFS_Exp.cpp $(TLIBF)
	$(CC) $(LFLAGS) -D DAG $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncFDFS_VarM $(TLIBF)
	$(CC) $(LFLAGS) -D DAG -D VARN $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncFDFS_VarN $(TLIBF)
	$(CC) $(LFLAGS) -D DIRECTED $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncFDFS_D_VarM $(TLIBF)
	$(CC) $(LFLAGS) -D DIRECTED -D VARN $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncFDFS_D_VarN $(TLIBF)

$(BIN)/IncADFS_VarN :  $(TST)/IncDFS_Exp.cpp $(TLIB) $(BIN)/IncADFS.o
	$(CC) $(LFLAGS) -D ADFS $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncADFS_VarM \
			$(TLIB) $(BIN)/IncADFS.o
	$(CC) $(LFLAGS) -D VARN -D ADFS $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncADFS_VarN \
			$(TLIB) $(BIN)/IncADFS.o

$(BIN)/IncWDFS_VarN :  $(TST)/IncDFS_Exp.cpp $(TLIBW) $(BIN)/IncWDFS.o
	$(CC) $(LFLAGS)  $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncWDFS_VarM $(TLIBW) $(BIN)/IncWDFS.o
	$(CC) $(LFLAGS) -D VARN $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncWDFS_VarN $(TLIBW) $(BIN)/IncWDFS.o
	
$(BIN)/IncSDFS_VarN :  $(TST)/IncDFS_Exp.cpp $(TLIBS)
	$(CC) $(LFLAGS) -D Static_DFS $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncSDFS_VarM $(TLIBS)
	$(CC) $(LFLAGS) -D Static_DFS -D VARN $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSDFS_VarN $(TLIBS)
	$(CC) $(LFLAGS) -D Static_DFS -D BROOM $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSBroom_VarM $(TLIBS)
	$(CC) $(LFLAGS) -D Static_DFS -D VARN -D BROOM $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSBroom_VarN $(TLIBS)
	$(CC) $(LFLAGS) -D Static_DFS -D BROOM -D COMPONENTS $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSBroomC_VarM $(TLIBS)
	$(CC) $(LFLAGS) -D Static_DFS -D BROOM -D BRISTLE $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSBroomB_VarM $(TLIBS)
	$(CC) $(LFLAGS) -D DIRECTED -D Static_DFS $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSDFS_D_VarM $(TLIBS)
	$(CC) $(LFLAGS) -D DIRECTED -D Static_DFS -D VARN $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSDFS_D_VarN $(TLIBS)
	$(CC) $(LFLAGS) -D DAG -D Static_DFS $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSDFS_DAG_VarM $(TLIBS)
	$(CC) $(LFLAGS) -D DAG -D Static_DFS -D VARN $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSDFS_DAG_VarN $(TLIBS)
	$(CC) $(LFLAGS) -D DAG -D Static_DFS -D BROOM $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSBroom_DAG_VarM $(TLIBS)
	$(CC) $(LFLAGS) -D DIRECTED -D Static_DFS -D BROOM $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSBroom_D_VarM $(TLIBS)
 

	
TLIBS2= $(BIN)/Tree.o $(BIN)/Graph.o $(BIN)/LevelAnc.o $(BIN)/Logger.o $(BIN)/StaticDFS_INT.o $(BIN)/IncSDFS.o

$(BIN)/IncSDFS2_VarN :  $(TST)/IncDFS_Exp.cpp $(TLIBS2)
	$(CC) $(LFLAGS) -D Static_DFS $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncSDFS2_VarM $(TLIBS2)
	$(CC) $(LFLAGS) -D Static_DFS -D VARN $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSDFS2_VarN $(TLIBS2)
	$(CC) $(LFLAGS) -D DIRECTED -D Static_DFS $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncSDFS2_D_VarM $(TLIBS2)
	$(CC) $(LFLAGS) -D DIRECTED -D Static_DFS -D VARN $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSDFS2_D_VarN $(TLIBS2)
	$(CC) $(LFLAGS) -D DAG -D Static_DFS $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncSDFS2_DAG_VarM $(TLIBS2)
	$(CC) $(LFLAGS) -D DAG -D Static_DFS -D VARN $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSDFS2_DAG_VarN $(TLIBS2)

########################
#### REAL DATA EXP #####
########################

TLIBf= $(BIN)/TreeF.o $(BIN)/GraphF.o $(BIN)/LevelAncF.o $(BIN)/LoggerF.o $(BIN)/StaticDFS.o
TLIBFF= $(TLIBf) $(BIN)/IncFDFS.o
TLIBSF= $(TLIBf) $(BIN)/IncSDFS.o
TLIBWF= $(TLIBf) $(BIN)/FTDFS.o $(BIN)/DS_WDFS.o

$(BIN)/IncFDFS_Real :  $(TST)/IncDFS_Exp.cpp $(TST)/IncDFS_Real_Fast.cpp $(TLIBFF)
	$(CC) $(LFLAGS) -D REAL -D DAG $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncFDFS_Real $(TLIBFF)
	$(CC) $(LFLAGS) -D REAL -D DIRECTED $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncFDFS_D_Real $(TLIBFF)
	$(CC) $(LFLAGS) -D DAG $(TST)/IncDFS_Real_Fast.cpp -o $(BIN)/IncFDFS_RealF $(TLIBFF)
	$(CC) $(LFLAGS) -D DIRECTED $(TST)/IncDFS_Real_Fast.cpp -o $(BIN)/IncFDFS_D_RealF $(TLIBFF)


$(BIN)/IncADFS_Real :  $(TST)/IncDFS_Exp.cpp $(TST)/IncDFS_Real_Fast.cpp $(TLIB) $(BIN)/IncADFS.o
	$(CC) $(LFLAGS) -D REAL -D ADFS $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncADFS_Real \
			$(TLIB) $(BIN)/IncADFS.o
	$(CC) $(LFLAGS) -D ADFS $(TST)/IncDFS_Real_Fast.cpp -o $(BIN)/IncADFS_RealF \
			$(TLIBf) $(BIN)/IncADFS.o

$(BIN)/IncWDFS_Real :  $(TST)/IncDFS_Exp.cpp $(TST)/IncDFS_Real_Fast.cpp $(TLIBW) $(BIN)/IncWDFS.o
	$(CC) $(LFLAGS) -D REAL  $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncWDFS_Real \
			$(TLIBW) $(BIN)/IncWDFS.o
	$(CC) $(LFLAGS) $(TST)/IncDFS_Real_Fast.cpp -o $(BIN)/IncWDFS_RealF \
			$(TLIBWF) $(BIN)/IncWDFS.o
	
$(BIN)/IncSDFS_Real :  $(TST)/IncDFS_Exp.cpp $(TST)/IncDFS_Real_Fast.cpp $(TLIBS)
	$(CC) $(LFLAGS) -D REAL -D Static_DFS $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncSDFS_Real $(TLIBS)
	$(CC) $(LFLAGS) -D REAL -D DIRECTED -D Static_DFS $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSDFS_D_Real $(TLIBS)
	$(CC) $(LFLAGS) -D REAL -D DAG -D Static_DFS $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSDFS_DAG_Real $(TLIBS)
	$(CC) $(LFLAGS) -D Static_DFS $(TST)/IncDFS_Real_Fast.cpp -o $(BIN)/IncSDFS_RealF $(TLIBSF)
	$(CC) $(LFLAGS) -D DIRECTED -D Static_DFS $(TST)/IncDFS_Real_Fast.cpp \
			-o $(BIN)/IncSDFS_D_RealF $(TLIBSF)
	$(CC) $(LFLAGS) -D DAG -D Static_DFS $(TST)/IncDFS_Real_Fast.cpp \
			-o $(BIN)/IncSDFS_DAG_RealF $(TLIBSF)
	
$(BIN)/IncSDFS2_Real :  $(TST)/IncDFS_Exp.cpp $(TST)/IncDFS_Real_Fast.cpp $(TLIBS2)
	$(CC) $(LFLAGS) -D REAL -D Static_DFS $(TST)/IncDFS_Exp.cpp -o $(BIN)/IncSDFS2_Real $(TLIBS2)
	$(CC) $(LFLAGS) -D REAL -D DIRECTED -D Static_DFS $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSDFS2_D_Real $(TLIBS2)
	$(CC) $(LFLAGS) -D REAL -D DAG -D Static_DFS $(TST)/IncDFS_Exp.cpp \
			-o $(BIN)/IncSDFS2_DAG_Real $(TLIBS2)
	


clean:
	rm bin/*.o
	rm bin/*

test: $(BIN)/testTG $(BIN)/testSDFS $(BIN)/testDS_WDFS $(BIN)/testFTDFS \
	 $(BIN)/testIncWDFS $(BIN)/testIncFDFS \
	 $(BIN)/testIncSDFS $(BIN)/testIncADFS 
	 $(BIN)/testTG
	 $(BIN)/testSDFS
	 $(BIN)/testIncFDFS
	 $(BIN)/testDS_WDFS
	 $(BIN)/testFTDFS
	 $(BIN)/testIncWDFS
	 $(BIN)/testIncADFS
	 $(BIN)/testIncSDFS

exp: $(BIN)/IncSDFS_VarN $(BIN)/IncWDFS_VarN $(BIN)/IncFDFS_VarN $(BIN)/IncSDFS2_VarN \
	$(BIN)/IncADFS_VarN $(BIN)/IncADFS_Real $(BIN)/IncSDFS_Real $(BIN)/IncWDFS_Real \
	$(BIN)/IncFDFS_Real $(BIN)/IncSDFS2_Real 
