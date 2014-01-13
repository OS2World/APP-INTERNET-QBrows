#	This file is part of QBrows.
#
#	QBrows is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	QBrows is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with QBrows. If not, see <http://www.gnu.org/licenses/>.

!isEmpty(libName){
	MOC_DIR = ../build/moc
	UIC_DIR = ../build/uic
	RCC_DIR = ../build/res
	unix{
		TEMPLATE = lib
		CONFIG += staticlib
		CONFIG(debug,debug|release){
			OBJECTS_DIR = "../build/unix/slib$${libName}/debug/obj"
			TARGET = "../dist/unix/$${libName}d"
		}
		CONFIG(release,debug|release){
			OBJECTS_DIR = "../build/unix/slib$${libName}/release/obj"
			TARGET = "../dist/unix/$${libName}"
		}
	}
	win32{
		TEMPLATE = lib
		CONFIG += staticlib
		CONFIG(debug,debug|release){
			OBJECTS_DIR = "../build/win32/slib$${libName}/debug/obj"
			TARGET = "../../dist/win32/$${libName}d"
		}
		CONFIG(release,debug|release){
			OBJECTS_DIR = "../build/win32/slib$${libName}/release/obj"
			TARGET = "../../dist/win32/$${libName}"
		}
	}
	os2{
		TEMPLATE = lib
		CONFIG += staticlib
		CONFIG(debug,debug|release){
			OBJECTS_DIR = "../build/os2/slib$${libName}/debug/obj"
			TARGET = "../build/target/os2/$${libName}d"
		}
		CONFIG(release,debug|release){
			OBJECTS_DIR = "../build/os2/slib$${libName}/release/obj"
			TARGET = "../build/target/os2/$${libName}"
		}
	}
	macx{
		TEMPLATE = lib
		CONFIG += staticlib
		CONFIG(debug,debug|release){
			OBJECTS_DIR = "../build/macx/slib$${libName}/debug/obj"
			TARGET = "../dist/macx/$${libName}d"
		}
		CONFIG(release,debug|release){
			OBJECTS_DIR = "../build/macx/slib$${libName}/release/obj"
			TARGET = "../dist/macx/$${libName}"
		}
	}
}else{
	message(set libName var to use this script)
}
