#################################################
#                                               #
#  C++ Makefile                                 #
#  Made by Phantomas <phantomas@phantomas.xyz>  #
#                                               #
#################################################

CXX = g++
ECHO = echo -e 
MKDIR = mkdir -p 
RM = rm -rf
SHELL = /bin/sh

CPPFLAGS += -I srcs

TEST_CPPFLAGS += -I tests/srcs

CXXFLAGS += -std=c++17 
CXXFLAGS += $(DEBUG) -ggdb

TEST_CXXFLAGS += -fprofile-arcs -ftest-coverage --coverage -fno-builtin -O0

DEPFLAGS += -MT $@ -MMD -MP -MF $(DEPSDIR)/$*.Td
TEST_DEPFLAGS += -MT $@ -MMD -MP -MF $(TEST_DEPSDIR)/$*.Td

LDFLAGS += -Wl,-E 

TEST_LDFLAGS += -lcriterion --coverage

SRCSDIR = srcs
OBJSDIR = objs
TESTDIR = tests
LOGSDIR = logs
DEPSDIR = .deps
OUTDIR = bin
REPORTDIR = reports

TEST_SRCSDIR = $(TESTDIR)/$(SRCSDIR)
TEST_OBJSDIR = $(TESTDIR)/$(OBJSDIR)
TEST_LOGSDIR = $(TESTDIR)/$(LOGSDIR)
TEST_DEPSDIR = $(TESTDIR)/$(DEPSDIR)
TEST_OUTDIR = $(OUTDIR)

ERRLOG = 2> $(patsubst $(OBJSDIR)/%,$(LOGSDIR)/%,$(@D))/$(shell basename $@).log

CLEANLOG = if [ ! -s $(patsubst $(OBJSDIR)/%,$(LOGSDIR)/%,$(@D))/$(shell basename $@).log ]; \
		   then $(RM) $(patsubst $(OBJSDIR)/%, $(LOGSDIR)/%,$(@D))/$(shell basename $@).log ; fi

POSTCOMP = mv -f $(DEPSDIR)/$*.Td $(DEPSDIR)/$*.d && touch $@

TEST_ERRLOG = 2> $(patsubst $(TEST_OBJSDIR)/%,$(TEST_LOGSDIR)/%,$(@D))/$(shell basename $@).log
TEST_CLEANLOG = if [ ! -s $(patsubst $(TEST_OBJSDIR)/%,$(TEST_LOGSDIR)/%,$(@D))/$(shell basename $@).log ]; \
		   then $(RM) $(patsubst $(TEST_OBJSDIR)/%, $(TEST_LOGSDIR)/%,$(@D))/$(shell basename $@).log ; fi

TEST_POSTCOMP = mv -f $(TEST_DEPSDIR)/$*.Td $(TEST_DEPSDIR)/$*.d && touch $@

DEFAULT = "\033[00m"
GREEN = "\033[0;32m"
TEAL = "\033[0;36m"
RED = "\033[0;31m"

NAME = 

TEST_NAME = test_container
TEST_REPORT_NAME = $(REPORTDIR)/report.html

SRCS += 

OBJS = $(patsubst $(SRCSDIR)/%,$(OBJSDIR)/%, $(SRCS:.cpp=.o))

TEST_SRCS += $(TEST_SRCSDIR)/test_basics.cpp
TEST_SRCS += $(TEST_SRCSDIR)/test_rvalue.cpp
TEST_SRCS += $(TEST_SRCSDIR)/test_manual.cpp
TEST_SRCS += $(TEST_SRCSDIR)/test_abstract.cpp
TEST_SRCS += $(TEST_SRCSDIR)/test_exceptions.cpp
TEST_SRCS += $(TEST_SRCSDIR)/test_duplicate.cpp
TEST_SRCS += $(TEST_SRCSDIR)/test_duplicate_manual.cpp

TEST_OBJS = $(patsubst $(TEST_SRCSDIR)/%, $(TEST_OBJSDIR)/%, $(TEST_SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@-$(MKDIR) $(OUTDIR)
	@-$(CXX) -o $(OUTDIR)/$(NAME) $^ $(LDFLAGS) \
	 $(ERRLOG) && \
	 $(ECHO) $(GREEN) "[OK]" $(TEAL) $@ $(DEFAULT) || \
	 $(ECHO) $(RED) "[XX]" $(TEAL) $@ $(DEFAULT)
	@-$(CLEANLOG)

$(OBJSDIR)/%.o: OBJSPATH = $(OBJSDIR)/$(*D)/
$(OBJSDIR)/%.o: LOGSPATH = $(LOGSDIR)/$(*D)/
$(OBJSDIR)/%.o: DEPSPATH = $(DEPSDIR)/$(*D)/
$(OBJSDIR)/%.o: $(SRCSDIR)/%.cpp $(DEPSDIR)/%.d
	@-$(MKDIR) $(OBJSPATH)
	@-$(MKDIR) $(LOGSPATH)
	@-$(MKDIR) $(DEPSPATH)
	@-$(RM) $@
	@-$(CXX) -c $< -o $@ $(DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) \
	 $(ERRLOG) && \
	 $(ECHO) $(GREEN) "[OK]" $(TEAL) $< $(DEFAULT) || \
	 $(ECHO) $(RED) "[XX]" $(TEAL) $< $(DEFAULT)
	@-$(POSTCOMP)
	@-$(CLEANLOG)

$(DEPSDIR)/%.d: ;

tests: clean $(TEST_NAME)

$(TEST_NAME): CPPFLAGS += $(TEST_CPPFLAGS)
$(TEST_NAME): CXXFLAGS += $(TEST_CXXFLAGS)
$(TEST_NAME): LDFLAGS += $(TEST_LDFLAGS)
$(TEST_NAME): $(OBJS) $(TEST_OBJS)
	@-$(MKDIR) $(TEST_OUTDIR)
	@-$(CXX) -o $(TEST_OUTDIR)/$(TEST_NAME) $^ $(LDFLAGS) \
	 $(TEST_ERRLOG) && \
	 $(ECHO) $(GREEN) "[OK]" $(TEAL) $@ $(DEFAULT) || \
	 $(ECHO) $(RED) "[XX]" $(TEAL) $@ $(DEFAULT)
	@-$(TEST_CLEANLOG)

$(TEST_OBJSDIR)/%.o: OBJSPATH = $(TEST_OBJSDIR)/$(*D)/
$(TEST_OBJSDIR)/%.o: LOGSPATH = $(TEST_DEPSDIR)/$(*D)/
$(TEST_OBJSDIR)/%.o: DEPSPATH = $(TEST_DEPSDIR)/$(*D)/
$(TEST_OBJSDIR)/%.o: $(TEST_SRCSDIR)/%.cpp $(TEST_DEPSDIR)/%.d
	@-$(MKDIR) $(OBJSPATH)
	@-$(MKDIR) $(LOGSPATH)
	@-$(MKDIR) $(DEPSPATH)
	@-$(RM) $@
	@-$(CXX) -c $< -o $@ $(TEST_DEPFLAGS) $(CPPFLAGS) $(CXXFLAGS) \
	 $(TEST_ERRLOG) && \
	 $(ECHO) $(GREEN) "[OK]" $(TEAL) $< $(DEFAULT) || \
	 $(ECHO) $(RED) "[XX]" $(TEAL) $< $(DEFAULT)
	@-$(TEST_POSTCOMP)
	@-$(TEST_CLEANLOG)

$(TEST_DEPSDIR)/%.d: ;

test_report: tests
	@-$(MKDIR) $(REPORTDIR)
	@-$(ECHO) $(TEAL) "Running test binary" $(DEFAULT)
	@-$(TEST_OUTDIR)/$(TEST_NAME) --verbose
	@-$(ECHO) $(TEAL) "Generating report" $(DEFAULT)
	@-gcovr --html-details $(TEST_REPORT_NAME) -s --exclude-unreachable-branches --exclude-throw-branches -e "$(TEST_SRCSDIR)/.*" -e "$(SRCSDIR)/exceptions"
	@-gcovr --exclude-unreachable-branches --exclude-throw-branches -d -e "$(TEST_SRCSDIR)/.*" -e  "$(SRCSDIR)/exceptions"

tags:
	@ctags -R $(SRCSDIR)

clean: cleandep
	@-$(RM) $(OBJSDIR)
	@-$(ECHO) $(TEAL) "Removing objects files" $(DEFAULT)

cleandep:
	@-$(RM) $(DEPSDIR)
	@-$(ECHO) $(TEAL) "Removing dependencies files" $(DEFAULT)

cleanlog:
	@-$(RM) $(LOGSDIR)
	@-$(ECHO) $(TEAL) "Removing logs files" $(DEFAULT)

test_clean: clean cleandep test_cleandep
	@-$(RM) $(TEST_OBJSDIR)
	@-$(ECHO) $(TEAL) "Removing test objects files" $(DEFAULT)

test_cleandep:
	@-$(RM) $(TEST_DEPSDIR)
	@-$(ECHO) $(TEAL) "Removing test dependencies files" $(DEFAULT)

test_cleanlog:
	@-$(RM) $(TEST_LOGSDIR)
	@-$(ECHO) $(TEAL) "Removing tests logs files" $(DEFAULT)

distclean: clean cleanlog test_clean test_cleanlog
	@-$(RM) $(OUTDIR)/$(NAME)
	@-$(RM) $(TEST_OUTDIR)/$(TEST_NAME)
	@-$(ECHO) $(TEAL) "Removing binary" $(DEFAULT)

re: distclean all

.PRECIOUS: $(DEPSDIR)/%.d
.PRECIOUS: $(TEST_DEPSDIR)/%.d
.PHONY: all clean cleanlog cleandep test_clean test_cleanlog test_cleandep distclean re tags

.SUFFIXES:
.SUFFIXES: .cpp .o

include $(patsubst $(SRCSDIR)/%,$(DEPSDIR)/%, $(SRCS:.cpp=.d))
include $(patsubst $(TEST_SRCSDIR)/%,$(TEST_DEPSDIR)/%, $(TEST_SRCS:.cpp=.d))
