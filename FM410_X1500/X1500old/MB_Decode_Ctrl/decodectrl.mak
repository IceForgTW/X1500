

################################################################

DCTRLDIR = $(TOP)/MB_Decode_Ctrl


CFLAGS += -I$(DCTRLDIR) \
			-I$(DCTRLDIR)/U_layer \
			-I$(DCTRLDIR)/X_layer/X_param \
			-I$(DCTRLDIR)/W_layer/W_info \
			-I$(DCTRLDIR)/W_layer/W_setc \
			-I$(DCTRLDIR)/W_layer/W_macro \
			-I$(DCTRLDIR)/Y_layer/Y_param \
			-I$(DCTRLDIR)/Y_layer/Y_cmos \
			-I$(DCTRLDIR)/Y_layer/Y_comm \
			-I$(DCTRLDIR)/Z_layer \
			-I$(DCTRLDIR)/V_layer/V_comm \
			-I$(DCTRLDIR)/NLS_SCProtocol \

VPATH += $(DCTRLDIR) \
			$(DCTRLDIR)/U_layer \
			$(DCTRLDIR)/X_layer/X_param \
			$(DCTRLDIR)/W_layer/W_info \
			$(DCTRLDIR)/W_layer/W_setc \
			$(DCTRLDIR)/W_layer/W_macro \
			$(DCTRLDIR)/Y_layer/Y_param \
			$(DCTRLDIR)/Y_layer/Y_cmos \
			$(DCTRLDIR)/Y_layer/Y_comm \
			$(DCTRLDIR)/Z_layer \
			$(DCTRLDIR)/V_layer/V_comm \
			$(DCTRLDIR)/NLS_SCProtocol \
			
SOURCES += 	$(DCTRLDIR)/U_layer/TopLayer.c \
			$(DCTRLDIR)/X_layer/X_param/DataTransform.c    \
			$(DCTRLDIR)/W_layer/W_info/SysInfoEdit.c       \
			$(DCTRLDIR)/W_layer/W_setc/SetCodeParse.c      \
			$(DCTRLDIR)/W_layer/W_macro/uImgMacro.c        \
			$(DCTRLDIR)/Y_layer/Y_cmos/Y_cmosAction.c      \
			$(DCTRLDIR)/Y_layer/Y_param/Y_paramStorage.c   \
			$(DCTRLDIR)/Y_layer/Y_comm/Y_commMethod.c      \
			$(DCTRLDIR)/Z_layer/Z_OSIn.c                   \
			$(DCTRLDIR)/V_layer/V_comm/ProtocolParse.c     \
			$(DCTRLDIR)/NLS_SCProtocol/SC_Function.c     \
			$(DCTRLDIR)/NLS_SCProtocol/SC_MapTable.c     \
			$(DCTRLDIR)/NLS_SCProtocol/SC_Protocol.c     \
			$(DCTRLDIR)/NLS_SCProtocol/NLSComm_Proc.c     \
			$(DCTRLDIR)/NLS_SCProtocol/SC_ExtFunc.c     \






