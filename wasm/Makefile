CPPFLAGS = -I. -Iegl_helper -I../common -Wall -fno-exceptions -fno-rtti -s WASM=1
LDFLAGS = --embed-file ../assets@/

OUT_DIR = build

MKDIR_P = mkdir -p
RMDIR = rm -rdf

SOURCES = MoreTeapots-wasm.cpp \
	egl_helper/esUtil.c \
	../common/MoreTeapotsRenderer.cpp \
	../common/shader.cpp \
	../common/vecmath.cpp \
	shader_utils.cpp \
	file_utils.c

OBJECTS = MoreTeapots-wasm.o \
	egl_helper/esUtil.o \
	../common/MoreTeapotsRenderer.o \
	../common/shader.o \
	../common/vecmath.o \
	shader_utils.o \
	file_utils.o

TARGET = ${OUT_DIR}/teapots.html

#targets start here
all: directories $(TARGET)

directories: ${OUT_DIR}

${OUT_DIR}:
	${MKDIR_P} ${OUT_DIR}

$(TARGET): $(OBJECTS)
	$(CC) $(CPPFLAGS) -o $@ $(LDFLAGS) $(OBJECTS)

clean:
	$(RMDIR) $(OUT_DIR) $(OBJECTS)

cleanobj:
	$(RM) $(OBJECTS)

depend:
	@$(CC) $(CPPFLAGS) -MM $(SOURCES)

# list targets that do not create files (but not all makes understand .PHONY)
.PHONY:	all clean depend directories cleanobj

MoreTeapots-wasm.o: MoreTeapots-wasm.cpp egl_helper/esUtil.h ../common/MoreTeapotsRenderer.h platform.h

egl_helper/esUtil.o: egl_helper/esUtil.c egl_helper/esUtil.h

../common/MoreTeapotsRenderer.o: ../common/MoreTeapotsRenderer.cpp ../common/MoreTeapotsRenderer.h ../common/shader.h ../common/vecmath.h

../common/shader.o: ../common/shader.cpp ../common/shader.h

../common/vecmath.o: ../common/vecmath.cpp ../common/vecmath.h

shader_utils.o: shader_utils.cpp platform.h file_utils.h ../common/shader.h

file_utils.o: file_utils.c file_utils.h
