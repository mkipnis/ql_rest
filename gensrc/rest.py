"""
   Copyright (C) 2022 Mike Kipnis

   This file is part of ql_rest, a free-software/open-source library
   for utilization of QuantLib over REST

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
"""

import os, sys

from gensrc.addins import addin
from gensrc.addins import cppexceptions
from gensrc.configuration import environment
from gensrc.categories import category
from gensrc.utilities import log
from gensrc.utilities import common

from gensrc.utilities import outputfile


class RESTAddin(addin.Addin):

    #############################################
    # public interface
    #############################################

    def generate(self, categoryList, enumerationList):
        """Generate source code for QLRest/C++ addin."""

        self.categoryList_ = categoryList
        self.enumerationList_ = enumerationList

        log.Log.instance().logMessage(' begin generating %s...' % self.name_)
        self.generateFunctions()
        log.Log.instance().logMessage(' done generating %s.' % self.name_)

    def generateFunctions(self):
        """Generate source code for all functions in all categories."""

        loopIncludes = ''
        pyfiles = ''
        hpp_files = ''
        cpp_files = ''

        for cat in self.categoryList_.categories("Cpp",
                                                 self.coreCategories_, self.addinCategories_):

            qlAddinName = cat.name()
            categoryIncludes = cat.includeList(loopIncludes)

            pyfile = '\t%s.py\n' % (cat.name())

            hpp_file = '\t%s_reader.hpp ' % (cat.name())
            cpp_file = '\t%s_reader.cpp ' % (cat.name())

            bufferCpp = ''
            bufferHpp = ''
            bufferPY = ''

            for func in cat.functions("Cpp"):
                bufferPY += self.generatePY(func, cat)
                bufferCpp += self.generateFunction(func, cat)
                bufferHpp += self.generateDataReaderDeclaration(func, cat)

            pyfiles += pyfile
            hpp_files += hpp_file
            cpp_files += cpp_file

            print("Processing catalog : " + cat.name())

            self.bufferBody_.set({
                'bufferCpp': bufferCpp,
                'qlAddinName': qlAddinName,
                'categoryName': cat.name(),
                'categoryIncludes': categoryIncludes})

            self.bufferHeader_.set({
                'categoryName': cat.name(),
                'bufferHpp': bufferHpp,
                'qlAddinName': qlAddinName,
                'categoryIncludes': categoryIncludes})

            self.bufferPythonModule_.set({
                'categoryName': cat.name(),
                'bufferPY': bufferPY})

            fileNameCpp = '%s%s_reader.cpp' % (self.rootPath_, cat.name())
            outputfile.OutputFile.STUBFILE_NAME = "/* Autogenerated from the following stub : %s */\n\n"
            outputfile.OutputFile(self, fileNameCpp, cat.copyright(), self.bufferBody_)

            fileNameHpp = '%s%s_reader.hpp' % (self.rootPath_, cat.name())
            outputfile.OutputFile.STUBFILE_NAME = "/* Autogenerated from the following stub : %s */\n\n"
            outputfile.OutputFile(self, fileNameHpp, cat.copyright(), self.bufferHeader_)

            fileNamePY = '%spython_package/src/ql_rest/%s.py' % (self.rootPath_, cat.name())
            outputfile.OutputFile.STUBFILE_NAME = "# Autogenerated from the following stub : %s\n\n"
            outputfile.OutputFile(self, fileNamePY, "", self.bufferPythonModule_, printHeader=False)

        self.bufferMakefile_.set({'headers': hpp_files, 'sources': cpp_files})
        fileNameMakefile = '%sMakefile.am' % self.rootPath_
        outputfile.OutputFile.STUBFILE_NAME = "/* Autogenerated from the following stub : %s */\n\n"
        outputfile.OutputFile(self, fileNameMakefile, '', self.bufferMakefile_, printHeader=False)


    def generateFunction(self, func, cat):
        """Generate source code for a given function."""
        returnType = self.functionReturnType_.apply(func.returnValue());
        qlCallResultDeclaration = ''
        qlCallResultType = '';
        returnStatement = ''
        libraryFunction = ''

        if returnType == "stringVector":
            qlCallResultType = 'std::vector<std::string>'
            qlCallResultDeclaration = qlCallResultType + ' returnObject;'
        elif returnType == "Property":
            qlCallResultType = 'ObjectHandler::property_t'
            qlCallResultDeclaration = qlCallResultType + ' returnObject;'
        else:
            if returnType == "String":
                qlCallResultType = 'std::string'
                qlCallResultDeclaration = qlCallResultType + ' returnObject;'
            else:
                qlCallResultType = returnType;
                qlCallResultDeclaration = qlCallResultType + ' returnObject;'

        return self.bufferDataReaderImplementation_.set({
            'functionDeclaration': func.parameterList().generate(self.functionDeclaration_),
            'functionName': func.name(),
            'functionReturnType': self.functionReturnType_.apply(func.returnValue()),
            'categoryName': cat.name(),
            'qlCallResultDeclaration': qlCallResultDeclaration,
            'libraryFunction': libraryFunction,
            'qlCallResultType': qlCallResultType
        })

    def generatePY(self, func, cat):
        """Generate py code for a given function."""

        return self.bufferPY_.set({
            'pyDeclaration': func.parameterList().generate(self.pyDeclaration_),
            'functionName': func.name(),
            'categoryName': cat.name()})

    def generateLib(self, func, cat, pyfile, readerImplHpp, readerImplCpp):
        return self.bufferLib_.set({
            'libname': cat.name(),
            'sharedname': cat.name(),
            'idlfile': pyfile,
            'readerImplHpp': readerImplHpp,
            'readerImplCpp': readerImplCpp})

    def generateLibJava(self, libname, idlfile):
        return self.bufferLibJava_.set({
            'libname': libname,
            'idlfile': idlfile
        })

    def generateLibDataReader(self, func, cat, pyfile, readerImplHpp, readerImplCpp):
        return self.bufferLibDataReader_.set({
            'libname': cat.name(),
            'sharedname': cat.name(),
            'pyfile': pyfile,
            'readerImplHpp': readerImplHpp,
            'readerImplCpp': readerImplCpp})

    def generateDataReaderDeclaration(self, func, cat):
        returnType = self.functionReturnType_.apply(func.returnValue());

        if returnType == "stringVector":
            qlCallResultType = 'std::vector<std::string>'
            qlCallResultDeclaration = qlCallResultType + ' returnObject;'
        elif returnType == "Property":
            qlCallResultType = 'ObjectHandler::property_t'
            qlCallResultDeclaration = qlCallResultType + ' returnObject;'
        else:
            if returnType == "String":
                qlCallResultType = 'std::string'
                qlCallResultDeclaration = qlCallResultType + ' returnObject;'
            else:
                qlCallResultType = returnType;
                qlCallResultDeclaration = qlCallResultType + ' returnObject;'

        """Generate source code for a given function."""
        return self.bufferDataReaderDeclaration_.set({
            'functionDeclaration': func.parameterList().generate(self.functionDeclaration_),
            'functionName': func.name(),
            'categoryName': cat.name(),
            'qlCallResultType': qlCallResultType,
            'functionReturnType': self.functionReturnType_.apply(func.returnValue())})

    #############################################
    # serializer interface
    #############################################

    def serialize(self, serializer):
        """Load/unload class state to/from serializer object."""
        super(RESTAddin, self).serialize(serializer)