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

defineTest(testPathAndName){
	isEmpty(libPath){
		return(false)
	}
	isEmpty(libName){
		return(false)
	}
	return(true)
}

testPathAndName(){
	isEmpty(workspacePath){
		workspacePath=..
	}
	INCLUDEPATH += $${workspacePath}/$${libPath}
	unix{
		LIBS += -L$${workspacePath}/dist/unix
		CONFIG(debug,debug|release){
			LIBS += -l$${libName}d
			POST_TARGETDEPS += $${workspacePath}/dist/unix/lib$${libName}d.a
		}
		CONFIG(release,debug|release){
			LIBS += -l$${libName}
			POST_TARGETDEPS += $${workspacePath}/dist/unix/lib$${libName}.a
		}
	}
	win32{
		LIBS += -L$${workspacePath}/dist/win32
		CONFIG(debug,debug|release){
			LIBS += -l$${libName}d
			POST_TARGETDEPS += $${workspacePath}/dist/win32/lib$${libName}d.a
		}
		CONFIG(release,debug|release){
			LIBS += -l$${libName}
			POST_TARGETDEPS += $${workspacePath}/dist/win32/lib$${libName}.a
		}
	}
	os2{
		LIBS += -L../build/target/os2
		CONFIG(debug,debug|release){
			LIBS += -l$${libName}d
			POST_TARGETDEPS += ../build/target/os2/$${libName}d.lib
		}
		CONFIG(release,debug|release){
			LIBS += -l$${libName}
			POST_TARGETDEPS += ../build/target/os2/$${libName}.lib
		}
	}
	macx{
		LIBS += -L$${workspacePath}/dist/macx
		CONFIG(debug,debug|release){
			LIBS += -l$${libName}d
			POST_TARGETDEPS += $${workspacePath}/dist/macx/lib$${libName}d.a
		}
		CONFIG(release,debug|release){
			LIBS += -l$${libName}
			POST_TARGETDEPS += $${workspacePath}/dist/macx/lib$${libName}.a
		}
	}
}else{
	message(set libPath not including ../ and libName not including lib*.a to use this script)
	message(use workspacePath var to import lib from another workspace)
}
