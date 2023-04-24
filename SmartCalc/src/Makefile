
ifeq ($(DIR), )
  DIR=executor
endif

all: clean build

install: install_libs dynamic_lib
	 pyinstaller --name=SmartCalculator --windowed main.py --paths . --hidden-import=view --add-data "view/ui_interface/main_window.ui:view/ui_interface" --add-data "view/info.txt:./view" --add-binary 'model/libcalculator.dylib:model'  --icon='view/images/icon.icns' -F --distpath $(DIR)
	 
uninstall:
	rm -rf build/ $(DIR) SmartCalculator.spec

dmg_build:
	 hdiutil create -srcfolder executor/SmartCalculator.app -format UDZO -volname "SmartCalc v3.0 Installer" SmartCalculator.dmg

build: clean dynamic_lib
	python3.11 main.py

clean:
	@rm -f model/*.dylib SmartCalculator.dmg

clang_format:
	cp ../materials/linters/.clang-format ./
	clang-format -n model/model_c_plus_plus/*.cpp model/model_c_plus_plus/*.hpp
	rm -f .clang-format

dynamic_lib: clean
	g++ -std=c++17 -shared -fPIC -o model/libcalculator.dylib model/model_c_plus_plus/model.cpp model/model_c_plus_plus/wrapper.cpp

pylint:
	pylint --rcfile pylintrc view/*.py presenter/*.py model/*.py *.py
	
install_libs:
	pip3 install -r requirements.txt
	
check: clang_format pylint

tests: clean dynamic_lib
	python3.11 tests.py

.PHONY: all dynamic_lib install uninstall dmg_build build clean clang_format pylint install_libs check tests
