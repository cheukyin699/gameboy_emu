CXX := g++
CXXFLAGS := -c -g
LD := g++
LDFLAGS := -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -g

EMU := GB_Emulator
DIS := GB_Disassembler

EMUOBJ := obj/Main.o obj/Emulator.o obj/LR35902.o
DISOBJ :=

EMUSRC :=
DISSRC :=

emu: $(EMU)

dis: $(DIS)

all: emu dis

obj/:
	@echo "Creating object directory"
	@mkdir $@

$(EMU): obj/ $(EMUOBJ)
	$(LD) $(LDFLAGS) $(EMUOBJ) -o $@

$(DIS): obj/ $(DISOBJ)
	$(LD) $(LDFLAGS) $(DISOBJ) -o $@

obj/Main.o: src/Main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

obj/Emulator.o: src/Emulator.cpp src/Emulator.h
	$(CXX) $(CXXFLAGS) $< -o $@

obj/LR35902.o: src/LR35902.cpp src/LR35902.h
	$(CXX) $(CXXFLAGS) $< -o $@
