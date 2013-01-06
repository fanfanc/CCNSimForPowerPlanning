#
# OMNeT++/OMNEST Makefile for ccnSim_modified_for_my_research
#
# This file was generated with the command:
#  opp_makemake -f --deep -X patch
#

# Name of target to be created (-o option)
TARGET = ccnSim$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I. \
    -Icontent_distribution \
    -Ihowto \
    -Imessages \
    -Imodules \
    -Inode \
    -Ioptimal \
    -Iresults \
    -Irouting \
    -Istatistics \
    -Itopologies \
    -Iutils

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/content_distribution/content_distribution.o \
    $O/node/base_cache.o \
    $O/node/ccn_node.o \
    $O/node/global_request_generator.o \
    $O/node/lfu_cache.o \
    $O/node/lru_cache.o \
    $O/node/repository.o \
    $O/node/request_generator.o \
    $O/routing/routing.o \
    $O/statistics/net_inspector.o \
    $O/statistics/statistics.o \
    $O/messages/access_data_m.o \
    $O/messages/ccn_data_m.o \
    $O/messages/ccn_interest_m.o \
    $O/messages/local_m.o

# Message files
MSGFILES = \
    messages/access_data.msg \
    messages/ccn_data.msg \
    messages/ccn_interest.msg \
    messages/local.msg

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f ccnSim_modified_for_my_research ccnSim_modified_for_my_research.exe libccnSim_modified_for_my_research.so libccnSim_modified_for_my_research.a libccnSim_modified_for_my_research.dll libccnSim_modified_for_my_research.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f content_distribution/*_m.cc content_distribution/*_m.h
	-rm -f howto/*_m.cc howto/*_m.h
	-rm -f messages/*_m.cc messages/*_m.h
	-rm -f modules/*_m.cc modules/*_m.h
	-rm -f node/*_m.cc node/*_m.h
	-rm -f optimal/*_m.cc optimal/*_m.h
	-rm -f results/*_m.cc results/*_m.h
	-rm -f routing/*_m.cc routing/*_m.h
	-rm -f statistics/*_m.cc statistics/*_m.h
	-rm -f topologies/*_m.cc topologies/*_m.h
	-rm -f utils/*_m.cc utils/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc content_distribution/*.cc howto/*.cc messages/*.cc modules/*.cc node/*.cc optimal/*.cc results/*.cc routing/*.cc statistics/*.cc topologies/*.cc utils/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/content_distribution/content_distribution.o: content_distribution/content_distribution.cc \
  content_distribution/content_distribution.h
$O/messages/access_data_m.o: messages/access_data_m.cc \
  messages/access_data_m.h
$O/messages/ccn_data_m.o: messages/ccn_data_m.cc \
  messages/ccn_data_m.h
$O/messages/ccn_interest_m.o: messages/ccn_interest_m.cc \
  messages/ccn_interest_m.h
$O/messages/local_m.o: messages/local_m.cc \
  messages/local_m.h
$O/node/base_cache.o: node/base_cache.cc \
  ./utils/ccn_utils.h \
  ./content_distribution/content_distribution.h \
  node/base_cache.h
$O/node/ccn_node.o: node/ccn_node.cc \
  ./node/repository.h \
  node/global_request_generator.h \
  node/caching_policy.h \
  node/never_policy.h \
  messages/ccn_interest_m.h \
  node/request_generator.h \
  node/lcd_policy.h \
  ./node/caching_policy.h \
  node/lru.hpp \
  ./statistics/stat_util.h \
  routing/routing.h \
  messages/local_m.h \
  messages/ccn_data_m.h \
  node/always_policy.h \
  node/distance_policy.h \
  node/random_cache.h \
  node/ccn_node.h \
  node/repository.h \
  node/two_cache.h \
  node/fix_policy.h \
  node/base_cache.h \
  content_distribution/content_distribution.h \
  ./node/ccn_node.h \
  messages/access_data_m.h \
  messages/ccn_interest.h \
  node/fifo_cache.h \
  statistics/statistics.h \
  ./utils/ccn_utils.h \
  ./content_distribution/content_distribution.h \
  node/lfu_cache.h \
  node/lru_cache.h
$O/node/global_request_generator.o: node/global_request_generator.cc \
  ./node/repository.h \
  node/global_request_generator.h \
  ./messages/local_m.h \
  messages/ccn_interest_m.h \
  node/request_generator.h \
  ./node/caching_policy.h \
  node/lru.hpp \
  routing/routing.h \
  messages/ccn_data_m.h \
  node/base_cache.h \
  content_distribution/content_distribution.h \
  statistics/statistics.h \
  messages/access_data_m.h \
  ./node/ccn_node.h \
  messages/ccn_interest.h \
  ./utils/ccn_utils.h \
  ./content_distribution/content_distribution.h \
  node/lru_cache.h
$O/node/lfu_cache.o: node/lfu_cache.cc \
  node/base_cache.h \
  ./content_distribution/content_distribution.h \
  node/lfu_cache.h
$O/node/lru_cache.o: node/lru_cache.cc \
  node/lru.hpp \
  node/base_cache.h \
  ./utils/ccn_utils.h \
  ./content_distribution/content_distribution.h \
  node/lru_cache.h
$O/node/repository.o: node/repository.cc \
  node/caching_policy.h \
  messages/ccn_interest_m.h \
  node/lru.hpp \
  routing/routing.h \
  messages/ccn_data_m.h \
  node/ccn_node.h \
  node/repository.h \
  node/base_cache.h \
  content_distribution/content_distribution.h \
  statistics/statistics.h \
  messages/access_data_m.h \
  messages/ccn_interest.h \
  ./utils/ccn_utils.h \
  ./content_distribution/content_distribution.h \
  node/lru_cache.h
$O/node/request_generator.o: node/request_generator.cc \
  ./node/repository.h \
  messages/ccn_interest_m.h \
  node/request_generator.h \
  node/lru.hpp \
  ./node/caching_policy.h \
  routing/routing.h \
  messages/local_m.h \
  messages/ccn_data_m.h \
  node/base_cache.h \
  content_distribution/content_distribution.h \
  messages/ccn_interest.h \
  ./node/ccn_node.h \
  messages/access_data_m.h \
  statistics/statistics.h \
  ./utils/ccn_utils.h \
  ./content_distribution/content_distribution.h \
  node/lru_cache.h
$O/routing/routing.o: routing/routing.cc \
  routing/routing.h
$O/statistics/net_inspector.o: statistics/net_inspector.cc \
  statistics/net_inspector.h
$O/statistics/statistics.o: statistics/statistics.cc \
  node/lru.hpp \
  ./statistics/stat_util.h \
  node/base_cache.h \
  statistics/statistics.h \
  ./utils/ccn_utils.h \
  ./content_distribution/content_distribution.h \
  node/lru_cache.h
