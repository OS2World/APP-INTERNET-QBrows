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

!isEmpty(appName){
	MOC_DIR = ../build/moc
	UIC_DIR = ../build/uic
	RCC_DIR = ../build/res
	unix{
		TEMPLATE = app
		CONFIG(debug,debug|release){
			OBJECTS_DIR = "../build/unix/$${appName}/debug/obj"
			TARGET = "../dist/unix/$${appName}_debug"
		}
		CONFIG(release,debug|release){
			OBJECTS_DIR = "../build/unix/$${appName}/release/obj"
			TARGET = "../dist/unix/$${appName}"
		}
	}
	win32{
		TEMPLATE = app
		CONFIG(debug,debug|release){
			OBJECTS_DIR = "../build/win32/$${appName}/debug/obj"
			TARGET = "../../dist/win32/$${appName}_debug"
		}
		CONFIG(release,debug|release){
			OBJECTS_DIR = "../build/win32/$${appName}/release/obj"
			TARGET = "../../dist/win32/$${appName}"
		}
	}
	os2{
		TEMPLATE = app
		CONFIG(debug,debug|release){
			OBJECTS_DIR = "../build/os2/$${appName}/debug/obj"
			TARGET = "../../dist/os2/$${appName}_debug"
		}
		CONFIG(release,debug|release){
			OBJECTS_DIR = "../build/target/os2/$${appName}/release/obj"
			TARGET = "../build/target/os2/$${appName}"
		}
	}
	macx{
		TEMPLATE = app
		CONFIG(debug,debug|release){
			OBJECTS_DIR = "../build/macx/$${appName}/debug/obj"
			TARGET = "../dist/macx/$${appName}_debug"
		}
		CONFIG(release,debug|release){
			OBJECTS_DIR = "../build/macx/$${appName}/release/obj"
			TARGET = "../dist/macx/$${appName}"
		}
	}
}else{
	message(set var appName to use this script)
}
